
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

#include "vitemsview.h"

vitemsview::vitemsview(BRect rect, char *name, uint32 resizingMode, uint32 flags)
	   	   : BView(rect, name, resizingMode, flags)
{
	//define objects
	lstItems = new BListView(BRect(5,5,rect.Width()-5-B_V_SCROLL_BAR_WIDTH,rect.Height()-5), "lstItems", B_SINGLE_SELECTION_LIST,
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
	scrItems = new BScrollView("scrItems", lstItems, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, false, true, B_PLAIN_BORDER);
	// set objects
	lstItems->SetHighColor(darkblue);
	lstItems->SetSelectionMessage(new BMessage(V_ACTION_ITEM_SELECTED));	
	//add objects
 	AddChild(scrItems);

}

vlistitem::vlistitem(const char* text, int32 vid)
			: BStringItem(text)
{
	id = vid;
}
