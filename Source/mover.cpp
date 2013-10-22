
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



#include "mover.h"
#include <Application.h>
#include <stdio.h>	// à virer
#include "colors.h"

static const char verticalMouse[] =
{ 16,1,7,7,
  0x0,0x0,0x01,0x80,0x01,0x80,0x01,0x80,
  0x01,0x80,0x11,0x88,0x31,0x8C,0x7D,0xBE,
  0x7D,0xBE,0x31,0x8C,0x11,0x88,0x01,0x80,
  0x01,0x80,0x01,0x80,0x01,0x80,0x0,0x0,

  0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,
  0x3B,0xDC,0x7B,0xDE,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0x7B,0xDE,0x3B,0xDC,
  0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0 };

static const char horizontalMouse[] =
{ 16,1,7,7,
  0x00,0x00,0x01,0x80,0x03,0xC0,0x07,0xE0,
  0x01,0x80,0x01,0x80,0x00,0x00,0x7F,0xFE,
  0x7F,0xFE,0x00,0x00,0x01,0x80,0x01,0x80,
  0x07,0xE0,0x03,0xC0,0x01,0X80,0x00,0x00,
 
  0x03,0xC0,0x07,0xE0,0x0F,0xF0,0x0F,0xF0,
  0x0F,0xF0,0x02,0xC0,0xFF,0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF,0xFF,0x03,0xC0,0x0F,0xF0,
  0x0F,0xF0,0x0F,0xF0,0x07,0xE0,0x03,0xC0 };
   
Mover::Mover(BRect r,uint32 rm,bool v)
	: BView(r,NULL,rm,B_WILL_DRAW)
{
	vertical = v;
	moving = false;
}

Mover::~Mover()
{
}

void Mover::Draw(BRect rect)
{
	BRect r = Bounds();
	
	SetHighColor(232,232,232);  //fill 232,232,232
	if (vertical)
	{
		StrokeLine(r.RightTop()+BPoint(-4,0),r.RightBottom()+BPoint(-4,0));
		StrokeLine(r.RightTop()+BPoint(-3,0),r.RightBottom()+BPoint(-3,0));
		StrokeLine(r.RightTop()+BPoint(-2,0),r.RightBottom()+BPoint(-2,0));
		StrokeLine(r.RightTop()+BPoint(-1,0),r.RightBottom()+BPoint(-1,0));
	}
	else
	{
		StrokeLine(r.LeftBottom()+BPoint(0,-4),r.RightBottom()+BPoint(0,-4));	
		StrokeLine(r.LeftBottom()+BPoint(0,-3),r.RightBottom()+BPoint(0,-3));	
		StrokeLine(r.LeftBottom()+BPoint(0,-2),r.RightBottom()+BPoint(0,-2));
		StrokeLine(r.LeftBottom()+BPoint(0,-1),r.RightBottom()+BPoint(0,-1));
		StrokeLine(r.LeftTop()+BPoint(0,1),r.RightTop()+BPoint(0,1));
	}
	
	SetHighColor(255,255,255);  //highligth 255,255,255
	if (vertical)
		StrokeLine(r.LeftTop()+BPoint(1,0),r.LeftBottom()+BPoint(1,0));
	else
		StrokeLine(r.LeftTop()+BPoint(0,1),r.RightTop()+BPoint(0,1));
		
		
	SetHighColor(152,152,152);  //border
	if (vertical)
	{
		StrokeLine(r.LeftTop(),r.LeftBottom());

		StrokeLine(r.RightTop(),r.RightBottom());
	}
	else
	{
		StrokeLine(r.LeftTop(),r.RightTop());
		StrokeLine(r.LeftBottom(),r.RightBottom());
		StrokeLine(r.LeftBottom()+BPoint(0,-6),r.RightBottom()+BPoint(0,-6));
		StrokeLine(r.LeftTop(),r.LeftBottom());
	}
}

void Mover::MouseUp(BPoint p)
{
	moving = false;
}

void Mover::MouseDown(BPoint p)
{
	clickPoint = p;
	moving = true;
	
	// calcul des limites de déplacement du curseur
	limits = Window()->ConvertToScreen(Window()->Bounds());
	limits.OffsetBy(-clickPoint.x,-clickPoint.y);
	//limits.InsetBy(75,50);
	limits.top += 22;  //19
	limits.left += 2;	//50
	limits.right -= B_V_SCROLL_BAR_WIDTH+2;	//50
	SetMouseEventMask(B_POINTER_EVENTS,B_NO_POINTER_HISTORY);
}

void Mover::MouseMoved(BPoint p,uint32 transit,const BMessage *bla)
{	
	if (moving)
	{	
		uint32 dummy;
		GetMouse(&p,&dummy,false);
		p = ConvertToScreen(p);
		BMessage moved(MOVER_MESSAGE);
		moved.AddPointer(MM_VIEW,this);
		
		float coord;
		if (vertical)
			coord = p.x-clickPoint.x;
		else
			coord = p.y-clickPoint.y;
		moved.AddFloat(MM_COORD,coord);
				
		// est-on dans les limites?
		bool l_InLimits = true;
		if (!vertical)
		{
			if ( (coord < limits.top) || (coord > limits.bottom))
				l_InLimits = false;
		}
		else
		{
			if ( (coord < limits.left) || (coord > limits.right))
				l_InLimits = false;
		}
		
		if (l_InLimits)		
			Window()->PostMessage(&moved);
	}
	else
		switch(transit)
		{
			case B_ENTERED_VIEW:
				if(vertical)
					be_app->SetCursor(verticalMouse);
				else
					be_app->SetCursor(horizontalMouse);
			break;
			
			case B_EXITED_VIEW:	
			case B_OUTSIDE_VIEW:
				be_app->SetCursor(B_HAND_CURSOR);
			break;
			
			default:
			break;
		}
}
