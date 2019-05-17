
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
#include <stdlib.h> //abs,atol function..
#include <string.h>

#include <unistd.h>
#include <signal.h> //darn posix!
#include <Autolock.h>
#include <Path.h>
#include <Beep.h>
#include <NodeInfo.h> //mime..
#include <Directory.h> //puttodir
#include <image.h> //TermHIre
#include <OS.h> //threads..
#include <FilePanel.h>
#include <Roster.h>
#include <Application.h> //be_app
#include <Alert.h>
#include <Clipboard.h>
#include <SupportDefs.h>

	struct UndoOp
	{
		char *dat;
		int p0,p1;
		int type;
	};


#include "cblist.h"
#include "Niue.h"
#include "UBuffer.h"
#include "NTree.h"
#include "Utils.h"

inline void MIX(int32 &a,int32 &b,int32 c){
	if (c<0)return;
	a=MIN(a,c);
	b=MAX(b,c);
}


char lkname[64]="Document 1";
Buff::Buff():BLooper(lkname){
	static int bf=1;
	sprintf(lkname,"Document %d",++bf);
	//printf("New UBuff\n");
	now=0;
	mybuf=NULL;
	entchanged=TRUE;
	for (int  i=0;i<257;i++){
		mypos[i]=NULL;
	}
	winlist=new CList();
	savepanel=NULL;

	mytree=new ntree(NULL);
	((ntree*)mytree)->data[0]=new ndata(mytree,TRUE);
	mytree->Resize();
	curs=140;
	MoveTo(curs,1);

	undostack=new CList();
	undoitem=0;
	usemarkers=TRUE;
	submerged=NULL;
//    foldlist=new tlist();

	lastchanged=0;
	alone=TRUE;
	term=FALSE;
	utf8=TRUE;
	rdthread=0;
	expid=0;
	pipefd[0]=pipefd[1]=0;
	cmdbuff=NULL;
	cmime[0]=0;
	isrec=TRUE;
	marklevel=0;
	encoding=0;
}

Buff::~Buff(){
	if (savepanel){
		delete (savepanel);
		savepanel=NULL;
	}
	for (int i=0;i<2;i++){
		if (pipefd[i]){
			close (pipefd[i]);
			pipefd[i]=0;
		}
	}
	FreeBuff();
}

void Buff::CheckBuff(){
	if (!mybuf){
		bsize=1024*512;
		mybuf=(char*)malloc(bsize);
	}
}

void Buff::FreeBuff(){
	if (mybuf){
		free(mybuf);
		mybuf=NULL;
	}
}


int myalnum(int32 v);
int myalnum(int32 v){
	v=v&255;
	if (v>'Z'){
		if (v>'z')
			return (v>=128);
		else
			return (v>='a');
	}else{
		if (v>'9')
			return (v>'A');
		else
			return (v>='0');
	}
}

void Buff::Squash(long cn,long typ)
{
	Own;
	cn++;
	typ=typ;
}

void Buff::Push(UndoOp *uo){
	undostack->Add(uo);
	undoitem=undostack->Num();
}

void Buff::PushMarker(int32 dat){
	if (!isrec) return;

	if (usemarkers){
		UndoOp *uo=(UndoOp*)undostack->Item(undostack->Num()-1);
		if (uo && uo->type==-1){
			if (uo->p0<dat)uo->p0=dat;
			return;
		}

		uo=new UndoOp();
		uo->type=-1;
		uo->p0=dat;
		Push(uo);
	}
}

void Buff::PushDelete(int32 p0,int32 p1)
{
	if (!isrec) return;
	UndoOp *up=new UndoOp();
	up->p0=p0;
	up->p1=p1;
	up->type=0;
//	printf("Pushdel %d\n",p1-p0);
	Push(up);
}

void Buff::PushAsc(int32 p0,char *txt,int32 len){
	if (len<=0) return;
	if (!isrec) return;
	UndoOp *uo=new UndoOp();
	uo->p0=p0;
	uo->dat=(char*)malloc(len);
	strncpy(uo->dat,txt,len);
	uo->type=1;
	uo->p1=len;
	Push(uo);
}


void Buff::Undo(NiueWindow *yw,int32 cn,int32 &std1,int32 &std2,int32 dir){
	if (dir!=-1){
		//printf(yw,"Warning, Redo doesn't work");
		//(new BAlert("Niue", "Warning, Redo doesn't work", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
	}
	int un=undostack->Num();

	if (!un){
		beep();
//		printf(yw,"Nothing To Undo");
		return;
	}
	int nundo=undoitem;
	PushMarker(20);
	usemarkers=FALSE;

	while (TRUE){
		nundo+=dir;
		if (nundo<0){
			nundo=0;
			beep();
//			printf(yw,"Nothing To Undo");
			break;
		}
		if (nundo>undostack->Num()){
			nundo=0;
			beep();
//			printf(yw,"Nothing To Redo");
			break;
		}

		UndoOp *uo=(UndoOp*)undostack->Item(nundo);
		if (!uo ){
			/*marker*/
			break;
		}

		if (uo->type==-1){
			if (uo->p0>=marklevel)break;
		}

		switch (uo->type){
			case 0:
//				printf("unrem %d\n",uo->p1-uo->p0);
				Remove(uo->p0,uo->p1);
				MoveTo(cn,uo->p0);
				MIX(std1,std2,uo->p0);
				MIX(std1,std2,uo->p1);
				break;
			case 1:
//				printf("unadd %d\n",uo->p1);
				MoveTo(cn,uo->p0);
				AddChar(cn,uo->dat,uo->p1,0);
				MIX(std1,std2,uo->p0);
				MIX(std1,std2,uo->p0+uo->p1);
				break;
		}
	}
//	printf("undone\n");
	usemarkers=TRUE;
	undoitem=nundo;
	if (nundo==0){
		Change(0);
	}
}

int32 Buff::AddChar(int32 cn,const char *s,int32 cnt,int32 dir){
	if (cnt<=0) return 0;
//	printf("AC %d\n",cnt);
	int pl=Pos(cn);
//	int ppl=pl;
	pl=BND(pl,1,mytree->Size()-1);
	MoveTo(cn,pl);
	PushDelete(pl,pl+cnt);
	int32 ll=AddCharReal(cn,s,cnt);
	if (dir){
		MoveFlat(cn,-ll);
		switch (dir){
			case 1:MoveBy(cn,0,1);break;
			case 2:MoveBy(cn,0,-1);break;
			case 3:MoveBy(cn,-1,0);break;
		}
	}
	return ll;
}

int32 Buff::AddCharReal(int32 cn,const char *s,int32 cnt)
{
	long length = cnt;	//save the length to decide whether we should do codecompletion
	
	if (cnt<=0) return 0;
	int ss=0;
	int32 ms=64;
	while (cnt>ms)
	{
		ss+=AddCharReal(cn,s,ms);
		s+=ms;
		cnt-=ms;
	}
	

	Own;
	int pl=Pos(cn);
	if (pl==-1){MoveTo(cn,pl=1);}
	now++;
	ss+=mytree->Insert(s,cnt,pl);
	Balance((ntree**)&mytree);
	Change(1);

	//sent the last typed word
	if(length < 6)		//if the chunk is not to big
	{
	
		char text[1];
		BString word = "";
		bool cha = true;
		int i = pl;
	
		while(cha)
		{
			int32 ll=Fetch(text, 1, i);
			if(ll==1)
			{
				if(strncmp(text, " ", 1) == 0 || strncmp(text, "\t", 1) == 0 || strncmp(text, "\n", 1) == 0 || strncmp(text, ".", 1) == 0 || strncmp(text, ">", 1) == 0 || strncmp(text, "(", 1) == 0 || strncmp(text, ")", 1) == 0)
				{
					cha = false;
				}
				else
				{
					word.Prepend(text[0], 1);
					i--;	
				}
			}
		}
	
		//puts(word.String());			//here we should send it to a hash-compare function instead
		if(strlen(word.String()) > 1 && strlen(word.String()) < 6)
		{
			BMessage msg(Y_COMPARE_HASH);
			msg.AddString("word", word);
			be_app->PostMessage(&msg);
		}
	
	}
	//end of 'send last typed word'
	
	return ss;
}

int32 Buff::Remove(int32 p0,int32 p1)
{
	if (p1<=p0) return 0;
	int sm=mytree->Size()-1;
	p0=BND(p0,1,sm);
	p1=BND(p1,1,sm);
	if (p1<=p0) return  0;
	now++;

//	CheckBuff();
//	int32 ll=mytree->Fetch(mybuf,p1-p0,p0);
	//PushAsc(p0,mybuf,ll);
	if (isrec){
		UndoOp *uo=new UndoOp();
		uo->p0=p0;
		int32 len=p1-p0;
		uo->dat=(char*)malloc(len);
		int32 ll=mytree->Fetch(uo->dat,p1-p0,p0);
		if (ll!=len)
			printf("ARGH! %ld,%ld\n",ll,len);
		uo->type=1;
		uo->p1=len;
		Push(uo);
	}


	int32 ss1=mytree->Size();
	int ss=mytree->Remove(p0,p1);
	int32 ss2=mytree->Size();
	if (ss1!=ss2+(p1-p0))
		printf("weird remove %ld,%ld  %ld,%ld\n",p0,p1,ss1,ss2);
//	printf("rem %d,%d\n",ll,p1-p0);
	Balance((ntree**)&mytree);
	return ss;
}

void Buff::MyDelChar(int32 cn ,int32 dir,int32 typedir)
{
	Own;
	int p1=Pos(cn),p2=p1;
	if (dir== 1){
		MoveBy(cn,1,0);	//UTF!
		p2=Pos(cn);//++;
	}
	if (dir==-1){
		MoveBy(cn,-1,0);
		p1=Pos(cn);//p1--;
	}
	int32 ss=Size()-1;
	p1=BND(p1,1,ss);
	p2=BND(p2,1,ss);
	printf("deleting from %d to %d\n",p1,p2);
	int32 ff=Remove(p1,p2);
	if (ff){
		now++;
		dir=ABS(1-dir)>>1;
		switch (typedir){
			case 1:MoveBy(cn,dir,1);break;
			case 2:MoveBy(cn,dir,-1);break;
		}
		Change(1);
	}
}



void Buff::GetFile(BFile *f){
	ssize_t len;
	Own;
	Capture();
	isrec=FALSE;
	CheckBuff();
	do{
		len=f->Read(mybuf,bsize);
		AddChar(0,mybuf,len);
	}while (len>0);
	FreeBuff();
	Change(0);
	isrec=TRUE;
	undoitem=0;
	undostack=new CList();
	now=0;
	Balance((ntree**)&mytree);
//  ((ntree*)mytree)->Show(0);
	for (int i=140;i<160;i++){
		if (Pos(i)>=0)MoveTo(i,1);
	}

	TX(1026,0);
}

void Buff::GetFromEnt(entry_ref *er){
	Own;
	BFile fil(er,B_READ_ONLY);
	if (fil.IsReadable() ){
		myent=*er;
		fil.Seek(0,SEEK_SET);

		GetFile(&fil);
		BNode   nod(&myent);
		BNodeInfo myinfo(&nod);
//		status_t rr=myinfo.GetType(cmime);
		myinfo.GetType(cmime);
	}
}

void Buff::PutToEnt(){
	Own;
	
	BFile fil(&myent,B_WRITE_ONLY);
	if (fil.IsWritable()){
		PutFile(&fil);
	}else{
		FError("Couldn't open file for writing");
	}
}


void Buff::PutToDir(entry_ref *ref,char *name){
	Own;
	BDirectory mydir(ref);

	BFile dummy;//(mydir,name,B_READ_ONLY);
	mydir.CreateFile(name,&dummy,TRUE);
	BEntry ben(&mydir,name);
	entry_ref nuref;
	ben.GetRef(&nuref);
	myent=nuref;
//    printf("%d,%d,'%s','%s'\n",0,ben.InitCheck(),nuref.name,myent.name);
	PutToEnt();
}


void Buff::IndentiQuote(int32 cnum,YAction type,int32 &std1,int32 &std2){
	Own;
	int32 p1,p2,ll=0;

	p1=Pos(141);
	p2=Pos(142);
	SORT32(p1,p2);
	if (p1==-1 || p1==p2){p1=p2=Pos(cnum);}

	int32 s1=0x7fffffff,s2=0;

//	char *tabs="        ";// 8 spaces..
	char spa[32];

	nplace *np=mytree->FindPrev(10,p2-1);
	nplace *bnp=np;

	while (np){
		switch (type)
		{
			case A_UNQUOTE:
			{
				ll=mytree->Fetch(spa,1,np->place+1);
				if (ll && (spa[0]=='>'))
					Remove(np->place+1,np->place+2);
				break;
			}
			case A_UNDENT:
			{
				ll=mytree->Fetch(spa,1,np->place+1);
				if (ll && (spa[0]=='\t' || spa[0]==' '))
					Remove(np->place+1,np->place+2);
				break;
			}
			case A_QUOTE:
			{
				mytree->Insert(">",1,np->place+1);
				PushDelete(np->place+1,np->place+2);
				p2++;
				break;
			}
			case A_INDENT:
			{
				mytree->Insert("\t",1,np->place+1);
				PushDelete(np->place+1,np->place+2);
				p2++;
				break;
			}
			default:
			{
				break;
			}
		}
		s1=np->place;
		if (np->place<p1)
			break;
		np=mytree->FindPrev(10,np->place-1);
	}
	if (bnp){s2=bnp->CalcPlace();}
	MIX(std1,std2,s1);
	MIX(std1,std2,s2+1);
	Change(1);
}



void Buff::PutMIMEType(int32 type){
	Own;
	BNode   nod(&myent);
	BNodeInfo myinfo(&nod);

	cmime[0]=0;
	status_t rr=myinfo.GetType(cmime);
	printf("NIUE::Kind was '%s',",cmime);
	char *typ=MyMimeType(type);
	if (typ){
		myinfo.SetType(typ);
		rr=myinfo.GetType(cmime);
		printf(" now '%s' (Tracker Optimisation: icon won't change)\n",cmime);
	}
}

void Buff::PutFile(BFile *f)
{
//	long n=0;

	Own;
	CheckBuff();
	f->SetSize(0);
	int32 pc=1;
	int32 ss=mytree->Size()-1;
	char *base=mybuf;
	int mymax=bsize;
	if(encoding){
		mymax/=2;
		base+=mymax;
	}
	int last=0;
	while (pc<ss){
		int sz=MIN(ss-pc,mymax);
		mytree->Fetch(base,sz,pc);
		int rsz=sz;
		if (encoding){
			int j=0;
			for (int i=0;i<sz;i++){
				char s=base[i];
				if (last==13 && s==10){
//					printf("skipping\n");
				}else{
					int ss=s;
					if (s==10 || s==13){
						if (encoding==2){mybuf[j++]=13;}
						ss=10;
					}
					mybuf[j++]=ss;
				}
				last=s;		
			}
			rsz=j;
			#if defined(_DebugObj)
				printf("%d bytes (was %d bytes)\n",rsz,sz);
			#endif
		}
		ssize_t written=f->Write(mybuf,rsz);
		if (written!=rsz){
			FError("An error occured writing to the file");
			break;
		}
		pc+=sz;
	}
	FreeBuff();
	lastchanged=1;
	Change(0);
}

status_t Buff::Find(const char *src,int32 &x1,int32 &x2,int32 dir){
	Own;
	if (x2<x1) return 0;
	if (!src || !*src){return 0;}
	CheckBuff();
	int l1=strlen(src);
//	int l2 = mytree->Fetch(mybuf,x2-x1,x1);
	mytree->Fetch(mybuf,x2-x1,x1);
	if (dir==1){
		int pt=-1;
		while (++pt<(x2-x1)){
			if (strncasecmp(src,mybuf+pt,l1)) continue;
			x1+=pt;
			x2=x1+l1;
			return l1;
		}
	}else{
		int pt=x2-x1-1;
		while (--pt>=0){
			if (strncasecmp(src,mybuf+pt,l1)) continue;
			x1+=pt;
			x2=x1+l1;
			return l1;
		}
	}
	return 0;
}



int32 Buff::ReaderFunc(void *bu){
	Buff *bb=(Buff*)bu;
	int32 cn=128;

//    close(bb->pipefd[0]);
	close(bb->pipefd[1]);
//    printf("Readerfunc started (%x,%x)\n",bb,f);

	ssize_t len,maxlen=32768;
	char *mybuf=(char*)malloc(maxlen);
	while ( (len=read(bb->pipefd[0],mybuf,maxlen))!=0){
//            printf("%x,%x,%x\n",dd,mybuf,len);

		BAutolock l2(bb);
		bb->Capture();
		bb->PushMarker(10);
//		int32 dl=bb->AddChar(cn,(char*)mybuf,len);
		bb->AddChar(cn,(char*)mybuf,len);
//      bb->MoveFlat(cn,dl);
		bb->TX(1026);
	}

//    printf("finished reading\n");

	int32 res=close(bb->pipefd[0]);

	char *dst=(char*)mybuf;
	if (res){
		dst+=sprintf(dst,"  Return Code=%ld",res);
		if (res<0 || res>9){
			dst+=sprintf(dst,"(0x%lx)",res);
		}
		beep();
	}else{
		dst+=sprintf(dst,"  Execution Complete.");
	}
	dst+=sprintf(dst,"\n");

	BAutolock l3(bb);
//	printf(bb->submerged);
	bb->expid=0;
	bb->Capture();
	bb->PushMarker(10);
//	int dl=bb->AddChar(cn,(char*)mybuf,strlen((char*)mybuf));
	bb->AddChar(cn,(char*)mybuf,strlen((char*)mybuf));
//      bb->MoveFlat(cn,dl);
	bb->TX(1026);

	free (mybuf);
	bb->rdthread=0;
//    printf("ReaderFunc return\n");
//    while (1){snooze(10000000.0);}
	return 0;
}


int32 Buff::StripToPound(int32 sty)
{
	return sty;
}

status_t Buff::MyExec(NiueWindow *yw,int32 mcs,char *cmd){
	if (!cmd || !*cmd) return 0;
	Own;
	mcs=mcs;
	submerged=yw;

	int res=pipe(pipefd);

	if (res<0){
		printf("NO PIPE!\n");
		exit(0);
	}

	if (cmdbuff==NULL){
		cmdbuff=(char*)malloc(32768);
	}
	strncpy (cmdbuff,cmd,32760);

	expid=fork();
	if (expid==0){
//        printf("writerfunc started\n");

		fflush(stdout);close(STDOUT_FILENO);dup(pipefd[1]);
		fflush(stderr);close(STDERR_FILENO);dup(pipefd[1]);
		close(pipefd[0]);//no readers..
		close(pipefd[1]);

		char *args[4];
		args[0]="/bin/sh";
		args[1]="-c";
		args[2]=cmdbuff;
		args[3]=NULL;
		execvp(args[0],args); //doesn't return
		printf("ARGH! DON'T RETURN!!\n");
	}else{
		rdthread=spawn_thread(ReaderFunc,"Reader",B_NORMAL_PRIORITY,this);
		if (rdthread>0){
			resume_thread(rdthread);
		}else{
			printf("massive failure in %s (%d)!\n",__FILE__,__LINE__);
			rdthread=0;
		}
	}
//    printf("forking %d,%d\n",expid,rdthread);
	return 0;
}

status_t Buff::CtrlChar(int32 cc){
	Own;
//    printf("Control Char (%d)!\n",cc);
	if (expid){
		if (cc==A_CTRL_CHAR3){
			send_signal(expid,SIGINT);
		}
		if (cc==A_CTRL_CHAR26){
			send_signal(expid,SIGTERM);
		}
	}
	return B_OK;
}

void Buff::Change(int32 type)
{
	Own;
	now++;
	int pst=((!type)^(!lastchanged));

	lastchanged=(type)?system_time():0;


if (pst)
{
		entchanged=TRUE;
		BMessage msg(Y_BUFF_CHANGE);
		msg.AddPointer("Buff",this);

		be_app->PostMessage(&msg);
}

}

int32 CopyToQuote(char *dest, char *src,int32 max);
int32 CopyToQuote(char *dest, char *src,int32 max){
	int32   cc=0;
	while (max-->0){
		if (*src==0)break;
		if (*src=='\"')break;
		*dest++=*src++;
		cc++;
	}
	*dest=0;
	return (cc);
}

int matol (char *src);
int matol (char *src){
	int v=0;
	while (1){
		char c=*src++;
		if (c>='0' && c<='9'){
			v=v*10+c-'0';
		}else{
			break;
		}
	}
	return v;
}

int mstrcmp(char *s,char *d,int c);
int mstrcmp(char *s,char *d,int c){
	if (!s || !d){printf("poos\n");return 0;}

	while (c--){
		char ss=*s++;
		char dd=*d++;

		if (!ss || !dd || ss!=dd) return -1;
	}
	return 0;
}

int mslen(char *s);
int mslen(char *s){
	if (!s){printf("argh!\n");return 0;}
	int c=0;
	while (TRUE){
		char cc=*s++;
		if (!cc) break;
		if (c++>1024){
			printf("string too long!\n");
			return 0;
		}
	}
	return c;
}

int runcolon(char *s,int off=32);
int runcolon(char *s,int off){
	int count=0;
	while ( count<128 ){
		signed char c=s[count];
		if (c==':') return count;
		if (c<=off ) return 0;
		count++;
	}
	return 0;
}

int32 Buff::NextError(NiueWindow *yw,char *txt, int32 *line){
	Own;
	*line=0;
	int32 sty=MAX(Pos(129),1);
	CheckBuff();
	int32 ll=mytree->Fetch(mybuf,bsize,sty);
	char *nu=mybuf;
	while (nu<mybuf+ll){
		//Metrowerks style:
		while (1){
			if (mstrcmp(nu,"File \"",6)) break;
			nu+=6;
			nu+=CopyToQuote(txt,nu,250)+1;
			if (mstrcmp(nu,"; Line",6)) break;
			*line=matol(nu+7);

			nplace *np1=NULL,*np2=NULL;
			np1=mytree->FindPrev(10,sty+(nu-mybuf));
			if (np1)np1=mytree->FindPrev(10,np1->place-1);
			if (np1)np2=mytree->FindPrev(10,np1->place-1);
			if (np2){
				char sha[256];
				int cc=mytree->Fetch(sha,MIN(250,np1->place-np2->place-1),np2->place+1);
				sha[cc]=0;
//				printf(yw,sha);
			}

			MoveTo(129,nu-mybuf+sty);
			return TRUE;
		}

		//gcc style
		//bozo.c:3: some_header_that_does_not_exist.h: No such file or directory
		while (1){
			int ff=runcolon(nu);
			if (!ff)break;
			int gg=runcolon(nu+ff+1);
			if (!gg) break;
			nplace *nx=mytree->FindNext(10,nu-mybuf+sty);
			if (!nx) break;
			char sha[256];
			int l=nx->place-(nu-mybuf+sty+ff+gg+2);
			l=MIN(l,200);
			int ll=mytree->Fetch(sha,l,nu-mybuf+sty+ff+gg+2);
			sha[ll]=0;
//			printf(yw,sha);

			strncpy(txt,nu,ff);
			txt[ff]=0;
			*line=matol(nu+ff+1);
			MoveTo(129,nx->place);
			return TRUE;
		}
		nu++;
	}
	MoveTo(129,-1);
	return 0;
}

#include <Alert.h>
void Buff::FError(const char *msg){
	char    myspace[256];
	sprintf(myspace,"%s: %s",mytitle+1,msg);
	BAlert *al=new BAlert(mytitle,myspace,"OK");
	al->Go();
}

long UTFWidth(char *s,long l){
	if (!s)return 0;
	int t=0;

	while (l-->0){
		uint c=*s++;
		if ((c&0xc0)!=0x80){
			if (c>31)t++;
			if (c==9)t+=4;
		}
	}
	return t;
}


long
WidthUTF(char *s,long &l,long max)
{
	if (!s)
		return 0;
	int t = 0;
//	int ff = 0;
	while (l > 0 && t++ <= max)
	{
		uint c=*s++;
		if ((c & 0xc0) != 0x80)
		{
			if (c > 31)
				l--;
			if (c == 9)
				l-=4;
		}
	}
	return t;
}


char *MyMimeType(int idx){
	char *tt=NULL;
	switch (idx){
		case 0:tt="";break;
		case 1:tt="text/x-source-code";break;
		case 2:tt="text/plain";break;
		case 3:tt="text/html";break;
		case 4:tt="text/x-email";break;
		case 5:tt="text/x-source-code";break;
	}
	return tt;
}

int32 Buff::Paste(BMessage *msg){
	char    *data=NULL;
	long    ll=0;

	//    msg->PrintToStream();
	status_t rr=msg->FindData("text/plain",B_MIME_TYPE,(const void**)&data,&ll);

	BClipboard *cl=NULL;
	if (rr<B_OK){
		cl=be_clipboard;
		cl->Lock();
		msg=cl->Data();
		rr=msg->FindData("text/plain",B_MIME_TYPE,(const void**)&data,&ll);
	}


	if (rr>=B_OK && ll>=0){
		PushMarker(10);
		ll=AddChar(curs,data,ll,cws?cws->typedir:0);
	}else{
		ll=0;
		beep();
	}
	if (cl) cl->Unlock();
	return ll;
}

int32 Buff::CopyQuote(int pp1,int pp2,int type){
	SORT(pp1,pp2);
	if (pp1==-1)return 0;
	int32 ss=mytree->Size()-1;
	pp1=BND(pp1,1,ss);
	pp2=BND(pp2,1,ss);
	CheckBuff();
	int32 ll=mytree->Fetch(mybuf,pp2-pp1,pp1);
	if (ll<=0){ beep();return 0;}
	BClipboard *cl=be_clipboard;
//  printf("lock\n");
	cl->Lock();
//  printf("clear\n");

	cl->Clear();
//  printf("msg\n");
	BMessage    *ms=cl->Data();
	ms->AddData("text/plain",B_MIME_TYPE,mybuf,ll);
//  printf("commit\n");
	cl->Commit();
//  printf("unlock\n");
	cl->Unlock();
//  printf("done\n");

	if (type==B_CUT ||
		type==A_CUT_LINE){
		PushMarker(10);
		Remove(pp1,pp2);
		Change(1);
		if (type!=A_CUT_LINE){
			MoveTo(141,-1);
			MoveTo(142,-1);
		}
	}
	return ll;
}


void Buff::MessageReceived(BMessage *msg){
	BAutolock l1(this);
	MessageReceived2(msg);

	BMessageQueue *mq=MessageQueue();

	while ((msg=mq->FindMessage((int32)0))!=NULL){
		MessageReceived2(msg);
		mq->RemoveMessage(msg);
	}
}


void
Buff::CCMsg(int32 am,int32 what,NiueWindow *yw)
{
	char *str = NULL;
	switch (what)
	{
		case A_CUT_LINE:
		case B_CUT:
		{
			str = "Cut";
			break;
		}
		case A_COPY_LINE:
		case B_COPY:
		{
			str = "Copy";
			break;
		}
		case A_KILL_SELECTION:
		{
			str = "Remove";
			break;
		}
	}

	char sha[256], *spa=sha;
	if (am)
	{
		spa += sprintf(spa,"%ld byte",am);
		if (am > 1)
			*spa++ = 's';
		*spa++ = 32;
		spa += sprintf(spa,"%s",str);
	}
	else
		spa += sprintf(spa,"Nothing to %s",str);
//	printf(yw,sha);
}


void Buff::MessageReceived2(BMessage *msg)
{
	int32 dx = 0, dy = 0, am = 0, txn = 0;
	int32 std2 = 0, std1 = mytree->Size();
//	int32 ls2 = std2;
	int32 pp1 = -1, pp2 = -1, ll = 0;
//	int32 df = 0;
	
	char *text = NULL;
	msg->FindString("Text",(const char**)&text);
	
	NiueWindow *yw = NULL;
	msg->FindPointer("YWin",(void**)&yw);
	
	WinStats *ws=NULL;
	if (yw)
	{
		for (int i=winlist->Num()-1;i>=0;i--)
		{
			WinStats *ww=(WinStats*)winlist->Item(i);
			if (ww->yw==yw)
			{
				ws = ww;
				break;
			}
		}
	}
//	else
//      printf("eek, no win!\n"); // not too serious (!)

	if (!ws)ws=(WinStats*)winlist->Item(0);
	cws=ws;
	curs=ws?ws->cursor:140;
	Capture();
	switch (msg->what){
		case A_MOVE_BOTTOM:dy=500000;break;
		case A_MOVE_TOP:dy=-500000;break;
		case A_PAGE_UP:dy=-ws->pgdn;break;
		case A_PAGE_DOWN:dy= ws->pgdn;break;
		case A_MOVE_LEFT:dx=-1;break;
		case A_MOVE_RIGHT:dx= 1;break;
		case A_MOVE_UP:dy=-1;break;
		case A_MOVE_DOWN:dy= 1;break;
		case A_START_LINE:dx=-65536;break;
		case A_END_LINE:dx=65536;break;
		case SAVE_FILE:
			ll=FALSE;
			{
				BEntry ben(&myent);
				if (ben.InitCheck()==B_NO_ERROR){
					printf("Saving..");
					PutToEnt();
					ll=TRUE;
				}
			}
			if (ll){
				printf("Saved");
				break;
			}else{
				printf("Couldn't Open File");
				//fall through to SAVE_AS_FILE
			}
		case SAVE_AS_FILE:
			if (!savepanel){
				BMessenger *mymsgr=new BMessenger(this);
				BMessage ms(MY_SAVE_FILE);
				entry_ref anent(myent.device,myent.directory,".");
				savepanel=new BFilePanel(B_SAVE_PANEL,mymsgr,
						&anent,0,FALSE,&ms,
						NULL,FALSE,TRUE);
//				char txt[1024];
//				sprintf(txt,"Save '%s'",mytitle);
//				savepanel->Window()->SetTitle(txt);
				savepanel->Show();
			}
			break;

		case B_CANCEL:
			if (savepanel){
				delete (savepanel);
				savepanel=NULL;
//                sview->ShowMsg("SavePanel Closed");
			}
			break;
		case A_SPLIT_LINE:
			text="\n";
			if (term){
				printf("Executing");
				nplace *np=mytree->FindPrev(10,Pos(curs)-1);
				if (!np){printf("no start line\n"); break;}
				nplace *nx=mytree->FindNext(10,Pos(curs));
				if (!nx){printf("no stop line\n"); break;}
				int32 l1=MIN(nx->place-np->place-1,200);
				
				char sha[256];
				int ll=mytree->Fetch(sha,l1,np->place+1);
				sha[ll]=0;

//				printf("done '%s'\n",sha);
				if (sha[0]!='$'){break;}
				MoveTo(128,nx->place+1);
				MyExec(yw,128,sha+1);
				text=NULL;
			}
			break;

		case A_UNDO:
		case A_REDO:
			Undo(yw,curs,std1,std2,msg->what==A_UNDO?-1:1);
			txn=1025;
			break;
		case A_GOTO_LINEY:
			{
			msg->FindInt32("Pos",&ll);
			nplace *np=mytree->FindNext(10,-1,ll);
			if (np){
				MoveTo(curs,np->place+1);
				DoFoc(yw);
			}else{
				beep();
			}
			txn=1024;
			break;
			}
		case A_GOTO_PLACE:
			msg->FindInt32("Pos",&ll);
			if (ll) MoveTo(curs,ll);
			DoFoc(yw);
			txn=1024;
			break;
		case A_DELETE_LEFT:
			PushMarker(1);
			MIX(std1,std2,Pos(curs));
			MyDelChar(curs,-1,cws?cws->typedir:0);
			MIX(std1,std2,Pos(curs));
			txn=1026;
			break;
		case A_DELETE_RIGHT:
			PushMarker(1);
			MIX(std1,std2,Pos(curs));
			MyDelChar(curs,1,cws?cws->typedir:0);
			MIX(std1,std2,Pos(curs)+1);//?
			txn=1026;
			break;
		case A_SELECT_NONE:
			MIX(std1,std2,Pos(141));
			MIX(std1,std2,Pos(142));
			MoveTo(141,-1);
			MoveTo(142,-1);
			txn=1026;
			break;
		case B_SELECT_ALL:
			MoveTo(141,1);
			MoveTo(142,mytree->Size()-1);
			EXG32(std1,std2);
			txn=1026;
			break;
		case A_START_DRAG:
			ll=Pos(curs);
			MIX(std1,std2,Pos(141));
			MoveTo(141,ll);
			MIX(std1,std2,ll);
			txn=1026;
			break;
		case A_FINISH_DRAG:
			ll=Pos(curs);
			MIX(std1,std2,Pos(142));
			MoveTo(142,ll);
			MIX(std1,std2,ll);
			txn=1026;
			break;
		case A_MOVE_TO:
			{
				int32 pp=0,code=7;
				msg->FindInt32("Place",&pp);
				msg->FindInt32("Code",&code);
				if (code&1){
					MIX(std1,std2,Pos(141));
					MoveTo(141,pp);
					MIX(std1,std2,pp);
				}
				if (code&2){
					MIX(std1,std2,Pos(142));
					MoveTo(142,pp);
					MIX(std1,std2,pp);
				}
				if (code&4)MoveTo(curs,pp);
//              printf("%d,%d\n",code,pp);
				txn=1024;
				if (code&3)txn=1026;
			}
			break;
		case A_CTRL_CHAR3:
		case A_CTRL_CHAR26:
			CtrlChar(msg->what);
			#if defined(_DebugObj)
				printf(yw,"CTRL Pressed");
			#endif
			break;//Ctrl-C (?)
		case A_CUT_LINE:
		case A_COPY_LINE:
			{
				nplace *np=mytree->FindPrev(10,Pos(curs)-1);
				if (np){
					pp1=np->place+1;
					np=mytree->FindNext(10,pp1);
					if (np){
						pp2=np->place+1;
						if (msg->what==A_CUT_LINE){
							MIX(std1,std2,pp1);
							MIX(std1,std2,pp2);
							txn=1026;
						}
						am=CopyQuote(pp1,pp2,msg->what);
					}
				}
				CCMsg(am,msg->what,yw);
			}
			break;
		case B_CUT:
		case B_COPY:
			pp1=Pos(141);
			pp2=Pos(142);
			SORT32(pp1,pp2);
			if (pp1!=-1 && pp1!=pp2){
				if (msg->what==B_CUT){
					MIX(std1,std2,pp1);
					MIX(std1,std2,pp2);
					txn=1026;
				}
				am=CopyQuote(pp1,pp2,msg->what);
			}
			CCMsg(am,msg->what,yw);
			break;
		case B_PASTE:
			PushMarker(10);
			ll=Pos(curs);
			MIX(std1,std2,ll);
			ll+=Paste(msg);
			MIX(std1,std2,ll);
			txn=1026;
			break;
		case A_KILL_SELECTION:
			pp1=Pos(141);
			pp2=Pos(142);
			SORT32(pp1,pp2);
			if (pp1!=-1){
				PushMarker(10);
				MIX(std1,std2,pp1);
				MIX(std1,std2,pp2);
				am=Remove(pp1,pp2);
				Change(1);
				txn=1026;
				MoveTo(141,-1);
				MoveTo(142,-1);
			}
			CCMsg(am,msg->what,yw);
			break; 
		case A_REVERT_BUFFER:
			PushMarker(20);
			printf("Reverting…");
			pp1=Pos(curs);
			Remove(1,mytree->Size()-1);
			GetFromEnt(&myent);
			txn=1026;
			EXG32(std1,std2);
			MoveTo(curs,pp1);
			printf("Reverted");
			break;
		case MY_SAVE_FILE:
			{
				PushMarker(20);
				entry_ref dir;
				char *txt=NULL;
				msg->FindRef("directory",&dir);
				msg->FindString("name",(const char**)&txt);
				PutToDir(&dir,txt);
			}
			break;
		case A_INDENT:
		case A_UNDENT:
		case A_QUOTE:
		case A_UNQUOTE:
			PushMarker(10);
			IndentiQuote(curs,(YAction)msg->what,std1,std2);
			txn=1026;
			break;

		case A_REPLACE_THIS:
			{
				pp1=Pos(141);
				pp2=Pos(142);
				SORT32(pp1,pp2);
				if (pp1==-1 || pp1==pp2){
					beep();
					printf("No Selection To Replace");
					break;
				}
				PushMarker(10);
				pp2=MIN(pp2,pp1+800);
				CheckBuff();
				MIX(std1,std2,pp1);
				MIX(std1,std2,pp2);
				Remove(pp1,pp2);
				AddChar(142,text,strlen(text));
				MoveTo(curs,Pos(142));
				MIX(std1,std2,Pos(142));
				DoFoc(yw);
				printf("Replaced");
				txn=1025;
				text=NULL;
			}
			break;

		case A_FIND_BACK:
		case A_FIND_FORWARD:
		{
			char *txt = text;
			text = NULL;
			int res = 0;
			if (!txt)
				break;
//			int ll = strlen(txt);
			int32 bx1,bx2;
			bx1 = Pos(curs) + 1;
			bx2 = mytree->Size() - 1;
			if (msg->what == A_FIND_FORWARD)
			{
				res = Find(txt,bx1,bx2,1);
				if (!res)
				{
					bx2 = bx1;
					bx1 = 1;
					res = Find(txt,bx1,bx2,1);
				}
			}
			else
			{
				int32 dd = bx2;
				bx2 = bx1;
				bx1 = 1;
				res = Find(txt,bx1,bx2,-1);
				if (!res)
				{
					bx1 = bx2;
					bx2 = dd;
					res = Find(txt,bx1,bx2,-1);
				}
			}
			if (!res)
			{
				beep();
				printf("Not Found");
				break;
			}
		
			MIX(std1,std2,Pos(141));
			MIX(std1,std2,Pos(142));
			MoveTo(141,bx1);
			MoveTo(142,bx2);
			MoveTo(curs,bx1);
			MIX(std1,std2,bx1);
			MIX(std1,std2,bx2);
//			MyScrollTo(0,((2*by1-1)*fh-Bounds().Height())/2);
			txn=1026;
			DoFoc(yw);
			printf("Found");
			break;
		}
		case A_ENTER_FIND_STRING:
			{
				pp1=Pos(141);
				pp2=Pos(142);
				SORT32(pp1,pp2);
				if (pp1==-1 || pp1==pp2){beep();break;}
				if (pp2>pp1+250)pp2=pp1+250;
				CheckBuff();
				ll=mytree->Fetch(mybuf,pp2-pp1,pp1);
				if (!ll){beep();break;}
				mybuf[ll]=0;
				if (yw){
					BMessage ms(S_FIND_STRING);
					ms.AddString("Text",mybuf);
					((BLooper*)yw)->PostMessage(&ms);
				}
//              yw->sview->SetSearch(rawtxt);
			}
			break;

		case A_RAW_CHAR:
		{
			break;
		}
		case A_INSERT_SNIPPET:
		{
			const char *code;
			msg->FindString("snip", &code);
			int32 len, ll;
			
			len = strlen(code);
			PushMarker(10);
			ll = Pos(curs);
			MIX(std1,std2,ll);
			ll += AddChar(curs,code,len,cws ? cws->typedir : 0);
			MIX(std1,std2,ll);
			txn = 1026;
			break;
		}
		case A_INSERT_HINT:
		{
			const char *code;
			msg->FindString("hint", &code);
			int32 len, ll;
			
			len = strlen(code);
			PushMarker(10);
			ll = Pos(curs);
			
			//find the last typed word
			char text[1];
			bool cha = true;
			int i = ll - 1;
			int j = 0;
			
			while (cha)
			{
				int32 le = Fetch(text, 1, i);
				if (le == 1)
				{
					if (strncmp(text, " ", 1) == 0 || strncmp(text, "\t", 1) == 0 ||
						strncmp(text, "\n", 1) == 0 || strncmp(text, ".", 1) == 0 ||
						strncmp(text, ">", 1) == 0 || strncmp(text, "(", 1) == 0 ||
						strncmp(text, ")", 1) == 0)
					{
						cha = false;
					}
					else
					{
						i--;
						j++;	
					}
				}
			}
			//and remove it
			Remove(ll - j, ll);
			
			MIX(std1, std2, ll);
			ll += AddChar(curs, code, len, cws ? cws->typedir : 0);
			MIX(std1,std2,ll);
			txn = 1026;
			break;
		}
		default:
		{
			if  (yw)
			{
//              printf("Msg sent here by mistake\n");
				((BLooper*)yw)->PostMessage(msg);
			}
			else
			{
				printf("unknown Buff message..\n");
				msg->PrintToStream();
			}
			break;
		}
	}

	if (dx||dy){
		int32 uu=0;
		msg->FindInt32("Shift",&uu);
		if (uu==1){
			MIX(std1,std2,Pos(141));
			MIX(std1,std2,Pos(142));
			int32 pp=Pos(curs);
			MoveTo(141,pp);
			MoveTo(142,pp);
			txn=1026;
		}
		MoveBy(curs,dx,dy);
		if (uu&1){
			MIX(std1,std2,Pos(142));
			int32 pp=Pos(curs);
			MoveTo(142,pp);
			MIX(std1,std2,pp);
			txn=1026;
		}
		if (!txn) txn=1024;
	}

	if (text){
		int crs=strlen(text);
		int32 pp=Pos(curs);
		PushMarker(1);
		MIX(std1,std2,pp);
		pp+=AddChar(curs,text,crs,cws?cws->typedir:0);
		MIX(std1,std2,pp);
		txn=1026;
	}

	if (txn)TX(txn,std1,std2);
	cws=NULL;
}


void Buff::Capture(){
	for (int i=0;i<winlist->Num();i++){
		WinStats *ws=(WinStats*)winlist->Item(i);
		int32 targetnum=ws->xtra;
		int32 mc=mytree->Count(10,Pos(targetnum),Pos(targetnum+1));
		ws->xtra2=mc;
	}
}

void Buff::TX(BMessage *ms){
	BAutolock(this);
	for (int i=0;i<winlist->Num();i++){
		WinStats *ws=(WinStats*)winlist->Item(i);
//        printf("tx %d,%d\n",type,ws->drprt);
		((BLooper*)ws->yw)->PostMessage(ms);
	}
}

void Buff::TX(int32 type,int32 dat0,int32 dat1){
	int32 dat[8];
	dat[0]=dat0;
	dat[1]=dat1==-1?Size():dat1;
	dat[2]=curs;

	for (int i=0;i<winlist->Num();i++){
		WinStats *ws=(WinStats*)winlist->Item(i);
		int32 t1=type;
		if (t1==1025 || t1==1026){
			t1=1025;
			int32 targetnum=ws->xtra;
			int32 mc=mytree->Count(10,Pos(targetnum),Pos(targetnum+1));
			if (mc!=ws->xtra2)t1++;
		}
		write_port(ws->drprt,t1,dat,3*sizeof(int32));
	}
}

void Buff::DeRegister(WinStats *w){
	if (!w) return;
	BAutolock (this);
	for (int i=winlist->Num()-1;i>=0;i--){
		WinStats *ws=(WinStats*)winlist->Item(i);
		if (ws->yw==w->yw){
			winlist->RemIndex(i);
			delete (ws);
		}
	}
}
int32 Buff::Register(WinStats *w){
	if (!w) return 0;
	BAutolock (this);
	DeRegister(w);
	int32 dce=160;
	int i=0;
	while (i<winlist->Num()){
		WinStats *ws=(WinStats*)winlist->Item(i++);
		if (ws->xtra==dce){dce+=2;i=0;}
	}

	WinStats *ws=new WinStats();
	*ws=*w;
	ws->xtra=dce;
	winlist->Add(ws,0);
	int pp=Pos(ws->cursor);
	if (pp==-1)MoveTo(ws->cursor,1);

	return dce;
}


WinStats::WinStats()
{
}


WinStats::~WinStats()
{
}


long
CountWhite(char *s,long max)
{
	if (!s)
		return 0;
	long c = 0;
	while (max-- > 0)
	{
		max -= 1024;
		if (*s == 32)
		{
			c++;
			max += 1024;
		}
		if (*s==9)
		{
			c += 4;
			max += 1024;
		}
		s++;
	}
	return c;
}


long
TrailWhite(char *s,long st)
{
	if (!s)
		return 0;
	long c = st, d = 0;
	while (s[--c] == 32 && d < 2048)
		d++;
	return d;
}


int32
Buff::Pos(int32 c)
{
	if (c < 0 || c > 256)
	{
		printf("strange, asking for cursor %ld\n",c);
		return -1;
	}
	
	nplace *np = mypos[c];
	return np ? np->CalcPlace() : -1;
}


int32
Buff::Size()
{
	return mytree->Size();
}


void
Buff::MoveTo(int32 c,int32 p)
{
	now++;
	if (c == 128)
		now--;
	if (c == 140)
		now--;
	if (c == 144)
		now--;
	if (c == 148)
		now--;
	if (c == 152)
		now--;
	if (c == 156)
		now--;
	if (c>=160 && c<=255)
		now--;
	nplace *np=mypos[c];
	if (p == -1)
	{
		np->Detach();
		delete np;
		mypos[c] = 0;
	}
	else
	{
		p = BND(p, 1, Size() - 1);
		if (!np)
			np = new nplace(c);
		mytree->Attach(np, p);
		mypos[c] = np;
	}
}


void Buff::MoveFlat(int32 cn,int32 delta){
	Own;
	nplace *np=mypos[cn];
	if (!np){
		printf("strange, no cursor\n");
		return;
	}
	int crs=np->CalcPlace();
	crs=BND(crs+delta,1,mytree->Size()-1);
	mytree->Attach(np,crs);
}

void Buff::MoveBy(int32 cn,int32 dx,int32 dy){
	Own;
	char spa[2048];
	nplace *np=mypos[cn];
	if (!np){
		printf("strange, no cursor\n");
		return;
	}
	int vm=cws?cws->vmode:2;
	int crs=np->CalcPlace();
	crs=BND(crs,1,Size()-1);
	int32 xx=0,ll=0,l2=0;
	switch (vm){
		case 0: //hex
			if (dx==-65536){crs=(crs-1)&(~7);dx=1;}
			if (dx==65536){crs=(crs-1)|7;dx=1;}
			crs+=dx+dy*8;
			break;
		default: //ascii etc
			nplace *nt=mytree->FindPrev(10,crs-1);
			nplace *lst=nt;
			ll=mytree->Fetch(spa,MIN(crs-lst->place-1,2040),lst->place+1);
			xx=UTFWidth(spa,ll)+1; //get current pos..
//			printf("at %d\n",xx);
			while (dy && nt){
				if (dy>0){
					dy--;nt=mytree->FindNext(10,nt->place+1);
				}else{
					dy++;nt=mytree->FindPrev(10,nt->place-1);
				}
				if (nt)lst=nt;
			}
			if (!lst)break;
			nplace *nx=mytree->FindNext(10,lst->place+1);
			if (!nx)nx=lst;
			ll=mytree->Fetch(spa,MIN(nx->place-lst->place,2040),lst->place);
			spa[0]=32;
			spa[ll++]=32;
			spa[ll]=32;
	//      spa[ll++]=32;
			l2=UTFWidth(spa,ll);
			//printf("width %d [dx=%d]",l2,dx);
			if (dx==-65536){
				if (xx==1){
					xx=CountWhite(spa+1,ll-2)+1;
				}else{
					xx=1;
				}
			}else if (dx==65536){
				xx=l2-1;
			}else if (dx){
//				printf("moveer %d\n",xx);
				xx=xx+dx; 
			}else{
				xx=BND(xx,1,l2-1);
			}

			long temp=xx+1;
			int ndx=WidthUTF(spa,temp,ll)-1;
			crs=lst->place+ndx;
			if ((temp>-3) && (temp<0) && dx==1 ){
				crs++;
//				printf("end correction (%d,%d)[%d]\n",xx,dx,l2);
			}
			//printf("xx=%d\n",ndx);
			break;
	}
	crs=BND(crs,1,mytree->Size()-1);
	mytree->Attach(np,crs);
}

int32 Buff::Now(){return now;}
int32 Buff::Fetch(char *d,int32 s,int32 p){return mytree->Fetch(d,s,p);}
int32 Buff::Count(int32 type,int32 p1,int32 p2){return mytree->Count(type,p1,p2);}

void Buff::DoFoc(NiueWindow *yw)
{
	if (yw){
		BMessage ms(S_RESET_FOCUS);
		ms.AddInt32("Pos",Pos(curs));
		((BLooper*)yw)->PostMessage(&ms);
	}
}


NiueWindow *Buff::WinAt(int32 idx){
	WinStats *ws=(WinStats*)winlist->Item(idx);
	return ws?ws->yw:NULL;
}
