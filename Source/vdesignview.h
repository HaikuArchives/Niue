
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


#ifndef VDESIGNVIEW_H
#define VDESIGNVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <String.h>
#include "vdefines.h"
#include "velementsview.h"
#include "colors.h"

class designview;
class vdesignview : public BView 
{

	public:
										  vdesignview(BRect frame, char *name, uint32 resizingMode, uint32 flags); 
			virtual void			FrameResized(float width, float height);
			virtual void			CenterTarget();
			designview*			vwDes;
			BStringView*		strStatus;			

	private:

			BScrollView*		scrDes;
			BView*				vwMain;

};

class designview : public BView
{

	public:
										designview(BRect rect, char *name, uint32 resizingMode, uint32 flags);
										~designview();
		virtual void			Draw(BRect);
		virtual void			MouseMoved(BPoint point, uint32 transit, const BMessage *message);
		virtual void			MouseDown(BPoint where);
		virtual void			AddElement(BPoint where, char *controltype);
		virtual void			ResetCounters();
		bool					   ShowGrid;
		bool					   ShowHandles;		
		bool						IsDragging;
		int						  scale;
		char*					  currenttype;

	private:

		BStringView*		  strLocation;	
		int							boxes, buttons, picturebuttons, radiobuttons, checkboxes, stringviews, textcontrols, textviews, listviews;

		

};

class placeholder : public BView
{

	public:
									placeholder(BRect prect, const char *name, const char *plabel, const char *ptype, int32 pid, uint32 resizingMode, uint32 flags);
		virtual void			  	Draw(BRect);
		virtual void				MouseDown(BPoint down);
		virtual void				MouseUp(BPoint up);
		virtual void				MouseMoved(BPoint move, uint32 transit, const BMessage *message);
		const char*					type;
		const char*					label;
		const char*					resX;
		const char*					resY;
		int32						id;
		bool						active;
			
	private:

		bool						moving;
		bool						resize;
		BPoint						clickPoint;
		BRect					 	initialSize;

};

#endif
