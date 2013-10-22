/*
	yourappview.h
*/

#ifndef YOURAPPVIEW_H
#define YOURAPPVIEW_H

#include <Application.h>
#include <InterfaceKit.h>

#define		BUTTON_ONE_MSG		'BTN1'

class yourappview : public BView 
{

	public:
							yourappview(BRect frame, char *name); 

	private:

			BStringView*  txtInfo;
			BButton*		btnOne;

};

#endif
