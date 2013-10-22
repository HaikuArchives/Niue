
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


#ifndef ICON_MENU_H
#define ICON_MENU_H


#include <be/interface/MenuItem.h>
#include <be/interface/Rect.h>


class BBitmap;

class TIconMenu : public BMenuItem
{
public:
    // Constructor
    // Description:	Creates a menu item with the provided icon as its
    //				label and menu as its content.
    // Requires:		Both icon and menu must be valid.
    // Result:		If icon is valid, then the icon it points to will
    //				be used as the label for the menu. If icon is NULL
    //				then the menu's name will be used instead. If icon
    //				is invalid, a crash occurs. If menu is invalid or
    //				NULL, a crash occurs.
    // Notes:			Client code is still responsible for deleting the
    //				data icon points to. IconMenu only copies the data,
    //				it does not take posession of it.
    TIconMenu(BBitmap* icon, BMenu* menu);

    // Constructor
    // Description:	Creates a menu item with the application's mini
    //				icon as its label and menu as its content.
    // Requires:		A valid BMenu pointer. The application should also
    //				have a mini icon.
    // Result:		If the application's mini icon is found then the
    //				menu will be created with the icon as the label.
    //				Otherwise, the name of menu will be used as the
    //				label instead. If menu is invalid or NULL, a
    //				crash will occur.
    TIconMenu(BMenu* menu);

    // Destructor
    virtual ~TIconMenu();

protected:
    // Overridden Hook Functions
    virtual void GetContentSize(float* width, float* height);
    virtual void DrawContent();

private:
    // Disabled Copy Constructor
    TIconMenu(const TIconMenu& iconMenu);

    // Disabled Assignment Operator
    TIconMenu& operator=(const TIconMenu& iconMenu);

    BRect	bounds;
    BBitmap* iconLabel;
};


#endif
