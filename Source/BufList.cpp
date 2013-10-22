
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


#include <stdio.h>
#include <string.h>
#include <Autolock.h>

#include "cblist.h"
#include "Niue.h"
#include "BufList.h"

BufList *gb=NULL;

BufList::BufList():BLocker("BufList")
{
	gb=this;
	fBufferList=new CList();
	fNameList=new CList();
	fCount=0;
}


BufList::~BufList()
{
}


Buff *
BufList::BuffAt(int32 index)
{
	return index<fCount?(Buff*)fBufferList->Item(index):NULL;
}


char *
BufList::NameAt(int32 index)
{
	return index<fCount?(char*)fNameList->Item(index):NULL;
}


void
BufList::SetBuff(int32 index,Buff*bb)
{
//	printf("aw %d,%x\n",index,bb);
	fBufferList->SwapWith(bb,index);
}


void
BufList::SetName(int32 i,const char *name)
{
	char *s=(char*)fNameList->Item(i);
//	printf("Name Set (%d),(%x),(%s),%d\n",i,s,name,strlen(name));
	if (!s)s=(char*)malloc(256);
	strcpy(s,name);
	fNameList->SwapWith(s,i);
}
