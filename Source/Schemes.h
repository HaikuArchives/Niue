
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


#ifndef COLOR_SCHEMES_H
	#define COLOR_SCHEMES_H

	#include <GraphicsDefs.h>

	struct colscheme{
		rgb_color   style[30];
		rgb_color   background;
		rgb_color   greycursor;
//		rgb_color   redcursor;
//		rgb_color   greencursor;
//		rgb_color   bluecursor;
	};


	//warning! don't use #000000 for black, it
	//maps to col 255 (transparent) when you use Invert()

	extern rgb_color col(int32 r,int32 g,int32 b);
	extern rgb_color col(char *HtmlFormatString); //#112233

	extern colscheme *schemetab[2];
	extern void MakeSchemes();

#endif  // COLOR_SCHEMES    _H
