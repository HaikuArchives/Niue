/*
	yourapp.h
*/

#ifndef _YOURAPP_H_
#define _YOURAPP_H_

#include "yourappwindow.h"
#include <Application.h>

#define YOURAPP_SIG "application/x-vnd.yourapp"

class yourapp: public BApplication
{
	public:
						yourapp();
		virtual void	ReadyToRun();
	private:
		yourappwindow*		yourappWindow;
};

#endif


