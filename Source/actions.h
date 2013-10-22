
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


#ifndef ACTIONS_H
	#define ACTIONS_H

	enum YAction
	{
		A_MAX_ACTION=32768,
		A_NEXT_BUFF=8192,
		A_UNHANDLED=0,
		A_NOTHING,
		A_FIRST_MOVE=512,
		A_MOVE_TOP,A_MOVE_BOTTOM,
		A_MOVE_UP,A_MOVE_DOWN,
		A_MOVE_LEFT,A_MOVE_RIGHT,
		A_PAGE_UP,A_PAGE_DOWN,
		A_START_LINE,A_END_LINE,
		A_MOVE_TO,
		A_LAST_MOVE,

		A_SCROLL_TOP=1024,A_SCROLL_BOTTOM,
		A_SCROLL_UP,A_SCROLL_DOWN,

		A_DELETE_LEFT,A_DELETE_RIGHT,
		A_QUOTE_PASTE,
		A_FIND_BACK,A_FIND_FORWARD,
		A_FIND_DIALOG,A_ENTER_FIND_STRING,
		A_REPLACE_THIS,
		A_TRANSPOSE_CHARS,
		A_RECENTER_SCROLL,
		A_SCROLL_TOP_BLOCK,
		A_ESCAPE_CHAR,A_RAW_CHAR,
		A_START_DRAG,A_FINISH_DRAG,

		A_SELECT_NONE,
		A_INDENT=2048,A_UNDENT,
		A_QUOTE,A_UNQUOTE,

		A_UNDO,A_REDO,
		A_KILL_SELECTION,
		A_KILL_LINE,
		A_CUT_LINE,
		A_COPY_LINE,

		A_CTRL_CHAR3,
		A_CTRL_CHAR26,
		A_GOTO_LINEX,
		A_GOTO_LINEY,
		A_GOTO_PLACE,

		A_WORDWRAP_NOW=4096,
		A_SAVE,

		A_DISCARD_BUFFER,A_REVERT_BUFFER, A_DISCARD_ALL_BUFFERS,
		A_OPEN_PANEL, A_BROWSE_PANEL, A_ADD_FILE,

		A_PREV_BUFF,
		A_SAVE_ALL,

		A_FOLD,A_UNFOLD,A_FOLD_ALL,A_UNFOLD_ALL,

		A_SPLIT_LINE,

		
		MAKE_MSG,MAKECLEAN_MSG,RUN_MSG,
		
		A_EXPORT_APP, A_BACKUP_APP,
		
		A_NEW_WINDOW,
		
		A_NEW_PROJECT, NEW_PROJECT_MSG,
		
		A_INSERT_SNIPPET,
		
		A_SHOW_VD, A_SHOW_SM, A_SHOW_BB, A_SHOW_GRAPE,
		
		A_REVERT_FACTORY,
		
		N_HELP_REQUESTED,
		
		S_ACTION_CHANGE_MSG, BACKUP_PROJECT_MSG,
		
		Y_COMPARE_HASH, A_CHANGE_COMPLETION, A_INSERT_HINT, A_GEN_MAKEFILE,
		
		A_FILE_UP, A_FILE_DOWN
		
	};

#endif
