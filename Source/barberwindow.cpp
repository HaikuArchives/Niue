
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



#include "barberwindow.h"

BRect rect(0,0,0,50);

BarberWindow::BarberWindow(const char *infoText) :
	BWindow(rect, "YourApp", B_MODAL_WINDOW,B_NOT_RESIZABLE)
{
	//resize window to text, and update rect
	int size = strlen(infoText);
	ResizeTo(size*6,rect.Height());
	rect=Bounds();
	
	CenterWindow();
	SetPulseRate(100000); //for the barberpole
	
	// define objects
	barber = new BarberPole(BRect(5,5,rect.Width()-5,rect.Height()-25), "barber",
							B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, 0, FROM_LEFT_TO_RIGHT);
	AddChild(barber);
	InfoString = new BStringView(BRect(5,rect.Height()-20,rect.Width()-5,rect.Height()-5),
							"InfoString", infoText, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	AddChild(InfoString);
	
	// set objects
	rgb_color white = {255,255,255,0};
	rgb_color green = {0,186,57,0};
	barber->SetHighColor(green);
	barber->SetLowColor(white);
}


bool
BarberWindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}


void
BarberWindow::CenterWindow()
{
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes = mScreen.Frame();
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2),
			(screenRes.bottom / 2) - (winBounds.bottom / 2));
}


BarberPole::BarberPole(BRect pRect, const char *pName, uint32 resizingMode, uint32 flags,
						int pDirection)
	:	BView(pRect, pName, resizingMode, flags | B_WILL_DRAW | B_PULSE_NEEDED)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	fStripes.data[0] = 0x0f;
	fStripes.data[1] = 0x1e;
	fStripes.data[2] = 0x3c;
	fStripes.data[3] = 0x78;
	fStripes.data[4] = 0xf0;
	fStripes.data[5] = 0xe1;
	fStripes.data[6] = 0xc3;
	fStripes.data[7] = 0x87;	
	fDirection = pDirection;
}


void
BarberPole::SetColors(rgb_color* colors, rgb_color c) 
{
	int alpha = 255/BB_NUM_SHADES;
	for (int i = 0; i < BB_NUM_SHADES; i ++)
	{
		colors[i].red = c.red * alpha / 255;
		colors[i].green = c.green * alpha / 255;
		colors[i].blue = c.blue * alpha / 255;
		colors[i].alpha = 255;
		alpha += 255/BB_NUM_SHADES;
	}
}


void
BarberPole::SetLowColor(rgb_color c) 
{
	SetColors(fHighColors, c);
}


void
BarberPole::SetHighColor(rgb_color c) 
{
	SetColors(fLowColors, c);
}


void
BarberPole::Draw(BRect r) 
{
	BRect b(Bounds());
	BRect b1;
	for (int i = 0; i < BB_NUM_SHADES; i ++)
	{
		BRegion r(b);
		if (i != (BB_NUM_SHADES-1))
		{
			b1 = b.InsetByCopy(1, 1);
			r.Exclude(b1);
		}
		ConstrainClippingRegion(&r);
		BView::SetHighColor(fHighColors[i]); BView::SetLowColor(fLowColors[i]);
		FillRect(b, fStripes);
		b.InsetBy(1, 1);
	}
}


void
BarberPole::Pulse()
{
	if(fDirection == FROM_RIGHT_TO_LEFT)
	{
		uchar tmp = fStripes.data[0];
		for (int j = 0; j < 7; ++j)
  			fStripes.data[j] = fStripes.data[j+1];
		fStripes.data[7] = tmp;
	} else
	{
		uchar tmp = fStripes.data[7];
		for (int j = 7; j > 0; --j)
  			fStripes.data[j] = fStripes.data[j-1];
		fStripes.data[0] = tmp;
	}
	Invalidate();
}
