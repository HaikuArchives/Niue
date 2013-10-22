
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


#ifndef UTILS_H
	#define  UTILS_H

	inline int SIGN(signed long x){return x >> 31;}


	//inline int SIGN(signed long x){return (x<0) ? ~1 : 0;}


	#ifndef MIN
		inline int MIN(int x, int y){int t = y - x;t &= SIGN(t);return x + t;}
	#endif
	#ifndef MAX
		inline int MAX(int x, int y){int t = y - x;t &= SIGN(t);return y - t;}
	#endif
	inline void EXG(int &x,int &y){int t=x;x=y;y=t;}
	inline void SORT(int &x,int &y){int t = y - x;t &= SIGN(t);x+=t;y-=t;}
	inline void EXG32(int32 &x,int32 &y){int t=x;x=y;y=t;}
	inline void SORT32(int32 &x,int32 &y){int t = y - x;t &= SIGN(t);x+=t;y-=t;}
	inline int BND(int x,int a,int b){return MIN(MAX(x,a),b);}

	#ifndef ABS
		inline int ABS(int x){int y=SIGN(x);return y^(y+x);}
	#endif

	#ifndef NULL
		#define NULL 0L
	#endif

#endif
