
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


#include "aboutwindow.h"

BRect arect(0,0,300,300);

aboutwindow::aboutwindow()
	:	BWindow(arect, "About Niue", B_TITLED_WINDOW,B_WILL_DRAW | B_NOT_RESIZABLE)
{
	CenterWindow();
		
	//define objects
	vwAbout = new BView(arect, "vwAbout", B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED);
	btnClose = new BButton(BRect(10, arect.Height()-30, arect.Width()-10, arect.Height()-10),
							"btnClose", "Thank you for using Niue",
							new BMessage(CLOSE_ABOUT_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM,
							B_WILL_DRAW);
	lblAuthor = new BTextView(BRect(10, 60, arect.Width()-10, arect.Height()-80),
							"lblAuthor", BRect(0,0, arect.Width()-11, arect.Height()-51),
							B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	txtCredits = new WebButton(BRect(10,arect.Height()-60,arect.Width()-10,
										arect.Height()-45),\
								"credits");
	
	AboutBitmap = BTranslationUtils::GetBitmap("about_bmap");
	
	
	//set objects
	lblAuthor->SetText("Version 5, October 2008 \ncopyright 2008 \nby: Tako Lansbergen "
						"(aka 0033)\n\nFeedback:\ntakolansbergen@gmail.com");
	lblAuthor->MakeEditable(false);
	
	//add objects
	AddChild(vwAbout);
	vwAbout->SetViewBitmap(AboutBitmap, B_FOLLOW_TOP | B_FOLLOW_LEFT);
	vwAbout->SetViewColor(255,255,255,0);
	vwAbout->AddChild(btnClose);
	vwAbout->AddChild(lblAuthor);
	vwAbout->AddChild(txtCredits);
}


bool
aboutwindow::QuitRequested()
{
	return true;
}


void
aboutwindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case CLOSE_ABOUT_MSG:
		{
			Quit();
		}
		break;
		case SET_ABOUT_TEXT:
		{
			BString string;
			message->FindString("text", &string);
			lblAuthor->SetText(string.String());
			vwAbout->RemoveChild(txtCredits);
			lblAuthor->ResizeBy(0,15);
		}
		break;
	}
}


void
aboutwindow::CenterWindow()
{
	
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}


WebButton::WebButton(BRect rect, const char *name)
	:	BView(rect, name, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
//	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}


void
WebButton::Draw(BRect)
{
	SetHighColor(blue);
	BPoint a,b;
	a.Set(0,Frame().Height()-2);

	DrawString(Name(), a);
	
	a.Set(1,Frame().Height()-1);
	b.Set(be_plain_font->StringWidth(Name()),Frame().Height()-1);
	StrokeLine(a,b);
}


void
WebButton::MouseDown(BPoint)
{
	int32			button;
	
//	SetMouseEventMask( B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS );
	if(Window()->CurrentMessage()->FindInt32("buttons",&button) == B_NO_ERROR)
	{
		if(button & B_PRIMARY_MOUSE_BUTTON)
		{
			if(strcmp(Name(), "credits") == 0)
			{
				BString creditstring;
				creditstring << "Niue would not have been possible without:\n\n";
				creditstring << "Chris Blackbourn, for parts of Yate,\n";
				creditstring << "Robert Polic, for parts of EZLauncher,\n";
				creditstring << "Gabriele Biffi, for parts of File-To-Resource,\n";
				creditstring << "Sylvain Tertois, for parts of his Mover class,\n";
				creditstring << "Rene Gollent, for parts of his IconMenu class,\n";
				creditstring << "Michael Pfeiffer, for parts of his BarberPole class,\n";
				creditstring << "The Haiku project, for keeping me going,\n";
				creditstring << "You as a user of Niue,\n\n";
				creditstring << "...and everyone I've forgotten here, Thanks!\n";
							
				BMessage msg = SET_ABOUT_TEXT;
				msg.AddString("text", creditstring);
				Window()->PostMessage(&msg);
			}
			
		}
		else if(button & B_SECONDARY_MOUSE_BUTTON)
		{
			BString text;
			text << "\n1991 is notable in the history of Be Inc. In June, Jean Louis "
				"Gassée decided that the hard working Be team needed a break. \n\nIn The "
				"BeOS Bible, Henry Bortman recorded the following account of Be's first "
				"off-site team building excursion.\n\nBe's cashflow situation didn't allow "
				"for a lavish affair; Gassée chose instead to take his compatriots on a "
				"forced march to the sea. \n\nThe Hike to the Sea took place on the summer "
				"solstice, June 21 1991. It began on the crest of the hills west of "
				"Saratoga, California, and ended some 12 hours later on the shore of the "
				"Pacific Ocean. Estimates of the length of the now-legendary Hike vary. "
				"Some say it was a mere 26 miles, others complain that it was at least 30. "
				"Reports of what time the Hike began also vary: Some report getting up at "
				"3am, others at the more sensible hour of 5. But everyone who participated "
				"agrees on one thing: It was painful.\n\nGassée recalls having prepared "
				"his troops for the adventure with some cautionary advice: Don't train, "
				"don't buy new shoes, and bring some Band-Aids and ibuprofen. The "
				"suggestion not to train was unnecessary; everyone was too busy working to "
				"have time for it. And there was no shortage of medical supplies.\n\nBut "
				"several people either never heard or chose not to hear Gassée's "
				"admonition against acquiring new footware. Cyril Meurillon, who had joined "
				"Be to work on the BeOS kernel, was among those who purchased new hiking "
				"boots for the occasion. His feet were bloody by the end of the day. Bob "
				"Herold also sprung for a pair of stiff new shoes. He was fortunate enough "
				"to have stashed some sneakers in his backpack for the end of the hike. "
				"Within the first mile, his new shoes were jettisoned in favor of the "
				"other, more confortable ones.\n\nFortune shined less brightly on Erich "
				"Raingewald. He also bought new shoes, but didn't carry a spare pair. "
				"Still, it isn't blisters from the new shoes that Ringewald remembers most "
				"vividly; he credits the Hike to the Sea with permanently ruining his "
				"knees. \n\"At about 20 miles, my knees were in such pain. But I kept "
				"going and I know I just continued to do more and more damage to them. "
				"Now I can't even sit cross-legged on the floor for a half hour. My knees "
				"just freeze up.\" \n\n\tDespite their injuries, everyone finished the "
				"hike. ";			

			BWindow* wdwHike = new BWindow(BRect(0,0,400,600),
											"Niue - The amazing story of the hike to the sea",
											B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
											B_WILL_DRAW);
			//center me
			BScreen mScreen(B_MAIN_SCREEN_ID);
			BRect screenRes, winBounds;
			screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
			winBounds = wdwHike->Bounds();
			wdwHike->MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
			
			BTextView *txtView = new BTextView(BRect(0,0,400-B_V_SCROLL_BAR_WIDTH,600-B_H_SCROLL_BAR_HEIGHT), "txtView", BRect(2,2,380,30000), B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
			BScrollView *scrView = new BScrollView("scrView", txtView, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, true, true, B_NO_BORDER);
			
			txtView->MakeEditable(false);
			txtView->SetViewColor(bright);
			txtView->SetFontAndColor(be_plain_font, B_FONT_ALL, &black);
			txtView->SetWordWrap(true);
			txtView->SetText(text.String());
			
			wdwHike->AddChild(scrView);
			
			wdwHike->Show();
		}
	}
	
	
}
