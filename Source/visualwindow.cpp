
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


#include "visualwindow.h"

BRect vrect(100,100,900,700);

VisualWindow::VisualWindow(BPoint point)
	:	BWindow(vrect, "Niue Visual Designer", B_DOCUMENT_WINDOW,B_WILL_DRAW)
{

	SetSizeLimits(350, 30000, 150, 30000);
	MoveBy(point.x, point.y);
	
	// define objects
	
	//metrics
	int mleft = 150;
	int mright = (int)vrect.Width()-150;
	int mhor = 205;	

	//movers
	mvrLeft = new Mover(BRect(mleft, 0, mleft+6, vrect.Height()),
						B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, true);
	mvrRight = new Mover(BRect(mright, 0, mright+6, vrect.Height()),
						B_FOLLOW_RIGHT | B_FOLLOW_TOP_BOTTOM, true);
	mvrHorz = new Mover(BRect(-1,mhor,mleft-1,mhor+4), B_FOLLOW_LEFT | B_FOLLOW_BOTTOM,
						false);
	
	vwItems = new vitemsview(BRect(0,0,mleft-1,mhor-1), "vwItems",
							B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM, B_WILL_DRAW);
	vwProps = new vpropsview(BRect(0,mhor+5,mleft-1,vrect.Height()), "vwProps",
							B_FOLLOW_LEFT | B_FOLLOW_BOTTOM,
							B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
	vwDesign = new vdesignview(BRect(mleft+7,0,mright-1,vrect.Height()), "vwDesign",
							B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM,
							B_WILL_DRAW | B_FRAME_EVENTS);
	vwElements = new velementsview(BRect(mright+7,0,vrect.Width(),vrect.Height()),
							"vwElements", B_FOLLOW_RIGHT | B_FOLLOW_TOP_BOTTOM,
							B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE);
	// set objects
	vwItems->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	vwProps->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	vwDesign->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	vwElements->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// add objects
	AddChild(mvrLeft);
	AddChild(mvrRight);
	AddChild(mvrHorz);

	AddChild(vwItems);
	AddChild(vwProps);
	AddChild(vwDesign);
	AddChild(vwElements);
}


codewindow::codewindow(BString text)
	:	BWindow(BRect(0,0,800,600), "Niue Visual Designer - Generated source",
				B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_WILL_DRAW)
{
	//center me
	BScreen mScreen(B_MAIN_SCREEN_ID);
	BRect screenRes, winBounds;
	screenRes.Set((mScreen.Frame().left), (mScreen.Frame().top), (mScreen.Frame().right), (mScreen.Frame().bottom));
	winBounds = Bounds();
	MoveBy((screenRes.right / 2) - (winBounds.right / 2), (screenRes.bottom / 2) - (winBounds.bottom / 2));

	txtView = new BTextView(BRect(0,0,winBounds.Width()-B_V_SCROLL_BAR_WIDTH,winBounds.Height()-B_H_SCROLL_BAR_HEIGHT), "txtView", BRect(2,2,30000,30000), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	scrView = new BScrollView("scrView", txtView, B_FOLLOW_ALL_SIDES, B_WILL_DRAW, true, true, B_NO_BORDER);
	
	txtView->MakeEditable(false);
	txtView->SetViewColor(bright);
	txtView->SetFontAndColor(be_plain_font, B_FONT_ALL, &darkblue);
	txtView->SetText(text.String());

	AddChild(scrView);
}


void
VisualWindow::BuildList()
{
	//clear all
	vwItems->lstItems->MakeEmpty();
	
	//add all
	for(int i = 0; i  < vwDesign->vwDes->CountChildren(); i++)
	{
		placeholder* control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
		vwItems->lstItems->AddItem(new vlistitem(control->Name(), control->id));
	}			

	//select last item
	if(vwItems->lstItems->CountItems() > 0)
	{
		vwItems->lstItems->Select(vwItems->lstItems->CountItems()-1);
	}
}


void
VisualWindow::MessageReceived(BMessage* message)
{
	switch (message->what)
	{
		case MOVER_MESSAGE:
		{
			Mover *mv;
			if (message->FindPointer(MM_VIEW,(void**)&mv) == B_OK )
			{
				float coord = message->FindFloat(MM_COORD);

				if (mv == mvrLeft)
				{
					BPoint p = ConvertFromScreen(BPoint(coord,0));

					vwItems->ResizeTo(p.x, vwItems->Frame().Height());
					vwProps->ResizeTo(p.x, vwProps->Frame().Height());

					vwDesign->ResizeTo(Frame().Width()-p.x-mvrLeft->Frame().Width()-mvrRight->Frame().Width()-vwElements->Frame().Width()-3, vwDesign->Frame().Height());
					vwDesign->MoveTo(p.x+mvrLeft->Frame().Width()+1 , vwDesign->Frame().top);					

					mvrHorz->ResizeTo(p.x, mvrHorz->Frame().Height());
					mvrLeft->MoveTo(p.x, mvrLeft->Frame().top);		
			
				}
				else if (mv == mvrRight)
				{
					BPoint p = ConvertFromScreen(BPoint(coord,0));

					vwDesign->ResizeTo(p.x-vwItems->Frame().Width()-mvrLeft->Frame().Width()-3, vwDesign->Frame().Height());
					
					vwElements->ResizeTo(Frame().Width()-p.x-mvrRight->Frame().Width(), vwElements->Frame().Height());
					vwElements->MoveTo(p.x+mvrRight->Frame().Width(), vwElements->Frame().top); 

					mvrRight->MoveTo(p.x, mvrRight->Frame().top);
				}
			}
			break;
		}
		case V_ACTION_SHOW_LOCATION:
		{
//			(new BAlert("TITLE", "TEXT", "BUTTON1", "BUTTON2", "BUTTON3", B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();

			BString location;
			message->FindString("loca", &location);
			vwDesign->strStatus->SetText(location.String());
			break;
		}
		case V_ACTION_ITEM_SELECTED:
		{
			placeholder* control;
			vlistitem* item;

			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				//enable fields
				vwProps->txtName->SetEnabled(true);
				vwProps->txtLabel->SetEnabled(true);
				vwProps->txtTop->SetEnabled(true);
				vwProps->txtRight->SetEnabled(true);
				vwProps->txtBottom->SetEnabled(true);
				vwProps->txtLeft->SetEnabled(true);
				vwProps->chkXLeft->SetEnabled(true);
				vwProps->chkXRight->SetEnabled(true);
				vwProps->chkXBoth->SetEnabled(true);
				vwProps->chkYTop->SetEnabled(true);
				vwProps->chkYBottom->SetEnabled(true);
				vwProps->chkYBoth->SetEnabled(true);
			

				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));

				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
					control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));

					if(control->id == item->id)
					{
						//name
						vwProps->txtName->SetText(control->Name());
						//label
						if(strlen(control->label) > 0)
						{
							if(control->ChildAt(0) != 0)
							{
								if(strcmp(control->type, "BBox") == 0)  //workaround for BBoxen
								{
									BBox* box = dynamic_cast<BBox*>(control->ChildAt(0));
									vwProps->txtLabel->SetText(box->Label());
								}
								else if(strcmp(control->type, "BStringView") == 0)	//workaround for stringviews
								{
									BStringView* stringview = dynamic_cast<BStringView*>(control->ChildAt(0));
									vwProps->txtLabel->SetText(stringview->Text());
								}
								else
								{	
									BControl* element = dynamic_cast<BControl*>(control->ChildAt(0));
									vwProps->txtLabel->SetText(element->Label());
								}
							}

						}
						else
						{
							vwProps->txtLabel->SetText("");
							vwProps->txtLabel->SetEnabled(false);
						}
						
						//bounds
						BString left, top, right, bottom;
						left << (int)control->Frame().left;
						vwProps->txtLeft->SetText(left.String());
						top << (int)control->Frame().top;
						vwProps->txtTop->SetText(top.String());
						right << (int)control->Frame().right-6;	//Width()
						vwProps->txtRight->SetText(right.String());
						bottom << (int)control->Frame().bottom-1;	//Height()
						vwProps->txtBottom->SetText(bottom.String());

						//resizing mode  (lengthy)
						if(strcmp(control->resX, "B_FOLLOW_LEFT") == 0)
							vwProps->chkXLeft->SetValue(1);
						else if(strcmp(control->resX, "B_FOLLOW_RIGHT") == 0)
							vwProps->chkXRight->SetValue(1);
						else 
							vwProps->chkXBoth->SetValue(1);

						if(strcmp(control->resY, "B_FOLLOW_TOP") == 0)
							vwProps->chkYTop->SetValue(1);
						else if(strcmp(control->resY, "B_FOLLOW_BOTTOM") == 0)
							vwProps->chkYBottom->SetValue(1);
						else
							vwProps->chkYBoth->SetValue(1);
						
						control->active = true;
					}
					else
						control->active = false;
					control->Invalidate();
				}
			}
			else   // no selection
			{
				//disable fields
				vwProps->txtName->SetEnabled(false);
				vwProps->txtLabel->SetEnabled(false);
				vwProps->txtTop->SetEnabled(false);
				vwProps->txtRight->SetEnabled(false);
				vwProps->txtBottom->SetEnabled(false);
				vwProps->txtLeft->SetEnabled(false);
				vwProps->chkXLeft->SetEnabled(false);
				vwProps->chkXRight->SetEnabled(false);
				vwProps->chkXBoth->SetEnabled(false);
				vwProps->chkYTop->SetEnabled(false);
				vwProps->chkYBottom->SetEnabled(false);
				vwProps->chkYBoth->SetEnabled(false);
			}
			break;
		}
		case V_ACTION_CHANGE_NAME:
		{
			placeholder* control;
			vlistitem* item;

			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));
				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
					control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
					if(control->id == item->id)
						control->SetName(vwProps->txtName->Text());
				}
			}
			BuildList();
			break;
		}
		case V_ACTION_CHANGE_LABEL:
		{
			placeholder* control;
			vlistitem* item;

			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));
				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
					control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
					if(control->id == item->id)
					{
						//label (if there is one)
						if(control->label)
						{
							//store our label
//								control->label = vwProps->txtLabel->Text();
							//set the displayed label
							if(control->ChildAt(0) != 0)
							{
								if(strcmp(control->type, "BBox") == 0)  //workaround for BBoxen
								{
									BBox* box = dynamic_cast<BBox*>(control->ChildAt(0));
									box->SetLabel(vwProps->txtLabel->Text());
								}
								else if(strcmp(control->type, "BStringView") == 0)	//workaround for stringviews
								{
									BStringView* stringview = dynamic_cast<BStringView*>(control->ChildAt(0));
									stringview->SetText(vwProps->txtLabel->Text());											
								}
								else
								{	
									BControl* element = dynamic_cast<BControl*>(control->ChildAt(0));
									element->SetLabel(vwProps->txtLabel->Text());
								}
							}				
						}
					}
				}
			}
			break;
		}
		case V_ACTION_CHANGE_BOUNDS:
		{
			placeholder* control;
			vlistitem* item;

			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				BRect newBounds(atoi(vwProps->txtLeft->Text()), atoi(vwProps->txtTop->Text()), atoi(vwProps->txtRight->Text()), atoi(vwProps->txtBottom->Text())); 
				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));

				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
						control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));

						if(control->id == item->id)
						{
							control->MoveTo(newBounds.left, newBounds.top);
							control->ResizeTo(newBounds.right+6 - newBounds.left, newBounds.bottom+1 - newBounds.top);
							control->Invalidate();
						}
				}
			}
			break;
		}
		case V_ACTION_CHANGE_RES:
		{
			placeholder* control;
			vlistitem* item;
			
			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));

				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
					control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
					BString resmode = "";

					if(control->id == item->id)
					{
						if(vwProps->chkXLeft->Value() == 1)
							control->resX = "B_FOLLOW_LEFT";
						else if(vwProps->chkXRight->Value() == 1)
							control->resX = "B_FOLLOW_RIGHT";
						else
							control->resX = "B_FOLLOW_LEFT_RIGHT";

						if(vwProps->chkYTop->Value() == 1)
							control->resY = "B_FOLLOW_TOP";
						else if(vwProps->chkYBottom->Value() == 1)
							control->resY = "B_FOLLOW_BOTTOM";
						else
							control->resY = "B_FOLLOW_TOP_BOTTOM";
					}
//					resmode << control->resX << " | " << control->resY;
//					uint32 resizingMode = (uint32)resmode.String();
//					control->SetResizingMode(resizingMode);
//					control->ChildAt(0)->SetResizingMode((uint32)resmode.String());
//					BString test;
//					test ;
//					(new BAlert("TITLE",  test.String(), "BUTTON1", "BUTTON2", "BUTTON3", B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
				}
			}
			break;
		}
		case V_ACTION_CREATE_CODE:
		{
			BString text;
			text << GetHeaders();
			text << GetConstructors();
			BWindow* wdwCode = new codewindow(text);
			wdwCode->Show();
			break;
		}
		case V_ACTION_START_PROJECT:
		{
			//bring up the save panel
			SavePanel = new BFilePanel(B_SAVE_PANEL, 0, 0, 0, false, new BMessage(V_NEW_PROJECT_MSG), 0, false, true);
			SavePanel->SetTarget(this);
			SavePanel->SetSaveText("project name");
			SavePanel->Window()->SetTitle("Niue: Select a location to save your project folder");
			
			SavePanel->Show();
			break;
		}
		case V_NEW_PROJECT_MSG:
		{
			// first create the project
			entry_ref destref;
			BEntry destentry;
			BString projectname;
			BPath destpath;
			
			message->FindRef("directory", &destref);
			message->FindString("name", &projectname);
			
			projectname.RemoveAll(" ");  //remove all spaces 'cause SaveProject doesn't like them, too bad for the user...
			
			destentry.SetTo(&destref, false);
			destentry.GetPath(&destpath);
			
			NiueWindow* wdwNiue = 0; // = new NiueWindow(BRect(), Buff);
			
			BEntry newentry;
			entry_ref ref = wdwNiue->SaveProject(destpath.Path(), projectname.String());
			newentry.SetTo(&ref, false);
			
			//now do some fileediting magic
			BString textstring;
			BPath path(&newentry);
			BEntry fileentry;

			//.cpp
			path.Append("yourappview.cpp");
			fileentry.SetTo(path.Path(), false);
			if(fileentry.Exists())
			{
				textstring << "/*\n\tyourappview.cpp\n*/\n\n#include \"yourappview.h\"\n\nyourappview::yourappview(BRect rect, char *name)\n : BView(rect, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW)\n{\n";
				textstring << GetConstructors();
				textstring << "}\n";
				
				BFile file(&fileentry, B_WRITE_ONLY | B_ERASE_FILE);
				file.Write(textstring.String(), strlen(textstring.String()));
			}
			else
			{
				(new BAlert("Niue", "There were errors while writing to your project", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();	
			}
			
			textstring = "";
			path.SetTo(&newentry);
			path.Append("yourappwindow.cpp");
			fileentry.SetTo(path.Path(), false);
			if(fileentry.Exists())
			{
				textstring	<< "BRect rect(100,100," 
							<< (int)vwDesign->vwDes->Frame().Width()+100 << "," 
							<< (int)vwDesign->vwDes->Frame().Height()+100 <<");";
				
				BFile file(&fileentry, B_WRITE_ONLY);

				file.WriteAt(54, textstring.String(), strlen(textstring.String()));

			}
			else
			{
				(new BAlert("Niue", "There were errors while writing to your project", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();	
			}

			
			//.h
			textstring = "";
			path.SetTo(&newentry);
			path.Append("yourappview.h");
			fileentry.SetTo(path.Path(), false);
			if(fileentry.Exists())
			{
				textstring << "/*\n\tyourappview.h\n*/\n\n"
							"#ifndef\tYOURAPPVIEW_H\n"
							"#define\tYOURAPPVIEW_H\n\n#include <Application.h>\n"
							"#include <InterfaceKit.h>\n\n"
							"#define\tBUTTON_ONE_MSG\t'BTN1'\n\n"
							"class yourappview : public BView\n{\n"
							"\tpublic:\n"
							"\t\tyourappview(BRect frame, char *name);\n\n"
							"\tprivate:\n";
				textstring << GetHeaders();
				textstring << "};\n\n#endif\n";
				
				BFile file(&fileentry, B_WRITE_ONLY | B_ERASE_FILE);
				file.Write(textstring.String(), strlen(textstring.String()));
			}
			else
			{
				(new BAlert("Niue", "There were errors while writing to your project", "Ok", 0, 0, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();	
			}
			
			//close the designer
			Close();
			break;
		}
		case V_ACTION_BUILD_LIST:
		{
			BuildList();
			break;
		}
		case V_OPTION_SHOW_GRID:
		{
			if(vwElements->chkGrid->Value()==1)
				vwDesign->vwDes->ShowGrid = true;
			else if(vwElements->chkGrid->Value()==0)
				vwDesign->vwDes->ShowGrid = false;
			vwDesign->vwDes->Invalidate();	
			break;
		}
		case V_OPTION_SHOW_HANDLES:
		{
			if(vwElements->chkHandles->Value()==1)
				vwDesign->vwDes->ShowHandles = true;
			else if(vwElements->chkHandles->Value()==0)
				vwDesign->vwDes->ShowHandles = false;
//			vwDesign->vwDes->Invalidate();

			//auch, need to invalidate all our items	
			for(int i = 0; i  < vwDesign->vwDes->CountChildren(); i++)
				vwDesign->vwDes->ChildAt(i)->Invalidate();
			break;
		}
		case V_OPTION_SET_WIDTH:
		{
			bool valid = true;
			const char *value;
			value = vwElements->txtCanvasWidth->Text();
			for(unsigned int i = 1; i < strlen(value); i++)
			{
				if(isdigit(value[i]) == 0)
					valid = false;
			}

			if(valid)
			{
				int width = atoi(vwElements->txtCanvasWidth->Text());
				vwDesign->vwDes->ResizeTo((float)width, vwDesign->vwDes->Frame().Height());
				vwDesign->CenterTarget();
			}
			else
			{
				(new BAlert("Niue Visual Designer", "Please enter a numeric value.", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
			}
			break;
		}
		case V_OPTION_SET_HEIGHT:
		{
			bool valid = true;
			const char *value;
			value = vwElements->txtCanvasHeight->Text();
			for(unsigned int i = 1; i < strlen(value); i++)
			{
				if(isdigit(value[i]) == 0)
					valid = false;
			}
			
			if(valid)
			{
				int height = atoi(vwElements->txtCanvasHeight->Text());
				vwDesign->vwDes->ResizeTo(vwDesign->vwDes->Frame().Width(), (float)height);
				vwDesign->CenterTarget();
			}
			else
			{
				(new BAlert("Niue Visual Designer", "Please enter a numeric value.", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT))->Go();
			}
			break;
		}
		case V_OPTION_ALIGN_VERT:
		{
			int32 elements = vwDesign->vwDes->CountChildren();
			int r = vwDesign->vwDes->scale*2;

			for(int i=0; i < elements; i++)
			{
				BView* control = vwDesign->vwDes->ChildAt(i);

				//do some rounding
				int y = (int)control->Frame().top;
				int ry = y%r;

				if(ry <= r/2)  //round down
				{
						y-=ry;
				}
				else	//round up
				{
						y+=r-ry;
				}
				
				control->MoveTo(control->Frame().left, y);
			}
			break;
		}
		case V_OPTION_ALIGN_HORZ:
		{
			int32 elements = vwDesign->vwDes->CountChildren();
			int r = vwDesign->vwDes->scale*2;

			for(int i=0; i < elements; i++)
			{
				BView* control = vwDesign->vwDes->ChildAt(i);

				//do some rounding
				int x = (int)control->Frame().left;
				int rx = x%r;

				if(rx <= r/2)	//round down
				{
						x-=rx;
				}
				else	//round up
				{
					x+=r-rx;
				}					

				control->MoveTo(x, control->Frame().top);
			}
			break;
		}
		case V_OPTION_ALIGN_GRID:
		{
			int32 elements = vwDesign->vwDes->CountChildren();
			int r = vwDesign->vwDes->scale*2;

			for(int i=0; i < elements; i++)
			{
				BView* control = vwDesign->vwDes->ChildAt(i);

				//do some rounding
				int y = (int)control->Frame().top;
				int x = (int)control->Frame().left;
				int rx = x % r;
				int ry = y % r;

				if(rx <= r/2)	//round down
					x-=rx;
				else	//round up
					x+=r-rx;
					
				if(ry <= r/2)  //round down
					y-=ry;
				else	//round up
					y+=r-ry;

				control->MoveTo(x, y);
			}
			break;
		}
		case V_OPTION_KILL_SEL:
		{
			placeholder* control;
			vlistitem* item;

			if(vwItems->lstItems->CurrentSelection() >= 0)
			{
				item = dynamic_cast<vlistitem*>(vwItems->lstItems->ItemAt(vwItems->lstItems->CurrentSelection()));

				for(int i = 0; i < vwDesign->vwDes->CountChildren(); i++)
				{
					control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));

					if(control->id == item->id)
						control->RemoveSelf();
				}
			}
			BuildList();
			break;
		}
		case V_OPTION_KILL_ALL:
		{
			//remove elements from grid
			while(vwDesign->vwDes->CountChildren() != 0)
				vwDesign->vwDes->ChildAt(0)->RemoveSelf();

			//reset counters
			vwDesign->vwDes->ResetCounters();

			//update the lsit
			BuildList();

			//and disable fields
			vwProps->txtName->SetEnabled(false);
			vwProps->txtLabel->SetEnabled(false);
			vwProps->txtTop->SetEnabled(false);
			vwProps->txtRight->SetEnabled(false);
			vwProps->txtBottom->SetEnabled(false);
			vwProps->txtLeft->SetEnabled(false);
			break;
		}
		case V_ADD_BOX_MSG:
		{
			vwDesign->vwDes->currenttype = "BOX";
			vwElements->radBox->SetValue(1);
			break;
		}
		case V_ADD_BUTTON_MSG:
		{
			vwDesign->vwDes->currenttype = "BUTTON";
			vwElements->radButton->SetValue(1);
			break;
		}	
		case V_ADD_PICTUREBUTTON_MSG:
		{
			vwDesign->vwDes->currenttype = "PICTUREBUTTON";
			vwElements->radPictureButton->SetValue(1);
			break;
		}	
		case V_ADD_RADIOBUTTON_MSG:
		{
			vwDesign->vwDes->currenttype = "RADIOBUTTON";
			vwElements->radRadioButton->SetValue(1);
			break;
		}	
		case V_ADD_CHECKBOX_MSG:
		{
			vwDesign->vwDes->currenttype = "CHECKBOX";
			vwElements->radCheckBox->SetValue(1);
			break;
		}	
		case V_ADD_STRINGVIEW_MSG:
		{
			vwDesign->vwDes->currenttype = "STRINGVIEW";
			vwElements->radStringView->SetValue(1);
			break;
		}	
		case V_ADD_TEXTCONTROL_MSG:
		{
			vwDesign->vwDes->currenttype = "TEXTCONTROL";
			vwElements->radTextControl->SetValue(1);
			break;
		}	
		case V_ADD_TEXTVIEW_MSG:
		{
			vwDesign->vwDes->currenttype = "TEXTVIEW";
			vwElements->radTextView->SetValue(1);
			break;
		}	
		case V_ADD_LISTVIEW_MSG:
		{
			vwDesign->vwDes->currenttype = "LISTVIEW";
			vwElements->radListView->SetValue(1);
			break;
		}
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}
	}
}


BString
VisualWindow::GetHeaders()
{
	BString headstring;
	headstring << "\n\t//header code\n";
	
	//define each placeholder/element
	for(int i = 0; i  < vwDesign->vwDes->CountChildren(); i++)
	{
		placeholder* control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
		headstring << "\t" <<  control->type << "*\t\t" << control->Name() << ";\n";
	}

	headstring << "\n";
	return headstring;		
}


BString
VisualWindow::GetConstructors()
{
	BString codestring;
	BString resandflags;
	codestring << "\n\n\t//construct elements\n";

	//create a constructor for each placeholder/element
	for(int i = 0; i  < vwDesign->vwDes->CountChildren(); i++)
	{
		placeholder* control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));

		//global
		resandflags = "";
		if(strcmp(control->resX, "B_FOLLOW_LEFT_RIGHT") == 0 || strcmp(control->resY, "B_FOLLOW_TOP_BOTTOM") == 0)
			resandflags << control->resX << " | " << control->resY << ", B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE";
		else
			resandflags << control->resX << " | " << control->resY << ", B_WILL_DRAW";
		
		//control specific
		if(strcmp(control->type, "BBox") == 0)
		{
			resandflags << " | B_FANCY_BORDER";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << "," 
						<< (int)control->Frame().top << "," 
						<< (int)control->Frame().right-6 << "," 
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\"," << resandflags << ");\n";
			
			codestring	<< "\t" << control->Name() << "->SetLabel(\"" 
						<< dynamic_cast<BBox*>(control->ChildAt(0))->Label() << "\");\n";
		}
		else if(strcmp(control->type, "BButton") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << "," 
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", \"" 
						<< dynamic_cast<BControl*>(control->ChildAt(0))->Label() 
						<< "\", new BMessage(), " << resandflags << ");\n";
		}
		else if(strcmp(control->type, "BPictureButton") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name()
						<< "\", new BPicture(), new BPicture(), new BMessage(), B_ONE_STATE_BUTTON, " 
						<< resandflags << ");\n";
		}
		else if(strcmp(control->type, "BRadioButton") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", \""
						<< dynamic_cast<BControl*>(control->ChildAt(0))->Label()
						<< "\", new BMessage(), " << resandflags << ");\n";
		}
		else if(strcmp(control->type, "BCheckBox") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", \""
						<< dynamic_cast<BControl*>(control->ChildAt(0))->Label()
						<< "\", new BMessage(), " << resandflags << ");\n";
		}
		else if(strcmp(control->type, "BStringView") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type 
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", \""
						<< dynamic_cast<BStringView*>(control->ChildAt(0))->Text()
						<< "\", " << resandflags << ");\n";
		}
		else if(strcmp(control->type, "BTextControl") == 0)
		{
			resandflags << " | B_NAVIGABLE";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", \"" <<
						dynamic_cast<BControl*>(control->ChildAt(0))->Label()
						<< "\", \"\", new BMessage(), " << resandflags << ");\n";
		}
		else if(strcmp(control->type, "BTextView") == 0)
		{
			resandflags << " | B_NAVIGABLE | B_FRAME_EVENTS | B_PULSE_NEEDED";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << ","
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", BRect(2,2,"
						<< (int)control->Frame().right-8 << ","
						<< (int)control->Frame().bottom-3 << "), "
						<< resandflags << ");\n";
		}
		else if(strcmp(control->type, "BListView") == 0)
		{
			resandflags << " | B_FRAME_EVENTS";
			codestring	<< "\t" << control->Name() << " = new " << control->type
						<< "(BRect(" << (int)control->Frame().left << ","
						<< (int)control->Frame().top << ","
						<< (int)control->Frame().right-6 << "-B_V_SCROLL_BAR_WIDTH,"
						<< (int)control->Frame().bottom-1 << "), \"" 
						<< control->Name() << "\", B_SINGLE_SELECTION_LIST, "
						<< resandflags << ");\n" 
						<< "\tBScrollView* scrScrollView = new BScrollView(\"scrScrollView\", "
						<< control->Name() << ", " << resandflags
						<< ", false, true, B_PLAIN_BORDER);\n";
		}
	
	}

	codestring << "\n\t//add elements\n";

	//add each placeholder/element
	for(int i = 0; i  < vwDesign->vwDes->CountChildren(); i++)
	{
		placeholder* control = dynamic_cast<placeholder*>(vwDesign->vwDes->ChildAt(i));
		
		if(strcmp(control->type, "BListView") == 0) //hack for listview insite scrollview
			codestring << "\tAddChild(scrScrollView);\n";
		else
			codestring << "\tAddChild(" << control->Name() << ");\n";
	}

	codestring << "\n";
	return codestring;	
}

