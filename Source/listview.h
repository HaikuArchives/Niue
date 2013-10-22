
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
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Roster.h>
#include <Bitmap.h>
#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Node.h>
#include <NodeInfo.h>
#include <Path.h>
#include <String.h>

#include "renamewindow.h"
#include "UBuffer.h"
#include "colors.h"

#define kDEFAULT_ITEM_HEIGHT			(B_MINI_ICON + 5)
#define kDRAG_SLOP						  4
#define kITEM_MARGIN					  5

#define	SHOW_FILE_MSG					'MSHF'
#define ITEM_MENU_INVOKED_MSG			'IMIM'
#define ITEM_MENU_SELECTED_MSG			'IMSM'
#define MNU_LEFT_OPEN_MSG				'MLOP'
#define MNU_LEFT_TRACKER_MSG			'MLTR'
#define MNU_LEFT_TRASH_MSG				'MLTH'
#define	MNU_LEFT_DUPLICATE_MSG			'MLDU'
#define	MNU_LEFT_INFO_MSG				'MLIN'
#define MNU_LEFT_NAME_MSG				'MLNM'
#define MNU_LEFT_COPY_MSG				'MLCO'
#define MNU_LEFT_MOVE_MSG				'MNMO'

#define MNU_LEFT_SET_SOURCE				'MASO'
#define MNU_LEFT_SET_MAKEFILE			'MAMA'
#define MNU_LEFT_SET_RESOURCE			'MARE'
#define MNU_LEFT_SET_TEXT				'MATE'
#define MNU_LEFT_SET_IMAGE				'MAIM'
#define MNU_LEFT_SET_AUDIO				'MAAU'
#define MNU_LEFT_SET_VIDEO				'MAVI'
#define MNU_LEFT_SET_APP				'MAAP'
#define MNU_LEFT_SET_ARCHIVE			'MAAR'

class RocketView;

class listview : public BView
{
public:
	listview(BRect frame, char *name);
	~listview();
	
	virtual void 			MessageReceived(BMessage*);
	virtual void 			AttachedToWindow();
	virtual void			SelectByName(const char *name);
	void 					BuildList(entry_ref ref);
	void					UpdateList(char *buffname,bool saved);
	entry_ref*				FindItem();
	RocketView*				fList;
	
private:
	
	BScrollView*			WorkScroll;
	BDirectory				dir;
	BEntry					entry;
	BFilePanel*				CopyPanel;
	BListItem*				source;
	BListItem*				res;
	entry_ref				startref;
};

//--------------------------------------------------------------------


class RocketView : public BListView
{
public:
					RocketView(BRect);
					~RocketView();
	virtual void	AttachedToWindow();
	virtual void	Draw(BRect);
	virtual void	MouseDown(BPoint);
	
private:

	BPopUpMenu*		mnuLeft;
	BMenu*			mnuMime;

};

//--------------------------------------------------------------------

class TListItem : public BListItem
{
public:

					TListItem(BEntry*,bool);
					TListItem(const char*);
					~TListItem();
	virtual void	DrawItem(BView*, BRect, bool);
	virtual	void	Update(BView*, const BFont*);
	BBitmap*		Bitmap() {return fIcon;};
	entry_ref*		Ref() {return &fRef;};
	char*			GetName() {return fName;};
	bool			save;
	bool			is_source;
	BBitmap			*fIcon;

private:
	char			fName[B_FILE_NAME_LENGTH];
	entry_ref		fRef;
	bool			super; // = false;

};

#endif
