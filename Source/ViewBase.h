
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


	#include <Rect.h>

	class BView;
	class nnode;
	class Buff;

	class ViewBase{
		public:
			ViewBase();
			~ViewBase();

			void Target(BView *vv,float fh,float ffh,float width,Buff *cb);

			void Draw(BRect dest,float tlx,float tly);
			void Locate2(int32 &x,int32 &y,int place);
			void Locate(float &x,float &y,int place);
			float LocateY(int place);
			int Place(float x,float y);

			int vmode;
			struct colscheme *csh;
		private:

			void DrawTxt(BRect dest,float tlx,float tly);
			float LocTxtY(int place);
			void LocTxt(float &x,float &y,int place);
			void LocTxt2(int32 &x,int32 &y,int place);

			void SHC(const struct rgb_color &n);


			int MassageTxt(char *s,int l,char *dst);

			Buff *cb;
			BView *vv;
			float fh,ffh,width;
			nnode *nt;
	};
