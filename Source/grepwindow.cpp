/*
	grepwindow.cpp
*/

#include "grepwindow.h"

BRect grect(100,100,700,700);

grepwindow::grepwindow(const char *path) :
	BWindow(grect, "Niue Grape", B_TITLED_WINDOW,B_WILL_DRAW)
{

	SetSizeLimits(200,30000,100,30000);
	CenterWindow();	

	// define objects
	vwQuery = new gqueryview(BRect(0,0,grect.Width(),70), "vwQuery", path);
	vwOutput = new goutputview(BRect(0,71,grect.Width(),grect.Height()), "vwOutput");
	
	// set objects
	vwQuery->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	vwOutput->SetViewColor(200,100,200,0);
	
	// add objects
	AddChild(vwQuery);
	AddChild(vwOutput);

	LoadPrefs();

}

void grepwindow::LoadPrefs()
{
		//read the settings file
		BPath prefspath;
		
		//find prefsfile
		find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
		prefspath.Append("Niue/niue_prefs.grape");
		
		//read file
		BFile prefsfile(prefspath.Path(), B_READ_ONLY);
		if (prefsfile.IsReadable())
		{
			BMessage msg('PREF');
			bool icase, eword, lsubdir;
			msg.Unflatten(&prefsfile);
			
			//options
			if(msg.FindBool("i_case", &icase) == B_OK)
			{
				if(icase)
				{
				vwQuery->chkCase->SetValue(1);
				}
				else
				{
				vwQuery->chkCase->SetValue(0);
				}
			}
			else
			{
				vwQuery->chkCase->SetValue(0);
			}

			if(msg.FindBool("e_word", &eword) == B_OK)
			{
				if(eword)
				{
				vwQuery->chkWord->SetValue(1);
				}
				else
				{
				vwQuery->chkWord->SetValue(0);
				}
			}
			else
			{
				vwQuery->chkWord->SetValue(0);
			}


			if(msg.FindBool("l_subdir", &lsubdir) == B_OK)
			{
				if(lsubdir)
				{
				vwQuery->chkSubdir->SetValue(1);
				}
				else
				{
				vwQuery->chkSubdir->SetValue(0);
				}
			}
			else
			{
				vwQuery->chkSubdir->SetValue(0);
			}

		}
	
}

void grepwindow::SavePrefs()
{
		//now let's save the settigns
		BPath prefspath;
		
		//find prefsfile
		find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
		prefspath.Append("Niue/niue_prefs.grape");
		
		//create file
		BFile prefsfile(prefspath.Path(), B_WRITE_ONLY | B_CREATE_FILE);
		if(prefsfile.IsWritable())
		{
			BMessage msg('PREF');
			
			//options
			if(vwQuery->chkCase->Value() == 1)
			{
				msg.AddBool("i_case", true);
			}
			else
			{
				msg.AddBool("i_case", false);
			}

			if(vwQuery->chkWord->Value() == 1)
			{
				msg.AddBool("e_word", true);
			}
			else
			{
				msg.AddBool("e_word", false);
			}

			if(vwQuery->chkSubdir->Value() == 1)
			{
				msg.AddBool("l_subdir", true);
			}
			else
			{
				msg.AddBool("l_subdir", false);
			}
			

			msg.Flatten(&prefsfile);
		}
	
}


bool grepwindow::QuitRequested()
{
	SavePrefs();

	Close();
}

void grepwindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case G_ACTION_DO_SEARCH:
		{
			//is there a query?
			if(strlen(vwQuery->txtString->Text()) < 2)
			{
				(new BAlert("Niue", "Sorry, this query is to short.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
				break;
			}

			//clear the outputview
			vwOutput->txtOutput->SelectAll();
			vwOutput->txtOutput->Clear();


			count = 0;

			BDirectory dir(vwQuery->txtPath->Text());
			BDirectory subdir;
			BEntry	entry, subentry;
			BString countstring;

			while (dir.GetNextEntry(&entry, false) == B_NO_ERROR) 
			{
				if (entry.IsFile())
				{
					DoSearch(entry);
				}
				else if(entry.IsDirectory())
				{
					if(vwQuery->chkSubdir->Value() == 1)
					{
						subdir.SetTo(&entry);
						while(subdir.GetNextEntry(&subentry, false) == B_NO_ERROR)
						{
							DoSearch(subentry);
						}
					}
				}	
			}

			countstring << count << " matches";
			vwOutput->strStatus->SetText(countstring.String());
			
		}
		break;
		
	}
}

void grepwindow::DoSearch(BEntry entry)
{

	int 		shellExit = -1;
	FILE		*fd;
	char		buff[256];
	char		name[B_FILE_NAME_LENGTH];
	BPath	path;

	entry.GetName(name);
	entry.GetPath(&path);						

	vwOutput->txtOutput->Insert(vwOutput->txtOutput->TextLength(), name, strlen(name));
	vwOutput->txtOutput->Insert(vwOutput->txtOutput->TextLength(), "\n", strlen("\n"));


	BString	command;
	command << "grep -hn";
	if(vwQuery->chkCase->Value() == 1)
	{
		command << "i";
	}
	
	if(vwQuery->chkWord->Value() == 1)
	{
		command << "w";
	}
	command << " --line-buffered";
	command << " '" << vwQuery->txtString->Text() << "' ";
	command << path.Path();


	fd = popen(command.String(), "r");
	
	if(fd) 
	{

		for(int32 i = 0; fgets(buff, 256, fd) != NULL; i++) 
		{
			if(buff && strlen(buff) > 0) 
			{
				vwOutput->txtOutput->Insert(vwOutput->txtOutput->TextLength(), "\t", strlen("\t"));
				vwOutput->txtOutput->Insert(vwOutput->txtOutput->TextLength(), buff, strlen(buff));
				vwOutput->txtOutput->ScrollToOffset(vwOutput->txtOutput->TextLength());
				//lcounter
				count++;
			}

		}

		shellExit = pclose(fd);	// Close filepointer.

		if(shellExit == 2)
		{
//			(new BAlert("Niue", "There was an error while searching.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		}
		else if(shellExit == 1)
		{
//			vwOutput->txtOutput->Insert(vwOutput->txtOutput->TextLength(), "\tno matches\n", strlen("\tno matches\n"));
		}

	}

}

void grepwindow::CenterWindow()
{
	
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}
