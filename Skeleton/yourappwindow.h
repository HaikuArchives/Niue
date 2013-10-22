/*
	yourappwindow.h
*/

#ifndef _YOURAPPWINDOW_H_
#define _YOURAPPWINDOW_H_

#include <Application.h>
#include <InterfaceKit.h>
#include "yourappview.h"


class yourappwindow : public BWindow
{
	public:
								yourappwindow();
		virtual bool			QuitRequested();
		virtual void			MessageReceived(BMessage* message);
	
	private:
	
		yourappview*		vwMain;

};

#endif
