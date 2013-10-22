
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

#include "cblist.h"
#include "BSBar.h"
#include "BMapView.h"

BSBar::BSBar(BRect frame, const char *name,port_id drawport, long min, long max,
			orientation posture)
	:	BScrollBar(frame,name,NULL,min,max,posture)
{
	fLastIndex=NULL;
	fIndexList=new CList();
	fValueList=new CList();
	fDrawPort=drawport;
}


BSBar::~BSBar()
{
	delete fIndexList;
	delete fValueList;
}


void
BSBar::Capture(float mult)
{
	long    tt;

	if (fLastIndex)
	{
		tt=fIndexList->IndexOf(fLastIndex);
		if (tt<0)
			printf("error in BSBar->ChangeBuff()\n");
		else
			fValueList->SwapWithF(Value()/mult,tt);
	}
}


void
BSBar::ChangeIndex(float mult,void *newindex,float def)
{
	long    tt;

	tt=fIndexList->IndexOf(newindex);
	fLastIndex=newindex;
	if (tt<0)
	{
		fIndexList->Add(fLastIndex);
		fValueList->AddF(def);
		SetValue(def);
	}
	else
		SetValue(fValueList->ItemF(tt)*mult);
}


void
BSBar::ValueChanged(float vv)
{
	if (fLastValue!=vv)
	{
		write_port(fDrawPort,SCROLLBAR_MOVE,NULL,0);
		fLastValue=vv;
	}
}
