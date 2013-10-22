
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
#include "Hash.h"
#include "Schemes.h"
#include "Utils.h"
#include "UBuffer.h"

#include <View.h>
#include <Region.h>

#include <stdio.h> //sprintf

void ViewBase::DrawTxt(BRect dest,float tlx,float tly){

	int pp1=cb->Pos(141),pp2=cb->Pos(142);
	float bx1=-1,by1=100000,bx2=-1,by2=by1;
	SORT(pp1,pp2);
	if (pp1>=0 && pp2>=0 && pp1!=pp2){
		Locate(bx1,by1,pp1);bx1-=tlx;by1-=tly;
		Locate(bx2,by2,pp2);bx2-=tlx;by2-=tly;
	}else{
		pp1=pp2=-1;
	}

	int vmax=nt->Size();
	char spa[256],sha[256*4];
	char *ds=(vmode==2)?sha:spa;

	BRegion back,sel;
	back.Include(dest);
	vv->SetDrawingMode(B_OP_COPY);
	vv->SetLowColor(csh->background);

	if (pp1>=-1){
		float xx=(by1==by2)?bx2:40000;
		sel.Include(BRect(bx1,by1,xx,by1+ffh));
		if (by2>by1){
			sel.Include(BRect(0,by1+fh,40000,by2-fh+ffh));
			sel.Include(BRect(0,by2,bx2,by2+ffh));
		}
		sel.IntersectWith(&back);
		back.Exclude(&sel);
		vv->SetHighColor(csh->greycursor);
		vv->FillRegion(&sel,B_SOLID_HIGH);
	}
	vv->FillRegion(&back,B_SOLID_LOW);
	vv->SetHighColor(255,255,255,255);

	int stt=MAX((int)floor((dest.top-fh+tly)/fh),0);
	int fin=(int)floor((dest.bottom+fh+tly)/fh);
	float y1=(stt-1.0)*fh-tly,y2=y1-1.0,xx=4-tlx;

	int npl=0;
	nplace *np2=nt->FindNext(10,-1,stt+1);
	if (!np2) return;
	if (np2) npl=np2->place;
	drawing_mode lff=B_OP_COPY;
	for(int i=stt;i<=fin;i++){
		nplace *np=nt->FindNext(10,npl);
		if (!np)break;
		y1+=fh;
		y2+=fh;
		int ppl=npl;
		npl=np->place+1;
		int l=npl-ppl;

		if (y1>by1 && y1<=by2+ffh){
			vv->SetLowColor(csh->greycursor);
		}else{
			vv->SetLowColor(csh->background);
		}

		drawing_mode ff=B_OP_COPY;
		if (y1-2.0*fh<=by1 && y1>=by1){ff=B_OP_OVER;}
		if (y1-2.0*fh<=by2 && y1>=by2){ff=B_OP_OVER;}
		if (ff!=lff){vv->SetDrawingMode(lff=ff);}

		if (l>200)l=200;
		
		nt->Fetch(spa,l,ppl);
		
		if (vmode!=2){
			vv->DrawString(ds,l,BPoint(xx,y1));
			continue;
		}
		int l2=MassageTxt(spa,l,ds);
		if (l2<1) continue;
		char *ms=ds;
		int32 s1=0,s2=0,f1=TRUE;
		float x1=xx;

		while (s2<l2 && x1<=dest.right){
			ms+=s2;l2-=s2;
			HTYPE style=HNORM;
			s1=0;s2=l2;
			if (style==HNORM)style=FindString(ms,l2,s1,s2);
			if ((s2==0) ||(s2>l2)){
				printf("%d,%d,%d\n",s1,s2,l2);
				break;
			}
			if (f1&&style!=HCTOKEN){
				if (*ms=='#'){style=HBASHCOMMENT;s1=0;s2=l2;}
				char *ss=ms;
				int cc=0;
				while (*ss++=='>')cc++;
				if (cc){
					style=cc&1?HMAILODD:HMAILEVEN;
					s1=0;s2=l2;
				}
			}
			f1=FALSE;
			if (s1>0){
				float nx=x1+vv->StringWidth(ms,s1);
				if (nx>dest.left){
					SHC(csh->style[HNORM]);
					vv->DrawString(ms,s1,BPoint(x1,y1));
				}
				x1=nx;
			}
			if (s2>s1){
				float nx=x1+vv->StringWidth(ms+s1,s2-s1);
				if (nx>dest.left){
					SHC(csh->style[style]);
					vv->DrawString(ms+s1,s2-s1,BPoint(x1,y1));
				}
				x1=nx;
			}
			if (s2==0){
				printf("barf\n");
				break;
			}
		}
	}
}

void ViewBase::LocTxt2(int32 &x,int32 &y,int place){
	x=0;
	y=nt->Count(10,-1,place)-1;
	nplace *nk=nt->FindPrev(10,place-1);
	char spa[256];
	if (nk){
		int l=place-(nk->place);
		if (l>200)l=200;
		nt->Fetch(spa,l,nk->place);
		if (vmode==2){
//            l=MassageTxt(spa,l,sha);
			l=UTFWidth(spa,l);
		}
		x+=l;
	}
}

void ViewBase::LocTxt(float &x,float &y,int place){
	x=-10;y=0;
	y=(nt->Count(10,-1,place)-1)*fh;
	nplace *nk=nt->FindPrev(10,place-1);
	char spa[256],sha[256*4];
	x=4;
	if (nk){
		int l=place-(nk->place+1);
		l=MIN(l,250);
		nt->Fetch(spa,l,nk->place+1);
		if (vmode==2){
			l=MassageTxt(spa,l,sha);
			x+=vv->StringWidth(sha,l);
		}else{
			x+=vv->StringWidth(spa,l);
		}
	}
}
float ViewBase::LocTxtY(int place){
	return (nt->Count(10,-1,place)-1)*fh;
}


int ViewBase::MassageTxt(char *s,int l,char *dst){
	char *d=dst;
	for (int i=0;i<l;i++){
		char c=*s++;
		if ((c>31) || (c<0)){
			*d++=c;
		}else{
			if (c==9){
				*d++=32;
				*d++=32;
				*d++=32;
				*d++=32;
			}
		}
	}
	return d-dst;
}
