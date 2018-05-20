
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
#include <stdlib.h>
#include <string.h>

#include <Region.h>

#include "cblist.h"
#include "UBuffer.h"
#include "SMap.h"
#include "Hash.h"
#include "Schemes.h"
#include "Utils.h"

rgb_color col(int32 r,int32 g,int32 b);
int darkback=TRUE;

struct txd{
	char *src;
	int32 len;
	rgb_color   over;
	float x,y,wid;
	rgb_color hi;
};


rgb_color SMap::ColForState(int32 state)
{
/*
	switch (state)
	{
		case 0:return myscheme->background;
		case 1:return myscheme->greycursor;
		case 2:return myscheme->redcursor;
		case 4:return myscheme->greencursor;
		case 8:return myscheme->bluecursor;
	}
*/
	int32 rr,gg,bb,brr,bgg,bbb;
	rr=brr=myscheme->background.red;
	gg=bgg=myscheme->background.green;
	bb=bbb=myscheme->background.blue;
	for (int i=0;i<4;i++){
		int j=1<<i;
		if (j&state){
			rgb_color uu=ColForState(j);
			rr+=(uu.red-brr);
			gg+=(uu.green-bgg);
			bb+=(uu.blue-bbb);
		}
	}
	rr=BND(rr,0,255);
	gg=BND(gg,0,255);
	bb=BND(bb,0,255);
	return (col(rr,gg,bb));
}


inline void SMap::MyLowColor(rgb_color cc){
	if ((*(int32*)&cc)!=(*(int32*)&llc)){
		llc=cc;
		bmv->SetLowColor(llc);
	}
}



SMap::SMap(color_space space,long xx,long yy):
BBitmap(BRect(0,0,xx-1,yy-1),space,TRUE)
{
	Lock();
	when=-1;
	uspace=space;
	dat=(char*)Bits();
	mod=BytesPerRow();


	bigx=xx-1;bigy=yy-1;

	overlist=new CList();
	copylist=new CList();
	freelist=new CList();

	bmv=new BView(BRect(0,0,bigx,bigy),"TextDraw",0,0);
	AddChild(bmv);
	cb=NULL;

	myfont.SetSize(1.0);

	bmv->SetLowColor(llc);//doesn't really matter at all..
	life=0;
	Unlock();
}

SMap::~SMap(){
	if (bmv){
		RemoveChild(bmv);
		delete (bmv);
	}
	delete overlist;
	delete copylist;
	delete freelist;
}


void SMap::MySetCol(rgb_color ){
/*
	unsigned int   cc;
	switch (uspace){
		case B_COLOR_8_BIT:
			cc=Get8Bit(col.red,col.green,col.blue);
			fillcol[0]=cc+(cc<<8)+(cc<<16)+(cc<<24);
			break;
		case B_RGB_32_BIT:
			cc=*(int32*)&col;
			fillcol[0]=((cc>>16)&0xff00) | ((cc)&0xff0000) | ((cc<<16)&0xff000000) | (cc&0xff) ;
			break;
		default:
			printf("unknown color space %d\n",uspace);
			fillcol[0]=0;
			break;
	}
	fillcol[1]=fillcol[0];
*/
}

void  SMap::MyFillRect(BRect rect){
	int x0,x1,y0,y1;
	y0=(int)MAX(rect.top,0);
	x0=(int)MAX(rect.left,0);
	x1=(int)MIN(rect.right,bigx)+1;
	y1=(int)MIN(rect.bottom,bigy)+1;

	char *dest=dat;
	int32 fci=fillcol[0];
	double fcd=*(double*)fillcol;
	dest+=mod*y0;y1-=y0;
	if (uspace==B_RGB_32_BIT){x0*=4;x1*=4;}
	dest+=x0;x1-=x0;

	if (x1<0) return;
	for (int y=0;y<y1;y++){
		char *stt=dest;
		char *fin=dest+x1;
		if ( ((atoi(stt))&4) && (stt+4<=fin)){
			*(int32*)stt=fci;
			stt+=4;
		}
		if ( ((atoi(fin))&4) && (stt<=fin-4)){
			fin-=4;
			*(int32*)fin=fci;
		}

		while ( ((atoi(stt))&3) && (stt<fin)){
			*stt++=fci;
		}
		while (((atoi(fin))&3) && (stt<fin)){
			*--fin=fci;
		}
		while (stt<fin){
			*(double*)stt=fcd;
			stt+=8;
		}
		dest+=mod;
	}
}

void SMap::MyFillRegion(BRegion *reg){
	int nv=reg->CountRects();
	for (int i=0;i<nv;i++){
		MyFillRect(reg->RectAt(i));
	}
}
