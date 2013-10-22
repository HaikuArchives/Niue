
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


#ifndef BMAP_VIEW_H
#define BMAP_VIEW_H

#include <View.h>
#include <Region.h>

#include "UBuffer.h"
#include "actions.h"
#include "snippetmenu.h"

class CList;
class NiueWindow;
class SMap;
class Buff;
class BSBar;
class ViewBase;
class WinStats;

#define SCROLL          100
#define SCROLLBAR_MOVE  160

class BMapView : public BView
{
public:
					BMapView(NiueWindow *,Buff*,BRect frame, char *name,BSBar *s1,BSBar *s2,port_id pp);
	virtual ~BMapView();
	virtual void    Draw(BRect updateRect);
	virtual void    MouseDown(BPoint where);
	virtual void    MouseMoved(BPoint,uint32,const BMessage*);
	virtual void    FrameResized(float new_width, float new_height);
	virtual void    Pulse();
	virtual void    KeyDown(const char *bytes, int32 numBytes);
	virtual void	DoRightMenu(BPoint point);
			bool	HasSelection();

			int32           DrawThread();

			SMap        *PrepareBlit(BRect dest, BRect *src);

			void            MyScrollTo(float x,float y);
			void            MyScrollTo(int32 pos);
			void            CleanScrollers();
			void            FlickTo(float x,float y);

			void            ChangeBuff(Buff *);
			void		HalfCursor(const BRect &r);
			void        HideCursor();
			void        HideCursorNow();
			void        PlaceCursor();
			void        DrawCursor();
			void		ChangeCursor(int32 newcursor);

			WinStats	*ws;
			int32		targetnum;
			float		fh,ffh;
			Buff		*cb; //Current Buffer
			BSBar		*scbv,*scbh;
			BFont		myfont;
			color_space	cspace;
			ViewBase	*vb;
			
			float		tax,tay,vvx,vvy,pox,poy;		

private:
			BRect		bnds;
		
			float		tlx,tly,wid;
			int32		ltop;
		
			BRegion		ScreenClean;
			BRegion		MyDirty;
			void		MyDraw();
		
			void		CleanScreen(BRect rr);
		
			void		SeekMouse();
			void		SeekCursor();
			void		SeekScrollBar();
		
			void		Closer(float &,float &vel,float dest,bigtime_t time);
			void		DoKeyDown(const char *bytes, int32 key,int32 modifiers);
			int32		Scribble(int32,int32,int32,int32,int32);
		
			int32		cstat,sleeping;
			int32		stx,sty;//for mouse-dragging
		
			float		prate;
		
			color_space	uspace;
			SMap			*smp1,*smp2;       //cached drawing
			int32			lastsmp,
							lastshift,
							needscrolls,
							smx,
							smy,
							mouseb,
							button,
							lmx,
							lmy,
							lcx,
							lcy;
		
			float			rpx,rpy;
		
			bigtime_t		lastscroll,mynow;
			BPoint			lastmouse;
			float			lex,ley;
			float			llx,lly;
			
			snippetmenu		*mnuSnip;
//			snippetwindow	*wdwSnip;
};


#endif //BMAPVIEW_H

