
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




#ifndef _EXPORTWINDOW_H_
#define _EXPORTWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <Bitmap.h>
#include <Node.h>
#include <NodeInfo.h>
#include <FilePanel.h>
#include <String.h>
#include <stdlib.h>
#include <stdio.h>
#include <Roster.h>
#include "colors.h"

#define		CLOSE_EXPORT_MSG		'CLHB'
#define		GO_EXPORT_MSG			'GEMS'
#define		SVG_EXPORT_MSG			'SEMS'
#define		BROWSE_SVG_EXPORT_MSG	'BSEM'
#define		NEW_SVG_EXPORT_MSG		'NSEM'
#define		ADD_EXPORT_MSG			'AEMS'
#define		BROWSE_ADD_EXPORT_MSG	'BAEM'
#define		SVG_FOUND_EXPORT_MSG	'SFEM'
#define		ADD_FOUND_EXPORT_MSG	'AFEM'
#define		REMOVE_ADD_EXPORT_MSG	'RAEM'
#define		READY_FOR_EXPORT_MSG	'RFEM'

#define eDEFAULT_ITEM_HEIGHT			(B_MINI_ICON + 5)
#define eITEM_MARGIN					5

class FileView;
class exportwindow : public BWindow
{
	public:
						exportwindow(BEntry proj_entry);
		virtual bool	QuitRequested();
		virtual void 	CenterWindow();
		virtual void	MessageReceived(BMessage* message);
		
	private:
	
		BEntry			entry;
		BView*			vwExport;
		BButton*		btnClose;
		BButton*		btnExport;
		BStringView*	strName;
		BTextControl*	txtName;
		BCheckBox*		chkSvg;
		BTextControl*	txtSvg;
		BButton*		btnSvg;
		BButton*		btnNewIcon;
		BCheckBox*		chkAdd;
		BScrollView*	WorkScroll;
		FileView*		fList;
		BButton*		btnAdd;
		BFilePanel*		brsPanel;
		BFilePanel*		savePanel;
		
};

class FileView : public BListView
{
public:
					FileView(BRect);
					~FileView();
	virtual void	AttachedToWindow();
	virtual void	MouseDown(BPoint where);
	virtual void	SetEnabled(bool enabled);

private:

	BPopUpMenu*		mnuLeft;

};

//--------------------------------------------------------------------

class ListItem : public BListItem
{
public:

					ListItem(BEntry*);
					~ListItem();
	virtual void	DrawItem(BView*, BRect, bool);
	virtual	void	Update(BView*, const BFont*);
	BBitmap*		Bitmap() {return fIcon;};
	entry_ref		Ref() {return fRef;};
private:
	char			fName[B_FILE_NAME_LENGTH];
	BBitmap			*fIcon;
	entry_ref		fRef;
	
};

class TDirFilter : public BRefFilter 
{

public:
  bool Filter(const entry_ref* entry, BNode* node, struct stat_beos* stat,
              const char* mimetype);
};

#endif

