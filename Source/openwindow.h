
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



#ifndef OPEN_WINDOW_H
#define OPEN_WINDOW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Roster.h>
#include <Bitmap.h>
#include <Node.h>
#include <NodeInfo.h>
#include <String.h>

#include "barberwindow.h"
#include "colors.h"


#define		OPEN_MSG			'OPEN'
#define		CANCEL_MSG			'CANC'
#define 	SELECT_MSG 			'SELE'
#define		GO_MSG				'GOOO'

#define DEFAULT_ITEM_HEIGHT			(B_MINI_ICON + 5)
#define DRAG_SLOP					4
#define ITEM_MARGIN					5

class ProjectsView;
class MainView;

class OpenWindow : public BWindow
{
	public:
						OpenWindow();
		virtual bool	QuitRequested();
		virtual void 	CenterWindow();
		
	private:
	
		BView*			vwMain;

};

class MainView : public BView
{
	public:
	
						MainView(BRect rect, char *name);
						~MainView();
		virtual void	MessageReceived(BMessage* message);
		virtual void	AttachedToWindow();
		virtual void	BuildList();
		virtual void	GetUserPath();
		virtual void	SaveUserPath();
		virtual void	SendMessage();
		virtual void	DisplayInfo();
	
	private:
		
		BTextControl*	txtPath;
		BButton*		btnGo;
		BButton*		btnOpen;
		BButton*		btnCancel;
		BScrollView*	WorkScroll;
		ProjectsView*	fList;
		BTextView*		txtInfo;
		BBox*			boxInfo;

		
};

class ProjectsView : public BListView
{
public:
					ProjectsView(BRect);
					~ProjectsView();
	virtual void	AttachedToWindow();
	//virtual void	Draw(BRect);
	

};

//--------------------------------------------------------------------

class pListItem : public BListItem
{
public:

					pListItem(BEntry*);
					~pListItem();
	virtual void	DrawItem(BView*, BRect, bool);
	virtual	void	Update(BView*, const BFont*);
	BBitmap*		Bitmap() {return fIcon;};
	entry_ref		Ref() {return fRef;};
private:
	char			fName[B_FILE_NAME_LENGTH];
	BBitmap			*fIcon;
	entry_ref		fRef;
	
};

#endif
