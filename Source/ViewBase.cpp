
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



#include "ViewBase.h"
#include "NTree.h"
#include "UBuffer.h"

#include <View.h>
#include <stdio.h>


ViewBase::ViewBase(){
}

void ViewBase::Target(BView *_vv,float _fh,float _ffh,float _width,Buff *_cb){
	vv=_vv;
	fh=_fh;
	ffh=_ffh;
	cb=_cb;
	width=_width;
	nt=cb->mytree;
}


void ViewBase::Draw(BRect dest,float tlx,float tly)
{
	
	DrawTxt(dest,tlx,tly);
	
}

void ViewBase::Locate(float &x,float &y,int place)
{
	LocTxt(x,y,place);
}

float ViewBase::LocateY(int place)
{
	return LocTxtY(place);
}

void ViewBase::Locate2(int32 &x,int32 &y,int place)
{
	LocTxt2(x,y,place);
}

void ViewBase::SHC(const rgb_color &n)
{
	vv->SetHighColor(n);
}

int ViewBase::Place(float x,float y){
//    BAutolock(nt);

	int p1=0;
	int p2=nt->Size();
	int stp=0;
	int  lm=-1;
	float xx=0,yy=0;
	float lx=1000.0,ly=-1;
	while (p2>p1){
		int mm=p1+p2>>1;
		if (mm==lm){p2--;}
		Locate(xx,yy,mm);
		if (yy<y){
			if (yy>y-fh){
				((xx<x)?p1:p2)=mm;
			}else{
				p1=mm;
			}
		}else{
			p2=mm;
		}
		if (stp++>50){
			printf("big file %d,%d\n",p1,p2);
			break;
		}
		lm=mm;
		lx=xx;
		ly=yy;
	}

	Locate(xx,yy,p1+1);
	if (ly==yy && (x-xx>lx-x))p1++;
	
	return p1;

}

