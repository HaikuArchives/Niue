
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

#ifndef VITEMSVIEW_H
#define VITEMSVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include "vdefines.h"
#include "colors.h"

class vitemsview : public BView 
{

	public:
										vitemsview(BRect frame, char *name, uint32 resizingMode, uint32 flags);
		BListView*				lstItems;

	private:
		
		BScrollView*			scrItems;

};

class vlistitem : public BStringItem
{
	public:
									vlistitem(const char* text, int32 vid);
		int32					id;

	private:
};

#endif
