
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


#ifndef _VISUALWINDOW_H_
#define _VISUALWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <ctype.h>	//for isdigit()
#include <stdlib.h>	//for atio()
#include "vitemsview.h"
#include "vpropsview.h"
#include "vdesignview.h"
#include "velementsview.h"
#include "mover.h"
#include "vdefines.h"
#include "colors.h"
#include "NiueWindow.h"

#define A_NEW_PROJECT	'vanp'

class VisualWindow : public BWindow
{
	public:
								VisualWindow(BPoint point);
		virtual void			MessageReceived(BMessage* message);
		virtual void			BuildList();
		BString					GetConstructors();
		BString					GetHeaders();
	
	private:
	
		vitemsview*		  	vwItems;
		vpropsview*			vwProps;
		vdesignview*		vwDesign;
		velementsview*		vwElements;

		Mover*				mvrLeft;
		Mover*				mvrRight;
		Mover*				mvrHorz;
		
		BFilePanel*			SavePanel;

};

class codewindow : public BWindow
{
	public:
									codewindow(BString text);
		BTextView*		 txtView;
		
	private:

		BScrollView*		scrView;
};

#endif
