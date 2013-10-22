
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



#include "renamewindow.h"


renamewindow::renamewindow(BRect rrect, BEntry entry) :
	BWindow(rrect, "About Niue", B_NO_BORDER_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_WILL_DRAW | B_NOT_RESIZABLE)
{
	
	//define objects
	vwAbout = new BView(BRect(0,0, rrect.Width(), rrect.Height()), "vwAbout", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED);
	txtName = new BTextControl(BRect(0, 1, rrect.Width(), rrect.Height()), "txtName", 0, "",  new BMessage(RENAME_MSG),B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	
	//set objects
	vwAbout->SetViewColor(ui_color(B_MENU_SELECTION_BACKGROUND_COLOR));
	char name[B_FILE_NAME_LENGTH];
	entry.GetName(name);
	txtName->SetText(name);
	rnentry = entry;
	entry.Unset();
	
	//add objects
	AddChild(vwAbout);
	vwAbout->AddChild(txtName);
	txtName->MakeFocus(true);

}




bool renamewindow::QuitRequested()
{
	Quit();
}

void renamewindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case RENAME_MSG:
		{
			rnentry.Rename(txtName->Text(), false);
			Quit();
		}
		break;
	}
}


