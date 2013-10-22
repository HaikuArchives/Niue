
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



#ifndef SNIPPETMENU_H
#include "snippetmenu.h"
#endif

snippetmenu::snippetmenu(const char *name)
	   	   : BPopUpMenu(name)
{
	SetRadioMode(false);

	
	//level 1
	AddItem(mnuSnippets = new BMenu("Insert Snippet"));
	
	//level 2
	mnuSnippets->AddItem(mnuInterfaceElements = new BMenu("Interface elements"));
	mnuSnippets->AddItem(mnuLoopsStatements = new BMenu("Loops & statements"));
	mnuSnippets->AddItem(mnuCommonCode = new BMenu("Common code"));
	mnuSnippets->AddSeparatorItem();
	mnuSnippets->AddItem(mnuUserCode = new BMenu("User Snippets"));

	
	//level 3
	mnuInterfaceElements->AddItem(new NSnipMenuItem("Alert", NULL, "(new BAlert(\"TITLE\", \"TEXT\", \"BUTTON1\", \"BUTTON2\", \"BUTTON3\", B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();\n"));
	mnuInterfaceElements->AddSeparatorItem();
	mnuInterfaceElements->AddItem(new NSnipMenuItem("Box", NULL, "BBox* boxBox = new BBox(BRect(0,0,200,250), \"boxBox\", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE_JUMP, B_FANCY_BORDER);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("Button", NULL, "BButton* btnButton = new BButton(BRect(0,0,100,20), \"btnButton\", \"Label\", new BMessage(), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("PictureButton", NULL, "BPictureButton* pbnPictureButton = new BPictureButton(BRect(0,0,100,20), \"pbnPictureButton\", new BPicture(), new BPicture(), new BMessage(), B_ONE_STATE_BUTTON,\n\tB_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("RadioButton", NULL, "BRadioButton* radRadioButton = new BRadioButton(BRect(0,0,120,20), \"radRadioButton\", \"Label\", new BMessage(), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("CheckBox", NULL, "BCheckBox* chkCheckBox = new BCheckBox(BRect(0,0,120,20), \"chkCheckBox\", \"label\", new BMessage(), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("StringView", NULL, "BStringView* strStringView = new BStringView(BRect(0,0,100,15), \"strStringView\", \"Label\", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("TextControl", NULL, "BTextControl* txtTextControl = new BTextControl(BRect(0,0,200,20), \"txtTextControl\", \"Label\", \"text\", new BMessage(), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("TextView", NULL, "BTextView* txtTextView = new BTextView(BRect(0,0,250,150), \"txtTextView\", BRect(2,2,248,148), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS | B_PULSE_NEEDED);\n"));



	mnuInterfaceElements->AddSeparatorItem();
	mnuInterfaceElements->AddItem(new NSnipMenuItem("Open panel", NULL, "BFilePanel *OpenPanel = new BFilePanel(B_OPEN_PANEL, this, 0, B_FILE_NODE, false, 0, 0, false, true);\nOpenPanel->Show();\n"));
	mnuInterfaceElements->AddItem(new NSnipMenuItem("Save panel", NULL, "BFilePanel *SavePanel = new BFilePanel(B_SAVE_PANEL, this, 0, 0, false, 0, 0, false, true);\nSavePanel->Show();\n"));
	
	mnuLoopsStatements->AddItem(new NSnipMenuItem("For", NULL, "\nfor( int i=1; i<X; i++ ) \n{\n\t//action\n}\n"));
	mnuLoopsStatements->AddItem(new NSnipMenuItem("While", NULL, "\nwhile( 'condition' ) \n{\n\t//action\n}\n"));
	mnuLoopsStatements->AddItem(new NSnipMenuItem("Do", NULL, "\ndo \n{\n\t//action\n} while ( 'condition' );\n"));
	mnuLoopsStatements->AddSeparatorItem();
	mnuLoopsStatements->AddItem(new NSnipMenuItem("If", NULL, "\nif( 'condition' ) \n{\n\t//action\n}\n"));
	mnuLoopsStatements->AddItem(new NSnipMenuItem("If/else", NULL, "\nif( 'condition' ) \n{\n\t//action\n}\nelse\n{\n\t//action\n}\n"));
	mnuLoopsStatements->AddItem(new NSnipMenuItem("If/elseif", NULL, "\nif( 'condition' ) \n{\n\t//action\n}\nelse if( 'condition' )\n{\n\t//action\n}\nelse\n{\n\t//action\n}\n"));
	mnuLoopsStatements->AddSeparatorItem();
	mnuLoopsStatements->AddItem(new NSnipMenuItem("Switch", NULL, "\nswitch( 'condition' )\n{\n\tcase 1:\n\t{\n\t\t//action\n\t}\n\tbreak;\n\n\tcase 2:\n\t{\n\t\t//action\n\t}\n\tbreak;\n\n\tdefault;\n\t{\n\t\t//action\n\t}\n}\n"));
	
	mnuCommonCode->AddItem(new NSnipMenuItem("Include local", NULL, "#include \"XXX.h\""));
	mnuCommonCode->AddItem(new NSnipMenuItem("Include global", NULL, "#include <XXX.h>"));
	mnuCommonCode->AddItem(new NSnipMenuItem("Define", NULL, "#define\tXXX\t'XXXX'"));
	
	//user snippets
	mnuUserCode->AddItem(new NSnipMenuItem("Manage Snippets", NULL, BString("ADD_ITEM")));
	
		//open the snippets
//		BString snippetspath;
		BPath prefspath;
		
		//find prefsfile
		find_directory(B_USER_SETTINGS_DIRECTORY, &prefspath, false);
		prefspath.Append("Niue/niue_snippets");
					
		//open file for reading
		BFile rdsnippetsfile;
		rdsnippetsfile.SetTo(prefspath.Path(), B_READ_ONLY);
		if (rdsnippetsfile.IsReadable())
		{
			
			BMessage msg('SNIP');
			msg.Unflatten(&rdsnippetsfile);
			
			if(msg.CountNames(B_STRING_TYPE) > 0)
			{
				mnuUserCode->AddSeparatorItem();
							
				int i = 0;
				char		*snipname;
				uint32		type;
				int32		count;
				BString		snipcode;
				
				while(msg.GetInfo(B_STRING_TYPE, i, &snipname, &type, &count) == B_OK)
				{
					if(msg.FindString(snipname, &snipcode) == B_OK)
					{
						mnuUserCode->AddItem(new NSnipMenuItem(snipname, NULL, snipcode));
					}
					i++;
				}			
				

			}			
		}
		
		rdsnippetsfile.Unset();
}


NSnipMenuItem::NSnipMenuItem(const char *label, BMessage *message, BString code)
		: BMenuItem(label, message, 0,0)
{
	CodeToInsert = code;
}

NSnipMenuItem::~NSnipMenuItem()
{
}
