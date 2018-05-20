
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


#include <Application.h> //for postmessage..

#include "Niue.h"
#include "UBuffer.h"
#include "actions.h"
#include "BMapView.h"
#include "NiueWindow.h" //(MoveCurs()
#include "SMap.h"
#include "Hash.h"
#include "Schemes.h"
#include "BufList.h"
#include "BSBar.h"
#include "ViewBase.h"

#include "cblist.h"
#include "Utils.h"

#include <Region.h>
#include <ScrollBar.h>
#include <Screen.h>
#include <MessageQueue.h> //input magic
#include <Autolock.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>

extern int ops[10];	//profiling..

bool ProcSmp(SMap **dest, SMap *tst,BRect rct,int32 now)
{
//  printf("%x,%x\n",tst,*dest);
	if (!tst)
		return FALSE;
	if (*dest)
		return FALSE;
//    printf("%d,%d\n",now,tst->When());
	if (now != tst->When())
		return FALSE;
//    tst->validrect.PrintToStream();
//    rct.PrintToStream();
	if (!tst->validrect.Contains(rct))
		return FALSE;
//    printf("%x,%x\n",dest,*dest);
	*dest=tst;
	return TRUE;
}


BMapView::BMapView(NiueWindow *yy,Buff *bb,BRect rect, char *name,BSBar *s1,BSBar *s2,port_id pp)
	:	BView(rect, name, B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS | 
										B_INPUT_METHOD_AWARE | B_PULSE_NEEDED | 
										B_NAVIGABLE | B_NAVIGABLE_JUMP)
{
	ws=new WinStats();

	ws->yw=yy;
	ws->cursor=140;
	cb=bb;
	ws->drprt=pp;
	scbv=s1;
	scbh=s2;

	SetMouseEventMask(~0);

	tlx=tly=0;
	tax=tay=vvx=vvy=pox=poy=0.0;
	lmx=lmy=lcx=lcy=0;
	lastscroll=0;
	lastshift=0;
	targetnum=0;
	ltop=-1;

	MakeSchemes();
	colscheme *myscheme=schemetab[1];
	vb=new ViewBase();
	vb->vmode=ws->vmode=2;
	vb->csh=myscheme;


	pox=poy=vvx=vvy=smx=smy=0;
	smp1=smp2=NULL;

	prate=500000.0;
	ws->typedir=0;

	sleeping=FALSE;
	myfont=*be_fixed_font;
	float   mysh=myfont.Size();
	cspace=B_COLOR_8_BIT;

	{
		BScreen myscreen(ws->yw);
		if (myscreen.IsValid()){
			cspace=myscreen.ColorSpace();
			BRect myfr=myscreen.Frame();
			if (myfr.Width()>900)   mysh+=3.0;
			if (myfr.Width()>1180)  mysh+=3.0;
		}
	}

//	myfont.SetSize(mysh);
	FrameResized(0,0);
	ws->cursor=0;
	SetViewColor(B_TRANSPARENT_32_BIT);
//	sscroll=false; //Smooth Scrolling not enabled by default
	stx=-1;
}


BMapView::~BMapView()
{
	if (smp1)
	{
		delete smp1;
		smp1=NULL;
	}
	
	if (smp2)
	{
		delete smp2;
		smp2=NULL;
	}
}


void
BMapView::KeyDown(const char *bytes, int32)
{
	BMessage *msg=ws->yw->CurrentMessage();
	int32 key,mod;
	msg->FindInt32("key",&key);
	msg->FindInt32("modifiers",&mod);
	DoKeyDown(bytes,key,mod);

	BMessageQueue *mq=ws->yw->MessageQueue();
	
	msg=mq->FindMessage(B_KEY_DOWN,0);
	while (msg!=NULL)
	{
		const char *str;
		msg->FindString("bytes",&str);
		msg->FindInt32("key",&key);
		msg->FindInt32("modifiers",&mod);
		DoKeyDown(str,key,mod);
		mq->RemoveMessage(msg);
		msg=mq->FindMessage(B_KEY_DOWN,0);
	}
}


//see InterfaceDefs.h
void
BMapView::DoKeyDown(const char *bytes, int32 key,int32 mod)
{
	const char *src=NULL;
	YAction  res=A_UNHANDLED;
	int32 chr=(uchar)bytes[0];
	int32 shift=mod&B_SHIFT_KEY;

	switch (chr)
	{
		case 2:
		{
			res=A_MOVE_LEFT;
			break;
		}
		case 3:
		{
			res=A_CTRL_CHAR3;
			break;
		}
		case 5:
		{
			res=A_DELETE_RIGHT;
			break;
		}
		case 6:
		{
			res=A_MOVE_RIGHT;
			break;
		}
		case 7:
		{
			res=A_NOTHING;
			break;//keyboard quit?!'
		}
		case B_BACKSPACE:
		{
			res=A_DELETE_LEFT;
			break;//help?
		}
		case B_TAB:
		{
			res=A_RAW_CHAR;
			src=bytes;
			break;//8 ^H
		}
		case B_ENTER:
		{
			if (shift)
			{
				res=A_RAW_CHAR;
				src=bytes;
			}
			else
				res=A_SPLIT_LINE;
			break; //^J
		}
		case B_FUNCTION_KEY:
		{
//			res=A_MOVE_UP;//^Q
			switch(key)
			{
				case B_F1_KEY:
				{
					res=A_START_DRAG;
					break;
				}
				case B_F2_KEY:
				{
					res=A_FINISH_DRAG;
					break;  //f1
				}
				case B_F3_KEY:
				{
					res=shift ? A_FIND_BACK : A_FIND_FORWARD;
					break;
				}
//				case B_F4_KEY:
//				{
//					res=A_NEXT_ERROR;
//					break;
//				}
				case B_F12_KEY:
				{
					if(ws->yw->fHintView->lstHints->CountItems() > 0)
					{
						ws->yw->fHintView->lstHints->MakeFocus(true);
						ws->yw->fHintView->lstHints->Select(0);	
					}
					break;
				}
			}
			break;
		}
		case 14:
		{
			res=A_MOVE_DOWN;
			break;
		}
		case 15:
		{
			res=A_NOTHING;
			break;//Open Line?!
		}
		case 17:
		{
			res=A_QUOTE_PASTE;
			break;//Quoted paste..
		}
		case 18:
		{
			res=A_FIND_BACK;
			break;
		}
		case 19:
		{
			res=A_FIND_FORWARD;
			break;
		}
		case 20:
		{
			res=A_TRANSPOSE_CHARS;
			break;
		}
		case 21:
		{
			res=A_NOTHING;
			break;//universal argumetn!?
		}
		case 22:
		{
			res=A_RECENTER_SCROLL;
			break;//scroll up?
		}
		case 23:
		{
			res=A_NOTHING;
			break; //kill region..
		}
		case 24:
		{
			res=A_NOTHING;
			break; //^X
		}
		case 25:
		{
			res=A_NOTHING;
			break; //yank!?
		}
		case 26:
		{
			res=A_CTRL_CHAR26;
			break;//Ctrl-Z
		}
		case B_LEFT_ARROW:
		{
			res=A_MOVE_LEFT;
			break;
		}
		case B_RIGHT_ARROW:
		{
			res=A_MOVE_RIGHT;
			break;
		
//			if(ws->yw->wdwHint->Lock() == true)
//			{
//				ws->yw->wdwHint->Activate();
//				if(ws->yw->wdwHint->lstHints->CountItems() > 0)
//				{
//					ws->yw->wdwHint->lstHints->Select(0);
//					ws->yw->wdwHint->lstHints->MakeFocus(true);
//					ws->yw->wdwHint->lstHints->SetHighColor(blue);
//					ws->yw->wdwHint->lstHints->Invalidate();
//				}
//				ws->yw->wdwHint->Unlock();
//			}

//			if(ws->yw->fHintView->lstHints->CountItems() > 0)
//			{
//				ws->yw->fHintView->lstHints->MakeFocus(true);
//				ws->yw->fHintView->lstHints->Select(0);	
//			}
//			else
//			{
//				
//			}
//			break;
		}
		case B_UP_ARROW:
		{
			res=A_MOVE_UP;
			break;
		}
		case B_DOWN_ARROW:
		{
			res=A_MOVE_DOWN;
			break;
		}
//		case 127:res=A_DELETE_RIGHT;break;
		case 127:
		{
			if(HasSelection())
				res=A_KILL_SELECTION;
			else
				res=A_DELETE_RIGHT;
		}
		break;
	}


	switch (chr)
	{	//PC
		case B_HOME:
		{
			res=A_START_LINE;
			break;
		}
		case B_END:
		{
			res=A_END_LINE;
			break;
		}
		case B_PAGE_UP:
		{
			res=A_PAGE_UP;
			break; //11
		}
		case B_PAGE_DOWN:
		{
			res=A_PAGE_DOWN;
			break;//hilite recenter?
		}
	}
	
	
	if (mod & (B_CONTROL_KEY | B_OPTION_KEY))
	{
		switch (chr)
		{
			case B_LEFT_ARROW:
			{
				res=A_START_LINE;
				break;
			}
			case B_RIGHT_ARROW:
			{
				res=A_END_LINE;
				break;
			}
			case B_UP_ARROW:
			{
				res=A_MOVE_TOP;
				break;
			}
			case B_DOWN_ARROW:
			{
				res=A_MOVE_BOTTOM;
				break;
			}
			case B_PAGE_UP:
			{
				res=A_SCROLL_UP;
				break;
			}
			case B_PAGE_DOWN:
			{
				res=A_SCROLL_DOWN;
				break;
			}
		}
	}
	
	if (!(mod & B_NUM_LOCK))
	{
		switch (key)
		{
			case 35:
			{
				break; //divide
			}
			case 36:
			{
				res=A_COPY_LINE;
				break;//Times
			}
			case 37:
			{
				res=A_CUT_LINE;
				break; //minus
			}
			case 58:
			{
				res=(YAction)B_PASTE;
				break;
			}
		}
	}
	
	if (res == A_UNHANDLED && chr > 31 && chr < 256)
	{
		res=A_RAW_CHAR;
		src=bytes;
	}
	
	int32 uu=0;
	if (shift)
		uu |= 1;
	if (lastshift)
		uu |= 2;
	lastshift = shift;
	
	BMessage msg(res);
	if (uu)
		msg.AddInt32("Shift",uu);
	if (src)
		msg.AddString("Text",src);
	switch (res)
	{
		case A_UNHANDLED:
		{
			printf("not accepted! (%ld) [%ld]\n",chr,key);
			break;
		}
		case A_FIND_BACK:
		case A_FIND_FORWARD:
		{
			ws->yw->PostMessage(&msg);
			break;
		}
		default:
		{
			msg.AddPointer("YWin",ws->yw);
			cb->PostMessage(&msg);
			break;
		}
	}

}


bool
BMapView::HasSelection()
{
	if(cb->Pos(141) == cb->Pos(142))
		return false;
	else
		return true;
}


void
BMapView::FrameResized(float ,float )
{
	BRect obnds(bnds);
	bnds=Bounds();
	wid=bnds.Width();

	if (Window())
	{
		BRegion ff;
		ff.Include(bnds);
		ConstrainClippingRegion(&ff);
	}
	
	int wr=FALSE;
	if (vb->vmode==3)
	{
		MyDirty.Include(BRect(tlx,tly,tlx+bnds.right,tly+bnds.bottom));
		wr=TRUE;
	}
	else
	{
		if (bnds.right>obnds.right)
		{
			MyDirty.Include(BRect(tlx+obnds.right,tly,tlx+bnds.right,tly+bnds.bottom));
			wr=TRUE;
		}
		if (bnds.bottom>obnds.bottom)
		{
			MyDirty.Include(BRect(tlx,tly+obnds.bottom,tlx+bnds.right,tly+bnds.bottom));
			wr=TRUE;
		}
	}
	if (wr)write_port(ws->drprt,SCROLL,0,0);
//	bnds.PrintToStream();
	font_height temp;
	myfont.GetHeight(&temp);
	fh=floor(temp.ascent)+2.0;
	ffh=ceil(temp.ascent)+ceil(temp.descent);
	ws->pgdn=(int)(bnds.Height()/fh)-3;
	if (ws->pgdn<2)
		ws->pgdn=2;

	CleanScrollers();
	if (cb)
		cb->Register(ws);

}


void
BMapView::Draw(BRect dest)
{
	SetFont(&myfont);
	vb->Target(this,fh,ffh,wid,cb);
	vb->Draw(dest,tlx,tly);
	HalfCursor(dest);
}


SMap *BMapView::PrepareBlit(BRect dest,BRect *src)
{
	// WARNING: deep magic lies within...
	
	// a grungy least-recently-used backing store
	BAutolock l1(cb);
	SMap *smp=NULL;
	int32 now=cb->Now();
	ProcSmp(&smp,smp1,dest,now);
//	ProcSmp(&smp,smp2,dest,now);
// 	smp=NULL;
	if (!smp)
	{
//		printf("new\n");
		int32 nmx,nmy;
		nmx=(int32)(dest.Width()+63)&(~31);
		nmy=(int32)(dest.Height()+63)&(~31);
		
		SMap **myst=&smp1;
		smp=*myst;
		
		if (!smp || smp->bigx<nmx || smp->bigy<nmy || cspace!= smp->uspace)
		{
			if (*myst)
				delete *myst;
			
			smx=nmx+256;smy=nmy+256;
			smp=new SMap(cspace,smx,smy);
			*myst=smp;
		}
		lastsmp=(smp==smp1)?1:2;
		smp->Lock();
		smp->bmv->SetFont(&myfont);
		BRect r2(dest);
		if (vb->vmode==3)
		{
			r2.top=dest.top-36.0;
			r2.bottom=dest.bottom+36.0;
		}
		else
		{
			r2.top=floor(dest.top/fh)*fh;
			r2.bottom=floor(dest.bottom/fh)*fh+fh-1.0;
		}
		if (r2.top<0.0)
			r2.top=0.0;
		
		smp->origx=r2.left;
		smp->origy=r2.top;
		smp->validrect=r2;
		vb->Target(smp->bmv,fh,ffh,wid,cb);
		BRect r3(r2);
		r3.OffsetTo(BPoint(0,0));
		vb->Draw(r3,r2.left,r2.top);
	}
	else
	{
		smp->Lock();
//      printf("f\n");
	}
	BRect ss(dest);
	ss.OffsetBy(-smp->validrect.left,-smp->validrect.top);
	*src=ss;
	smp->bmv->Sync();
	smp->when=now;
	smp->Unlock();
	return smp;
}


void
BMapView::MyDraw()
{
	if (rpx!=tlx || rpy!=tly)
	{
		BRegion fish,sheep;
		GetClippingRegion(&fish);
		GetClippingRegion(&sheep);
		fish.OffsetBy(tlx,tly);
		sheep.OffsetBy(rpx,rpy);
		sheep.Exclude(&fish);
		MyDirty.Include(&sheep);
	}
	
	{
		BRegion crush;
		BRect jj=bnds;
		jj.OffsetBy(rpx,rpy);
		crush.Include(jj);
		MyDirty.IntersectWith(&crush);
	}
	
	int drw=-1;
	while (drw<20)
	{
		drw++;
		BRect dest=MyDirty.RectAt(0);//(0,0,-1,-1);
		if (!dest.IsValid())
			break;
		MyDirty.Exclude(dest);


		SetDrawingMode(B_OP_OVER);
		BRect src;
		SMap *pmt=NULL;
		pmt=PrepareBlit(dest,&src);

		if (rpx!=tlx || rpy!=tly)
		{
			BRect ss=bnds;
			ss.OffsetBy(BPoint(tlx-rpx,tly-rpy));
			
			ss=bnds&ss;
			if (ss.IsValid())
			{
				BRect dd=ss;
				dd.OffsetBy(BPoint(rpx-tlx,rpy-tly));
				CopyBits(dd,ss);
			}
			tlx=rpx;tly=rpy;
		}
		BRect ddst=dest;
		ddst.OffsetBy(BPoint(-tlx,-tly));
		DrawBitmap(pmt,src,ddst);

		HalfCursor(ddst);
	}
}


void
BMapView::CleanScrollers()
{
	BAutolock l1(ws->yw);
	float max=0,yy=bnds.Height();
	if (cb)
	{
		BAutolock l2(cb);
		float x=0,y=0;
		if (vb->vmode==3)
			y = fh * 100.0;
		else
		{
			vb->Target(this,fh,ffh,wid,cb);
			vb->Locate(x,y,cb->Size()-1);
		}
		max=y-yy+fh*3;
	}
	if (max<0)
		max=0;
	scbv->SetRange(0,max);
	scbv->SetSteps(fh,ws->pgdn*fh);

	needscrolls=FALSE;
}


void
BMapView::HalfCursor(const BRect &ddst)
{
	if (llx>=0.0)
	{
		float x0 = llx - tlx, y0 = lly - tly, y1 = y0 + ffh;
		if (x0>=ddst.left && x0<=ddst.right)
		{
			float z0=y0,z1=y1;
			if (ddst.top>z0)
				z0=ddst.top;
			if (ddst.top>z1)
				z1=ddst.top;
			if (ddst.bottom<z0)
				z0=ddst.bottom;
			if (ddst.bottom<z1)
				z1=ddst.bottom;
			if (z0>=y0 && z1<=(y1))
			{
				BRect jj(x0,z0,x0,z1); //see next line
				InvertRect(jj);
			}
		}
	}
}


void
BMapView::HideCursor()
{
	if (llx>=0.0)
	{
		BRect jj(llx,lly,llx,lly+ffh);
		llx=-1.0;
		MyDirty.Include(jj);
	}
}


void
BMapView::HideCursorNow()
{
	if (llx>=0.0)
	{
		BRect jj(llx-tlx,lly-tly,llx-tlx,lly-tly+ffh);
		llx=-1.0;
		BRegion ff;
		ff.Include(jj);
		ff.Exclude(&MyDirty);
		if (ff.CountRects())
		{
			ConstrainClippingRegion(&ff);
			Draw(jj);
			ff.Include(bnds);
			ConstrainClippingRegion(&ff);
		}
	}
}


void
BMapView::PlaceCursor()
{
	if (vb->vmode==3)
		return;
	SetFont(&myfont);
	vb->Target(this,fh,ffh,wid,cb);
	vb->Locate(lex,ley,cb->Pos(ws->cursor));
	lex-=1.0;
}


void BMapView::DrawCursor()
{
	if (vb->vmode<3)
	{
		if (lex>=0.0 && (llx!=lex  || lly!=ley))
		{
			BRect jj(lex-tlx,ley-tly,lex-tlx,ley-tly+ffh);
			InvertRect(jj);
			llx=lex;lly=ley;
		}
	}
}


//2 minutes..
#define TIMEOUT 120

void
BMapView::Pulse()
{
	ASSERT(Window()->LockingThread()==find_thread(0));
	float nprate=500000.0; //half a second
	if (sleeping && !smp1 && !smp2)
		return;

	int nudrw=FALSE;
	BRegion reg;
	GetClippingRegion(&reg);
	if (reg.CountRects()==0)
	{
//		printf("away\n");
		cstat=1;
		sleeping=TRUE;
	}

	if (sleeping)
	{
		if (smp1)
			smp1->life=TIMEOUT;
		if (smp2)
			smp2->life=TIMEOUT;
		nprate*=3.0;
	}

	if (nprate!=prate)
	{
		ws->yw->SetPulseRate(nprate);
		prate=nprate;
	}
	
	if (smp1 && ++smp1->life>TIMEOUT)
	{
		delete (smp1);
		smp1=NULL;
	}
	
	if (smp2 && ++smp2->life>TIMEOUT)
	{
		delete (smp2);
		smp2=NULL;
	}
	
	if (sleeping)
		return;
	
	if (cstat > -TIMEOUT)
	{
		if (--cstat < 1)
			nudrw = TRUE;
	}
	
	if (nudrw)
	{
		if (llx>=0)
			HideCursor();
		else
		{
			PlaceCursor();
			DrawCursor();
		}
		Flush();
	}

	if (needscrolls)
		CleanScrollers();
}


void
BMapView::MouseDown(BPoint where)
{
	MakeFocus();

	BMessage *msg=ws->yw->CurrentMessage();
	
	msg->FindInt32("buttons", &button);
	
	if(button & B_PRIMARY_MOUSE_BUTTON)
	{
		#if defined(_DebugObj)
			msg->PrintToStream();
		#endif
		mouseb=((msg->FindInt32("clicks")-1)%3)+1;//0,1,2=lines
		where.x+=tlx;where.y+=tly;
	
		{
			BAutolock lck(cb);
			vb->Target(this,fh,ffh,wid,cb);
			int pp=vb->Place(where.x,where.y);
			BMessage msg (A_MOVE_TO);
			msg.AddInt32("Code",7);
			msg.AddInt32("Place",pp);
			msg.AddPointer("YWin",ws->yw);
			cb->PostMessage (&msg);
		}
	}
	else if(button & B_SECONDARY_MOUSE_BUTTON)
	{
		BPoint	point = where;
		ConvertToScreen(&point);
		
		//show pop-up menu
		DoRightMenu(point);
	}
}


void
BMapView::DoRightMenu(BPoint point)
{
	mnuSnip = new snippetmenu("mnuSnip");
	BMenuItem *selection;
	selection = mnuSnip->Go(point, true,true,(BRect(point.x,point.y,point.x + 1,point.y + 1)),false);
	
	if(selection)
	{
		NSnipMenuItem *item = dynamic_cast<NSnipMenuItem *>(selection);
		BString itemcode = item->CodeToInsert;

		if(strcmp(itemcode.String(), "ADD_ITEM") == 0)
		{
			//show manager
			be_app->PostMessage(A_SHOW_SM);
//			wdwSnippet = new snippetwindow();
//			wdwSnippet->Show();
		}
		else if(strcmp(itemcode.String(), "REMOVE_ITEMS") == 0)
		{
			//clean list
			BPath prefspath;
			
			//find prefsfile
			find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
			prefspath.Append("Niue/niue_snippets");				
			//open file for reading
			BEntry entry(prefspath.Path(), false);
			if(entry.Remove() != B_OK)
			{
				(new BAlert("Niue", "There was an error while trying to clean up your snippets", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}

		}
		else
		{
			//use the item
			BMessage msg(A_INSERT_SNIPPET);
			msg.AddString("snip", itemcode);
			cb->PostMessage(&msg);
			selection = 0; //cleanup
		}
	}
}


void
BMapView::SeekMouse()
{
	BPoint  where;
	ulong   but=0;
	GetMouse(&where, NULL);
	where.x+=tlx;where.y+=tly;
	if (lastmouse!=where)
	{
		BAutolock lck(cb);
		vb->Target(this,fh,ffh,wid,cb);
		int pp=vb->Place(where.x,where.y);
		BMessage msg (A_MOVE_TO);
		msg.AddInt32("Code",6);
		msg.AddInt32("Place",pp);
		msg.AddPointer("YWin",ws->yw);
		cb->PostMessage (&msg);
//      if (mouseb>1) cb->Squash(blinkcurs,mouseb);
	}
	
	if (but==0)
	{
		stx=-1;mouseb=0;
		lastmouse=BPoint(-1,-1);
	}
	else
		lastmouse=where;
}

void BMapView::MouseMoved(BPoint,uint32 tr,const BMessage*msg)
{
	#if defined(_DebugObj)
		if (msg){
			printf("MouseMoved\n");
			msg->PrintToStream();
		}
	#endif

	switch (tr)
	{
		case B_ENTERED_VIEW:
		{
			be_app->SetCursor(B_I_BEAM_CURSOR);
			MakeFocus();
			break;
		}
		case B_EXITED_VIEW:
		{
			be_app->SetCursor(B_HAND_CURSOR);
			break;
		}
//		case B_INSIDE_VIEW:
//		{
//			MakeFocus();
//			break;
//		}
	}
}


void
BMapView::ChangeBuff(Buff *bb)
{
	cb=bb;
	scbh->Capture(ffh);
	scbv->Capture(ffh);//wild huh?
	CleanScrollers();
	SeekCursor();
	scbh->ChangeIndex(ffh,bb,tax);
	scbv->ChangeIndex(ffh,bb,tay);//wild huh?
	FlickTo(scbh->Value(),scbv->Value());
	Invalidate(bnds);
	ltop=-1;
}


// a used-to-be-sample-rate-invariant exponential decay
// (with wicked acceleration…)
void
BMapView::Closer(float &src,float &vel,float dest,bigtime_t mut)
{
	double smaller=0.95-0.3*(1.0-1.0/(1.0+fabs(src-dest)*0.004));
	if (smaller<0.4)
		smaller=0.4;
	if (smaller>0.95)
		smaller=0.95;
	double smallerlog=log(smaller);
	double overlog=1.0/smallerlog;
	
	double mult=mut/32768.0;
	dest=floor(dest)+0.5;
	double diff =(dest-src) * smallerlog - vel; //thanx maple!

	vel+=diff;

	double fac=pow(smaller,mult);
	src+=vel*(1.0-fac)*overlog;
	vel=vel*fac;
}



void BMapView::SeekCursor()
{
	float   nx=0,ny=0;
	SetFont(&myfont);
	cb->Lock();
	int32 p=cb->Pos(ws->cursor);
	vb->Target(this,fh,ffh,wid,cb);
	vb->Locate(nx,ny,p);
	cb->Unlock();
	
	BRect   bb=bnds;
	float   t;
	
	t=nx-wid+20;if (tax<t) tax=t;
	t=ny-bb.Height()+fh*3.5;if (tay<t) tay=t;
	
	t=nx-20;if (tax>t)tax=t;
	t=ny-fh*2.5;if (tay>t)tay=t;
	
	
	if (tax<0.0)
		tax=0.0;
	if (tay<0.0)
		tay=0.0;
}


void
BMapView::SeekScrollBar()
{
	tax=scbh->Value();
	tay=scbv->Value();
}


void
BMapView::MyScrollTo(int32 pos)
{
	if (pos<0)
		return;
	float x,y;
	vb->Target(this,fh,ffh,wid,cb);
	vb->Locate(x,y,pos);
	y-=(bnds.Height()-fh)*0.5;
	x-=wid*0.75;
	MyScrollTo(x,y);
}


void
BMapView::MyScrollTo(float x,float y)
{
	tax=x;tay=y;
	if (y<0)
		y=0;
	if (x<0)
		x=0;
	scbh->SetValue(x);
	scbv->SetValue(y);
	write_port(ws->drprt,SCROLL,0,0);
}


void
BMapView::FlickTo(float tax,float tay)
{
	pox=tax;poy=tay;
	scbh->SetValue(tax);
	scbv->SetValue(tay);
}


int32
BMapView::Scribble(int32 redw,int32 willscroll,int32 tophit,int32 bothit,int32 chcu)
{
	BAutolock l1(ws->yw);
	BAutolock l2(cb);

//  for (int i=0;i<9;i++){ops[i]=0;}
	bothit=MAX(tophit,bothit);
	
	float xx,yy;
	vb->Target(this,fh,ffh,wid,cb);
	vb->Locate(xx,yy,tophit);
	BRect rr(0,0,-1,0);
	
	if (!targetnum)
		printf("no targetnum!!!\n");
	
	switch (ws->vmode)
	{
		case 1: //text
		case 2://c++
		{
			{
				int32 jj=cb->Count(10,0,cb->Pos(targetnum))-ltop;
				if (ltop>-1 && jj){
					float mv=jj*fh;
					tly+=mv;
					tay+=mv;
					rpy+=mv;
					poy+=mv;
					ley+=mv;
					lly+=mv;
					MyDirty.OffsetBy(0,mv);
					if (smp1)smp1->when=-1;
					if (smp2)smp2->when=-1;
					scbv->SetValue(scbv->Value()+mv);
//                  printf("change! %d\n",jj);
				}
			}
			
			if (redw&2)
				rr.Set(0,yy,40000,yy+40000);
			else
			{
				if (tophit!=bothit)
				{
					float x2,y2;
					vb->Locate(x2,y2,bothit);
					rr.Set(0,yy,40000,y2+ffh);
				}
			}
			break;
		}
		case 3://html (!!)
		default:
		{
			rr.Set(0,yy,40000,yy+40000);
			break;
		}
	}

	if (rr.IsValid())
		MyDirty.Include(rr);

	if (llx>=0 || chcu)
	{
		PlaceCursor();
		if (lex!=llx || ley!=lly)
		{
			HideCursor();
			DrawCursor();
		}
	}

	if (chcu)
		cstat=2;
	redw=0;

	if (mouseb)
		SeekMouse();
	
	{
		int32 p=cb->Pos(ws->cursor),cx,cy;
		vb->Locate2(cx,cy,p);
		if (cx!=lcx || cy!=lcy)
		{
			ws->yw->MoveCurs(cx,cy);
			lcx=cx;lcy=cy;
		}
	}
	
	
	switch(willscroll)
	{
		case 1:
		{
			SeekCursor();
			scbh->SetValue(tax);
			scbv->SetValue(tay);
			break;
		}
		case 2:
		{
			SeekScrollBar();
			break;
		}
	}

	mynow=system_time();

	pox=tax;poy=tay;
	
	if (pox<0.0)
		pox=0.0;
	if (poy<0.0)
		poy=0.0;

	rpx=floor(pox);rpy=floor(poy);
	if (rpx<0.0)
		rpx=0.0;
	if (rpy<0.0)
		rpy=0.0;
	if (cb)
	{
		BRect bb=bnds;
		bb.OffsetBy(BPoint(rpx,rpy));
//      CleanScreen(bb);
		MyDraw();
		
		int p1=0,p2=0;
		p1=vb->Place(tlx,tly-fh);
		p2=vb->Place(tlx,tly+bnds.Height()+fh);
		p1=MAX(1,p1);
		p2=MAX(2,p2);
		cb->MoveTo(targetnum,p1);
		cb->MoveTo(targetnum+1,p2);
		ltop=cb->Count(10,0,cb->Pos(targetnum));
	}
	
	Sync();
	return 0;
}


long
BMapView::DrawThread()
{
	int32 cod=0;

	int willscroll=1;
	int redw=0;

	int32 spare[32];
	int32 tophit=0,bothit=0x7fffffff;

	while (read_port(ws->drprt,&cod,spare,sizeof(int32)*32)>=B_NO_ERROR)
	{
		int chcu=0;
		
		if (cod==SCROLLBAR_MOVE)
			willscroll=2;
		
		if (cod==1024)
			redw|=4;
		if (cod==1025 || cod==1026)
		{
			if (spare[1]>spare[0])
			{
				tophit=MIN(tophit,spare[0]);
				bothit=MAX(tophit,spare[1]);
			}
			if (cod==1025)
				redw|=1;
			if (cod==1026)
				redw|=2;
		}
		if (cod>1023 && cod<1026)
		{
			if (spare[2]==ws->cursor)
			{
				chcu=1;
				willscroll=1;
			}
		}
		
		if (port_count(ws->drprt)==0)
		{
			if (vb->vmode==3)
				willscroll=2;
			Scribble(redw,willscroll,tophit,bothit,chcu);
			tophit=0x7fffffff;
			bothit=0;
			if (redw&3)
				needscrolls=TRUE;
			redw=0;
			sleeping=FALSE;
			
			snooze(5000);// 200fps (yeah, right..)
			int outtag=0;
			if (mouseb || floor(pox)!=floor(tax) ||	floor(poy)!=floor(tay))
				outtag=SCROLL;

			if (outtag)
				write_port(ws->drprt,outtag,NULL,0);

			lastscroll=port_count(ws->drprt)?mynow:0;
		}
	}
	return 0;
}


void BMapView::ChangeCursor(int32 n)
{
	BAutolock l1(cb);
	int32 cc=cb->Pos(n);
	if (cc<0)
		cb->MoveTo(n,1);

	ws->cursor=n;
}
