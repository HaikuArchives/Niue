
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


#ifndef SMAP_H
	#define SMAP_H

	#include <Bitmap.h>
	#include <View.h>

	#define MAX_REG 16

	class SMap : public BBitmap{

		public:
						SMap(color_space space,long xx, long yy);
			virtual     ~SMap();
			float       origx,origy,bigx,bigy;

//            void        GetCXY(int cnum,float &x,float &y);
			void        AssertStats(class Buff *,class BFont *,int32 style,struct colscheme *);
			BRect       FixDamage(BRect src);
			color_space uspace;

			BRect validrect;
			int32       When(){return when;};
			int32       life,when;

			BView       *bmv;

		private:
			void       ReDrawLine(char *txt,int sx2,float scrx,float scry);
			void       ReDrawLin(char *txt,float scrx,float scry);
			void        Mark(BRegion *reg,int curs);

			int32       sstyle;

			BRect       cdrw;
			float       fh,ffh,spcw,szex;
			BFont       myfont;
			Buff        *cb;

			BRegion     rg[MAX_REG];
			int32           rcod[MAX_REG];
			rgb_color   rcode[MAX_REG];
			int32           creg;
			int32           mod;
			char            *dat;

			void        Inserter(BRegion *in,int32 fr,int32 cc);
			void        MySetCol(rgb_color col);
			int32       fillcol[2];
			void        MyFillRect(BRect rect);
			void        MyFillRegion(BRegion *reg);

			class CList       *overlist,*copylist,*freelist;

			float QueueString(char *,int32,float,float,rgb_color);
			void FlushDrwQueue(CList *,int32 type);

			rgb_color ColForState(int32 state);

			struct colscheme *myscheme;

			void MyLowColor(rgb_color);
			rgb_color   llc;
	};

#endif //SMAP_H
