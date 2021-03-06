
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


#ifndef OUTPUTVIEW_H
#define OUTPUTVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <String.h>
#include <stdio.h>			// for popen()
#include <stdlib.h>			// for EXIT_SUCCESS, WIFEXITED, etc..
#include <sys/wait.h>
#include <fstream>			// for ifstream
#include <iostream>
#include "colors.h"

class OutputView : public BView
{
public:
					OutputView(BRect frame, char *name);
	virtual void	FrameResized(float width, float height);
			int		DisplayShellProcess(const char *command);
	virtual void	ClearText();

private:
		BScrollView	*fScrollView;
		BTextView	*fTextView;
};

#endif
