
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


#ifndef VPROPSVIEW_H
#define VPROPSVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include "mover.h"
#include "vdefines.h"
#include "colors.h"

class vpropsview : public BView 
{

	public:
									vpropsview(BRect frame, char *name, uint32 resizingMode, uint32 flags);
		virtual void			Draw(BRect);
		BTextControl*  	  txtName;
		BTextControl*	    txtLabel;
		BTextControl*		txtLeft;
		BTextControl*		txtTop;
		BTextControl*		txtRight;
		BTextControl*		txtBottom;
		BRadioButton*			chkXLeft;
		BRadioButton*			chkXRight;
		BRadioButton*			chkXBoth;
		BRadioButton*			chkYTop;
		BRadioButton*			chkYBottom;
		BRadioButton*			chkYBoth;

	private:
			
		BStringView*		strName;
		BStringView*		strLabel;
		BStringView*		strBounds;
		BStringView*		strRes;
		BView*				vwResLeft;
		BView*				vwResRight;
		BButton*			btnCreateCode;
		BButton*			btnStartProject;
		Mover*				mvrSplit;

};

#endif
