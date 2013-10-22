/*
	ssnippetview.h
*/

#ifndef	SSNIPPETVIEW_H
#define	SSNIPPETVIEW_H

#include <Application.h>
#include <InterfaceKit.h>
#include <StorageKit.h>
#include <String.h>
#include "colors.h"
#include "actions.h"

#define	S_ACTION_CLOSE_MSG	'sacm'
#define	S_ACTION_NEW_MSG		'sanm'
#define	S_ACTION_SAVE_MSG		'sasm'
#define	S_ACTION_REMOVE_MSG		'sarm'

class snippetlist;
class ssnippetview : public BView
{
	public:
								ssnippetview(BRect frame, char *name);
		virtual void			BuildList();
		virtual void			NewSnippet();
		snippetlist*			 lstSnippets;

	private:
	
	BStringView*	strName;
	BStringView*	strCode;
	BButton*		btnRemove;
	BButton*		btnNew;
	BButton*		btnSave;
	BButton*		btnClose;

};

class snippetlist : public BListView
{
	public:
								snippetlist(BRect rect, const char *name, list_view_type type, uint32 resizingMode, uint32 flags);
		virtual void			MouseDown(BPoint where);
		BWindow*				wdwEdit;
	private:
};

class slistitem : public BListItem
{
	public:
								slistitem(BString name, BString text);
		virtual void			DrawItem(BView*, BRect, bool);
		virtual void			Update(BView*, const BFont*);
		BString				sname;
		BString				stext;

	private:
		
};


class seditwindow : public BWindow
{
	public:
								seditwindow(BRect rect, const char *text, bool name);
		virtual void			MessageReceived(BMessage* message);

	private:

		BView*					vwMain;
		BTextControl*			txtText;
};


#endif
