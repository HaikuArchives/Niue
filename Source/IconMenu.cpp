
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


#include <be/storage/AppFileInfo.h>
#include <be/interface/Bitmap.h>
#include <be/app/Application.h>
#include <be/app/Roster.h>

#include "IconMenu.h"

TIconMenu::TIconMenu(BBitmap* icon, BMenu* menu)
	:	BMenuItem(menu),
        iconLabel(NULL)
{
    if (icon)
    {
        bounds = icon->Bounds();
        iconLabel = new BBitmap(bounds, B_COLOR_8_BIT);
        iconLabel->SetBits(icon->Bits(), icon->BitsLength(), 0, B_COLOR_8_BIT);
    }
}


TIconMenu::TIconMenu(BMenu* menu)
	:	BMenuItem(menu),
        bounds(0.0, 0.0, 15.0, 15.0),
        iconLabel(NULL)
{
    app_info info;
    if (be_app->GetAppInfo(&info) == B_OK)
    {
        BFile appFile(&(info.ref), O_RDONLY);
        BAppFileInfo appFileInfo(&appFile);

        iconLabel = new BBitmap(bounds, B_COLOR_8_BIT);

        if (appFileInfo.GetIcon(iconLabel, B_MINI_ICON) != B_OK)
        {
            delete iconLabel;
            iconLabel = NULL;
        }
    }
}


TIconMenu::~TIconMenu()
{
    delete iconLabel;
    iconLabel = NULL;
}


void
TIconMenu::GetContentSize(float* width, float* height)
{
    if (iconLabel)
    {
        *width = bounds.Width();
        *height = bounds.Height();
    }
    else
        BMenuItem::GetContentSize(width, height);
}


void
TIconMenu::DrawContent()
{
    if (iconLabel)
    {
        Menu()->SetDrawingMode(B_OP_OVER);
		
        float width, height;
		
        Menu()->GetPreferredSize(&width, &height);
		
        BRect destBounds = bounds;
        destBounds.OffsetBy(8.0, ((height - bounds.Height()) * 0.5) - 1);
		
        // Scaling the icon is left as an exercise for the reader :)
        Menu()->DrawBitmap(iconLabel, bounds, destBounds);
    }
    else
        BMenuItem::DrawContent();
}
