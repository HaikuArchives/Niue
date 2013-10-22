
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


#ifndef HASH_H
	#define HASH_H

	enum HTYPE{
		HNORM=0,            // Normal text
		HSTRING,            // "strings"
		HCCOMMENT,          // //  style comments
		HCTOKEN,            // C and C++ tokens
		HOPERATOR,          // C-style operators & Be elements
		HCONSTANT,       	// Be Constants
		HINTEGER,           // 1   2   3
		HFLOAT,             // 1.0 2.0 3.0
		HFIND,              // the 'find' string (unimplemented)
		HBASHCOMMENT,       // # BASH comments
		HMAILEVEN=10,       // >> Even Emaillers
		HMAILODD,           // >>> Odd Emaillers
		HURL,               // http://www.url.net (not implemented yet)
		HHTMLTAGON,         // <HtmlTag>
		HHTMLTAGOFF,        // </HtmlTag>
		HHTMLTRIGRAPH=14,   //  &gt; HTML &lt;
		/* 15..63 undefined.. */
		HASHMAX=63
	};

	extern HTYPE QuickCheck(char *text,int length);
	extern HTYPE FindString(char *text,int32 length,int32 &start,int32 &finish,int32 flags=0);
	extern BList ComparePart(char *lookup, int length);
/*
	Sample Code:

	extern void DrawStyledString(char *source,int32 length,HTYPE style);

	void OutputString(char *source){
		int32 length=strlen(source); //used below
		int32 start,finish;  //filled in by FindString

		//for a performance boost, remove leading spaces here...

		HTYPE style=FindString(source,length,start,finish);

		if (start>0)
			DrawColoredString(source,start-1,HNORM);

		if (finish>start)
			DrawColoredString(source+start,finish-start,style);

		if (finish<length)
		   OutputString(source+finish); //Warning: Recursion
	}

*/


#endif
