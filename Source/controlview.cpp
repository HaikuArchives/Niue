
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



#include "controlview.h"

ControlView::ControlView(BRect rect, char *name)
	:	BView(rect, name, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW)
{
	//define objects
	btnMake = new BButton(BRect(5,5,rect.Width()-5,25), "btnMake", "Make",
						new BMessage(MAKE_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
						B_WILL_DRAW | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);
	btnMakeClean = new BButton(BRect(5,30,rect.Width()-5,50), "btnMakeClean",
						"Make Clean", new BMessage(MAKECLEAN_MSG),
						B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
						B_WILL_DRAW | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);
	btnRun = new BButton(BRect(5,55,rect.Width()-5,75), "btnRun", "Run",
						new BMessage(RUN_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
						B_WILL_DRAW | B_NAVIGABLE | B_FULL_UPDATE_ON_RESIZE);
	
	// set objects
	btnMake->SetEnabled(false);
	btnMakeClean->SetEnabled(false);
	btnRun->SetEnabled(false);
	
	//add objects
	AddChild(btnMake);
	AddChild(btnMakeClean);
	AddChild(btnRun);
}

