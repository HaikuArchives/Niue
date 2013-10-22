
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
#include <string.h>
#include <stdio.h>

#include <OS.h>

#include "Utils.h"
#include "NTree.h"
#include "cblist.h"
int ops[10];
//=============================

nnode::nnode(nnode *t){
	dad=t;
	cnt10=0;
	maxdepth=1;
}
nnode::~nnode(){}

//=============================

ntree *GetNT();
ntree *GetNT(){
	static int cnt=0;
	static ntree *spc;
	if (cnt==0){
		cnt=128;
		spc=new ntree[cnt];
	}
	cnt--;
	return spc++;
}
ndata *GetND();
ndata *GetND(){
	static int cnt=0;
	static ndata *spc;
	if (cnt==0){
		cnt=128;
		spc=new ndata[cnt];
	}
	cnt--;
	return spc++;
}

ndata::ndata(nnode *t,int special):nnode(t){
//	printf("New NData\n");
	dat=NULL;
	siz=asiz=canfree=0;
	alist=new CList(0);
	type=0;
//	printf("Special %d\n",special);
	if (special){
		nplace *np1=new nplace(10);
//		printf("dat=%x\n",dat);
		SizeTo(32);
//		printf("dat=%x\n",dat);
		dat[0]=10;
		dat[1]=10;
		siz=2;
//		printf("Attaching\n");
		Attach(np1,0);
//		printf("attach1\n");
		Attach(new nplace(10),1);
	}
//	printf("Complete\n");
}

ndata::ndata():nnode(NULL){
	dat=NULL;
	siz=asiz=canfree=0;
	alist=new CList(0);
	type=0;
}

ndata::~ndata(){
	if (canfree){
		free(dat);
	}
}

int ndata::Resize(){return siz;}

int ndata::Remove(int p1,int p2){
	int s=Size();
//	int pp1=p1;
	p1=BND(p1,0,s);
	p2=BND(p2,0,s);
	if (p1==p2 || p2==0)return 0;

	int i=alist->Num();
	while (i-->0){
		nplace *np=(nplace*)alist->Item(i);
		if (np->ofs>=p1){
			if (np->ofs>=p2){
				np->ofs+=p1-p2;
			}else{
				if (np->type>=128){
					np->ofs=p1;
				}else{
					np->Detach();
					delete np;
				}
			}
		}
	}

	memcpy(dat+p1,dat+p2,siz-p2);
	siz-=p2-p1;
	return p2-p1;
}
/*
				if (np->ofs==0 && pp1<0){
					np->ofs=0;
					ntree *top=(ntree*)dad;
					while(top->dad)top=(ntree*)top->dad;
					top->Attach(np,np->CalcPlace());
					printf("Tricky Stuff (%x,%x)\n",this,np->dest);
	
				}
*/

int ndata::Insert(const char *dt,int sz,int pl){
	if (pl<0) return 0;
	if (pl>=siz) return 0;

	for (int i=0;i<alist->Num();i++){
		nplace *np=(nplace*)alist->Item(i);
		if (np->ofs>=pl) np->ofs+=sz;
	}

	SizeTo(sz+siz);
	for (int i=siz-1;i>=pl;i--){
		dat[i+sz]=dat[i];
	}
	memcpy(dat+pl,dt,sz);
	siz+=sz;
	for (int i=0;i<sz;i++){
		char c=dat[pl+i];
		if (c==10 || c==13){
			Attach(new nplace(10),pl+i);
		}
	}
//    printf("Got %d (%d,%d)\n",dt[0],siz,sz);
	return sz;
}

int ndata::Fetch(char *dt,int sz,int place){
	if (place>Size()){return 0;}
	if (place<0){
		sz+=place;
		dt-=place;
		place=0;
	}
	sz=MIN(sz,Size()-place);
	if (sz<=0) return 0;
	memcpy(dt,dat+place,sz);
	return sz;
}

int ndata::SizeTo(int nu){
	if (nu>asiz){
		nu=(nu+15)&(~15);
//		printf("about to copy %x,%x(%d)\n",dat,siz,nu);
		char *nn=(char*)malloc(nu);
		if (siz && dat) memcpy(nn,dat,siz);
//		printf("copied\n");
		if (canfree && dat){
			delete(dat);
		}
		dat=nn;
		canfree=1;
		asiz=nu;
	}
	return 1;
}

int ndata::Resolve(int p1,int p2){
	p1=p2;
	return 0;
}


nplace *ndata::FindNext(int type,int place,int count)
{
	if (place>Size()) return NULL;
	nplace *bst=NULL;
	while (count-->0){
		bst=NULL;
		int slp=0x7fffffff;
		for (int i=0;i<alist->Num();i++){
			nplace *np=(nplace*)alist->Item(i);
			if (np->type!=type) continue;
			int df=np->ofs-place;
			if (df>=0 && df<slp){
				bst=np;
				slp=df;
			}
		}
		if (bst)place=bst->ofs+1;
	}
	if (bst){bst->place=bst->ofs;}
	return bst;
}
nplace *ndata::FindPrev(int type,int place)
{
	if (place<0) return NULL;

	nplace *bst=NULL;
	int slp=0x7fffffff;
	for (int i=0;i<alist->Num();i++){
		nplace *np=(nplace*)alist->Item(i);
		if (np->type!=type) continue;
		int df=-(np->ofs-place);
		if (df>=0 && df<slp){
			bst=np;
			slp=df;
		}
	}
	if (bst){bst->place=bst->ofs;}
	return bst;
}

int ndata::Count(int type,int p1,int p2){
	int cnt=0;
	if (p2<p1) return 0;
	if (p2<0 || p1>=Size()) return 0;
	if (p1<0 && p2>=Size() && type==10){return cnt10;}

	for (int i=0;i<alist->Num();i++){
		nplace *np=(nplace*)alist->Item(i);
		if (np->type!=type) continue;
		if (np->ofs>p1 && np->ofs<p2)cnt++;
	}
	return cnt;
}


int ndata::Attach(nplace *dat,int place){
	if (place<0 || place>=Size())return 0;
	dat->Detach();
	alist->Add(dat);
	dat->dest=this;
	dat->place=0;//place;
	dat->ofs=place;
	if (dat->type==10){
		nnode *ds=this;
		while (ds){
			ds->cnt10++;
			ds=ds->dad;
		}
	}
	return 1;
}

void ndata::SplitTo(ndata *n2,int pos){
	int s2=Size()-pos;
	if (s2){
		int s1=n2->Size();
		n2->SizeTo(s1+s2);
		for (int i=s1-1;i>=0;i--){
			n2->dat[i+s2]=n2->dat[i];
		}
		memcpy(n2->dat,dat+pos,s2);
		n2->siz+=s2;
		int j=n2->alist->Num();
		while(--j>=0){
			nplace *np=(nplace*)n2->alist->Item(j);
			np->ofs+=s2;
		}
	}
	int i=alist->Num();
	while (--i>=0){
		nplace *np=(nplace*)alist->Item(i);
		if (np->ofs>=pos) n2->Attach(np,np->ofs-pos);
	}
	siz-=s2;
}



//=================================

ntree::ntree(nnode *t):nnode(t){
	for (int i=0;i<MAXNTN;i++){
		data[i]=NULL;
	}
	type=1;
}

ntree::ntree():nnode(NULL){
	for (int i=0;i<MAXNTN;i++){
		data[i]=NULL;
	}
	type=1;
}

ntree::~ntree(){}

int ntree::Resize(){
	ops[0]++;
	int s=0,mx=0,c10=0;
	for (int i=0;i<MAXNTN;i++){
		if (data[i]){
			s+=data[i]->Size();
			mx+=data[i]->maxdepth;
			c10+=data[i]->cnt10;
		}
	}
	siz=s;
	maxdepth=mx;
	if (mx==0){
		printf("hollow\n");
	}
	cnt10=c10;
	return s;
}

int ntree::Insert(const char *dat,int size,int place){
	ops[1]++;
	if (place<0) return 0;
	if (size<=0) return 0;
	if (!dat) return 0;
	if (place>=siz) return 0;

	int s=0,oplace=place;
	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		s+=data[i]->Insert(dat,size,place);
		if (s){
			siz+=s;
			Balance((ntree**)data+i);
			break;
		}
		place-=data[i]->Size();
	}

	if (!s){
		printf("no add! %d,%d,%d\n",oplace,size,siz);
	}

	return s;
}

int ntree:: Fetch(char *dat,int size,int place){
	ops[2]++;
	if (place>Size()){return 0;}
	if (place+size<0){return 0;}

	int s=0;
	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		s+=data[i]->Fetch(dat,size,place);
		place-=data[i]->Size();
	}
	return s;
}

int ntree::Remove(int p1,int p2){
	ops[3]++;
	int ss=Size();
	p1=BND(p1,0,ss);
	p2=BND(p2,0,ss);
	if (p1==p2 || p2==0)return 0;
	int s=0;

	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		int ss=data[i]->Size();
		s+=data[i]->Remove(p1,p2);
		Balance((ntree**)data+i);
		p1-=ss;
		p2-=ss;
	}
	Resize();
	return s;
}

int ntree::Count(int type,int p1,int p2){
	ops[4]++;

	if (p2<p1) return 0;
	if (p2<0 || p1>Size()) return 0;
	if (p1<0 && p2>=Size() && type==10){return cnt10;}

	int s=0,c=0;
	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		c+=data[i]->Count(type,p1-s,p2-s);
		s+=data[i]->Size();
	}
	return c;
}

int ntree::Attach(nplace *dat,int place){
	ops[5]++;
	if (!dat) return 0;
	if (place<0 || place>=Size()) return 0;
	int s=0,c=0;
	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		c=data[i]->Attach(dat,place-s);
		if (c) return 1;
		s+=data[i]->Size();
	}
	return 0;
}

nplace *ntree::FindPrev(int type,int place){
	ops[6]++;

	if (place<0) return NULL;
	int s=Size();
	for (int i=MAXNTN-1;i>=0;i--){
		if (!data[i])continue;
		s-=data[i]->Size();
		nplace *rr=data[i]->FindPrev(type,place-s);
		if (rr){
			rr->place+=s;
			return rr;
		}
	}
	return 0;
}
nplace *ntree::FindNext(int type,int place,int count){
	ops[7]++;
	if (place>Size()) return NULL;
	int s=0;
	for (int i=0;i<MAXNTN;i++){
		if (!data[i])continue;
		if (type<128){
			int cc=data[i]->Count(type,place-s-1,0x7fffffff);
			if (cc>=count){
				nplace *rr=data[i]->FindNext(type,place-s,count);
				if (rr){rr->place+=s;return rr;}
			}
			count-=cc;
		}else{
			nplace *rr=data[i]->FindNext(type,place-s,count);
			if (rr){rr->place+=s;return rr;}
		}
		if (count<0){
			printf("Problems in FindNext()\n");
			return NULL;
		}
		s+=data[i]->Size();
	}
	return NULL;
}

void DoDad(nnode *dad,nnode *daughter);
void DoDad(nnode *dad,nnode *daughter){
	if (daughter)daughter->dad=dad;
}

void Balance(ntree **ntt,int ){
	if (!ntt) return;
	for (;;){
		ntree *nt=*ntt;
		if (!nt) return;
		if (nt->type!=1) return;

		if (nt->dad){
			if (!nt->data[0]){
//				printf("SliceL\n");
				*ntt=(ntree*)nt->data[1];
				DoDad(nt->dad,*ntt);
				continue;
			}
			if (!nt->data[1] ){
//				printf("SliceR\n");
				*ntt=(ntree*)nt->data[0];
				DoDad(nt->dad,*ntt);
				continue;
			}

			if (nt->Size()<16){
				Balance((ntree**)nt->data+0);
				Balance((ntree**)nt->data+1);
//				printf("Null thingy (%x,%x,%x)\n",nt,nt->data[0],nt->data[1]);
				if (nt->Size()>=16){
					printf("aaarrrrggghh\n");
					continue;
				}
				((ndata*)nt->data[0])->SplitTo((ndata*)nt->data[1],0);
				nt->data[0]=NULL;
				continue;
			} //killer
		}

		for (int hh=0;hh<MAXNTN;hh++){
			ndata *ev=(ndata*)nt->data[hh];
			if (ev && ev->type==0){
				int  ss=ev->Size();
				if (ss>48*2){
					int sp=ss/2;
					ntree *nk=GetNT();
					nk->dad=nt;
					nk->data[0]=ev;
					nk->data[0]->dad=nk;
					nt->data[hh]=nk;
					nk->Resize();
					nk->cnt10=ev->cnt10;

					nk->data[1]=GetND();
					nk->data[1]->dad=nk;
					((ndata*)nk->data[0])->SplitTo((ndata*)nk->data[1],sp);
					Balance((ntree**)nt->data+hh);
				}
			}
		}

		nt->Resize();
		int d0=0,d1=0;
		if (nt->data[0])d0=nt->data[0]->maxdepth;
		if (nt->data[1])d1=nt->data[1]->maxdepth;

		if ( (d1>1) && (d1>d0*2) ){
//			printf("splittingL (%x,%d,%d)\n",nt,d0,d1);
			ntree *nx=(ntree*)nt->data[1];
			*ntt=nx;
			nx->dad=nt->dad;

			nt->data[1]=nx->data[0];
			DoDad(nt,nt->data[1]);

			nx->data[0]=nt;
			nt->dad=nx;
			nt->Resize();
			nx->Resize();

			Balance((ntree**)nx->data+0);
			Balance((ntree**)nx->data+1);
			break;
		}

		if ( (d0>1) && (d0>d1*2) ){
//	         printf("splittingR (%x,%d,%d)\n",nt,d0,d1);
			ntree *nx=(ntree*)nt->data[0];
			*ntt=nx;
			nx->dad=nt->dad;

			nt->data[0]=nx->data[1];
			DoDad(nt,nt->data[0]);

			nx->data[1]=nt;
			nt->dad=nx;
			nt->Resize();
			nx->Resize();

			Balance((ntree**)nx->data+0);
			Balance((ntree**)nx->data+1);
			break;
		}
		break;
	}
}


int ntree::Resolve(int ,int){return 0;}


int ntree::Show(int l){
	l++;
	char *sp="                                                                           ";
	int ll=strlen (sp);
	for (int i=0;i<MAXNTN;i++){
		if (!data[i]) continue;
		printf ("%s%d",sp+ll-l,data[i]->Size());
		if (data[i]->type==0){
			printf("(+%d)",((ndata*)data[i])->alist->Num());
		}
		printf("\n");
		if (data[i]->type==1) ((ntree*)data[i])->Show(l);
	}
	return 0;
}


//=================================


nplace::nplace(int i){
	type=i;
	dest=NULL;
//		printf("half att\n");
}
nplace::~nplace(){}

int nplace::Detach(){
	if (!this) return 0;
	if (dest){
		int idx=((ndata*)dest)->alist->IndexOf(this);
		((ndata*)dest)->alist->RemIndex(idx);
		if (type==10){
			nnode *ds=dest;
			while (ds){
				ds->cnt10--;
				ds=ds->dad;
			}
		}
		dest=NULL;
	}
	return 1;
}

int nplace::CalcPlace(){
	if (!this){
		printf("wonky CalcPlace!\n");
		return -1;
	}
	int ss=ofs;
	nnode *son=dest;
	if (!son) return -1;
	nnode *dad=son->dad;
	while (dad){
		int s2=0;
		for (int i=0;i<MAXNTN;i++){
			nnode *tst=((ntree*)dad)->data[i];
			if (!tst) continue;
			if (tst==son){
				ss+=s2;
				break;
			}
			s2+=tst->Size();
		}
		son=dad;dad=son->dad;
	}
	return ss;
}
