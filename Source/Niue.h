
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


#ifndef NIUE_H

#define NIUE_H

#define Y_CHANGE_BUFF       0x15f90
#define Y_BUFF_CHANGE   0x15f9c
#define REDRAW              0x17f90
#define SAVE_FILE           0x17f91
#define SAVE_AS_FILE        0x17f92
#define MY_SAVE_FILE        0x17f93

#define Y_GOTO_LINE         0x22113
#define S_GOTO_LINEY        0x22116
#define S_GOTO_PLACE        0x22117
#define S_FIND_STRING        0x22118
#define S_RESET_FOCUS     0x22119

#define Y_CHANGE_BACKSTYLE    0x26100
#define Y_DEBUG             0x26111

#define Y_HSCROLL_ON        0x26220
#define Y_VSCROLL_ON        0x26221
#define Y_SYSFONT_CHANGE 	0x26223

class BFont;
extern BFont *be_plain_font2;
extern BFont *be_bold_font2;
extern BFont *be_fixed_font2;

#if DEBBUILD
	#define DEBUG 1
#endif
#include <Debug.h>

#define Own (ASSERT(LockingThread()==find_thread(NULL)))

#endif // NIUE_H

