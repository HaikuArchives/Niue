
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



#ifndef CBLIST_H
#define CBLIST_H

class CList
{
public:
			CList(int num=16);
			~CList();
	CList &	operator= (const CList &);

	void	Reset();
	int		Num() const;

	int		Load(const char *src,int maxsize=-1);
	int		Save(char *dst,int maxsize=-1);

	int		Merge(const CList &); // non-shared points..

	long	operator[] (int index) const;
	long	Item(int index) const;
	void *	ItemV(int index) const;
	float	ItemF(int index) const;

	void	Add(long value);
	void	Add(long value,long index);
	void	Add(const void *value);
	void	Add(const void *value,long index);
	void	AddF(float value);
	void	AddF(float value,long index);

	void	SwapWith(long value,long index);
	void	SwapWith(const void *value,long index);
	void	SwapWithF(float value,long index);
	
	int		IndexOf(long value) const;
	int		IndexOf(const void *value) const;
	bool	operator<= (long value) const;   //contains..
	bool	operator<= (const void*value) const;   //contains..

	void	RemIndex(long index);

	int		Save(int type,int (*SaveItem)(CList *,void *,char *,int),char *dest,
				int maxsize);

private:
	void	CheckMax(long nucount);
	
	long	mycount;
	long	*data;
	long	max;
};

inline long CList::operator[] (int index)const { return Item(index); }
inline int CList::Num()  const { return mycount; }
inline void CList::Add(long vv) { Add(vv,mycount); }
inline void CList::AddF(float vv) { AddF(vv,mycount); }
inline void CList::Add(const void *vv) { Add(vv,mycount); }

#endif

