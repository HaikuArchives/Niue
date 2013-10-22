
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



#ifndef NIUE_WINDOW_H
#define NIUE_WINDOW_H

#include <Rect.h>
#include <Window.h>
#include <TextView.h>
#include <Clipboard.h>
#include <Roster.h>
	
#include "mover.h"
#include "listview.h"
#include "openwindow.h"
#include "IconMenu.h"
#include "outputview.h"
#include "exportwindow.h"
#include "makefilewindow.h"
#include "docwindow.h"
#include "visualwindow.h"
#include "grepwindow.h"
#include "snippetwindow.h"

class BMenu;
class BMenuBar;
class BMenuItem;
class BFilePanel;
class BSBar;

class BMapView;
class StatusView;
class HintView;
//	class HintWindow;

class NiueWindow : public BWindow 
{
public:
					NiueWindow(BRect frame,class Buff *);
	virtual			~NiueWindow();
	virtual bool    QuitRequested();
	virtual void    MessageReceived(BMessage *msg);
	virtual void    MenusBeginning();
	virtual void    ScreenChanged(BRect screen_size, color_space depth);
	entry_ref		SaveProject(const char *dest, const char *name);
	virtual void	LoadPrefs();
	virtual void	SavePrefs();
	void            ChangeBuff(Buff*);
	Buff*           MyBuff(){return(cb);}

	void            MyMetrics();
	void            Shove(BView *,BRect pl);
	void            FixDisp();

	void            MoveCurs(int cx,int cy);
	int32           mycurs;//for errors.. fixme :-)
	void            HandleFontDrop(BMessage *msg);

	StatusView      *fStatusView;
	HintView*		fHintView;
	Mover*			fVMover;
	Mover*			fHMover;
	listview*		fListView;
	OutputView*		fOutputView;
	BRect			fFrame;
	BView*			fFillView;
	
				
	bool			hasProjectOpen;
	bool 			doCodeCompletion;
	virtual void	AddFile(entry_ref fileref);
	virtual void	LoadDir(entry_ref dirref);
	
	snippetwindow*	fSnippetWindow;
//	HintWindow*		wdwHint;
	
private:
	friend class StatusView;
	
	Buff            *cb;
	BMapView        *fMapView;
	long            KeyDown(BMessage *msg);
//	void            ChangeFont(BMessage*); using fontdrop now..
	void            GotoLine(BMessage *);
	int32           wincnt;
	int32           IsHScroll; //,IsMsg;

	BSBar           *scbh,*scbv;
	BMenu           *MenuFrom(struct MenIt *mi);
	void            CreateMenus(BMenu  *);

	BMenu           *topmenu;

	BMenu           *fname,*fstyle,*fsize;
	BMenu           *mdir,*mwin, *mproj, *mtools, *mapp, *mpref;
	BMenu           *mdoc,*medit1,*medit2,*mdoclist;
	BMenu           *mmime,*mcurs,*mview,*menc;
	BMenuItem       *icurs,*ipref,*ifname,*ifstyle,*ifsize;
	BMenuItem       *idir,*imime,*iview;
	TIconMenu       *fApp;

	BMenuItem       *mnxt,*mprv;
	BList           bmen,fnl,fsl;

	thread_id       fDrawThread;
	port_id         fDrawPort;

	char            fOldTitle[1024];

	void            DoMenuFontName();
	void            DoMenuFontSize();

	BRect       	rmyview,rssv,rsbh,rsbv,rmover,rlist, hmover, routput, rfill, rhint;
	
	int 			vsplit, hsplit, ysplit, xsplit;  //= vert mover, bottom filelist, bottom sourceview, right output
	
	float 			mbh;
	
	BWindow			*fOpenWindow;
	BWindow			*fHelpWindow;
	BWindow			*fExportWindow;
	BWindow			*fMakeWindow;
	BWindow			*fVisualWindow;
	BWindow			*fGrepWindow;

	BEntry 			fEntry;
	entry_ref 		fRef;
	
	BFilePanel		*fSavePanel;
	BFilePanel		*fBackupPanel;
	
	int				fResult;

	void Fumble(BMenu *top,BMenuItem **mi,BMenu **mn,MenIt *mit);
	friend class NiueApplication;
};


class STextView:public BTextView
{
public:
				STextView(BRect frame,const char *name,uint32 flags,long msg);
		virtual void KeyDown(const char *bytes, int32 numBytes);
		virtual void Draw(BRect);
		virtual void MakeFocus(bool focusState = TRUE);

private:
		long mymsg;
};


class StatusView : public BView
{
public:
					StatusView(BRect rect, char *name,port_id pp);
	virtual 		~StatusView();
	virtual void	Draw(BRect);
			void    SetPos(int xx,int yy);
			void    SetSearch(const char *);
			void    SetReplace(const char *);
			void    ShowMsg(const char *m);
			void	MyNav(STextView *,int dir);
	
			BRect		xpos,ypos,spos,rpos,mpos;
			STextView   *xrv,*yrv,*srv,*rrv,*mrv;
private:
			int cvx,cvy;
};


class HintView : public BView
{
public:
					HintView(BRect hrect, const char *name);
					~HintView();
	
	BListView*		lstHints;
	
private:
	Mover			*split;
	BScrollView		*scrHints;
};

/*	
	class HintWindow : public BWindow {
		public:
							HintWindow(BRect hrect, BList list);
							~HintWindow();
			virtual void	MessageReceived(BMessage *msg);
			virtual void	UpdateList(BList list);
			BListView*		lstHints;
		private:
			BScrollView*	scrHints;
			
	};
*/

#endif



