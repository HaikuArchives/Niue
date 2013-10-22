
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

#include "vdesignview.h"

vdesignview::vdesignview(BRect rect, char *name, uint32 resizingMode, uint32 flags)
	   	   : BView(rect, name, resizingMode, flags)
{
	//define objects
	vwMain = new BView(BRect(0,0,rect.Width()-B_V_SCROLL_BAR_WIDTH,rect.Height()-B_H_SCROLL_BAR_HEIGHT),  "vwMain", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	scrDes = new BScrollView("scrDes", vwMain, 0, B_FOLLOW_ALL_SIDES, true, true, B_NO_BORDER);
	vwDes = new designview(BRect(0,0,470,580),  "vwDes", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	strStatus = new BStringView(BRect(4,rect.Height()-B_H_SCROLL_BAR_HEIGHT+1,120,rect.Height()), "strStatus", "", B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW);

	// set objects
	vwMain->SetViewColor(bright);
	vwDes->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	scrDes->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	scrDes->ScrollBar(B_HORIZONTAL)->ResizeBy(-125,0);
	scrDes->ScrollBar(B_HORIZONTAL)->MoveBy(125,0);

	//add objects
	AddChild(scrDes);
	vwMain->AddChild(vwDes);
	scrDes->AddChild(strStatus);

	//hack to resize our scrollers
	FrameResized(Bounds().right, Bounds().bottom);
		
}

void vdesignview::FrameResized(float width, float height)
{

	CenterTarget();
		
}

void vdesignview::CenterTarget()
{

	//scroll math & center view
	float width = Frame().Width();
	float height = Frame().Height();

	float x=vwDes->Bounds().Width()+B_V_SCROLL_BAR_WIDTH;
	float y=vwDes->Bounds().Height()+B_H_SCROLL_BAR_HEIGHT;

	if(width < x)
	{
		scrDes->ScrollBar(B_HORIZONTAL)->SetRange(0, x-width);
		vwDes->MoveTo(0, vwDes->Frame().top);
	}
	else
	{
		scrDes->ScrollBar(B_HORIZONTAL)->SetRange(0,0);
		vwDes->MoveTo((width - x)/2, vwDes->Frame().top);
	}

	if(height < y)
	{
		scrDes->ScrollBar(B_VERTICAL)->SetRange(0, y-height);
		vwDes->MoveTo(vwDes->Frame().left, 0);
	}
	else
	{
		scrDes->ScrollBar(B_VERTICAL)->SetRange(0,0);
		vwDes->MoveTo(vwDes->Frame().left, (height - y)/2);		
	}

		

}

designview::designview(BRect rect, char *name, uint32 resizingMode, uint32 flags)
			: BView(rect, name, resizingMode,flags)
{
	//settings
	IsDragging = false;
	ShowGrid = true;
	ShowHandles = true;
	scale = 10;
	currenttype="BUTTON";
	
	//reset
	ResetCounters();
}

designview::~designview()
{
}

placeholder::placeholder(BRect prect, const char *name, const char *plabel, const char *ptype, int32 pid, uint32 resizingMode, uint32 flags)
			: BView(prect, name, resizingMode, flags)
{
	label = plabel;
	type = ptype;
	id = pid;
	resX = "B_FOLLOW_LEFT";
	resY = "B_FOLLOW_TOP";

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	moving = false;
	active = false;
}

void designview::Draw(BRect)
{
	//grid
	if(ShowGrid == true)
	{
		SetHighColor(black);
		

		int width = (int)Frame().Width()-5;
		int height = (int)Frame().Height()-5;
		int x; // = scale;
		int y; // = scale;
			
	
		for(x=scale; x < width; x+=scale)
		{
			for(y=scale; y < height; y+=scale)
			{
					
				StrokeRect(BRect(x,y,x,y));		
			}
			y=scale;
			
		}
	}	
}


void designview::AddElement(BPoint where, char *controltype)
{
//		IsDragging = false;


		if(strcmp(controltype, "BOX")==0)
		{
			BBox* box = new BBox(BRect(0,0,200,100), "box", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			box->SetLabel("Box Title");
			boxes += 1;
			BString name;
			name << "boxBox" << boxes;
			BView* phbox = new placeholder(BRect(where.x,where.y,where.x+206,where.y+101), name.String(), "Box Title", "BBox", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_FRAME_EVENTS
				| B_FULL_UPDATE_ON_RESIZE);
			AddChild(phbox);
			phbox->AddChild(box);
		}
		else if(strcmp(controltype, "BUTTON")==0)
		{
			BButton* button = new BButton(BRect(0,0,60,20), "button", "Button", new BMessage(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			buttons += 1;
			BString name;
			name << "btnButton" << buttons;
			BView* phbutton = new placeholder(BRect(where.x,where.y,where.x+66,where.y+21), name.String(), "Button", "BButton", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phbutton);
			phbutton->AddChild(button);
		}
		else if(strcmp(controltype, "PICTUREBUTTON")==0)
		{
			BButton* picturebutton = new BButton(BRect(0,0,80,20), "picturebutton", "Picture Button", new BMessage(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			picturebuttons += 1;
			BString name;
			name << "pbnPictureButton" << picturebuttons;
			BView* phpicturebutton = new placeholder(BRect(where.x,where.y,where.x+86,where.y+21), name.String(), "", "BPictureButton", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phpicturebutton);
			phpicturebutton->AddChild(picturebutton);
		}
		else if(strcmp(controltype, "RADIOBUTTON")==0)
		{
			BRadioButton* radiobutton = new BRadioButton(BRect(0,0,100,16), "radiobutton", "Radio Button", new BMessage(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			radiobuttons += 1;
			BString name;
			name << "radRadioButton" << radiobuttons;
			BView* phradiobutton = new placeholder(BRect(where.x,where.y,where.x+106,where.y+17), name.String(), "Radio Button", "BRadioButton", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phradiobutton);
			phradiobutton->AddChild(radiobutton);
		}
		else if(strcmp(controltype, "CHECKBOX")==0)
		{
			BCheckBox* checkbox = new BCheckBox(BRect(0,0,100,16), "checkbox", "Check Box", new BMessage(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			checkboxes += 1;
			BString name;
			name << "chkCheckBox" << checkboxes;
			BView* phcheckbox = new placeholder(BRect(where.x,where.y,where.x+106,where.y+17), name.String(), "Check Box", "BCheckBox", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phcheckbox);
			phcheckbox->AddChild(checkbox);
		}
		else if(strcmp(controltype, "STRINGVIEW")==0)
		{
			BStringView* stringview = new BStringView(BRect(0,0,80,16), "stringview", "String View", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			stringviews += 1;
			BString name;
			name << "strStringView" << stringviews;
			BView* phstringview = new placeholder(BRect(where.x,where.y,where.x+86,where.y+17), name.String(), "String View", "BStringView", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phstringview);
			phstringview->AddChild(stringview);
		}
		else if(strcmp(controltype, "TEXTCONTROL")==0)
		{
			BTextControl* textcontrol = new BTextControl(BRect(0,0,200,16), "textcontrol", "Text Control", "", new BMessage(), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			textcontrols += 1;
			BString name;
			name << "txtTextControl" << textcontrols;
			BView* phtextcontrol = new placeholder(BRect(where.x,where.y,where.x+206,where.y+17), name.String(), "Text Control", "BTextControl", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phtextcontrol);
			phtextcontrol->AddChild(textcontrol);
		}
		else if(strcmp(controltype, "TEXTVIEW")==0)
		{
			BTextView* textview = new BTextView(BRect(0,0,250,150), "textview", BRect(2,2,248,148), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			textview->SetText("Text View");
			textviews += 1;
			BString name;
			name << "txtTextView" << textviews;
			BView* phtextview = new placeholder(BRect(where.x,where.y,where.x+256,where.y+151), name.String(), "", "BTextView", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phtextview);
			phtextview->AddChild(textview);
		}
		else if(strcmp(controltype, "LISTVIEW")==0)
		{
			BListView* listview = new BListView(BRect(1,1,150-B_V_SCROLL_BAR_WIDTH,250), "listview", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);
			BScrollView* scrollview = new BScrollView("scrollview", listview, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, false, true, B_PLAIN_BORDER);  
			listviews += 1;
			BString name;
			name << "lstListView" << listviews;
			BView* phlistview = new placeholder(BRect(where.x,where.y,where.x+157,where.y+251), name.String(), "", "BListView", CountChildren()+1, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
			AddChild(phlistview);
			phlistview->AddChild(scrollview);
		}
		
	
		Window()->PostMessage(V_ACTION_BUILD_LIST);		
}

void designview::MouseMoved(BPoint point, uint32 transit, const BMessage *message)
{
		BString string;

		//prepare string
		if(Parent()->Bounds().Contains(point))
		{
			string << (int)point.x << "," << (int)point.y;
		}
		else
		{
			string << "";
		}
		//prepare massege
		BMessage msg(V_ACTION_SHOW_LOCATION);
		msg.AddString("loca", string);
		//send message
		Window()->PostMessage(&msg);
		
}

void designview::MouseDown(BPoint where)
{
		AddElement(where, currenttype);
}

void designview::ResetCounters()
{
	boxes = buttons = picturebuttons = radiobuttons = checkboxes = stringviews = textcontrols = textviews = listviews = 0;
}

void placeholder::Draw(BRect)
{
	if(  ((designview *)Parent())->ShowHandles == true)
	{
		rgb_color handlecolor;
		if(active)
		{
			handlecolor = darkgreen;
		}
		else
		{
			handlecolor = red;
		}
		
		//box1
		BRect drect(Bounds().right-4,Bounds().bottom-4,Bounds().right,Bounds().bottom);
		SetHighColor(handlecolor);
		StrokeRect(drect);
		SetHighColor(white);
		FillRect(BRect(drect.left+1,drect.top+1,drect.right-1,drect.bottom-1));
		SetHighColor(lightgrey);
		FillRect(BRect(drect.left+3,drect.top+3,drect.right-1,drect.bottom-1));
	
		//box2
		drect.OffsetBy(0,-6);
		SetHighColor(handlecolor);
		StrokeRect(drect);
		SetHighColor(white);
		FillRect(BRect(drect.left+1,drect.top+1,drect.right-1,drect.bottom-1));
		SetHighColor(lightgrey);
		FillRect(BRect(drect.left+2,drect.top+2,drect.right-2,drect.bottom-2));

	}

}

void placeholder::MouseDown(BPoint down)
{
	moving=true;
	clickPoint = down;

	float border = Bounds().bottom-5; //border is an imaginary line between the two handles
	if(down.y < border)
	{
		resize = false;
	}
	else
	{
		resize = true;
	}
	SetMouseEventMask(B_POINTER_EVENTS,B_NO_POINTER_HISTORY);
	initialSize = Bounds();  											//get element size
	
}

void placeholder::MouseUp(BPoint up)
{
	moving=false;
	Invalidate();		//redraw the handles
}

void placeholder::MouseMoved(BPoint move, uint32 transit, const BMessage *message)
{
	if(moving)  //are we draggin'?
	{
		if(resize)  //resizing:
		{
			ResizeTo(initialSize.Width()+(move.x-clickPoint.x), initialSize.Height()+(move.y-clickPoint.y));
		}
		else  //moving:
		{
			MoveBy(move.x-initialSize.Width(), move.y-(initialSize.Height()-5));
		}

	}

}

