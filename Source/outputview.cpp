
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
#include "outputview.h"

OutputView::OutputView(BRect rect, char *name)
	:	BView(rect, name, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM,
				B_WILL_DRAW | B_FRAME_EVENTS)
{
//	BRect textrect(0,0,rect.Width()-B_V_SCROLL_BAR_WIDTH-1,rect.Height());
	
	//define objects
	fTextView = new BTextView(BRect(0,0,rect.Width() - B_V_SCROLL_BAR_WIDTH - 1,
							rect.Height()), "tview",
							BRect(4,2, rect.Width() - B_V_SCROLL_BAR_WIDTH - 1,1),
							B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	fScrollView = new BScrollView("scroll_view", fTextView, B_FOLLOW_ALL_SIDES, 0,
									false, true, B_FANCY_BORDER);
	
	// set objects
	fTextView->SetFontAndColor(be_plain_font, B_FONT_ALL, &black);
	fTextView->SetViewColor(bright);
	fTextView->MakeEditable(false);
	fTextView->SetWordWrap(true);

	//add objects
	AddChild(fScrollView);
}


void
OutputView::FrameResized(float width, float height)
{
	// Manually change the text region.
	fTextView->SetTextRect(BRect(4,3,Bounds().Width() - B_V_SCROLL_BAR_WIDTH - 1,1));
}


int
OutputView::DisplayShellProcess(const char *command)
{
	int shellExit = -1;
	FILE *fd;
	char buff[256];

	fd = popen(command, "r");
	
	if (fd)
	{
		for(int32 i = 0; fgets(buff, 256, fd) != NULL; i++) 
		{
			//AppendText(buff);
			if (buff) 
			{
				fTextView->Insert(fTextView->TextLength(), buff, strlen(buff));
				fTextView->ScrollToOffset(fTextView->TextLength());
			}
			
//			if(i <= 1) 
//			{	// only show win if any text in first 2 loops.
//				if(buff != NULL) 
//				{
//					Minimize(false);
//				}
//			}
		}	
		
		shellExit = pclose(fd);	// Close filepointer.
		
		if (shellExit == 127) 
		{
			(new BAlert("Niue", "Error: program not found, exec failed.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		else if(shellExit == -1) 
		{
			(new BAlert("Niue", "Error: got -1, couldn't create program process, fork failed.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		else if(WIFEXITED(shellExit) != 0) 
		{	// prog exited normally with exit
			int progExit = WEXITSTATUS(shellExit);
			BString	stattext = "Shell command executed, returned ";
			stattext << (int)progExit;
			
			if (progExit == EXIT_SUCCESS) 
			{		// prog returned 0 (OK)
//				(new BAlert("Niue", stattext.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
			else 
			{							// prog returned !=0 (error likely)
//				(new BAlert("Niue", stattext.String(), "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
		}
		else if(WIFSIGNALED(shellExit) != 0) 
		{
			(new BAlert("Niue", "Serious Error: program terminated because it received a signal that was not handled.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		else if(WIFSTOPPED(shellExit) != 0) 
		{
			(new BAlert("Niue", "Serious Error: program was stopped.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		else if(WIFCORED(shellExit) != 0) 
		{
			(new BAlert("Niue", "Serious Error: program terminated and produced a core dump.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
	}
	else
	{
		(new BAlert("Niue", "Internal error opening stdout stream!", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
	}
	
	// At last, check if any text was outputted at all
	if(fTextView->TextLength() == 0) 
	{
		(new BAlert("Niue", "Error: there is no output at all", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();	
	}
	
	return shellExit;
}


void
OutputView::ClearText()
{
	fTextView->SelectAll();
	fTextView->Clear();
}
