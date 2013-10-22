
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


#include <string.h> //strlen()
#include <stdlib.h> //rand() for rcol()

#include <Application.h> //rgb_color
#include "Schemes.h"

rgb_color   rcol();
colscheme *schemetab[2];    //icky
rgb_color col(int32 r,int32 g,int32 b){
	rgb_color cc;
	cc.red=r;
	cc.green=g;
	cc.blue=b;
	return cc;
}

int vvv(char a,char b);
int vvv(char a,char b){
	int aa=0,bb=0;
	if (a>='a' && a<='f')aa=a-'a'+10;
	if (a>='A' && a<='F')aa=a-'A'+10;
	if (a>='0' && a<='9')aa=a-'0';
	if (b>='a' && b<='f')bb=b-'a'+10;
	if (b>='A' && b<='F')bb=b-'A'+10;
	if (b>='0' && b<='9')bb=b-'0';
	return aa*16+bb;
}

rgb_color col(char *src){
	int32   rr=0,gg=0,bb=0;
	if (src && strlen(src)>6){
		rr=vvv(src[1],src[2]);
		gg=vvv(src[3],src[4]);
		bb=vvv(src[5],src[6]);
	}
	return col (rr,gg,bb);
}



rgb_color rcol(){return col (rand()&255,rand()&255,rand()&255);}

void MakeSchemes(){
	static int dead=TRUE;
		//warning! don't use "pure" black, it
		//maps to col 255 (transparent)
	if (dead)
	{
		colscheme *ms;

		for (int i=0;i<2;i++){
			ms=schemetab[i]=new colscheme();
		}

		ms=schemetab[0];
		ms->background=col("#0A0A0A"); //See above!
		ms->greycursor=col(120,120,120);
//		ms->redcursor=col(150,0,0);
//		ms->greencursor=col(0,120,0);
//		ms->bluecursor=col(30,30,120);

		char *DarkStyle[]={
				"#F4E020","#DFE4E1","#FF3474",
				"#ACC3FF","#ECAD6E","#F4E020",
				"#53FBAB","#B8DB8A","#FFFFFF",
				"#FF8494","#A8DFEf","#DCADBE", //comment,maileven,mailodd
				"#ACC3FF","#ACC3FF","#2FFF62",
				"-1"};

		int32 cc=0;
		for (int i=0;i<30;i++)
		{
			rgb_color   nc=ms->style[0];
			if (DarkStyle[cc][0]=='#')
			{
				nc=col(DarkStyle[cc++]);
			}
			ms->style[i]=nc;
		}

		ms=schemetab[1];
		ms->background=col(255,255,255);
		ms->greycursor=col(255,236,124);
//		ms->redcursor=col(255,200,200);
//		ms->greencursor=col(200,255,200);
//		ms->bluecursor=col(200,200,255);
		ms->style[0]=col(10,10,10); //See above!
		ms->style[1]=col(63,72,132);	//blue
		ms->style[2]=col(161,90,14);	//brown
		ms->style[3]=col(57,116,121);	//seablue
		ms->style[4]=col(68,138,0);		//green
		ms->style[5]=col(170,0,0);	//darkred			//(20,140,0);

/*
		ms=schemetab[2];
		ms->background=rcol();
		ms->greycursor=rcol();
		ms->redcursor=rcol();
		ms->greencursor=rcol();
		ms->bluecursor=rcol();
*/
		dead=FALSE;
	}
}
