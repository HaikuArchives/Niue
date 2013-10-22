
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



#include "openwindow.h"
//#include "niue.h"

OpenWindow::OpenWindow()
	:	BWindow(BRect(0,0,300,400), "Niue - Browse projects", B_TITLED_WINDOW,B_WILL_DRAW)
{
	CenterWindow();
		
	//define objects
	vwMain = new MainView(BRect(0,0,300,400), "vwMain");
	
	// set objects
	SetSizeLimits(300,1000,400,1200);
	
	//add objects
	AddChild(vwMain);
}


MainView::MainView(BRect rect, char *name) :
	BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW)
{
	int32 hsplit = 285;

	txtPath = new BTextControl(BRect(10,7,rect.Width()-75,25), "txtPath",\
								"Look for projects in:","/boot/home", new BMessage(GO_MSG),
								B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnGo = new BButton(BRect(rect.Width() - 70, 5, rect.Width() - 7, 25), "btnGo", "Search",
								new BMessage(GO_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP,
								B_WILL_DRAW | B_NAVIGABLE);
	btnOpen = new BButton(BRect(rect.Width() - 75,rect.Height() - 30,rect.Width() - 10,
								rect.Height() - 10),
						  "btnOpen", "Open", new BMessage(OPEN_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM,
						  B_WILL_DRAW | B_NAVIGABLE);
	btnCancel = new BButton(BRect(rect.Width()-150,rect.Height()-30,rect.Width()-85,rect.Height()-10), "btnCancel", "Cancel", new BMessage(CANCEL_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	fList = new ProjectsView(BRect(10,35,rect.Width()-(B_V_SCROLL_BAR_WIDTH+10),hsplit));
	WorkScroll = new BScrollView("WorkScroll", fList, B_FOLLOW_ALL, B_WILL_DRAW, false, true, B_PLAIN_BORDER);
	boxInfo	= new BBox(BRect(10, hsplit+10, rect.Width()-10, rect.Height()-35), "boxInfo", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_FRAME_EVENTS, B_FANCY_BORDER);
	txtInfo = new BTextView(BRect(2,2,rect.Width()-24, rect.Height()-(hsplit+50)), "txtInfo", BRect(2,2,rect.Width()-24,144), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);

	
	txtPath->SetDivider(be_plain_font->StringWidth("Look for projects in: "));
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	txtInfo->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	txtInfo->SetText("\n   …Hit search to look for projects");
	

	AddChild(txtPath);
	AddChild(btnGo);
	AddChild(btnOpen);
	AddChild(btnCancel);
	AddChild(WorkScroll);
	AddChild(boxInfo);
	boxInfo->AddChild(txtInfo);
	
	GetUserPath();
	
//	BuildList();  //instead of that:
	btnGo->MakeDefault(true);
}


MainView::~MainView()
{
}


bool
OpenWindow::QuitRequested()
{
	return true;
}


void
MainView::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case OPEN_MSG:
		
		SendMessage();
		
		break;
		
		case CANCEL_MSG:
		
		//((niue *)be_app)->appWindow->wdwOpen->Quit();
		Window()->Close();
		
		break;
		
		case SELECT_MSG:
		
		DisplayInfo();
		//swith default button
		if(btnGo->IsDefault())
		{
			btnGo->MakeDefault(false);
//			btnCancel->MakeDefault(true);  // this is really annoying
		}
		
		break;
		
		case GO_MSG:
		
		BuildList();	
					
		break;
	}
}



void
OpenWindow::CenterWindow()
{
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}


void
MainView::DisplayInfo()
{
	
	
	int32 		index;
	pListItem	*item;
	entry_ref	ref;
	BEntry		entry;
	char		name[B_FILE_NAME_LENGTH];
	BString		info;	
	BDirectory	dir;
	int32		entries;
	BPath		path;
	
	//get selection
	index = fList->CurrentSelection();
	
	if(index >= 0)
	{
		try
		{
			item = dynamic_cast<pListItem *>(fList->ItemAt(index));
			ref = item->Ref();
			entry.SetTo(&ref, true);
			entry.GetName(name);
			entry.GetPath(&path);
		
			dir.SetTo(&entry);
			entries = dir.CountEntries();
		
			info << "Project: ";
			info << name;
			info << "\n";
			info << "Files: ";
			info << entries;
			info << "\n";
			info << "Location: ";
			info << path.Path();
		
			txtInfo->SetText(info.String());
		}
		catch(int)
		{
			txtInfo->SetText("Error loading file info");
		}
	}
	
}

void MainView::SendMessage()
{
	
		BMessage 	msg(B_REFS_RECEIVED);
		int32		index;
		pListItem	*item;
		entry_ref	ref;
		
		//get selected item
		index = fList->CurrentSelection();
		if (index >= 0)
		{
			item = dynamic_cast<pListItem *>(fList->ItemAt(index));
			ref = item->Ref();
			msg.AddRef("refs", &ref);
		
			//send message		
			//((niue *)be_app)->appWindow->PostMessage(&msg);
			be_app->PostMessage(&msg);
		
			//and save the find argument
			SaveUserPath();
			
			//and close
			//((niue *)be_app)->appWindow->wdwOpen->Quit();
			Window()->Close();
		}
		else
		{
			(new BAlert("Niue", "Please select a project to open", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
	
}

void MainView::BuildList()
{
	
	BWindow* barberWindow = new BarberWindow("Searching…");
	barberWindow->Show();
	
	//let's first clear the list
	fList->MakeEmpty();
	
	BVolumeRoster roster;
	BVolume boot;
	BQuery  query;
		
	roster.GetBootVolume(&boot);
	query.SetVolume(&boot);
	
	query.PushAttr("name");
	query.PushString("makefile");
	query.PushOp(B_EQ);

	query.Fetch();
	
	BEntry entry;
	BDirectory dir;
	BPath path;
	
	BString userpath;
	BString foundpath;
	BString trimmedpath;
	
	userpath.SetTo(txtPath->Text());
	int32 length = userpath.CountChars();
	
	while(query.GetNextEntry(&entry) == B_OK)
	{
		if (entry.InitCheck() == B_OK)
		{
			if (entry.IsFile())
			{
				entry.GetPath(&path);
				foundpath.SetTo(path.Path());
				foundpath.CopyInto(trimmedpath, 0, length);
				if (trimmedpath == userpath)
				{
					entry.GetParent(&dir);
					entry.SetTo(&dir, NULL, false);
					fList->AddItem(new pListItem(&entry));
				}
				
			}
		}
	}
	
	fList->Select(0);
	fList->MakeFocus(true);
	
	barberWindow->Lock();
	barberWindow->Quit();
	
}


void
MainView::GetUserPath()
{
	//read the settings file
	BPath prefspath;
	
	//find prefsfile
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
	prefspath.Append("Niue/niue_prefs.browse");
	
	//create file
	BFile prefsfile(prefspath.Path(), B_READ_ONLY);
	if (prefsfile.IsReadable())
	{
		BMessage msg('PREF');
		BString browsestring;
		msg.Unflatten(&prefsfile);
		if(msg.FindString("browse_path", &browsestring) != B_OK)
			txtPath->SetText("/boot/home");
		else
			txtPath->SetText(browsestring.String());
	}
}


void
MainView::SaveUserPath()
{
		//now let's save the search argument
		BPath prefspath;
		
		//find prefsfile
		find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
		prefspath.Append("Niue/niue_prefs.browse");
		
		//create file
		BFile prefsfile(prefspath.Path(), B_WRITE_ONLY | B_CREATE_FILE);
		if(prefsfile.IsWritable())
		{
			BMessage msg('PREF');
			
			msg.AddString("browse_path", txtPath->Text());
			msg.Flatten(&prefsfile);
		}
}


void 
MainView::AttachedToWindow()
{
	fList->SetTarget(this);
	btnOpen->SetTarget(this);
	btnCancel->SetTarget(this);
	txtPath->SetTarget(this);
	btnGo->SetTarget(this);
	
	int32 items = fList->CountItems();
	float listheight = items * DEFAULT_ITEM_HEIGHT; //	- 200;
	
	BScrollBar *pBar = WorkScroll->ScrollBar(B_VERTICAL);
	
	if (pBar)
		pBar->SetRange(0, listheight);
}


ProjectsView::ProjectsView(BRect rect)
	:	BListView(rect, "list_view", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL,
				B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS)
{
}


ProjectsView::~ProjectsView()
{
	int32		index = 0;
	
	pListItem	*item = dynamic_cast<pListItem *>(ItemAt(index++));
	while (item != NULL) 
	{
		delete item;
		item = dynamic_cast<pListItem *>(ItemAt(index++));
	}
}


void 
ProjectsView::AttachedToWindow()
{
	SetInvocationMessage(new BMessage(OPEN_MSG));
	SetSelectionMessage(new BMessage(SELECT_MSG));
	//SetViewColor(B_TRANSPARENT_32_BIT);
}


pListItem::pListItem(BEntry *entry)
	:	BListItem()
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


pListItem::~pListItem()
{
	delete fIcon;
}


void 
pListItem::DrawItem(BView *view, BRect nrect, bool)
{
	float		offset = 10;
	BFont		font = be_plain_font;
	font_height	finfo;

	// set background color
	if (IsSelected()) 
	{
		// fill color
		view->SetHighColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
		// anti-alias color
		view->SetLowColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
	}
	else 
	{
		view->SetHighColor(white);
		view->SetLowColor(white);
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
		view->SetHighColor(ui_color(B_MENU_SELECTED_ITEM_TEXT_COLOR));
	else
		view->SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
	
	// set up font
	font.SetSize(10);
	font.GetHeight(&finfo);
	view->SetFont(&font);

	// position pen
	//view->MovePenTo(offset, nrect.top + 21);
	
	view->MovePenTo(offset,	nrect.top + ((nrect.Height() - 
											(finfo.ascent + finfo.descent + finfo.leading)) / 2) +
							(finfo.ascent + finfo.descent) - 2);
	
	// and draw label
	view->DrawString(fName);
}


void 
pListItem::Update(BView *owner, const BFont *finfo)
{
	// we need to override the update method so we can make sure are
	// list item size doesn't change
	BListItem::Update(owner, finfo);
	if ((fIcon) && (Height() < fIcon->Bounds().Height() + ITEM_MARGIN)) 
		SetHeight(fIcon->Bounds().Height() + ITEM_MARGIN);
	else
		SetHeight(21);
}
