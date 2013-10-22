/*
	gqueryview.h
*/

#ifndef GQUERYVIEW_H
#define GQUERYVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include "colors.h"

#define		G_ACTION_DO_SEARCH		'gads'
#define		G_ACTION_CHANGE_PATH	'gacp'
#define		G_ACTION_DO_CASE			 'gadc'
#define		G_ACTION_DO_WORD			'gadw'
#define		G_ACTION_DO_SUB				'gadu'

class gqueryview : public BView 
{

	public:
							 			gqueryview(BRect frame, char *name, const char *path); 
			virtual void			Draw(BRect);
			BTextControl*	  txtString;
			BTextControl*  	txtPath;
			BCheckBox*		chkCase;
			BCheckBox*		chkWord;
			BCheckBox*		chkSubdir;

	private:

			BButton*		btnGo;

};

#endif
