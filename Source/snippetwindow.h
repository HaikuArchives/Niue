/*
	snippetwindow.h
*/

#ifndef _SNIPPETWINDOW_H_
#define _SNIPPETWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include "ssnippetview.h"


class snippetwindow : public BWindow
{
	public:
								snippetwindow();
		virtual bool			QuitRequested();
		virtual void			MessageReceived(BMessage* message);
	
	private:
	
		ssnippetview*		vwMain;
		bool				saved;
		bool				close;
};

#endif
