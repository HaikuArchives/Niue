/*
	ssnippetview.cpp
*/

#include "ssnippetview.h"

ssnippetview::ssnippetview(BRect rect, char *name)
 : BView(rect, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{


	//construct elements
	strName = new BStringView(BRect(8,5,90,15), "strName", "Name:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	strCode = new BStringView(BRect(103,5,rect.Width()-5,15), "strCode", "Snippet Code:", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	lstSnippets = new snippetlist(BRect(5,20,595-B_V_SCROLL_BAR_WIDTH,260-B_H_SCROLL_BAR_HEIGHT), "lstSnippets", B_SINGLE_SELECTION_LIST,
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS);
	BScrollView* scrScrollView = new BScrollView("scrScrollView", lstSnippets,
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_FRAME_EVENTS, true, true, B_PLAIN_BORDER);
	btnRemove = new BButton(BRect(5,270,180,290), "btnRemove", "Remove Selected Snippet", new BMessage(S_ACTION_REMOVE_MSG), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnNew = new BButton(BRect(275,270,375,290), "btnNew", "New Snippet", new BMessage(S_ACTION_NEW_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnSave = new BButton(BRect(385,270,485,290), "btnSave", "Save Snippets", new BMessage(S_ACTION_SAVE_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnClose = new BButton(BRect(495,270,595,290), "btnClose", "Close", new BMessage(S_ACTION_CLOSE_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);

	//add elements
	AddChild(strName);
	AddChild(strCode);
	AddChild(scrScrollView);
	AddChild(btnRemove);
	AddChild(btnNew);
	AddChild(btnSave);
	AddChild(btnClose);

	BuildList();

}

snippetlist::snippetlist(BRect rect, const char *name, list_view_type type, uint32 resizingMode, uint32 flags)
 : BListView(rect, name, type, resizingMode, flags)
{

}

seditwindow::seditwindow(BRect rect, const char *text, bool name)
 : BWindow(rect, "", B_NO_BORDER_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_WILL_DRAW | B_NOT_RESIZABLE)
{
	BMessage msg(S_ACTION_CHANGE_MSG);
	msg.AddBool("name", name);	
	
	vwMain = new BView(BRect(0,0,rect.Width(), rect.Height()), "vwMain", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	txtText = new BTextControl(BRect(2,2,rect.Width()-2, rect.Height()-2), "txtText", NULL, text, new BMessage(msg), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);

	vwMain->SetViewColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));

	AddChild(vwMain);
	vwMain->AddChild(txtText);

	txtText->MakeFocus(true);

}

void seditwindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case S_ACTION_CHANGE_MSG:
		{	
			message->AddString("cont", txtText->Text());
			be_app->PostMessage(message);
			Close();
		}
		break;
	}
}

void ssnippetview::BuildList()
{

	lstSnippets->MakeEmpty();
	
	//find prefsfile
	BPath prefspath;
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
	prefspath.Append("Niue/niue_snippets");
				
	//open file for reading
	BFile rdsnippetsfile;
	rdsnippetsfile.SetTo(prefspath.Path(), B_READ_ONLY);
	if (rdsnippetsfile.IsReadable())
	{
		
		BMessage msg('SNIP');
		msg.Unflatten(&rdsnippetsfile);
		
		if(msg.CountNames(B_STRING_TYPE) > 0)
		{
			int i = 0;
			char		*snipname = 0;
			uint32		type;
			int32		count;
			BString		snipcode = "";
			BString		name = "";

			
			while(msg.GetInfo(B_STRING_TYPE, i, &snipname, &type, &count) == B_OK)
			{
				snipcode = "";
				if(msg.FindString(snipname, &snipcode) == B_OK)
				{
					name = snipname;
					lstSnippets->AddItem(new slistitem(name, snipcode));
				}
				i++;
			}			
		}			
	}
	
	rdsnippetsfile.Unset();
	
	if(lstSnippets->CountItems() > 0)
	{
		lstSnippets->Select(0, false);
	}

}

void ssnippetview::NewSnippet()
{
	BString name;
	name << "Snippet" << lstSnippets->CountItems()+1;

	lstSnippets->AddItem(new slistitem(name.String(), "code"));

	//select our newbie
	lstSnippets->Select(lstSnippets->CountItems()-1);
}

void snippetlist::MouseDown(BPoint where)
{
	int32 buttons;
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
				// select this item
				Select(item);
			}
			
			// if clicked with primary mouse button
			else if (buttons & B_PRIMARY_MOUSE_BUTTON) 
			{
				int32 index = CurrentSelection();

				if (index == item)
				{
					BRect itemrect = ItemFrame(index);
					BRect textrect = ConvertToScreen(itemrect);
					BString text = "";
					bool name;

					if(where.x < 100)  //edit name
					{
						textrect.right -= textrect.Width()-100;
						text << dynamic_cast<slistitem *>(ItemAt(index))->sname;
						name = true;
					}
					else	//edit code
					{
						textrect.left += 100;
						text << dynamic_cast<slistitem *>(ItemAt(index))->stext;
						name = false;
					}

					wdwEdit = new seditwindow(textrect, text.String(), name);
					wdwEdit->Show(); 
				}
				else
				{
					Select(item);
				}

			}
		}	
		
	}
}



slistitem::slistitem(BString name, BString text)
			: BListItem()
{

	SetHeight(20);
	sname = name;
	stext = text;

}

void slistitem::DrawItem(BView *view, BRect rect, bool)
{

	if (IsSelected()) 
	{
		// fill color
		view->SetHighColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
		// anti-alias color
		view->SetLowColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
	}
	else 
	{
		if(dynamic_cast<BListView*>(view)->IndexOf(this)%2 == 0)  //check for even/uneven for the background
		{
			view->SetHighColor(bright);
			view->SetLowColor(bright);
		}
		else
		{
			view->SetHighColor(white);
			view->SetLowColor(white);
				
		}

	}

	view->FillRect(rect);   //was: FillRect(rect)

	//text color
	if (IsSelected()) 
	{
		view->SetHighColor(ui_color(B_MENU_SELECTED_ITEM_TEXT_COLOR));
	}
	else
	{
		view->SetHighColor(black);
	}

	// set up font
	BFont		font = be_plain_font;
	font_height	finfo;
	font.SetSize(10);
	font.GetHeight(&finfo);
	view->SetFont(&font);


	// position pen
	view->MovePenTo(rect.left+5,rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +(finfo.ascent + finfo.descent) - 2);
	// and draw name
	view->DrawString(sname.String(), 20);

	// position pen
	view->MovePenTo(rect.left+100,rect.top + ((rect.Height() - (finfo.ascent + finfo.descent + finfo.leading)) / 2) +(finfo.ascent + finfo.descent) - 2);
	// and draw text
	view->DrawString(stext.String());

//	view->SetHighColor(lightgrey);
//	view->StrokeLine(BPoint(rect.left+95,rect.top), BPoint(rect.left+95,rect.bottom), B_MIXED_COLORS);
	

}



void slistitem::Update(BView *owner, const BFont *finfo)
{
		SetHeight(20);
}
