
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



#ifndef _DOCWINDOW_H_
#define _DOCWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include "colors.h"


#define HELP_ITEM_MENU_SELECTED	'22'

#define hDEFAULT_ITEM_HEIGHT			(B_MINI_ICON)
#define hDRAG_SLOP						  4
#define hITEM_MARGIN					  3

class hRocketView;
class docwindow : public BWindow
{
	public:
								docwindow();
		virtual bool			QuitRequested();
		virtual void 			CenterWindow();
		virtual void			MessageReceived(BMessage* message);
//		virtual void			AttachedToWindow();
		virtual void			BuildList();	
	private:
	
		BView*					vwMain;
		BTextView*				txtText;
		hRocketView*				fList;
		BScrollView*			WorkScroll;
		BScrollView*			TextScroll;
};



class hRocketView : public BListView
{
public:
						hRocketView(BRect);
						~hRocketView();
	virtual void	MouseDown(BPoint where);
	virtual void	AttachedToWindow();
	virtual void	Draw(BRect);

};

class HListItem : public BListItem
{
public:

						HListItem(BEntry*);
						~HListItem();
	virtual void	DrawItem(BView*, BRect, bool);
	virtual void	Update(BView*, const BFont*);
	entry_ref		Ref() {return fRef;};
private:
	char			fName[B_FILE_NAME_LENGTH];
	entry_ref		fRef;
	
};

#endif

