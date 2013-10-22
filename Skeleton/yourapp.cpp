/*
	yourapp.cpp
*/

#include "yourapp.h"
#include "yourappwindow.h"

int main(int, char**)
{
	yourapp* app = new yourapp();
	app->Run();
	
	return(0);
}

yourapp::yourapp()
	: BApplication(YOURAPP_SIG)
{
}

void
yourapp::ReadyToRun()
{
	yourappWindow = new yourappwindow();
	yourappWindow->Show();
}


