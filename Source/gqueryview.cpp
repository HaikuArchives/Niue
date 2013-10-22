/*
	gqueryview.cpp
*/
#include "gqueryview.h"

gqueryview::gqueryview(BRect rect, char *name, const char *path)	   	   
	: BView(rect, name, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	//define objects
	txtString = new BTextControl(BRect(10,10,rect.Width()-75,25), "txtPath", "Search for: ", "", new BMessage(G_ACTION_DO_SEARCH), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
	 B_WILL_DRAW | B_NAVIGABLE);
	btnGo = new BButton(BRect(rect.Width()-70, 8, rect.Width()-7, 23), "btnGo", "Search", new BMessage(G_ACTION_DO_SEARCH), B_FOLLOW_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	chkCase = new BCheckBox(BRect(10,37,110,60), "chkCase", "Ignore case", new BMessage(G_ACTION_DO_CASE), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	chkWord = new BCheckBox(BRect(120,37,220,60), "chkWord", "Entire word", new BMessage(G_ACTION_DO_WORD), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	chkSubdir = new BCheckBox(BRect(230,37,380,60), "chkSubdir", "Look in subdirectories", new BMessage(G_ACTION_DO_SUB), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtPath = new BTextControl(BRect(400,40,rect.Width()-5,65), "txtPath", "Iterate through:", path, new BMessage(G_ACTION_CHANGE_PATH), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
	 B_WILL_DRAW | B_NAVIGABLE);
	
	// set objects
//	btnGo->MakeDefault(true);
	txtString->MakeFocus(true);
	txtString->SetDivider(be_plain_font->StringWidth("Search for:  "));
	txtPath->SetDivider(be_plain_font->StringWidth("Iterate through: "));
	
	//add objects
	AddChild(btnGo);
	AddChild(txtString);
	AddChild(chkCase);
	AddChild(chkWord);
	AddChild(txtPath);
	AddChild(chkSubdir);

}

void gqueryview::Draw(BRect)
{
	SetHighColor(black);
	StrokeLine(BPoint(Bounds().left,Bounds().bottom), BPoint(Bounds().right,Bounds().bottom));
}
