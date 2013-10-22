
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
#include "listview.h"


listview::listview(BRect rect, char *name)
	: BView(rect, name, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW)
{
	
	fList = new RocketView(BRect(rect.left + 7,rect.top - 11,rect.right - 22,
								rect.Height() - 5));
	WorkScroll = new BScrollView("WorkScroll", fList, B_FOLLOW_ALL_SIDES,
								B_WILL_DRAW, false, true, B_PLAIN_BORDER);
	AddChild(WorkScroll);
}


void
listview::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case ITEM_MENU_SELECTED_MSG:
		{
			entry_ref *fileref = FindItem();
			
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);
//			nodeInfo.SetTo();
			char type[256];
			nodeInfo.GetType(type);
			BString t;
			t << type;
			
//			printf(type);
			
//			const char *text = type;
//			(new BAlert("Niue", type, "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			
//			if (t == "text/plain" || t == "text/rtf" || t == "text/x-makefile" || t == "text/x-vnd.Be.ResourceDef" || t == "text/x-source-code")
//			{
				BMessage msg(SHOW_FILE_MSG);
				msg.AddRef("refs", fileref);
				msg.AddPointer("YWin",this);
				be_app->PostMessage(&msg);
//			}
//			else
//			{
//				//be_roster->Launch(fileref);
//			}
			break;
		}
			
		case ITEM_MENU_INVOKED_MSG:
		{
//			entry_ref *fileref = FindItem();
//			be_roster->Launch(fileref);
			break;
		}
		case MNU_LEFT_SET_SOURCE:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("text/x-source-code");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_MAKEFILE:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("text/x-makefile");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_RESOURCE:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("application/x-be-resource");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_TEXT:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("text/plain");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_IMAGE:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("image");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_AUDIO:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("audio");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_VIDEO:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("video");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_APP:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("application/x-vnd.Be-elfexecutable");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_SET_ARCHIVE:
		{
			entry_ref *fileref = FindItem();
						
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);	
			
			nodeInfo.SetType("application/zip");
			
			//rebuild list
			BuildList(startref);
			break;
		}
		case MNU_LEFT_OPEN_MSG:
		{
			entry_ref *fileref = FindItem();
			be_roster->Launch(fileref);
			break;
		}
		case MNU_LEFT_TRACKER_MSG:
		{
			BString sysstring;
			BEntry	direntry;
			BPath	dirpath;
			
			dir.GetEntry(&direntry);
			direntry.GetPath(&dirpath);
			
			sysstring << "/system/Tracker ";
			sysstring << dirpath.Path(); 
			
			//(new BAlert("Niue", sysstring.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			system(sysstring.String());
			break;
		}
		case MNU_LEFT_TRASH_MSG:
		{
			//(new BAlert("Niue", "trash it", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			
			entry_ref *fileref = FindItem();
			BEntry	fileentry;
			BDirectory trashdir;
			BPath trashpath;
			find_directory(B_TRASH_DIRECTORY, &trashpath, false);
			
			fileentry.SetTo(fileref);
			trashdir.SetTo(trashpath.Path());
			
			fileentry.MoveTo(&trashdir, 0, true);
			break;
		}
		case MNU_LEFT_DUPLICATE_MSG:
		{
			BString duplistring;
			BEntry duplientry;
			BPath duplipath;
			entry_ref *fileref = FindItem();
			
			duplientry.SetTo(fileref);
			duplientry.GetPath(&duplipath);
			
			duplistring << "cp ";
			duplistring << duplipath.Path();
			duplistring << " ";
			duplistring << duplipath.Path();
			duplistring << "_copy";
			
			//(new BAlert("Niue", duplistring.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			system(duplistring.String());
			break;
		}
		case MNU_LEFT_INFO_MSG:
		{
			//get item, create vars
			entry_ref *fileref = FindItem();
			BEntry fileentry;
			fileentry.SetTo(fileref, true);

			//name
			char name[B_FILE_NAME_LENGTH];
			fileentry.GetName(name);
			
			//size
			BFile file(fileref, B_READ_ONLY);
			off_t size;
			file.GetSize(&size);
			
			//kind
			BNode node(fileref);
			BNodeInfo nodeInfo(&node);
			char type[256];
			nodeInfo.GetType(type);
			
			//path
			BPath path;
			fileentry.GetPath(&path);
			
			//build info string
			BString infostring;
			infostring << "\n";
			infostring << name;
			infostring << "\n\nSize: ";
			infostring << size << " bytes";
			infostring << "\n\nKind: ";
			infostring << type;
			infostring << "\n\nPath: ";
			infostring << path.Path();
			infostring << "\n\n";
			
			(new BAlert("Niue file info", infostring.String(), "Close", 0, 0, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
			break;
		}
		case MNU_LEFT_NAME_MSG:
		{
			entry_ref *fileref = FindItem();
			BEntry fileentry;
			fileentry.SetTo(fileref);
			
			int32 index = fList->CurrentSelection();
			BRect itemrect = fList->ItemFrame(index);
			BRect textrect = ConvertToScreen(itemrect);
			textrect.top += 6;
			textrect.bottom += 5;
			textrect.left += 28;
			//textrect.right += 5;
			
			
			BWindow* renameWindow = new renamewindow(textrect, fileentry);
			renameWindow->Show();
			break;
		}
		case MNU_LEFT_COPY_MSG:
		{
			entry_ref *fileref = FindItem();
			BEntry fileentry;
			fileentry.SetTo(fileref);
			char name[B_FILE_NAME_LENGTH];
			fileentry.GetName(name);
			
			BMessage savemsg(B_SAVE_REQUESTED);
			savemsg.AddBool("keepfile", true);  //we are copying so we have to keep the original
			
			CopyPanel = new BFilePanel(B_SAVE_PANEL, 0, 0, B_DIRECTORY_NODE | B_FILE_NODE, false, &savemsg, 0, false, true);
			CopyPanel->SetTarget(this);
			CopyPanel->SetSaveText(name);
			CopyPanel->Show();	
			break;
		}
		case MNU_LEFT_MOVE_MSG:
		{
			entry_ref *fileref = FindItem();
			BEntry fileentry;
			fileentry.SetTo(fileref);
			char name[B_FILE_NAME_LENGTH];
			fileentry.GetName(name);
			
			BMessage savemsg(B_SAVE_REQUESTED);
			savemsg.AddBool("keepfile", false);   //we are moving so no need to keep the original
			
			CopyPanel = new BFilePanel(B_SAVE_PANEL, 0, 0, B_DIRECTORY_NODE | B_FILE_NODE, false, &savemsg, 0, false, true);
			CopyPanel->SetTarget(this);
			CopyPanel->SetSaveText(name);
			CopyPanel->Show();	
			break;
		}
		case B_SAVE_REQUESTED:
		{
			//get item entry
			entry_ref *fileref = FindItem();
			BEntry fileentry;
			fileentry.SetTo(fileref);
			BPath filepath;
			fileentry.GetPath(&filepath);
			
			//get destination
			entry_ref dirref;
			BEntry direntry;
			BPath dirpath;
			BString namestring;
			bool keepfile;
			if (msg->FindRef("directory", &dirref) == B_NO_ERROR && msg->FindString("name", &namestring) == B_NO_ERROR && msg->FindBool("keepfile", &keepfile) == B_NO_ERROR)
			{
				direntry.SetTo(&dirref, true);
				direntry.GetPath(&dirpath);
				
				BString copystring;
				if (keepfile == true)
					copystring << "cp ";
				else
					copystring << "mv ";
				copystring << filepath.Path();
				copystring << " ";
				copystring << dirpath.Path();
				copystring << "/";
				copystring << namestring;
				
				//(new BAlert("Niue", copystring.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
				system(copystring.String());
			}
			break;
		}
		case B_SIMPLE_DATA:  //someone dropped something on us
		{
			//(new BAlert("Niue", "Drop", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			be_app->PostMessage(msg);
			break;
		}
	}
}


entry_ref *
listview::FindItem()
{
//	char		string[512];
	int32		index;
	entry_ref	*ref = NULL;
//	status_t	result;
	TListItem	*item;
	
	index = fList->CurrentSelection();
	item = dynamic_cast<TListItem *>(fList->ItemAt(index));
	if (item)
	{
		ref = item->Ref();
		if (ref) 
			return(ref);
	}
	return NULL;
}


void
listview::SelectByName(const char *name)
{
	for(int i = 0; i < fList->CountItems(); i++)
	{
		char *listname = dynamic_cast<TListItem*>(fList->ItemAt(i))->GetName(); 
		if (strcmp(name, listname) == 0)
			fList->Select(i);
	}		
	
}


void
listview::BuildList(entry_ref ref)
{
	startref = ref;
	
//	(new BAlert("Niue", "Rebuilding list", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();

	//first off, clear all items
	fList->MakeEmpty();
	
	//seperators
	BListItem *seperator;
	fList->AddItem(new TListItem("Source files:"));
	fList->AddItem(seperator = new TListItem(" "));
	fList->AddItem(new TListItem("Resources:"));
	
	// and build the list
	dir.SetTo(&ref);	
	
//	int count;
	while (dir.GetNextEntry(&entry, true) == B_NO_ERROR) 
	{
		if (entry.IsFile())
		{
			BNode node(&entry);
			BNodeInfo nodeInfo(&node);
			
			char type[256];
			nodeInfo.GetType(type);
			BString t;
			t << type;
			
			if (t == "text/x-source-code")
				fList->AddItem(new TListItem(&entry, true),
								fList->IndexOf(seperator));
			else
				fList->AddItem(new TListItem(&entry, false));
		}
	}
			
	UpdateList(NULL,false);
}


void
listview::UpdateList(char *buffname, bool need_save)
{
//	(new BAlert("Niue", "Updating..", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();

	BString string, newstring;

	for(int i=0; i < fList->CountItems(); i++)
	{
		string = buffname;
		TListItem *item = dynamic_cast<TListItem *>(fList->ItemAt(i));
					
		if(item->is_source == true)
		{
			newstring = item->GetName();

			Window()->Lock();			
			if(string == newstring)
			{

				if(need_save)
				{
//					(new BAlert("Niue", "Updating.., need save", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
					item->save = false;
//					item->fIcon= NULL;
					item->DrawItem(this->fList,BRect(0,(i*42),item->Width(), item->Height()+2), true);
//					item->Update(this->fList, be_plain_font);
				}
				else
				{
//					(new BAlert("Niue", "Updating.., no need save", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
					item->save = true;
//					item->fIcon= NULL;
					item->DrawItem(this->fList,BRect(0,(i*42),item->Width(), item->Height()+2), true);
//					item->Update(this->fList, be_plain_font);
				}
				fList->Invalidate();
			}
			
//				BuildList(startref);
				Window()->Unlock();
		}
		else
			item->save = true;
	}	
}


void
listview::AttachedToWindow()
{
	//make sure we get our own messages
	fList->SetTarget(this);

	//do scrollbar math
	int32 items = fList->CountItems();
	float listheight = items * 21 - 200;

	BScrollBar *pBar = WorkScroll->ScrollBar(B_VERTICAL);
	if (pBar)
		pBar->SetRange(0, listheight);
}

listview::~listview()
{
}


RocketView::RocketView(BRect rect)
	:	BListView(rect, "list_view", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL,
				B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS)
{
	
}


RocketView::~RocketView()
{
	int32 index = 0;
	TListItem *item;
	
	item = dynamic_cast<TListItem *>(ItemAt(index++));
	while (item != NULL) 
	{
		delete item;
		item = dynamic_cast<TListItem *>(ItemAt(index++));
	}
}


void
RocketView::AttachedToWindow()
{
	SetInvocationMessage(new BMessage(ITEM_MENU_INVOKED_MSG));
	SetSelectionMessage(new BMessage(ITEM_MENU_SELECTED_MSG));
	SetViewColor(B_TRANSPARENT_32_BIT);
	
	//create left-click popup menu
	// build menu
//	BMenuItem* item;
	
	mnuLeft = new BPopUpMenu("context menu");
	mnuLeft->SetRadioMode(false);
	mnuLeft->AddItem(mnuMime = new BMenu("Mark as..."));
	mnuMime->AddItem(new BMenuItem("Sourcecode",
									new BMessage(MNU_LEFT_SET_SOURCE)));
	mnuMime->AddItem(new BMenuItem("Makefile",
									new BMessage(MNU_LEFT_SET_MAKEFILE)));
	mnuMime->AddItem(new BMenuItem("Resource",
									new BMessage(MNU_LEFT_SET_RESOURCE)));
	mnuMime->AddSeparatorItem();
	mnuMime->AddItem(new BMenuItem("Text", new BMessage(MNU_LEFT_SET_TEXT)));
	mnuMime->AddItem(new BMenuItem("Image", new BMessage(MNU_LEFT_SET_IMAGE)));
	mnuMime->AddItem(new BMenuItem("Audio", new BMessage(MNU_LEFT_SET_AUDIO)));
	mnuMime->AddItem(new BMenuItem("Video", new BMessage(MNU_LEFT_SET_VIDEO)));
	mnuMime->AddItem(new BMenuItem("Application",
									new BMessage(MNU_LEFT_SET_APP)));
	mnuMime->AddItem(new BMenuItem("Archive",
									new BMessage(MNU_LEFT_SET_ARCHIVE)));
	
//	item->SetEnabled(false);
	mnuLeft->AddSeparatorItem();
	mnuLeft->AddItem(new BMenuItem("Open with Tracker",
									new BMessage(MNU_LEFT_OPEN_MSG)));
	mnuLeft->AddItem(new BMenuItem("Get info",
									new BMessage(MNU_LEFT_INFO_MSG)));
	mnuLeft->AddItem(new BMenuItem("Edit name",
									new BMessage(MNU_LEFT_NAME_MSG)));
	mnuLeft->AddItem(new BMenuItem("Duplicate",
									new BMessage(MNU_LEFT_DUPLICATE_MSG)));
	mnuLeft->AddItem(new BMenuItem("Remove from project",
									new BMessage(MNU_LEFT_TRASH_MSG)));
	mnuLeft->AddSeparatorItem();
	mnuLeft->AddItem(new BMenuItem("Copy to…",
									new BMessage(MNU_LEFT_COPY_MSG)));
	mnuLeft->AddItem(new BMenuItem("Move to…",
									new BMessage(MNU_LEFT_MOVE_MSG)));
	mnuLeft->AddSeparatorItem();
	mnuLeft->AddItem(new BMenuItem("View in tracker",
									new BMessage(MNU_LEFT_TRACKER_MSG)));
	
	
	// set window as target so we get the message
	mnuLeft->SetTargetForItems(this);
	mnuMime->SetTargetForItems(this);
}


void
RocketView::MouseDown(BPoint where)
{
	// focus the list for scrolling
	MakeFocus(true);	
	
	uint32 buttons;
	
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
				
				//see if we need to enable some features
				//mark as source
				int32		index;
				entry_ref	*ref = NULL;
				TListItem	*item;
				
				//finds selected item	
				index = CurrentSelection();
				item = dynamic_cast<TListItem *>(ItemAt(index));
				if (item)
					ref = item->Ref();
  
				BNode node(ref);
				BNodeInfo nodeInfo(&node);
				char type[256];
				nodeInfo.GetType(type);
				BString t;
				t << type;
/*				
				if (t == "text/x-source-code")
					mnuLeft->ItemAt(0)->SetEnabled(false);
				else
					mnuLeft->ItemAt(0)->SetEnabled(true);
*/				
				mnuLeft->Go(point, true,true,(BRect(point.x,point.y,point.x + 1,point.y + 1)),true);
				return;
				
			}
			else if (buttons & B_PRIMARY_MOUSE_BUTTON) 
			{
				if (CurrentSelection() == item)
					Invoke();
				else
					Select(item);
			}
			 
		}
	}
}


void
RocketView::Draw(BRect where)
{
	// intersection of the view and update rect
	BRect	r = Bounds() & where;

	// if rect is valid, fill rect with our background color
	if (r.IsValid())
	{
		SetHighColor(white);
		FillRect(r);
	}
	
	// draw all items
	BListView::Draw(where);
}


TListItem::TListItem(BEntry *entry, bool source)
	:	BListItem()
{
	BNode		node;
	BNodeInfo	node_info;
	is_source = source;

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
		SetHeight(fIcon->Bounds().Height() + kITEM_MARGIN);
		// cache ref
		entry->GetRef(&fRef);
		//save = true;
	}
	else 
	{
		// no icon
		fIcon = NULL; 
		// make dummy for seperators
		strcpy(fName, " "); 
		SetHeight(kDEFAULT_ITEM_HEIGHT);
		save = true;
	}
	
	super = false;
}


TListItem::TListItem(const char* name)
	:	BListItem()
{
	is_source  = false;
	//no icon
	fIcon = NULL; 
	// set name
	strcpy(fName, name); 
	//and looks
	super = true;
	save = true;
	SetHeight(kDEFAULT_ITEM_HEIGHT);
}


TListItem::~TListItem()
{
	delete fIcon;
}


void
TListItem::DrawItem(BView *view, BRect rect, bool)
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
	view->FillRect(rect);

	// if we have an icon, draw it
	if (fIcon) 
	{
		view->SetDrawingMode(B_OP_OVER);		//B_OP_OVER
		view->DrawBitmap(fIcon, BPoint(rect.left + 2, rect.top + 3)); //
		view->SetDrawingMode(B_OP_COPY);		//B_OP_COPY	
		offset = fIcon->Bounds().Width() + 10;
	}

	// set text color
	if(IsEnabled())
		view->SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
	else
		view->SetHighColor(grey);
					 
	if(!save)
		view->SetHighColor(blue);
	
	if(super)
	{
		// set up super font
//		font.SetSize(10);
		font.SetFace(B_BOLD_FACE);
		font.GetHeight(&finfo);
		view->SetFont(&font);
	}
	else
	{
		// set up normal font
//		font.SetSize(10);
		font.GetHeight(&finfo);
		view->SetFont(&font);
	}
	
	// position pen
	view->MovePenTo(offset, rect.top + ((rect.Height() - 
										(finfo.ascent + finfo.descent +
										finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2);
	// and draw label
	view->DrawString(fName);
}


void 
TListItem::Update(BView *owner, const BFont *finfo)
{
	// we need to override the update method so we can make sure are
	// list item size doesn't change
	BListItem::Update(owner, finfo);
	if ((fIcon) && (Height() < fIcon->Bounds().Height() + kITEM_MARGIN))
		SetHeight(fIcon->Bounds().Height() + kITEM_MARGIN);
	else
		SetHeight(kDEFAULT_ITEM_HEIGHT);
}

