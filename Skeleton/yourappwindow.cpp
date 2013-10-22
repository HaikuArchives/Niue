/*
	yourappwindow.cpp
*/

#include "yourappwindow.h"

BRect rect(100,100,300,200);

yourappwindow::yourappwindow() :
	BWindow(rect, "YourApp", B_TITLED_WINDOW,B_WILL_DRAW)
{

	SetSizeLimits(50,30000,50,30000);
	
	// define objects
	vwMain = new yourappview(BRect(0,0,rect.Width(),rect.Height()), "vwMain");
	
	// set objects
	vwMain->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// add objects
	AddChild(vwMain);

}


bool yourappwindow::QuitRequested()
{
	be_app_messenger.SendMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

void yourappwindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case BUTTON_ONE_MSG:
		{
			(new BAlert("alert", "Alert text", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
		}
		break;
		
	}
}
