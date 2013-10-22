
/*//-----------------------------------------------------------------------------
	Niue Development Environment
	Copyright 2008 T. Lansbergen, All Rights Reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted for non-commercial use only.
	
	This software is provided ``as is'' and any express or implied warranties,
	including, but not limited to, the implied warranties of merchantability and
	fitness for a particular purpose are disclaimed. In no event shall
	authors be liable for any direct, indirect, incidental, special,
	exemplary, or consequential damages (including, but not limited to,
	procurement of substitute goods or services; loss of use, data, or profits;
	or business interruption) however caused and on any theory of liability,
	whether in contract, strict liability, or tort (including negligence or
	otherwise) arising in any way out of the use of this software, even if
	advised of the possibility of such damage. 

*///-----------------------------------------------------------------------------




#include "exportwindow.h"

BRect hrect(0,0,400,400);

exportwindow::exportwindow(BEntry proj_entry) :
	BWindow(hrect, "Niue - Export", B_TITLED_WINDOW,B_WILL_DRAW | B_NOT_RESIZABLE)
{
	entry = proj_entry;
	char name[B_FILE_NAME_LENGTH];
	entry.GetName(name);
	
	CenterWindow();
		
	//define objects
	vwExport = new BView(hrect, "vwExport", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	
	btnClose = new BButton(BRect(hrect.Width()-200, hrect.Height()-30, hrect.Width()-110, hrect.Height()-10), "btnClose", "Cancel", new BMessage(CLOSE_EXPORT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	btnExport = new BButton(BRect(hrect.Width()-100, hrect.Height()-30, hrect.Width()-10, hrect.Height()-10), "btnExport", "Export", new BMessage(GO_EXPORT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	
	strName = new BStringView(BRect(10,10,hrect.Width()-10,25), "strName", "Application name:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtName = new BTextControl(BRect(10,30,hrect.Width()-10,50), "txtName", 0, name, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE); 
	
	chkSvg = new BCheckBox(BRect(10,75,hrect.Width()-180,90), "chkName", "Add SVG icon", new BMessage(SVG_EXPORT_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtSvg = new BTextControl(BRect(10,102,hrect.Width()-10,122), "txtSvg", 0, "/boot/", 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnNewIcon = new BButton(BRect(hrect.Width()-175, 73, hrect.Width()-85,93), "btnNewIcon", "New Icon", new BMessage(NEW_SVG_EXPORT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	btnSvg = new BButton(BRect(hrect.Width()-75, 73, hrect.Width()-10,93), "btnSvg", "Browse", new BMessage(BROWSE_SVG_EXPORT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	
	chkAdd = new BCheckBox(BRect(10,145,hrect.Width()-80,160), "chkAdd", "Add additional files (for example: readme's or install scripts)", new BMessage(ADD_EXPORT_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP);
	fList = new FileView(BRect(10,172,hrect.Width()-(B_V_SCROLL_BAR_WIDTH+12),350));
	WorkScroll = new BScrollView("WorkScroll", fList, B_FOLLOW_ALL, B_WILL_DRAW, false, true, B_PLAIN_BORDER);
	btnAdd = new BButton(BRect(hrect.Width()-75, 142, hrect.Width()-10,162), "btnAdd", "Browse", new BMessage(BROWSE_ADD_EXPORT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	
	//set objects
	vwExport->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	btnClose->MakeDefault(true);
	
	txtSvg->SetEnabled(false);
	btnSvg->SetEnabled(false);
	btnNewIcon->SetEnabled(false);
	fList->SetEnabled(false);
	btnAdd->SetEnabled(false);

	
	//add objects
	AddChild(vwExport);
	vwExport->AddChild(btnClose);
	vwExport->AddChild(btnExport);
	vwExport->AddChild(strName);
	vwExport->AddChild(txtName);
	vwExport->AddChild(chkSvg);
	vwExport->AddChild(txtSvg);
	vwExport->AddChild(btnNewIcon);
	vwExport->AddChild(btnSvg);
	vwExport->AddChild(chkAdd);
	vwExport->AddChild(WorkScroll);
	vwExport->AddChild(btnAdd);
	
	txtName->MakeFocus(true);
}


bool
exportwindow::QuitRequested()
{
	return true;
}


void exportwindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case CLOSE_EXPORT_MSG:
		{
			Quit();
		}
		break;
		
		case GO_EXPORT_MSG:
		{
			if(strlen(txtName->Text()) != 0)  //see if we have a name
			{
				//show savepanel
				savePanel = new BFilePanel(B_SAVE_PANEL, 0, 0, 0, false, new BMessage(READY_FOR_EXPORT_MSG), 0, false, true);
				savePanel->SetTarget(this);
				savePanel->SetSaveText(txtName->Text());
				savePanel->Show();
			}
			else
			{
				(new BAlert("Niue", "Please fill in a name for your appllication.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}			
		}
		break;
		
		case READY_FOR_EXPORT_MSG:
		{
		//get ref, entry & path
			entry_ref export_ref;
			BString export_name;
			message->FindRef("directory", &export_ref);
			message->FindString("name", &export_name);
			BEntry export_entry(&export_ref); 
			BPath export_path(&export_entry);
			
		//create folder
			BString destination;
			destination << export_path.Path() << "/" << export_name.String();
			if (create_directory(destination.String(), 077) != B_OK)
			{
				(new BAlert("Niue", "There was an error while creating your project's folder", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
				return;
			}
			
		//copy our executable
			BPath path(&entry);
			
			BString pathstring = path.Path();
			BString copystring;
			pathstring << "/obj.x86/";
			BDirectory dir(pathstring.String());
			BEntry file_entry;
			
			//ugly hack, chech if file, get last 2 chars, if .o skip, else lauch...
			while (dir.GetNextEntry(&file_entry, true) == B_NO_ERROR)
			{
				if (file_entry.IsFile())
				{
					char name[B_FILE_NAME_LENGTH];
					BString string, ext;
					file_entry.GetName(name);
					string.SetTo(name);
					int32 length = string.CountChars();
				
					if (length > 1)
					{
						string.CopyInto(ext, length-2, 2);
						if (ext != ".o")
						{
							if (file_entry.GetPath(&path) != B_OK)
							{
								(new BAlert("Niue", "There was an error while creating your project's folder", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
								return;
							}

							copystring << "cp " << path.Path() << " " << destination.String() << "/" << export_name;
							system(copystring.String());

						}

					}
				}

			}
			
		
			
		//Add svg icon
		if(chkSvg->Value() == true)
		{
			BString apppath;
			apppath << destination << "/" << export_name;
			const char *ifn = txtSvg->Text();
			const char *ofn = apppath.String();
		
			BFile	svgf(ifn,B_READ_ONLY);
			off_t	len;
			svgf.GetSize(&len);
			char	*buf=new char[len];
			len=svgf.Read(buf,len);
			if(len==0)
			{
				(new BAlert("Niue", "The provided icon can not read.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
			char	icon_d[8];
			sprintf(icon_d,"IDXZ%cS%c",5,0);
		
				BFile	f(ofn,B_READ_WRITE);
				BResources	r(&f);
				r.RemoveResource('iICO',101);
				r.RemoveResource('zICO',101);
				if(r.AddResource('iICO',101,icon_d,7,"BEOS:D:STD_ICON") != B_OK)
				{
					(new BAlert("Niue", "The provided icon cannot be added to the application's resource.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
				}
				if(r.AddResource('zICO',101,buf,len,"BEOS:V:STD_ICON") != B_OK)
				{
					(new BAlert("Niue", "The provided icon cannot be added to the application's resource.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();					
				}
				BNode	n(ofn);
				if(n.WriteAttr("BEOS:D:STD_ICON",'iICO',0,icon_d,7)!=7)
				{
					(new BAlert("Niue", "The provided icon cannot be added to the application's attribute.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();					
				}
				if(n.WriteAttr("BEOS:V:STD_ICON",'zICO',0,buf,len)!=len)
				{
					(new BAlert("Niue", "The provided icon cannot be add to the application's resource.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
				}
		}
		
		
		//add xtra files
		BString addstring;
		if(chkAdd->Value() == true)
		{
		
			
			addstring << "cp ";
			BPath addpath;

			for(int i = 0; i <= fList->CountItems(); i++)
			{
				entry_ref	ref; // = NULL;
				BEntry		added_entry;
				ListItem	*item;
				
				item = dynamic_cast<ListItem *>(fList->ItemAt(i));
				if (item)
					{
//						char name[B_FILE_NAME_LENGTH];
						ref = item->Ref();
						added_entry.SetTo(&ref, true);
						added_entry.GetPath(&addpath);
						addstring << " " << addpath.Path();
						
					}
			}
			
			
			addstring << " " << destination.String();
			system(addstring.String());
		}
		
		//zip up destination
		BString zipstring, rmstring;
		zipstring << "zip -rj " << destination.String() << " " << destination.String();
		
		if(system(zipstring.String()) != 0)
		{
			(new BAlert("Niue", "There was an error while zipping-up your application", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		} 
		else
		{
			rmstring << "rm -r " << destination.String();
			system(rmstring.String());
		}			
		//when we are finaly completely done,
		
//			BString alertstring;
//			alertstring << addstring.String();
//			(new BAlert("Niue", alertstring.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();

		
		Quit();	
			
		}
		break;
		
		case SVG_EXPORT_MSG:
		{
			if(chkSvg->Value() == 1)
			{
				txtSvg->SetEnabled(true);
				btnSvg->SetEnabled(true);
				entry_ref ref;
				if(be_roster->FindApp("application/x-vnd.haiku-icon_o_matic", &ref) != B_OK)  //check if we should offer this functionality
				{
					btnNewIcon->SetEnabled(false);
				}
				else
				{
					btnNewIcon->SetEnabled(true);	
				}
			}
			else
			{
				txtSvg->SetEnabled(false);
				btnSvg->SetEnabled(false);
				btnNewIcon->SetEnabled(false);
			}
		}
		break;
		
		case ADD_EXPORT_MSG:
		{
			if(chkAdd->Value() == 1)
			{
				fList->SetEnabled(true);
				btnAdd->SetEnabled(true);
			}
			else
			{
				fList->SetEnabled(false);
				btnAdd->SetEnabled(false);
			}
		}
		break;
		
		case BROWSE_SVG_EXPORT_MSG:
		{
			TDirFilter* filter = new TDirFilter;
			brsPanel = new BFilePanel(B_OPEN_PANEL, 0, 0, B_FILE_NODE, false, new BMessage(SVG_FOUND_EXPORT_MSG), filter, false, true);
			brsPanel->SetTarget(this);
			brsPanel->Show();
		}
		break;
		
		case BROWSE_ADD_EXPORT_MSG:
		{
			brsPanel = new BFilePanel(B_OPEN_PANEL, 0, 0, B_FILE_NODE, true, new BMessage(ADD_FOUND_EXPORT_MSG), 0, false, true);
			brsPanel->SetTarget(this);
			brsPanel->Show();
		}
		break;
		
		case SVG_FOUND_EXPORT_MSG:
		{
			entry_ref svg_ref;
			message->FindRef("refs", &svg_ref);
			
			BEntry svg_entry(&svg_ref); 
			BPath svg_path(&svg_entry);
			
			txtSvg->SetText(svg_path.Path());
		}
		break;
		
		case ADD_FOUND_EXPORT_MSG:
		{
			entry_ref add_ref;
			BEntry add_entry;
			
			int i = 0;
			while(message->FindRef("refs", i, &add_ref) == B_NO_ERROR)
			{
				add_entry.SetTo(&add_ref);
				fList->AddItem(new ListItem(&add_entry));
				
				i++;
			}	
		}
		break;
		
		case NEW_SVG_EXPORT_MSG:
		{
			if(be_roster->Launch("application/x-vnd.haiku-icon_o_matic") != B_OK)
			{
				(new BAlert("Niue", "Niue: Couldn't open Icon-o-Matic.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
		}
		break;
		
		case REMOVE_ADD_EXPORT_MSG:
		{
			fList->RemoveItem(fList->CurrentSelection());	
		}
		break;
	}
}



void exportwindow::CenterWindow()
{
	
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}

FileView::FileView(BRect rect)
				:BListView(rect, "list_view", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS)
{
}

FileView::~FileView()
{
	int32		index = 0;
	ListItem	*item;

	while ((item = dynamic_cast<ListItem *>(ItemAt(index++))) != NULL) 
	{
		delete item;
	}
}

void FileView::AttachedToWindow()
{
	mnuLeft = new BPopUpMenu("right-click menu");	
	mnuLeft->SetRadioMode(false);
	mnuLeft->AddItem(new BMenuItem("Remove", new BMessage(REMOVE_ADD_EXPORT_MSG)));
	mnuLeft->SetTargetForItems(this);
}


void FileView::MouseDown(BPoint where)
{
	uint32		buttons;

	// retrieve the button state from the MouseDown message
	if (Window()->CurrentMessage()->FindInt32("buttons", (int32 *)&buttons) == B_NO_ERROR) 
	{
		// find item at the mouse location
		int32 item = IndexOf(where);
		// make sure item is valid
		if ((item >= 0) && (item < CountItems())) 
		{
			// if clicked with second mouse button, let's do a context-sensitive menu
			if (buttons & B_SECONDARY_MOUSE_BUTTON) 
			{
				
				BPoint	point = where;
				ConvertToScreen(&point);
				// select this item
				Select(item);
				
				mnuLeft->Go(point, true,true,(BRect(point.x,point.y,point.x + 1,point.y + 1)),true);
				return;
				
			}
			
			// if clicked with primary mouse button
			else if (buttons & B_PRIMARY_MOUSE_BUTTON) 
			{
					Select(item);
			}
			
		}
	}
	
}



void FileView::SetEnabled(bool enabled)
{
	if(enabled == false)
	{
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
		Invalidate();
	}
	else
	{
		SetViewColor(bright);
		Invalidate();
	}
}

ListItem::ListItem(BEntry *entry)
		  :BListItem()
{
	BNode		node;
	BNodeInfo	node_info;

	// try to get node info for this entry
	if ((node.SetTo(entry) == B_NO_ERROR) && (node_info.SetTo(&node) == B_NO_ERROR)) 
	{
		// cache name
		entry->GetName(fName);
		// create bitmap large enough for icon
		fIcon = new BBitmap(BRect(0, 0, B_MINI_ICON - 1, B_MINI_ICON -1 ), B_COLOR_8_BIT);
		// cache the icon
		node_info.GetTrackerIcon(fIcon, B_MINI_ICON);
		// adjust size of item to fit icon
		SetHeight(fIcon->Bounds().Height() + 5);
		// cache ref
		entry->GetRef(&fRef);
	}
	else 
	{
		// no icon
		fIcon = NULL; 
		// make dummy for seperators
		strcpy(fName, " "); 
		SetHeight(21);
	}
	
}


ListItem::~ListItem()
{
	delete fIcon;
}


void 
ListItem::DrawItem(BView *view, BRect nrect, bool)
{
	float		offset = 10;
	BFont		font = be_plain_font;
	font_height	finfo;

	// set background color
	if (IsSelected()) {
		// fill color
		view->SetHighColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
		// anti-alias color
		view->SetLowColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
	}
	else 
	{
		view->SetHighColor(bright);
		view->SetLowColor(bright);
	}
	
	// fill item's rect
	view->FillRect(nrect);
//	view->FillRoundRect(nrect, 2, 2);   //was: FillRect(rect)

	//if selected do some pretifying
	

	// if we have an icon, draw it
	if (fIcon) 
	{
		view->SetDrawingMode(B_OP_OVER);
		view->DrawBitmap(fIcon, BPoint(nrect.left + 2, nrect.top + 3));
		view->SetDrawingMode(B_OP_COPY);
		offset = fIcon->Bounds().Width() + 30;
		
	}
	
	if (IsSelected()) 
	{
		view->SetHighColor(ui_color(B_MENU_SELECTED_ITEM_TEXT_COLOR));
	}
	else
	{
		view->SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
	}
	// set up font
	font.SetSize(12);
	font.GetHeight(&finfo);
	view->SetFont(&font);

	// position pen
	//view->MovePenTo(offset, nrect.top + 21);
	
	view->MovePenTo(offset,
		nrect.top + ((nrect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2);
	
	// and draw label
	view->DrawString(fName);
	
}


void 
ListItem::Update(BView *owner, const BFont *finfo)
{
	// we need to override the update method so we can make sure are
	// list item size doesn't change
	BListItem::Update(owner, finfo);
	if ((fIcon) && (Height() < fIcon->Bounds().Height() + eITEM_MARGIN)) 
	{
		SetHeight(fIcon->Bounds().Height() + eITEM_MARGIN);
	}
	else
	{
		SetHeight(21);
	}
}

bool TDirFilter::Filter(const entry_ref* entry, BNode* node,
                   struct stat* stat, const char* mimetype)
{
  if (strcmp("application/x-vnd.Be-directory", mimetype) == 0)
  {
    return true;
  }
  else if (strcmp("application/x-vnd.Be-volume", mimetype) == 0)
  {
    return true;
  }
  else if (strcmp("image/xml+svg", mimetype) == 0)
  {
  	return true;
  }
  else
  {
    return false;
  }
}
