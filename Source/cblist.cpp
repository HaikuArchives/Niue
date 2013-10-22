
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
#include <malloc.h>
#include <stdio.h>

#include <OS.h>
#include "cblist.h"

CList::CList(int num)
{
	num = 4;
	max = num;
	if (max)
	{
		data = (long*)malloc(sizeof(long) * max);
//		printf("%x\n",data);
	}
	else
		data = NULL;
	Reset();
}


CList::~CList()
{
	free(data);
	data = NULL;
}


CList &
CList::operator=(const CList &t)
{
	int i, mx = t.Num();
	Reset();
	
	for (i = 0; i < mx; i++)
		Add(t[i]);
	return *this;
}


void
CList::Reset()
{
	mycount = 0;
}


long
CList::Item(int idx) const
{
	if (idx < 0 || idx >= mycount)
		return 0;
	return data[idx];
}


void
CList::CheckMax(long nucount)
{
	if (mycount >= max)
	{
		printf("SERIOUS PROBLEMS IN CB_LIST!!\n");
		printf("0x%p :%ld,%ld,%ld\n",data,mycount,nucount,max);
	}
	
	if (nucount >= max)
	{
		int omax = max;
		if (!omax)
			max = 4;
		while (nucount >= max)
			max *= 2;

//		printf("data moved, was 0x%x (%d,%d,%d,%d)\n",data,mycount,nucount,max,omax);
		data=(long*)realloc(data,sizeof(long) * max);
//		printf("now 0x%x(%d)\n",data,find_thread(0));
	}
}


void
CList::Add(long vv,long index)
{
	if (index < 0)
		return;

	CheckMax(index + 2);
	while (mycount<index)
		data[mycount++] = 0;

	CheckMax(mycount + 1);
	mycount++;
	for (int i = mycount; i > index; i--)
		data[i] = data[i - 1];
	data[index] = vv;
}


void
CList::RemIndex(long index)
{
	if (index >= 0)
	{
		for (int i = index; i < mycount; i++)
			data[i] = data[i + 1];
		if (index < mycount)
			mycount--;
	}
}


void
CList::SwapWith(long val,long index)
{
	while (mycount <= index)
		Add((long)NULL,mycount);
	if (index >= 0)
		data[index] = val;
}


bool
CList::operator<=(long h) const
{
	for (int i=0;i<mycount;i++)
		if (data[i] == h)
			return TRUE;
	return FALSE;
}


int
CList::IndexOf (long h) const
{
	for (int i = 0; i < mycount; i++)
		if (data[i] == h)
			return i;
	return -1;
}


void
CList::AddF(float vv,long index)
{
	Add(*(long*)&vv,index);
}


void
CList::SwapWithF(float vv,long index)
{
	SwapWith(*(long*)&vv,index);
}


float
CList::ItemF(int idx) const
{
	if (idx < 0 || idx >= mycount)
		return 0.0;
	return *(float*)(data + idx);
}


void *
CList::ItemV(int idx) const
{
	return (void*)Item(idx);
}


void
CList::Add(const void *value,long index)
{
	Add((long)value,index);
}


int
CList::IndexOf(const void *value) const
{
	return IndexOf((long)value);
}


void
CList::SwapWith(const void *vv,long index)
{
	SwapWith((long)vv,index);
}


bool
CList::operator<=(const void* value) const
{
	return *this <= (void*)value;
}

