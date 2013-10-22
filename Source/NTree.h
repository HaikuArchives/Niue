
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


#ifndef NTREE_H
	#define NTREE_H

	class nnode;
	class CList;
	class nplace;


	class nplace{
		public:
			nplace(int type=0);
			~nplace();
			int place;
			int lasthit;
			int Detach();
			int CalcPlace();
		private:
			int owner;
			nnode *dest;
			int ofs;
			int type;
		friend class ntree;
		friend class ndata;
	};


	class nnode{
		public:
			nnode(nnode *dad);
			virtual ~nnode();
			int type;
			int siz;
			int cnt10;
			int maxdepth;

			inline int Size(){return siz;}
			virtual int Resize()=0;
			virtual int Insert(const char *dat,int size,int place)=0;
			virtual int Fetch(char *dat,int size,int place)=0;
			virtual int Remove(int p1,int p2)=0;

			virtual int Count(int type,int p1,int p2)=0;
			virtual int Attach(nplace *dat,int place)=0;
			virtual nplace *FindPrev(int type,int place)=0;
			virtual nplace *FindNext(int type,int place,int count=1)=0;
			virtual int Resolve(int p1,int p2)=0;

			nnode *dad;

			int Lock();
			int Unlock();

	};

	class ndata:  public nnode{
		public:
			ndata(nnode *top,int special=0);
			ndata();
			~ndata();

			virtual int Resize();
			virtual int Insert(const char *dat,int size,int place);
			virtual int Fetch(char *dat,int size,int place);
			virtual int Remove(int p1,int p2);

			virtual int Count(int type,int p1,int p2);
			virtual int Attach(nplace *dat,int place);
			virtual nplace *FindPrev(int type,int place);
			virtual nplace *FindNext(int type,int place,int count=1);

			virtual int Resolve(int p1,int p2);

			void SplitTo(ndata *n2,int pos);

		private:
			int SizeTo(int nus);
			char *dat;
			int asiz;
			int canfree;

			CList *alist;
			friend class nplace;
			friend class ntree;
			friend void Balance(ntree**,int);
	};

//  #define MAXNTN 8
	#define MAXNTN 2
	class ntree : public nnode{
		public:
			ntree(nnode *top);
			ntree();
			~ntree();

			virtual int Resize();
			virtual int Insert(const char *dat,int size,int place);
			virtual int Fetch(char *dat,int size,int place);
			virtual int Remove(int p1,int p2);

			virtual int Count(int type,int p1,int p2);
			virtual int Attach(nplace *dat,int place);
			virtual nplace *FindPrev(int type,int place);
			virtual nplace *FindNext(int type,int place,int count=1);

			virtual int Resolve(int p1,int p2);

			int Show (int ll);

			nnode   *data[MAXNTN];
		private:

			friend void Balance(ntree **nt,int recurse);
	};

/*
	class nfile:  public nnode{
		public:
		private:
			nfile();
			class BFile *fil;
			int ofs;
			int size;
	};
*/

	void Balance(ntree **nt,int recurse=0);


#endif
