
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




#include "makefilewindow.h"

BRect mrect(0,0,400,400);

makefilewindow::makefilewindow(BEntry proj_entry) :
	BWindow(mrect, "Niue - Build Rules", B_TITLED_WINDOW,B_WILL_DRAW | B_NOT_RESIZABLE)
{
	entry = proj_entry;
	entry.GetName(name);

	
	CenterWindow();
	
	//menus
	mnuType = new BMenu("APP");
	mnuType->AddItem(new BMenuItem("APP", new BMessage(MC_APP)));
	mnuType->AddItem(new BSeparatorItem);
	mnuType->AddItem(new BMenuItem("SHARED", new BMessage(MC_SHARED)));
	mnuType->AddItem(new BMenuItem("ADDON", new BMessage(MC_ADDON)));
	mnuType->AddItem(new BMenuItem("STATIC", new BMessage(MC_STATIC)));
	mnuType->AddItem(new BMenuItem("DRIVER", new BMessage(MC_DRIVER)));
	mnuType->AddItem(new BMenuItem("MODULE", new BMessage(MC_MODULE)));
	mnuOpt = new BMenu("FULL");
	mnuOpt->AddItem(new BMenuItem("FULL", new BMessage(MC_FULL)));
	mnuOpt->AddItem(new BMenuItem("SOME", new BMessage(MC_SOME)));
	mnuOpt->AddItem(new BMenuItem("NONE", new BMessage(MC_NONE)));
	
	type = "APP";
	opt = "FULL";
		
	//define objects
	vwMake = new BView(mrect, "vwMake", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	
	btnClose = new BButton(BRect(mrect.Width()-200, mrect.Height()-30, mrect.Width()-110, mrect.Height()-10), "btnClose", "Cancel", new BMessage(CLOSE_MAKE_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	btnGenerate = new BButton(BRect(mrect.Width()-100, mrect.Height()-30, mrect.Width()-10, mrect.Height()-10), "btnGenerate", "Save", new BMessage(GO_SAVE_MSG), B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	btnHelp = new BButton(BRect(10, mrect.Height()-30, 100, mrect.Height()-10), "btnHelp", "Help", new BMessage(GO_HELP_MSG), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW);
	
	strName = new BStringView(BRect(10,10,(mrect.Width()/2)-5,25), "strName", "Application name:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtName = new BTextControl(BRect(10,30,(mrect.Width()/2)-5,50), "txtName", 0, NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strType = new BStringView(BRect((mrect.Width()/2)+15,10,mrect.Width()-10,25), "strType", "Application Type:", B_FOLLOW_LEFT | B_FOLLOW_TOP); 
	cmbType = new BMenuField(BRect((mrect.Width()/2)+15,30,mrect.Width()-10,50), "cmbType", NULL, mnuType, true, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strSources = new BStringView(BRect(10,60,mrect.Width()-10,75), "strSources", "Sourcefiles (*.cpp); space separated:", B_FOLLOW_LEFT | B_FOLLOW_TOP); 
	txtSources = new BTextControl(BRect(10,80,mrect.Width()-10,100), "txtSources", 0, NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strLibs = new BStringView(BRect(10,115,mrect.Width()-10,130), "strLibs", "Additional libraries (f.i: be tracker); space separated:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtLibs = new BTextControl(BRect(10,135,mrect.Width()-10,155), "txtLibs", 0, NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strRes = new BStringView(BRect(10,170,(mrect.Width()/2)-5,185), "strRes", "Resources (*.rsrc).", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtRes = new BTextControl(BRect(10,190,(mrect.Width()/2)-5,210), "txtRes", 0, NULL, 0, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strOpt = new BStringView(BRect((mrect.Width()/2)+15,170,mrect.Width()-10,185), "strOpt", "Optimization:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	cmbOpt = new BMenuField(BRect((mrect.Width()/2)+15,190,mrect.Width()-10,210), "cmbOpt", NULL, mnuOpt, true, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	
	strAdv = new BStringView(BRect(10,225,mrect.Width()-10,240), "strAdv", "Advanced settings:", B_FOLLOW_LEFT | B_FOLLOW_TOP);
	txtAdv = new BTextView(BRect(10,245,(mrect.Width()-10)-B_V_SCROLL_BAR_WIDTH,(mrect.Height()-50-B_H_SCROLL_BAR_HEIGHT)), "txtAdv", BRect(2,2,1000,1), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	srcAdv = new BScrollView("srcAdv", txtAdv, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW, true, true, B_PLAIN_BORDER);
	
	//set objects
	mnuType->SetLabelFromMarked(true);
	mnuOpt->SetLabelFromMarked(true);
	vwMake->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
//	btnClose->MakeDefault(true);
	txtAdv->SetText("#Additional makerules go here, use '#' to comment out lines");
	

	
	//add objects
	AddChild(vwMake);
	vwMake->AddChild(btnClose);
	vwMake->AddChild(btnGenerate);
	vwMake->AddChild(btnHelp);
	vwMake->AddChild(strName);
	vwMake->AddChild(txtName);
	vwMake->AddChild(strType);
	vwMake->AddChild(cmbType);
	vwMake->AddChild(strSources);
	vwMake->AddChild(txtSources);
	vwMake->AddChild(strLibs);
	vwMake->AddChild(txtLibs);
	vwMake->AddChild(strRes);
	vwMake->AddChild(txtRes);
	vwMake->AddChild(strOpt);
	vwMake->AddChild(cmbOpt);
	vwMake->AddChild(strAdv);
	vwMake->AddChild(srcAdv);
	
	FillFields(entry);
	
	txtName->MakeFocus(true);
}

void makefilewindow::FillFields(BEntry entry)
{
	//name
	txtName->SetText(name);
	
	//(re)sources
	BString sources;
	BString res;
	BDirectory dir;
	BEntry fileentry;
	dir.SetTo(&entry);
	while (dir.GetNextEntry(&fileentry, true) == B_NO_ERROR) 
			{
				if (fileentry.IsFile())
					{
						char filename[B_FILE_NAME_LENGTH];
						fileentry.GetName(filename);
						BString string, ext;
						string.SetTo(filename);
						int32 length = string.CountChars();
						
						if (length > 4)
						{
						string.CopyInto(ext, length-4, 4);
						
							if (strcmp(ext.String(), ".cpp") == 0)
							{
								sources << filename << " ";
							}
							else if (strcmp(ext.String(), "rsrc") == 0)
							{
								res << filename << " ";
							}
						}
					}
			}
	txtSources->SetText(sources.String());
	txtRes->SetText(res.String());
	
	//Libs&advanced
		//read the settings file
		BPath prefspath;
		
		//find prefsfile
		find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
		prefspath.Append("Niue/niue_prefs.make");
		
		//create file
		BFile prefsfile(prefspath.Path(), B_READ_ONLY);
		if (prefsfile.IsReadable())
		{
			BMessage msg('PREF');
			
			BString libstring, advstring;
			BString libquery, advquery;
			libquery << name << "_libs";
			advquery << name << "_adva";	

			msg.Unflatten(&prefsfile);
			
			if(msg.FindString(libquery.String(), &libstring) != B_OK)
			{
				txtLibs->SetText("be ");
			}
			else
			{
				txtLibs->SetText(libstring.String());
			}
			
			if(msg.FindString(advquery.String(), &advstring) != B_OK)
			{
				//do nothing (yet)
			}
			else
			{
				txtAdv->SetText(advstring.String());
			}
		}
		else
		{
			txtLibs->SetText("be ");
		}
	
}

void makefilewindow::WriteMakefile()
{

	BDirectory dir;
	dir.SetTo(&entry);
	BEntry makeentry;

	//first let backup existing makefile	
	while (dir.GetNextEntry(&makeentry, true) == B_NO_ERROR) 
			{
				if (makeentry.IsFile())
					{
						char filename[B_FILE_NAME_LENGTH];
						makeentry.GetName(filename);
						
						if (strcmp(filename, "makefile") == 0)
						{
							makeentry.Rename("makefile-backup", true);		
						}	
					}
			}
			
			
			
	//now write our file
	BString makestring;
	makestring << "##Niue makefile v1\n";
	makestring << "\nNAME := " << txtName->Text();
	makestring << "\nTYPE := " << type;
	makestring << "\nSRCS := " << txtSources->Text();
	makestring << "\nLIBS := " << txtLibs->Text();
	makestring << "\nOPTIMIZE := " << opt;
	makestring << "\nRSRCS := " << txtRes->Text();
	makestring << "\n\n" << txtAdv->Text();
	makestring << "\n\ninclude /boot/develop/etc/makefile-engine\n";
	
	BPath makepath;
	makepath.SetTo(&entry);
	makepath.Append("makefile");
	
	BFile file(makepath.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	file.Write(makestring.String(), strlen(makestring.String()));
	
	//and set types
	BNode bnode(&makeentry);
	BNodeInfo bnodeInfo(&bnode);	

	BNode nnode(makepath.Path());
	BNodeInfo nnodeInfo(&nnode);	

	bnodeInfo.SetType("text/x-makefile");
	nnodeInfo.SetType("text/x-makefile");

		
}


bool makefilewindow::QuitRequested()
{
	Quit();
}

void makefilewindow::MessageReceived(BMessage* message)
{
	
	switch(message->what)
	{
		case GO_HELP_MSG:
		{
			BString helpstring;
			
			helpstring << "Sources:\n\n";
			helpstring << "Specify the source files to use. Full paths or paths relative to the makefile can be included. All files, regardless of directory, will have their object files created in the common object directory. Note that this means this makefile will not work correctly if two source files with the same name (source.c or source.cpp) are included from different directories. Also note that spaces in folder names do not work well with this makefile.
If you leave this blank, then all *.c *.cpp *.S *.s *.asm files in the top level local directory are automatically included.\n\n\n";
			
			helpstring << "Libraries:\n\n";
			helpstring <<"Specify additional libraries to link against. There are two acceptable forms of library specifications:\n
- If your library follows the naming pattern of: libXXX.so or libXXX.a you can simply specify XXX library: libbe.so entry: be \n
- If your library does not follow the standard library naming scheme you need to specify the path to the library and it's name. Library: my_lib.a entry: my_lib.a or path/my_lib.a\n\n";
			(new BAlert("Niue - Build Rules", helpstring.String(), "Close", 0, 0, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
		}
		break;
		
		case CLOSE_MAKE_MSG:
		{
			Quit();
		}
		break;
		
		case GO_SAVE_MSG:
		{
			if(strlen(txtName->Text()) != 0)  //see if we have a name
			{
				//save for internals
					//now let's save the search argument
					BPath prefspath;
				
					//find prefsfile
					find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
					prefspath.Append("Niue/niue_prefs.make");
				
					//create file
					BFile prefsfile(prefspath.Path(), B_WRITE_ONLY | B_CREATE_FILE);
					if(prefsfile.IsWritable())
					{
						BMessage msg('PREF');
						
						BString libquery, advquery;
						libquery << name << "_libs";
						advquery << name << "_adva";	
			
						msg.AddString(libquery.String(), txtLibs->Text());
						msg.AddString(advquery.String(), txtAdv->Text());
	
						msg.Flatten(&prefsfile);
						
					}
				
				//do magic
				WriteMakefile();
				
				//and close
				Quit();
			}
			else
			{
				(new BAlert("Niue", "Please fill in a name for your appllication.", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
			}			
		}
		break;
		
		case MC_APP:
		{
			type = "APP";
		}
		break;
 		case MC_SHARED:
		{
			type = "SHARED";	
		}
		break;
 		case MC_ADDON:
		{
			type = "ADDON";	
		}
		break;				
 		case MC_STATIC:
		{
			type = "STATIC";	
		}
		break;				
 		case MC_DRIVER:
		{
			type = "DRIVER";	
		}
		break;				
 		case MC_MODULE:
		{
			type = "MODULE";	
		}
		break;				
 		case MC_FULL:
		{
			opt = "FULL";
		}
		break;					
 		case MC_SOME:
		{
			opt = "SOME";	
		}
		break;					
 		case MC_NONE:
		{
			opt = "NONE";	
		}
		break;					
		
		
	}
}



void makefilewindow::CenterWindow()
{
	
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));
	
}

