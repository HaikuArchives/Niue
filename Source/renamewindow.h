
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




#ifndef _RENAMEWINDOW_H_
#define _RENAMEWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <TranslationUtils.h>
#include <Entry.h>


#define		RENAME_MSG			'RNAM'

class renamewindow : public BWindow
{
	public:
						renamewindow(BRect rrect, BEntry entry);
		virtual bool	QuitRequested();
		virtual void	MessageReceived(BMessage* message);
		
	private:
	
		BView*			vwAbout;
		BTextControl*	txtName;
		BEntry			rnentry;

};

#endif

