
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


#ifndef UBUFFER_H
	#define UBUFFER_H

	#include <Looper.h>
	#include <Entry.h>
	#include <MessageQueue.h>
	#include <String.h>

	#include "actions.h"
	#define Y_NAME_LENGTH 20

	class BFilePanel;
	class NiueWindow;

	class WinStats{
		public:
			WinStats();
			~WinStats();
			NiueWindow *yw;

			int32           typedir,pgdn,vmode,cursor;
			port_id         drprt;      //redraw port

			int32 xtra,xtra2;  //user defined (!)

			void Copy(WinStats*);

	};

	class Buff:public BLooper{
			friend class BufList;
			public:
							Buff();//constructor
			virtual ~Buff();//destructor

			virtual void MessageReceived(BMessage *msg);

			void        MoveTo(int32 cnum,int32 pos);
			int32   Pos(int32 cnum);
			int32   AddChar(int32 cnum,const char *source,int32 length,int32 dir=0);
			int32   Size();
			status_t    Find(const char *src,int32 &pos1,int32 &pos2,int32 dir);

			void        GetFromEnt(entry_ref*);
			void        PutToEnt();
			void        PutToDir(entry_ref *ref,char *name);

			char        cmime[256];
			char        mytitle[Y_NAME_LENGTH];
			char        myname[B_FILE_NAME_LENGTH];
			bigtime_t   lastchanged;

			entry_ref   myent;
			int32       entchanged;
			void        PutMIMEType(int32 type);
			void        Change(int32 type);

			int32       term,utf8,alone,fried;
			int32       NextError(NiueWindow *,char *text,int32 *line);

			int        Find(const char *search,int32 dir);
			int        Replace(const char *search,const char *replace);

			int32 Fetch(char *dest,int32 size,int32 place);
			int32 Count(int32 type,int32 p1,int32 p2);
			class nnode *mytree;

			void TX(BMessage*);
			void TX(int32 type,int32 dat1=0,int32 dat2=-1); // transmit to drawport..
			int32 Register(WinStats*);
			void DeRegister(WinStats*);
			int32 Now();
			NiueWindow* WinAt(int32 idx);

			int encoding;

		private:
			void MessageReceived2(BMessage *msg);

			void MoveBy(int32 cnum,int32 dx,int32 dy);
			void MoveFlat(int32 cnum,int32 delta);

			int32	AddCharReal(int32 cnum,const char *source,int32 length);
			int32	Remove(int32 p0,int32 p1);

			void        Push(class UndoOp *uo);
			void        PushMarker(int32 dat);
			void        PushDelete(int32 x0,int32 x1);
			void        PushAsc(int32 x,char *txt,int32 len);
			void        Undo(NiueWindow *yw,int32 cn,int32 &s1,int32 &s2,int32 dir);

			void        Squash(long cnum,long typ);
			void        MyDelChar(int32 cnum,int32 dir,int32 typedir=0);
//			void        Browse(NiueWindow *,YAction type);
			void        IndentiQuote(int32 cnum,YAction type,int32 &s1,int32 &s2);
			status_t      CtrlChar(int32 ch);
			int32           StripToPound(int32 starty);
			status_t       MyExec(NiueWindow*,int32 starty,char *);

			void GetFile(class BFile *);
			void PutFile(class BFile *);

			char    *mybuf;//internal storage..
			void    CheckBuff();
			void    FreeBuff();
			BFilePanel      *savepanel;

			void			CCMsg(int32 am,int32 what,NiueWindow *yw);
			int32           CopyQuote(int pos1,int pos2,int type);
			int32           Paste(BMessage*);
			static int32 ReaderFunc(void *);

			char    *cmdbuff;
			int32     bsize,now;
			int     pipefd[2];
			thread_id   rdthread;
			pid_t       expid;
			NiueWindow  *submerged;

			void FError(const char*);
			class CList       *undostack;
			int32   undoitem,usemarkers,isrec,marklevel;

			void Capture();
			void DoFoc(NiueWindow*);
			class nplace *mypos[257];
			CList   *winlist;
			int32   curs;
			WinStats *cws;

			friend class NiueApplication;
	};

	extern long UTFWidth(char *s,long l);
	extern long WidthUTF(char *s,long &l,long max);
	extern long CountWhite(char *s,long max);
	extern long TrailWhite(char *s,long st);

	extern char *MyMimeType(int idx);

#endif //UBUFFER_H

