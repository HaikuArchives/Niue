
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

#include "vpropsview.h"

vpropsview::vpropsview(BRect rect, char *name, uint32 resizingMode, uint32 flags)
	   	   : BView(rect, name, resizingMode, flags)
{
	//define objects
	strName = new BStringView(BRect(5,10,rect.Width()-5,20), "strName", "Name:", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	txtName = new BTextControl(BRect(5,25,rect.Width()-5,45), "txtName", NULL, "", new BMessage(V_ACTION_CHANGE_NAME), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strLabel = new BStringView(BRect(5,55,rect.Width()-5,70), "strLabel", "Label:", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	txtLabel = new BTextControl(BRect(5,75,rect.Width()-5,95), "txtLabel", NULL, "", new BMessage(V_ACTION_CHANGE_LABEL), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	strBounds = new BStringView(BRect(5,105,rect.Width()-5,120), "strBounds", "Bounds:", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	txtLeft = new BTextControl(BRect(6,160,40,180), "txtLeft", NULL, "", new BMessage(V_ACTION_CHANGE_BOUNDS), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtTop = new BTextControl(BRect(rect.Width()/2-17,130,rect.Width()/2+17,150), "txtTop", NULL, "", new BMessage(V_ACTION_CHANGE_BOUNDS), B_FOLLOW_H_CENTER | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtRight = new BTextControl(BRect(rect.Width()-40,160,rect.Width()-6,180), "txtRight", NULL, "", new BMessage(V_ACTION_CHANGE_BOUNDS), B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtBottom = new BTextControl(BRect(rect.Width()/2-17,190,rect.Width()/2+17,210), "txtBottom", NULL, "", new BMessage(V_ACTION_CHANGE_BOUNDS), B_FOLLOW_H_CENTER | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);

	strRes = new BStringView(BRect(5,230,rect.Width()-5,245), "strRes", "Resizing mode:", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	vwResLeft = new BView(BRect(5,250,rect.Width()/2-5,320), "vwResLeft", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	vwResRight = new BView(BRect(rect.Width()/2+5,250,rect.Width()-5, 320), "vwResRight", B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkXLeft = new BRadioButton(BRect(0,0,100,15), "chkXLeft", "Left", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkXRight = new BRadioButton(BRect(0,20,100,35), "chkXRight", "Right", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkXBoth = new BRadioButton(BRect(0,40,100,55), "chkXBoth", "Both", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkYTop = new BRadioButton(BRect(0,0,100,15), "chkYTop", "Top", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkYBottom = new BRadioButton(BRect(0,20,100,35), "chkYBottom", "Bottom", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	chkYBoth = new BRadioButton(BRect(0,40,100,55), "chkYBoth", "Both", new BMessage(V_ACTION_CHANGE_RES), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);

	mvrSplit = new Mover(BRect(-1,rect.Height()-64,rect.Width()-1,rect.Height()-60), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, false);
	btnCreateCode = new BButton(BRect(5,rect.Height()-55, rect.Width()-5, rect.Height()-35), "btnCreateCode", "Generate Code", new BMessage(V_ACTION_CREATE_CODE), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 
		B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE);
	btnStartProject = new BButton(BRect(5,rect.Height()-30, rect.Width()-5, rect.Height()-10), "btnStartProject", "Start Project", new BMessage(V_ACTION_START_PROJECT), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 
		B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE);

	// set objects
	txtName->SetEnabled(false);
	txtLabel->SetEnabled(false);
	txtTop->SetEnabled(false);
	txtRight->SetEnabled(false);
	txtBottom->SetEnabled(false);
	txtLeft->SetEnabled(false);
	chkXLeft->SetEnabled(false);
	chkXRight->SetEnabled(false);
	chkXBoth->SetEnabled(false);
	chkYTop->SetEnabled(false);
	chkYBottom->SetEnabled(false);
	chkYBoth->SetEnabled(false);
	chkXLeft->SetValue(1);
	chkYTop->SetValue(1);
	txtLeft->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);
	txtTop->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);	
	txtRight->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);	
	txtBottom->SetAlignment(B_ALIGN_LEFT, B_ALIGN_CENTER);
	vwResLeft->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	vwResRight->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));	
	//add objects
	AddChild(strName);	
	AddChild(txtName);
	AddChild(strLabel);
	AddChild(txtLabel);
	AddChild(strBounds);
	AddChild(txtLeft);
	AddChild(txtTop);
	AddChild(txtRight);
	AddChild(txtBottom);
	AddChild(strRes);
	AddChild(vwResLeft);
	AddChild(vwResRight);
	
	vwResLeft->AddChild(chkXLeft);
	vwResLeft->AddChild(chkXRight);
	vwResLeft->AddChild(chkXBoth);
	vwResRight->AddChild(chkYTop);
	vwResRight->AddChild(chkYBottom);
	vwResRight->AddChild(chkYBoth);

	AddChild(mvrSplit);
	AddChild(btnCreateCode);
	AddChild(btnStartProject);


	 
}

void vpropsview::Draw(BRect)
{
	BRect rect = Bounds();
	BRect drect(25, 140, rect.Width()-25, 200);

	SetHighColor(black);
	StrokeRect(drect);

	drect.InsetBy(1,1);

	SetHighColor(blueish);
	FillRect(drect);

}
