/*
	docwindow.cpp
*/

#include "docwindow.h"

BRect drect(0,0,800,400);

docwindow::docwindow() :
	BWindow(drect, "Niue Documentation", B_DOCUMENT_WINDOW,B_WILL_DRAW)
{
	CenterWindow();
	
	SetSizeLimits(drect.Width(), 30000, drect.Height(), 30000);
	
	// define objects
	vwMain = new BView(BRect(0,0,drect.Width(),drect.Height()), "vwMain", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	txtText = new BTextView(BRect(150, 0, drect.Width()-B_V_SCROLL_BAR_WIDTH, drect.Height()-B_H_SCROLL_BAR_HEIGHT), "txtText", BRect(10,10,10000, 10000), 
		B_FOLLOW_ALL_SIDES | B_WILL_DRAW); 
	fList = new hRocketView(BRect(10,10,135,drect.Height()-10));
	TextScroll = new BScrollView("TextScroll", txtText, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, true, true, B_PLAIN_BORDER);
	WorkScroll = new BScrollView("WorkScroll", fList, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW, false, false, B_PLAIN_BORDER);
	// set objects
	vwMain->SetViewColor(doccolor);
	
	//set objects
	txtText->MakeEditable(false);
	

	// add objects
	AddChild(vwMain);
	vwMain->AddChild(WorkScroll);
	vwMain->AddChild(TextScroll);

	BuildList();

}


bool
docwindow::QuitRequested()
{
	return true;
}


void docwindow::CenterWindow()
{
	
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}

void docwindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case HELP_ITEM_MENU_SELECTED:
		{
			
				int32		index;
				entry_ref	ref;
				HListItem	*item;
				
				index = fList->CurrentSelection();
				item = dynamic_cast<HListItem *>(fList->ItemAt(index));
				if (item)
					{
							ref = item->Ref();
							BFile file(&ref, B_READ_ONLY);
							//get the text size
							off_t size;
							file.GetSize(&size);
							//and write the text
							try
							{
								txtText->SetText(&file, 0, size, NULL);
							}
							catch(int)
							{
								const char *error = "Couldn't read the contents of the documentation file!";
								txtText->Insert(error);
							}			
					}
		}
	}
}


void docwindow::BuildList()
{

	//first off, clear all items
	fList->MakeEmpty();
	
	BPath prefspath;
		
	//find prefsfile
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
	prefspath.Append("Niue/Docs");
	
	BEntry entry;
	BDirectory dir(prefspath.Path());
	
	//place files
	while (dir.GetNextEntry(&entry, true) == B_NO_ERROR)
	{
		if (entry.IsFile())
		{
					fList->AddItem(new HListItem(&entry));				
		}	
	}
	
	if(fList->CountItems() > 0)
	{
		fList->Select(0);
	}
	

}


hRocketView::hRocketView(BRect rect)
				:BListView(rect, "list_view", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS)
{
}


hRocketView::~hRocketView()
{
	int32		index = 0;
	HListItem	*item;

	while ((item = dynamic_cast<HListItem *>(ItemAt(index++))) != NULL) 
	{
		delete item;
	}
}

void hRocketView::MouseDown(BPoint where)
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
		if (buttons & B_PRIMARY_MOUSE_BUTTON) 
			{
					Select(item);
			}
			
		}
	}
}

void 
hRocketView::AttachedToWindow()
{
	SetSelectionMessage(new BMessage(HELP_ITEM_MENU_SELECTED));
	SetViewColor(B_TRANSPARENT_32_BIT);
	SetTarget(this);
	
}

/*
void 
docwindow::AttachedToWindow()
{
fList->SetTarget(this);

int32 items = fList->CountItems();
float listheight = items * 21 - 200;

BScrollBar *pBar = WorkScroll->ScrollBar(B_VERTICAL);
	if (pBar)
	{
	pBar->SetRange(0, listheight);
	}
}
*/

void 
hRocketView::Draw(BRect where)
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

HListItem::HListItem(BEntry *entry)
		  :BListItem()
{
	BNode		node;
	BNodeInfo	node_info;

	// try to get node info for this entry
	if ((node.SetTo(entry) == B_NO_ERROR) && (node_info.SetTo(&node) == B_NO_ERROR)) 
	{
		// cache name
		entry->GetName(fName);
		// cache ref
		entry->GetRef(&fRef);
	}
	else 
	{
		// make dummy for seperators
		strcpy(fName, " "); 
		SetHeight(hDEFAULT_ITEM_HEIGHT);
	}
}


HListItem::~HListItem()
{
}


void 
HListItem::DrawItem(BView *view, BRect rect, bool)
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
	view->FillRect(rect);   //was: FillRect(rect)

	//text color
	if (IsSelected()) 
	{
		view->SetHighColor(ui_color(B_MENU_SELECTED_ITEM_TEXT_COLOR));
	}
	else
	{
		view->SetHighColor(blue);
	}

	// set up font
	font.SetSize(10);
	font.GetHeight(&finfo);
	view->SetFont(&font);

	// position pen
	view->MovePenTo(offset,
		rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2);
	// and draw label
	view->DrawString(fName);
	//draw web-like underscore
	view->StrokeLine(BPoint(offset,rect.bottom-3), BPoint(offset+font.StringWidth(fName), rect.bottom-3));
	
	
	/*
	// set text color
	if(IsEnabled())
	{ 	
		view->SetHighColor(grey) :
	}
	else
	{
		view->SetHighColor(grey);
	}
	
	// set up font
	font.SetSize(10);
	font.GetHeight(&finfo);
	view->SetFont(&font);

	// position pen
	view->MovePenTo(offset,
		rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +
					(finfo.ascent + finfo.descent) - 2);
	// and draw label
	view->DrawString(fName);
	*/
}


void HListItem::Update(BView *owner, const BFont *finfo)
{
		SetHeight(hDEFAULT_ITEM_HEIGHT);
}
