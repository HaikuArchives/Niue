
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


#include "actions.h"
#include "cblist.h"
#include "UBuffer.h"    //the high-level text representation
#include "NiueWindow.h"
#include "Niue.h"
#include "FFont.h"
#include "BufList.h"
#include "Utils.h"
#include "aboutwindow.h"

#include <File.h>
#include <FilePanel.h>
#include <Application.h>
#include <Autolock.h>
#include <Alert.h>  //about grrrrrrr
#include <Entry.h> //suffix
#include <Beep.h>
#include <Roster.h>

#include <stdio.h>
#include <string.h>

extern void BroadCast(Buff*,BWindow*,int32 type);

BFont *be_plain_font2=NULL;
BFont *be_bold_font2=NULL;
BFont *be_fixed_font2=NULL;

class NiueApplication : public BApplication //standard fare
{
	public:
						NiueApplication();
						virtual ~NiueApplication();
		virtual void    ReadyToRun(void);
		virtual void	CheckPrefs();
		virtual void	RefsReceived(BMessage *message);
		virtual void    ArgvReceived(int32 argc, char **argv);
		virtual void    MessageReceived(BMessage *msg);
		virtual void    AboutRequested();
		virtual bool    QuitRequested();
		virtual void    Quit();


	private:
		BWindow*		aboutWindow;
		void            RefsReceived2(BMessage *msg,NiueWindow *ww);
		NiueWindow*     OpenWin(BRect Place,Buff *StartBuff);
		void            Transmit(BMessage*);
		void            NewFontReq(BMessage*);

		BRect           aRect;      //default window position..
		BFilePanel*		OpenPanel;
		BFilePanel*		AddPanel;
		CList           *winlist,*buflist;

//------------------------------------
		Buff *GetBuff(entry_ref *);//NULL if unable
		Buff *GetBuff(char *path);
		Buff *FirstShell();

		Buff *NewBuff();
		Buff *NewShell();

		void Discard(Buff*);

		Buff *BuffAt(int32 index);
		Buff *Shift(Buff*,int32 ofs);
		int32   CountItems();

		void    MakePerfect();
		void    MySave(YAction type);
		void    YExec(NiueWindow*,char *);
		void    CheckAlone(Buff *);
		void    CheckAllTitle();
		void    Sort();
		int32   bufcnt,shellcnt;
		void    SendMsg(Buff *,BMessage*);
		void    DoDocText();
};


int
main(int a,char **b)
{
	NiueApplication *ya_app = new NiueApplication();
	ya_app->Run();
	delete ya_app;
	return 0;
}


NiueApplication::NiueApplication()
	:	BApplication("application/x-vnd.Niue4")
{
	new BufList();

	be_plain_font2=new BFont();
	be_bold_font2=new BFont();
	be_fixed_font2=new BFont();
	*be_plain_font2=*be_plain_font;
	aRect.Set(100, 100, 900, 700);   //we should grab this out of the preferences
	winlist=new CList(100);
	buflist=new CList(100);
	shellcnt=bufcnt=1;
}


NiueApplication::~NiueApplication()
{
	delete winlist;
//    delete bl;
}


bool
NiueApplication::QuitRequested()
{
//  return TRUE;
//	if (quitnow)return TRUE;

	int mx=CountItems();
	bool happy=TRUE;
	Buff *badb=NULL;
	for (int idx=0;idx<mx;idx++){
		Buff *bb=BuffAt(idx);
		if (bb->alone && !bb->fried){
			if (bb->lastchanged!=0){
				happy=FALSE;
				if (!badb)badb=bb;
			}
		}
	}

	mx=winlist->Num();

	if (!happy)
	{

		BAlert *bal=new BAlert("Quitting Alert",
			"Do you want to save changes before quitting?",
			"Don't Save",
			"Cancel",
			NULL);

		if (mx==0)
		{
			OpenWin(aRect,badb);
		}
		else
		{
			NiueWindow *yw=(NiueWindow*)winlist->Item(0);
			{
				BAutolock l1(yw);
				yw->ChangeBuff(badb);
			}
			MakePerfect();
		}
		int32 res=bal->Go();
		switch (res){
			case 0:
				return TRUE;
			case 1:
				return FALSE;
//			case 2:
//				PostMessage(A_SAVE_ALL);
//				return FALSE;
		}
	}

	return TRUE;
}


void NiueApplication::Quit()
{
	BApplication::Quit();
}


NiueWindow *
NiueApplication::OpenWin(BRect frame, Buff *bb)
{
	NiueWindow *yw=new NiueWindow(frame,bb);
	winlist->Add(yw);
//	CheckTitle(yw,bb);
	yw->Show();
	MakePerfect();
	return yw;
}


void
NiueApplication::Transmit(BMessage *src)
{
	Buff *bb=NULL;
	NiueWindow *notme=NULL;
	int32 type;

	status_t rr;
	rr=src->FindInt32("Type",&type);
	rr=src->FindPointer("Buff",(void**)&bb);
	rr=src->FindPointer("YWin",(void**)&notme);

	BMessage *msg=NULL;

	long mx=winlist->Num();
	for (long idx=0;idx<mx;idx++)
	{
		NiueWindow *yw=(NiueWindow*)winlist->Item(idx);
		if (yw!=notme){
			int go=FALSE;

			if (bb==NULL)go=TRUE;
			if (yw->MyBuff()==bb) go=TRUE;
			if (go==FALSE) continue;
			BMessageQueue *mq=yw->MessageQueue();
			if (!mq) continue;
			if (mq->FindMessage(type,1)) continue;
			if (!msg) msg=new BMessage(type);
			yw->PostMessage(msg);
		}
	}
	if (msg) delete(msg);
}


void NiueApplication::RefsReceived(BMessage *message)
{
	RefsReceived2(message,NULL);
}


void
NiueApplication::RefsReceived2(BMessage *msg,NiueWindow *ww)
{
	//get our window
	NiueWindow *yw = 0;
	if(CountItems() > 0)
	{
		yw=(NiueWindow*)winlist->Item(0);
	}	
	yw->Lock();		
	
	//objects
	entry_ref	openref;
	BEntry 		openentry;
//	msg->FindPointer("YWin",(void**)&ww);

	int i = 0;
	while (msg->FindRef("refs", i, &openref) == B_NO_ERROR)
	{
		openentry.SetTo(&openref, true);
		if (openentry.IsDirectory())
		{
			//we are opening a new project so first empty all text buffers from the current one
			while(CountItems() > 1)
			{
				Discard(BuffAt(CountItems()-1));
			}	
			yw->LoadDir(openref);
			i++;
		}
		else if (openentry.IsFile())
		{
			if(yw->hasProjectOpen)
			{
				//add the file to the open project
				yw->AddFile(openref);	
			}
			else
			{
				//show the file
				BMessage msg(SHOW_FILE_MSG);
				msg.AddRef("refs", &openref);
				PostMessage(&msg);
			}	
			i++;
		}
		else
		{
			(new BAlert("Niue", "This entry is not valid", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			i++;
		}
	}

	yw->Unlock();
}


void
NiueApplication::ArgvReceived(int32 argc,char **argv)
{
	int i,erc;
	BFile       ff;
	Buff        *bb=NULL,*fb=NULL;

	erc=0;
	for (i=1;i<argc;i++)
	{
		bb=GetBuff(argv[i]);

		if (bb && !fb)
			fb=bb;
		if (!bb)
			erc++;
	}

	if (erc)
		printf("Try using a more explicit pathname\n");
	
	if (fb)
		OpenWin(aRect,fb);   //if we opened a file
}


void
NiueApplication::ReadyToRun(void)
{
	if (CountItems()==0)    //no buffers?
	{
		CheckPrefs();
		
		//show
		Buff *cb=FirstShell();
		OpenWin(aRect,cb);
	}
}


void
NiueApplication::CheckPrefs()
{
	//first check if our settings dir still exists
	BPath prefspath, vprefspath;
	find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
	prefspath.Append("Niue");
	
	//root
	BEntry prefs(prefspath.Path(), false);
	if(!prefs.Exists())
	{
		//create it
		BDirectory dir;
		dir.CreateDirectory(prefspath.Path(), &dir);
	}
	
	//version
	bool latest = false;
	vprefspath = prefspath;
	vprefspath.Append("niue_prefs");
	//read file
	BFile prefsfile(vprefspath.Path(), B_READ_ONLY);
	if (prefsfile.IsReadable())
	{
		BMessage msg('PREF');
		int8 version;
		msg.Unflatten(&prefsfile);
		
		//backstyle
		if(msg.FindInt8("version", &version) == B_OK)
		{
			if(version == 5)
			{
				latest = true;
			}	
		}
	}
	
	//docs
	prefspath.Append("Docs");
	BEntry docs(prefspath.Path(), false);
	if(!docs.Exists() || latest == false)
	{
		//create dir
		BDirectory dir;
		dir.CreateDirectory(prefspath.Path(), &dir);
		
		//fill
		app_info myinfo;
		size_t length;
		const void *data;
		FILE *output_file;
		BString temppath;
		temppath << prefspath.Path() << "/niue_docs_temp.zip";
		
		be_app->GetAppInfo(&myinfo);
		BFile fill(&myinfo.ref, B_READ_ONLY);
		BResources *bres = new BResources(&fill);
		if(!(bres->HasResource('DOCS', 8)))
		{
			(new BAlert("Niue", "Can't find the documentation resources", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}	
		
		data = bres->LoadResource('DOCS', 8, &length);
		if(data)
		{
			output_file = fopen(temppath.String(), "wb");
			if(fwrite(data, 1, length, output_file) != length)
			{
				(new BAlert("Niue", "There were errors while trying to fetch the documentation.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
			fclose(output_file);
		}
		
		BString sysstring;
		sysstring << "cd " << prefspath.Path() << "; unzip -o niue_docs_temp; rm niue_docs_temp.zip";
		if (system(sysstring.String()) != 0)
		{
			(new BAlert("Niue", "There were errors while trying to unfold the documentation", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		
	}	
}


void
NiueApplication::AboutRequested()
{
	aboutWindow = new aboutwindow;
	aboutWindow->Show();
}


void NiueApplication::NewFontReq(BMessage *msg)
{
	FindMessageFont(msg,"be_plain_font",0,be_plain_font2);
	FindMessageFont(msg,"be_bold_font" ,0,be_bold_font2 );
	FindMessageFont(msg,"be_fixed_font",0,be_fixed_font2);
	BMessage txm(Y_SYSFONT_CHANGE);
	Transmit(&txm);
}


void
NiueApplication::MessageReceived(BMessage *msg)
{
//	Buff    *nb=NULL;
//	int32   idx,mx;
	int32	tt;

	NiueWindow *yw=NULL;
	Buff *cb=NULL;
	msg->FindPointer("Buff",(void**)&cb);
	msg->FindPointer("YWin",(void**)&yw);

	tt = msg->what;
	switch (tt)
	{
		case SHOW_FILE_MSG:
		{
			NiueWindow *ww=(NiueWindow*)winlist->Item(0);
			entry_ref ref;
			msg->FindRef("refs", &ref);
			Buff *newb = GetBuff(&ref);
			ww->Lock();
			ww->ChangeBuff(newb);
			ww->Unlock();
			break;
		}
		case B_SIMPLE_DATA:
		{
			if (msg->CountNames(B_REF_TYPE))
			{
				RefsReceived2(msg,yw);
			}
			break;
		}
		case B_KEY_DOWN + 1:
		{
			tt=0;
			msg->FindInt32("yak:window",&tt);
			if (tt<0 || tt>winlist->Num())tt=0;
			yw=(NiueWindow*)winlist->Item(tt);
			if (yw) yw->PostMessage(msg);
		
			break;
		}	
		case S_ACTION_CHANGE_MSG:
		{
			yw=(NiueWindow*)winlist->Item(0);
			if (yw->fSnippetWindow)
				yw->fSnippetWindow->PostMessage(msg);
			break;
		}	
		case A_SHOW_SM:
		{
			yw=(NiueWindow*)winlist->Item(0);
			if(yw)
				yw->PostMessage(msg);
			break;
		}
		case A_PREV_BUFF:
		case A_NEXT_BUFF:
		{
			if (yw)
			{
				Buff *bb=Shift(cb,msg->what==A_NEXT_BUFF?1:-1);
				BAutolock l2(yw);
				yw->ChangeBuff(bb);
			}
			break;
		}
		case Y_CHANGE_BUFF:
		{
			if (yw)
			{
				BAutolock l7(yw);
				yw->ChangeBuff(cb);
				MakePerfect();
			}
			break;
		}
		case A_OPEN_PANEL:
		{
//			if (!OpenPanel)
//			{
			OpenPanel = new BFilePanel(B_OPEN_PANEL, 0, 0, B_DIRECTORY_NODE, false, 0, 0, false, true);
			OpenPanel->SetTarget(this);
			OpenPanel->Window()->SetTitle("Niue: Select the project folder to open");
			OpenPanel->Show();
//			}
			break;
		}
		case A_ADD_FILE:
		{
//			if(!AddPanel)
//			{
			AddPanel = new BFilePanel(B_OPEN_PANEL, 0, 0, B_FILE_NODE, true, 0, 0, false, true);
			AddPanel->SetTarget(this);
			AddPanel->Window()->SetTitle("Niue: Select files to add to your project");
			AddPanel->Show();					
//			}
			break;
		}
		case '!FNT':
		{
			NewFontReq(msg);
			break;
		}
		case Y_BUFF_CHANGE:
		{
			Buff* buffer = NULL;
			msg->FindPointer("Buff",(void**)&buffer);
			entry_ref buffref;
			buffref = buffer->myent;
			BEntry buffentry;
			buffentry.SetTo(&buffref, true);
			char buffname[B_FILE_NAME_LENGTH];
			buffentry.GetName(buffname);
			
			if(buffer->lastchanged != 0)
			{
				NiueWindow *yw=(NiueWindow*)winlist->Item(0);
				yw->fListView->UpdateList(buffname, true);				
			}
			else
			{
				NiueWindow *yw=(NiueWindow*)winlist->Item(0);
				yw->fListView->UpdateList(buffname, false);
			}
			break;
		}
		case A_DISCARD_BUFFER:Discard(cb);break;
		case A_SAVE_ALL:
		{
			MySave((YAction)msg->what);
			break;
		}
		case B_SOME_APP_ACTIVATED:
		{
			break;
		}	
		case Y_COMPARE_HASH:
		{
			yw=(NiueWindow*)winlist->Item(0);
			if(yw) yw->PostMessage(msg);
			break;
		}	
		case A_DISCARD_ALL_BUFFERS:
		{
//			(new BAlert("Niue", "Discard all buffers", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			
			//get our window
			NiueWindow *yw = 0;
			if(CountItems() > 0)
				yw=(NiueWindow*)winlist->Item(0);
			
			yw->Lock();		
			while(CountItems() > 1)
				Discard(BuffAt(CountItems()-1));
			yw->Unlock();
		}
		break;
			
//		case A_INSERT_HINT:
//			yw=(NiueWindow*)winlist->Item(0);
//			if(yw) yw->MyBuff()->PostMessage(msg);
//			break;
		default:
		{
			printf("unknown Niue message:\n");
			msg->PrintToStream();
			BApplication::MessageReceived(msg);
			break;
		}
	}
}


Buff *
NiueApplication::BuffAt(int32 idx)
{
	return (Buff*)buflist->Item(idx);
}


Buff *
NiueApplication::Shift(Buff *stt,int32 ofs)
{
	int32 tt=buflist->IndexOf(stt)+ofs;
	int32 mx=buflist->Num();
	tt = (tt + mx) % mx;
	return BuffAt(tt);
}


Buff *
NiueApplication::NewBuff()
{	
	Buff *cb=new Buff();
	cb->Run();
	buflist->Add(cb);
	sprintf(cb->myname,"Untitled %ld",bufcnt);
	sprintf(cb->mytitle,"!Untitled %ld",bufcnt);
	bufcnt++;
	return cb;
}


Buff *
NiueApplication::NewShell()
{
	Buff *cb=new Buff();
	cb->Run();
	buflist->Add(cb);
	cb->term=TRUE;
	sprintf(cb->myname,"(Shell %ld)",shellcnt);
	sprintf(cb->mytitle,">(Shell %ld)",shellcnt);
	shellcnt++;
	return cb;
}


void
NiueApplication::CheckAlone(Buff *b1)
{
	BAutolock l2(b1);

	int al=TRUE,max=buflist->Num();
	for (int jj=0;jj<max;jj++)
	{
		Buff *b2=BuffAt(jj);
		if (b1!=b2)
		{
			BAutolock l3(b2);
			if (b1->myent==b2->myent)
			{
				al=FALSE;
				if (b2->alone)
				{
					b2->entchanged=TRUE;
					b2->alone=FALSE;
				}
			}
		}
	}
	b1->alone=al;
}


void
NiueApplication::DoDocText()
{
	BAutolock l1(gb);
	int32 idx=-1;
	for (;;)
	{
		Buff *tp = BuffAt(++idx);
		if (!tp)
			break;
		gb->SetBuff(idx,tp);
		//gb->SetName(idx,tp->mytitle);
	}
	gb->SetCount(idx);
}


void
NiueApplication::MakePerfect()
{
	int32 mx=buflist->Num();
	for (int32 i=0;i<mx;i++)
	{
		Buff *cb=BuffAt(i);
		BAutolock l1(cb);
		CheckAlone(cb);
		//SetName(cb);
	}
	Sort();
//	int32 idx=0;
//	NiueWindow *yw=NULL;
//	while ( (yw=(NiueWindow*)winlist->Item(idx++))!=0)
//	{
//		CheckTitle(yw,yw->cb);
//	}
	DoDocText();
}


void
NiueApplication::Discard(Buff *bb)
{
	if (!bb)
		printf("discard got bad args");

	Buff *aa=Shift(bb,1);
	buflist->RemIndex(buflist->IndexOf(bb));
	if (aa==bb)
		aa=BuffAt(0);
	if (!aa)
		aa = NewBuff();

	int idx=0;
	NiueWindow *yw=NULL;
	while ( (yw=bb->WinAt(idx))!=0)
	{
		BAutolock  l7(yw);
		yw->ChangeBuff(aa);
		MakePerfect();
	}

	bb->PostMessage(B_QUIT_REQUESTED);
}


void
NiueApplication::Sort()
{
	int go=TRUE;

	while (go)
	{
		go=FALSE;
		int i,mx=buflist->Num();
		for (i=0;i<mx-1;i++)
		{
			Buff *a=BuffAt(i);
			Buff *b=BuffAt(i+1);
			if (strcmp(a->myname,b->myname)>0)
			{
				buflist->SwapWith(b,i);
				buflist->SwapWith(a,i+1);
				go=TRUE;
			}
		}
	}
}


int32
NiueApplication::CountItems()
{
	return buflist->Num();
}


void
NiueApplication::MySave(YAction type)
{
	int mx=buflist->Num();

	for (int idx=1;idx<mx;idx++)  //idx=0
	{
		Buff *bb=BuffAt(idx);
		BAutolock l2(bb);
		bb->PutToEnt();
	}
	
	
}


Buff *
NiueApplication::FirstShell()
{
	int32 max=buflist->Num();
	for (int i=0;i<max;i++)
	{
		Buff *nb=BuffAt(i);
		if (!nb)
			break;
		if (nb->term)
			return nb;
	}

	return NewShell();
}


Buff *
NiueApplication::GetBuff(entry_ref *er)
{
	for (int ll=0;TRUE;ll++)
	{
		Buff *bb=BuffAt(ll);
		if (!bb)
			break;
		if (*er==bb->myent)
			return bb;
	}

	BFile   fil(er,B_READ_ONLY);
	if (fil.IsReadable())
	{
		Buff *bb=NewBuff();
		BAutolock l7(bb);
		bb->GetFromEnt(er);
		return bb;
	}
	
	return NULL;
};


Buff *
NiueApplication::GetBuff(char *path)
{
	entry_ref   er;
//	status_t rr=get_ref_for_path(path,&er);
	get_ref_for_path(path,&er);
	if (BEntry(&er).Exists())
		return GetBuff(&er);

	printf("Niue:<sniff> Couldn't open '%s'\n",path);
	return  NULL;
}


void
NiueApplication::YExec(NiueWindow *yw,char *cmd)
{
	if (!cmd)
		return;
	MySave(A_SAVE_ALL);
	Buff *cb=FirstShell();
	BAutolock l7(cb);
	int ll=strlen(cmd);
	cb->MoveTo(128,1);
	cb->AddChar(128,"$ ",2);
	cb->AddChar(128,cmd,ll);
	cb->AddChar(128,"\n",1);
	for (int i=140;i<160;i+=4)
	{
		if (cb->Pos(i)>=1)
			cb->MoveTo(i,1);
	}
	cb->MoveTo(129,-1);
	cb->TX(1026);
	cb->StripToPound(128);
	cb->MyExec(yw,128,cmd);
}
