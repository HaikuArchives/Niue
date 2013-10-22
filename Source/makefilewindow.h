
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




#ifndef _MAKEFILEWINDOW_H_
#define _MAKEFILEWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>


#define		CLOSE_MAKE_MSG			'CMMS'
#define		GO_SAVE_MSG				'GSMS'
#define		GO_HELP_MSG				'GHMS'

#define		MC_APP					'MCAP'
#define		MC_SHARED				'MCSH'
#define		MC_ADDON				'MCAD'
#define		MC_STATIC				'MCST'
#define		MC_DRIVER				'MCDR'
#define		MC_MODULE				'MCMO'
#define		MC_FULL					'MCFU'
#define		MC_SOME					'MCSO'
#define		MC_NONE					'MCNO'


class makefilewindow : public BWindow
{
	public:
						makefilewindow(BEntry proj_entry);
		virtual bool	QuitRequested();
		virtual void 	CenterWindow();
		virtual void	MessageReceived(BMessage* message);
		virtual void	FillFields(BEntry entry);
		virtual void	WriteMakefile();
		
	private:
	
		char name[B_FILE_NAME_LENGTH];	//project name
		const char*		type;			//project type
		const char*		opt;			//optimization settings
	
		BEntry			entry;
		BMenu*			mnuType;
		BMenu*			mnuOpt;
		BView*			vwMake;
		BButton*		btnClose;
		BButton*		btnGenerate;
		BButton*		btnHelp;
		BStringView*	strName;
		BTextControl*	txtName;
		BStringView*	strType;
		BMenuField*		cmbType;
		BStringView*	strSources;
		BTextControl*	txtSources;
		BStringView*	strLibs;
		BTextControl*	txtLibs;
		BStringView*	strRes;
		BTextControl*	txtRes;
		BStringView*	strOpt;
		BMenuField*		cmbOpt;
		BStringView*	strAdv;
		BTextView*		txtAdv;
		BScrollView*	srcAdv;
		
		
};




#endif

