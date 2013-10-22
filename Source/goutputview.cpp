/*
	goutputview.cpp
*/
#include "goutputview.h"

goutputview::goutputview(BRect rect, char *name)	   	   
	: BView(rect, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{

	txtOutput = new BTextView(BRect(0,0,rect.Width()-B_V_SCROLL_BAR_WIDTH,rect.Height()-B_H_SCROLL_BAR_HEIGHT), "txtOutput", BRect(5,10,1000,1000),  B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	scrOutput = new BScrollView("scrOutput", txtOutput, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, true, true, B_NO_BORDER);
	strStatus = new BStringView(BRect(4,rect.Height()-B_H_SCROLL_BAR_HEIGHT+1,120,rect.Height()), "strStatus", "", B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	
	txtOutput->MakeEditable(false);
	txtOutput->SetViewColor(bright);
	scrOutput->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	scrOutput->ScrollBar(B_HORIZONTAL)->ResizeBy(-125,0);
	scrOutput->ScrollBar(B_HORIZONTAL)->MoveBy(125,0);

	AddChild(scrOutput);
	scrOutput->AddChild(strStatus);
}

