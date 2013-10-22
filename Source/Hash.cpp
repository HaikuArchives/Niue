
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



#include <stdio.h> //error message.. icky
#include <malloc.h>
#include <string.h>
#include <String.h>

#include <Application.h>
#include <Resources.h>
#include <Roster.h>
#include <ListItem.h>

#include "Hash.h"

struct hsh
{
	struct hsh* follow;
	const uchar *txt;
	int len;
};

	/* if you change this, change HashFunc() as well*/
	#define HASHENT 512


	void InitHashTable();

	class Hash *cpphash,*behash, *consthash;

	class Hash
	{
		public:
			Hash();
			~Hash();
			int ReadDat(char *src,int length);
			int Contains(const uchar *src,int count);
			int uselowercase;
			hsh* myhashtab[HASHENT];
			int HashFunc(const uchar *src,int count);
		private:
			void Insert(const uchar *src,int count);
			int HashCompare(hsh *top,const uchar *source,int count);
			
	};
	bool IsInteger(const uchar *src, int count);
	bool IsFloat(const uchar *src, int count);
	HTYPE IsNumber(const uchar *src,int count);




int Hash::HashCompare(hsh * top,const uchar *src,int count)
{

	while (top)
	{
		if (top->len==count)
		{
			if (uselowercase)
			{
				if (strncasecmp((char*)src,(char*)top->txt,count)==0) return TRUE;
			}else
			{
				if (strncmp((char*)src,(char*)top->txt,count)==0) return TRUE;
			}
		}
		top=top->follow;
	}
	return FALSE;
}

BList ComparePart(char *lookup, int length)
{
	
	BList result;
	hsh *test;

	if(length < 2 || length > 6)
	{
		return result;
	}
	
	for(int i=0; i<HASHENT; i++)
	{
		test = behash->myhashtab[i];
		while(test)
		{
			if (strncmp(lookup,(char*)test->txt,length)==0)
			{
//				 result << (char*)test->txt << ": " << i << "\n";
				result.AddItem(new BStringItem((char*)test->txt));
			}
			test=test->follow;
		}
		
		test = cpphash->myhashtab[i];
		while(test)
		{
			if (strncmp(lookup,(char*)test->txt,length)==0)
			{
//				 result << (char*)test->txt << ": " << i << "\n";
				result.AddItem(new BStringItem((char*)test->txt));
			}
			test=test->follow;
		}
		
		if(length > 2)
		test = consthash->myhashtab[i];
		while(test)
		{
			if (strncmp(lookup,(char*)test->txt,length)==0)
			{
//				 result << (char*)test->txt << ": " << i << "\n";
				result.AddItem(new BStringItem((char*)test->txt));
			}
			test=test->follow;
		}


		

	}
	
	return result;	
}



void Hash::Insert(const uchar *src,int count)
{
	int dd=HashFunc(src,count);
	hsh *hs=new hsh();
	hs->follow=myhashtab[dd];
	hs->txt=src;
	hs->len=count;
	myhashtab[dd]=hs;
//    printf("%d,%x,%x\n",dd,hs,hs->follow);
}

int Hash::HashFunc(const uchar *src,int count)
{
	int res=count;
	if (uselowercase){
		for (int i=0;i<count;i++)
		{
			int s=src[i]&(255-32);
			res^=((s)^(res<<3)^(res>>6))&(HASHENT-1);
		}
	}else{
		for (int i=0;i<count;i++)
		{
			int s=src[i]&255;
			res^=((s)^(res<<3)^(res>>6))&(HASHENT-1);
		}
	}
	return res&(HASHENT-1);
}

Hash::Hash()
{
	for (int i=0;i<HASHENT;i++)
	{
		myhashtab[i]=NULL;
	}
	uselowercase=FALSE;
}

int Hash::ReadDat(char *_dest,int )
{
	uchar *dest=(uchar*)_dest;
	int cnt=0;
	while (*dest)
	{
		int bad=FALSE;
		uchar *last=dest;
		while (*dest!='\n')
		{
			if (!*dest){bad=TRUE;break;}
			dest++;
		}
		if (!bad)
		{
			*dest++=0;
			if (dest-last>1){ Insert(last,dest-last-1);cnt++;}
		}
	}
//	printf("%d entries loaded\n",cnt);
	return cnt;
}
Hash::~Hash(){}

int Hash::Contains(const uchar *src,int count)
{
	int dd=HashFunc(src,count);
	if (HashCompare(myhashtab[dd],src,count)) return HCTOKEN;
	return 0;
}

void InitHashTable()
{
	app_info    myinfo;
	static int alive=FALSE;
	if (alive) return;
	cpphash=new Hash();
	behash=new Hash();
	consthash=new Hash();
//	htmlhash->uselowercase=TRUE;
	alive=TRUE;

	status_t rr=be_app->GetAppInfo(&myinfo);
	if (rr<B_OK) return;
	BFile fill(&myinfo.ref,B_READ_ONLY);
	if (!fill.IsReadable()) return;
	BResources *bres=new BResources(&fill);
	if (!bres) return;

	int badcp=0;
	size_t  l=0;
	bres->FindResource('TOKE',1,&l);
	if (l>0)
	{
		char *tokbuf;
		tokbuf=(char*)malloc(l);*tokbuf=0;
		bres->ReadResource('TOKE',1,tokbuf,0,l);
		if (!cpphash->ReadDat(tokbuf,l)) badcp=2;
	}
	else
	{
		badcp=10;
	}
	if (badcp)
	{
		printf("Couldn't locate Resources.. (%d)\n",badcp);
	}
	
	bres->FindResource('TOKE',2,&l);
	if (l>0)
	{
		char *tokbuf;
		tokbuf=(char*)malloc(l);*tokbuf=0;
		bres->ReadResource('TOKE',2,tokbuf,0,l);
		behash->ReadDat(tokbuf,l);
//	    printf("read %d\n",yy);
	}
	

	bres->FindResource('TOKE',3,&l);
	if (l>0)
	{
		char *tokbuf;
		tokbuf=(char*)malloc(l);*tokbuf=0;
		bres->ReadResource('TOKE',3,tokbuf,0,l);
		consthash->ReadDat(tokbuf,l);
//	    printf("read %d\n",yy);
	}

	delete bres;
	return;
}

bool IsInteger(const uchar *src, int count)
{
	int mode=1;

	if (count>2 && src[0]=='0'){
		if (src[1]=='x' || src[1]=='X'){
			src+=2;count-=2;mode=2;
		}
	}

	int ll=0,uu=0;
	int sfx=TRUE;
	while (sfx){
		sfx=FALSE;
		char c=src[count-1];
		if (c=='l' || c=='L'){count--;sfx=TRUE;ll++;} //long or Long
		if (c=='u' || c=='U'){count--;sfx=TRUE;uu++;} //unsigned
		if (count<=0)return FALSE;
	};
	if (ll>2 || uu>1) return FALSE;

	int i=0;
	if (mode>1){
		while (i<count){
			char c=src[i++];
			if (c>='0' && c<='9')continue;
			if (c>='a' && c<='f') continue;
			if (c>='A' && c<='F') continue;
			if (c==0)printf("strange null\n");
			return FALSE;
		}
		return TRUE;
	}

	int nddec=FALSE;
	while (i<count){
		char c=src[i++];
		if (c>='8')nddec=TRUE;
		if (c>='0' && c<='9') continue;
		return FALSE;
	}
	if  (src[0]=='0' && nddec)return FALSE; //not octal!
	return TRUE;
}

bool IsFloat(const uchar *src,int count){
	int ll=0,dd=0;
	while (TRUE){
		if (count<=0)return FALSE;
		char c=src[--count];
		if (c=='l' || c=='L'){ll++;continue;} //long or Long
		if (c=='d' || c=='D'){dd++;continue;} //double
		count++;
		break;
	};
	if (ll>2 || dd>1) return FALSE;

	int numdot=0;
	int i=0;
	int uexp=0;
	int num=FALSE;
	while (i<count){
		char c=src[i++];
		if (c>='0' && c<='9'){num=TRUE; continue;}
		if (c=='.'){
			if (numdot++>1) return FALSE;
			continue;
		}
		if (c=='e' || c=='E'){
			if (uexp++>1) return FALSE;
			c=src[i];
			if (c=='+' || c=='-') i++;
			numdot++;
			continue;
		}
		if (c==0)printf("strange null\n");

		return FALSE;
	}
	if (!num) return FALSE; //contains no numerals!
	if ( !ll && !dd && !numdot && !uexp) return FALSE; //integer!!
	if (( ll || dd) && !numdot ) return FALSE;
	return TRUE;
}


HTYPE QuickCheck(char *_src,int count)
{
	InitHashTable();
	uchar *src=(uchar*)_src;
	if (!src || !*src)return HNORM;
	if (count<0)return HNORM;

	if (cpphash->Contains(src,count)) return HCTOKEN;
	if (behash->Contains(src,count)) return HOPERATOR;
	if (consthash->Contains(src,count)) return HCONSTANT;
	if (IsInteger(src,count))return HINTEGER;
	if (IsFloat(src,count)) return HFLOAT;
	return HNORM;
}


HTYPE FindString(char *nnu,int32 fin,int32 &tx1,int32 &tx2,int32 flags){
	uchar *nu=(uchar*)nnu;
	flags=0;
	static char chktab[256];
	static int dead=TRUE;

	if (dead){
		int i;
		for (i=0;i<256;i++){chktab[i]=0;}
		char *tok="!@$%^&*()-=[]|<>?/:;";
		i=0;
		while (tok[i]){chktab[tok[i++]&255]=2;}

		for (i='A';i<='Z';i++) chktab[i]=1;
		for (i='a';i<='z';i++) chktab[i]=1;
		for (i='0';i<='9';i++) chktab[i]=1;
		chktab['#']=1;
		chktab['_']=1;
		chktab['.']=1;  //numbers..

		dead=FALSE;
		InitHashTable();
	}

	int stt=0,mode=0,lck=0;;
	tx1=0;tx2=fin;
	int lchar=-1;

	while (stt<fin){
		int   c=nu[stt++];if (!c) break;

		switch (mode){
			case 0:
				if (c=='<'){ //grungy HTML look-ahead
					int fs=stt,ff=stt,tagcnt=0;
					HTYPE rr=HHTMLTAGON;
					if (nu[ff]=='/'){ff++;fs++;rr=HHTMLTAGOFF;}
					while (ff<fin){
						char mc=nu[ff++]&255;
						if (chktab[mc]!=1){
							if (ff-fs-1>0){
//								if (!htmlhash->Contains(nu+fs,ff-fs-1))
//								{
//									break;
//								}
								tagcnt++;
							}
							fs=0;

							if (mc=='>' &&tagcnt){
								tx1=stt-1;
								tx2=ff;
								return rr;
							}
							if (mc==9) fs=ff; //tab..(!)
							if (mc==32) fs=ff; //spacebar
							if (mc=='=') fs=ff; //equals
							if (mc=='\"'){
								while (ff<fin){
									mc=nu[ff++]&255;
									if (mc=='\"'){
										fs=ff;
										break;
									}
								}
							}
							if (fs==0){
								break;
							}
						}
					}
				}
				if (lck!=chktab[c] && lck){
					if (lchar!=-1){
						HTYPE rr=HNORM;
						if (lck==1) rr=QuickCheck((char*)nu+lchar,stt-1-lchar);
						if (lck==2) rr=HOPERATOR;
						if (rr){
							tx1=lchar;
							tx2=stt-1;
							return rr;//token
						}
						lchar=-1;
					}
				}
				lck=chktab[c];
				if (lck && lchar==-1)lchar=stt-1;

				if (c=='\\'){
					c=nu[stt++];if (!c)break;
					continue;
				}
				if (c=='\"'){
					mode=1;
					tx1=stt-1;
					continue;
				}

				if (c=='/'){
					char nc=nu[stt];
					if (nc=='/'){
						tx1=stt-1;
						tx2=fin;

						HTYPE rr=HNORM;
						if (lchar!=-1){
							if (lck==1) rr=QuickCheck((char*)nu+lchar,stt-1-lchar);
							if (lck==2) rr=HOPERATOR;
						}
						if (rr && lchar!=stt-1){
							tx1=lchar;
							tx2=stt-1;
							return rr;
						}

						return HCCOMMENT;
					}// C++ comment..
					if (nc=='*'){
						tx1=stt-1;
						mode=2;
						continue;
					}/* C comment */
				}
				break;

			case 1:// inside string;
				if (c=='\\'){
					c=nu[stt++];
					if (!c)break;
					continue;
				}

				if (c=='\"'){
					tx2=stt;
					return HSTRING;
				}
				break;
			case 2:/*inside comment */
				if (c=='*' && nu[stt]=='/'){
					tx2=stt+1;
					return HCCOMMENT;
				}
				break;
		}
	}
	if (mode>1){
		tx2=fin;
		return HCCOMMENT;
	}
	HTYPE rr=HNORM;
	if (lchar!=-1){
		if (lck==1) rr=QuickCheck((char*)nu+lchar,fin-lchar);
		if (lck==2) rr=HOPERATOR;
	}
	if (rr){
		tx1=lchar;
		tx2=fin;
		return rr;
	}
	tx1=0;
	tx2=fin;
	return HNORM;
}
