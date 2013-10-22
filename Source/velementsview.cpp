
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

#include "velementsview.h"


velementsview::velementsview(BRect rect, char *name, uint32 resizingMode, uint32 flags)
	   	   : BView(rect, name, resizingMode, flags)
{
	y = 0;
	//define objects
	
	//elements
//	radBox = new BRadioButton(BRect(5,y+13,20,20), "radBox", NULL, new BMessage(V_ADD_BOX_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
//	BButton* btnBox = new BButton(BRect(25,y+=10,rect.Width()-19,y+=20), "btnBox", "BBox", new BMessage(V_ADD_BOX_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radButton = new BRadioButton(BRect(5,y+8,20,20), "radButton", NULL, new BMessage(V_ADD_BUTTON_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnButton", "Button", new BMessage(V_ADD_BUTTON_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radPictureButton = new BRadioButton(BRect(5,y+8,20,20), "radPictureButton", NULL, new BMessage(V_ADD_PICTUREBUTTON_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnPicButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnPicButton", "Picture Button", new BMessage(V_ADD_PICTUREBUTTON_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radRadioButton = new BRadioButton(BRect(5,y+8,20,20), "radRadioButton", NULL, new BMessage(V_ADD_RADIOBUTTON_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnRadioButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnRadioButton", "Radio Button", new BMessage(V_ADD_RADIOBUTTON_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radCheckBox = new BRadioButton(BRect(5,y+8,20,20), "radCheckBox", NULL, new BMessage(V_ADD_CHECKBOX_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnCheckButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnCheckBox", "Check Box", new BMessage(V_ADD_CHECKBOX_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radStringView = new BRadioButton(BRect(5,y+8,20,20), "radStringView", NULL, new BMessage(V_ADD_STRINGVIEW_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnStringButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnStringButton", "String View", new BMessage(V_ADD_STRINGVIEW_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radTextControl = new BRadioButton(BRect(5,y+8,20,20), "radTextControl", NULL, new BMessage(V_ADD_TEXTCONTROL_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnTextContButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnTextContButton", "Text Control", new BMessage(V_ADD_TEXTCONTROL_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
	 B_WILL_DRAW);
	radTextView = new BRadioButton(BRect(5,y+8,20,20), "radTextView", NULL, new BMessage(V_ADD_LISTVIEW_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnTextViewButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnTextViewButton", "Text View", new BMessage(V_ADD_TEXTVIEW_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	radListView = new BRadioButton(BRect(5,y+8,20,20), "radListView", NULL, new BMessage(V_ADD_LISTVIEW_MSG), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	BButton* btnListViewButton = new BButton(BRect(25,y+=5,rect.Width()-19,y+=20), "btnListViewButton", "List View", new BMessage(V_ADD_LISTVIEW_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);



	//options
	y = (int)rect.Height()/2;
	mvrSplit = new Mover(BRect(-1,y,rect.Width(),y+=4), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, false);
	chkGrid = new BCheckBox(BRect(10,y+=15,rect.Width()-5, y+=15), "chkGrid", "Show Grid", new BMessage(V_OPTION_SHOW_GRID),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	chkHandles = new BCheckBox(BRect(10,y+=5,rect.Width()-5, y+=15), "chkHandles", "Show Handles", new BMessage(V_OPTION_SHOW_HANDLES),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	boxCanvas = new BBox(BRect(5,y+=20,rect.Width()-5,y+=60), "boxCanvas", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW, B_FANCY_BORDER);
	txtCanvasWidth = new BTextControl(BRect(5,15,rect.Width()-15,30), "txtCanvasWidth", "Width:", "470", new BMessage(V_OPTION_SET_WIDTH),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtCanvasHeight = new BTextControl(BRect(5,35,rect.Width()-15,55), "txtCanvasheight", "Height:", "580", new BMessage(V_OPTION_SET_HEIGHT),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnAlignHorz = new BButton(BRect(5,y+=15,rect.Width()-5,y+=20), "btnAlignHorz", "Align Horizontally", new BMessage(V_OPTION_ALIGN_HORZ),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnAlignVert = new BButton(BRect(5,y+=5,rect.Width()-5,y+=20), "btnAlignVert", "Align Vertically", new BMessage(V_OPTION_ALIGN_VERT),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnAlignGrid = new BButton(BRect(5,y+=5,rect.Width()-5,y+=20), "btnAlignGrid", "Align To Grid", new BMessage(V_OPTION_ALIGN_GRID),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnKillSel = new BButton(BRect(5, y+=15,rect.Width()-5,y+=20), "btnKillSel", "Remove Selected", new BMessage(V_OPTION_KILL_SEL),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	btnKillAll = new BButton(BRect(5, y+=5,rect.Width()-5,y+=20), "btnKillAll", "Remove All", new BMessage(V_OPTION_KILL_ALL),
	 B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE);
	

	//view & scroller
	vwAvEls = new BView(BRect(0,0,rect.Width()-B_V_SCROLL_BAR_WIDTH, rect.Height()/2), "vwAvEls", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);
	scrAvEls = new BScrollView("scrAvEls", vwAvEls, B_FOLLOW_ALL, B_WILL_DRAW, false, true, B_NO_BORDER);
	scrAvEls->ScrollBar(B_VERTICAL)->SetRange(0,0);

	// set objects
	vwAvEls->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	chkGrid->SetValue(1);
	chkHandles->SetValue(1);
	radButton->SetValue(1);
	boxCanvas->SetLabel("Canvas size");

	//add objects
	AddChild(scrAvEls); 
//	vwAvEls->AddChild(radBox);
	vwAvEls->AddChild(radButton);
	vwAvEls->AddChild(radPictureButton);
	vwAvEls->AddChild(radRadioButton);
	vwAvEls->AddChild(radCheckBox);
	vwAvEls->AddChild(radStringView);
	vwAvEls->AddChild(radTextControl);
	vwAvEls->AddChild(radTextView);
	vwAvEls->AddChild(radListView);

//	vwAvEls->AddChild(btnBox);
	vwAvEls->AddChild(btnButton);
	vwAvEls->AddChild(btnPicButton);
	vwAvEls->AddChild(btnRadioButton);
	vwAvEls->AddChild(btnCheckButton);
	vwAvEls->AddChild(btnStringButton);
	vwAvEls->AddChild(btnTextContButton);
	vwAvEls->AddChild(btnTextViewButton);
	vwAvEls->AddChild(btnListViewButton);
	

	AddChild(mvrSplit);

	AddChild(chkGrid);
	AddChild(chkHandles);
	AddChild(boxCanvas);
	boxCanvas->AddChild(txtCanvasWidth);
	boxCanvas->AddChild(txtCanvasHeight);
	AddChild(btnAlignHorz);
	AddChild(btnAlignVert);
	AddChild(btnAlignGrid);
	AddChild(btnKillSel);
	AddChild(btnKillAll);
}

void velementsview::FrameResized(float width, float height)
{	
		//scroll math
		if(height < y+10)
		{
			scrAvEls->ScrollBar(B_VERTICAL)->SetRange(0,y+10- height);
		}
		else
		{
			scrAvEls->ScrollBar(B_VERTICAL)->SetRange(0,0);
		}
		
}

