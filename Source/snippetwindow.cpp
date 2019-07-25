/*
	snippetwindow.cpp
*/

#include "snippetwindow.h"

BRect srect(100,100,700,400);

snippetwindow::snippetwindow() :
	BWindow(srect, "Niue Snippet Manager", B_TITLED_WINDOW,B_WILL_DRAW)
{

	SetSizeLimits(515,30000,100,30000);

	// define objects
	vwMain = new ssnippetview(BRect(0,0,srect.Width(),srect.Height()), "vwMain");

	// set objects
	vwMain->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	// add objects
	AddChild(vwMain);

	// internal setting
	saved = true;
	close = false;
}


bool snippetwindow::QuitRequested()
{
	PostMessage(S_ACTION_CLOSE_MSG);
}

void snippetwindow::MessageReceived(BMessage* message)
{
	switch(message->what)
	{
		case S_ACTION_CLOSE_MSG:
		{
			if(saved)
			{
				Close();
				break;
			}
			else
			{
				BAlert *savealert=new BAlert("Quitting Alert", "Do you want to save changes before quitting?", "Cancel", "Don't save", "Save and Close");

				int32 res=savealert->Go();
				switch(res)
				{
					case 1:
						Close();
						break;
					case 2:
						close = true;
						//fall trhrough to S_ACTION_SAVE
				}
			}
		}

		case S_ACTION_SAVE_MSG:
		{
			BPath prefspath;

			// find prefsfile
			find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
			prefspath.Append("Niue/niue_snippets");

			// create file, override when it already exists
			BFile snippetsfile;
			snippetsfile.SetTo(prefspath.Path(), B_WRITE_ONLY | B_CREATE_FILE);

			BMessage msg('SNIP');

			for(int i=0; i < vwMain->lstSnippets->CountItems(); i++)
			{
				slistitem* item = dynamic_cast<slistitem*>(vwMain->lstSnippets->ItemAt(i));
				msg.AddString(item->sname.String(), item->stext);
			}

			msg.Flatten(&snippetsfile);

			// close
			snippetsfile.Unset();
			saved = true;

			if(close)
			{
				Close();
			}
		}
		break;

		case S_ACTION_NEW_MSG:
		{
			saved = false;
			vwMain->NewSnippet();
		}
		break;

		case S_ACTION_REMOVE_MSG:
		{
			int32 index = vwMain->lstSnippets->CurrentSelection();
			vwMain->lstSnippets->RemoveItem(index);

			// be automatically shifts the indices so now we can:
			if(index > 0)
			{
				vwMain->lstSnippets->Select(index-1);
			}
			else
			{
				vwMain->lstSnippets->Select(index);
			}
			saved = false;

		}
		break;

		case S_ACTION_CHANGE_MSG:
		{
			bool name;
			const char *cont;

			if(message->FindBool("name", &name) == B_OK)
			{
				if(name)
				{
					if(message->FindString("cont", &cont) == B_OK)
					{
						for(int i=0; i < vwMain->lstSnippets->CountItems(); i++)
						{
							slistitem* item = dynamic_cast<slistitem *>(vwMain->lstSnippets->ItemAt(i));
							if(strcmp(item->sname.String(), cont) == 0)
							{
								(new BAlert("Niue", "Please choose a unique name for your snippet.", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
								break;
							}
						}
						dynamic_cast<slistitem *>(vwMain->lstSnippets->ItemAt(vwMain->lstSnippets->CurrentSelection()))->sname = cont;
					}
				}
				else
				{
					if(message->FindString("cont", &cont) == B_OK)
					{
						dynamic_cast<slistitem *>(vwMain->lstSnippets->ItemAt(vwMain->lstSnippets->CurrentSelection()))->stext = cont;
					}
				}
			}

			saved = false;
		}
		break;
	}
}
