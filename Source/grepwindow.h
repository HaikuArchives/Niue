/*
	grepwindow.h
*/

#ifndef _GREPWINDOW_H_
#define _GREPWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include <stdio.h>			// for popen()
#include <stdlib.h>			// for EXIT_SUCCESS, WIFEXITED, etc..
#include <fstream.h>		// for ifstream
#include <iostream>
#include "gqueryview.h"
#include "goutputview.h"


class grepwindow : public BWindow
{
	public:
									  grepwindow(const char *path);
		virtual bool			QuitRequested();
		virtual void			MessageReceived(BMessage* message);
		virtual void			CenterWindow();
		virtual void			DoSearch(BEntry entry);
		virtual void			LoadPrefs();
		virtual void			SavePrefs();
	
	private:
	
		gqueryview*		vwQuery;
		goutputview*		vwOutput;
		int							count;

};

#endif
