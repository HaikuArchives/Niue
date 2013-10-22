
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



#ifndef BARBER_WINDOW_H
#define BARBER_WINDOW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <Region.h>

const int FROM_RIGHT_TO_LEFT = 0;
const int FROM_LEFT_TO_RIGHT = 1;

#define BB_NUM_SHADES 2


class BarberPole;

class BarberWindow : public BWindow
{
	public:
						BarberWindow(const char *infoText);
		virtual void 	CenterWindow();
		virtual bool	QuitRequested();
	
	private:
	
			BarberPole*		barber;
			BStringView*	InfoString;

};

class BarberPole : public BView
{
public:
						BarberPole(BRect pRect, const char *pName, uint32 resize,
								uint32 flags,
								int pDirection = FROM_LEFT_TO_RIGHT);
			void		SetLowColor(rgb_color c);
			void		SetHighColor(rgb_color c);
			void		Draw(BRect rect);
			void		Pulse();
	
private:
	static	void		SetColors(rgb_color* colors, rgb_color c);
	
			int			fDirection;
			pattern		fStripes;
			rgb_color	fHighColors[BB_NUM_SHADES];
			rgb_color	fLowColors[BB_NUM_SHADES];
};

#endif
