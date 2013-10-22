/*
	goutputview.h
*/

#ifndef GOUTPUTVIEW_H
#define GOUTPUTVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include "colors.h"

//#define		G_ACTION_DO_SEARCH		'gads'
//#define		G_ACTION_CHANGE_PATH	'gacp'

class goutputview : public BView 
{

	public:
								goutputview(BRect frame, char *name); 
		BTextView*	 txtOutput;
		BStringView*	strStatus;

	private:

		BScrollView*	scrOutput;

};

#endif
