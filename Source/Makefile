
#-----------------------------------------------------------------------------
#
#	Niue Development Environment
#	Copyright 2008 T. Lansbergen, All Rights Reserved.
#
#	Redistribution and use in source and binary forms, with or without
#	modification, are permitted for non-commercial use only.
#	
#	This software is provided ``as is'' and any express or implied warranties,
#	including, but not limited to, the implied warranties of merchantability and
#	fitness for a particular purpose are disclaimed. In no event shall
#	authors be liable for any direct, indirect, incidental, special,
#	exemplary, or consequential damages (including, but not limited to,
#	procurement of substitute goods or services; loss of use, data, or profits;
#	or business interruption) however caused and on any theory of liability,
#	whether in contract, strict liability, or tort (including negligence or
#	otherwise) arising in any way out of the use of this software, even if
#	advised of the possibility of such damage. 
#
#-----------------------------------------------------------------------------





# User definible options:  (uncomment for debug, comment for release)
#DEBUG           =  TRUE

ifneq ($(DEBUG),TRUE)
	DEBUG  = FALSE
endif
ifeq ($(BE_HOST_CPU),ppc)
	PLATFORM       = PPC
else
	PLATFORM        = X86
endif

APPNAME         = Niue
AUTODEPEND      = TRUE

## Files
SOURCES =   	cblist.cpp     \
			Hash.cpp Schemes.cpp     \
			SMap.cpp        \
			BMapView.cpp    \
			BSBar.cpp       \
			NiueWindow.cpp  \
			NTree.cpp UBuffer.cpp  BufList.cpp  \
			Niue.cpp  \
			ViewText.cpp ViewBase.cpp \
			FFont.cpp \
			mover.cpp \
			aboutwindow.cpp renamewindow.cpp \
			openwindow.cpp barberwindow.cpp exportwindow.cpp makefilewindow.cpp \
			listview.cpp controlview.cpp outputview.cpp \
			IconMenu.cpp snippetmenu.cpp docwindow.cpp \
			visualwindow.cpp \
			vdesignview.cpp velementsview.cpp vitemsview.cpp vpropsview.cpp \
			grepwindow.cpp gqueryview.cpp goutputview.cpp \
			snippetwindow.cpp ssnippetview.cpp

ifeq ($(DEBUG),TRUE)
	OBJDRAW     = obj.x86
	X86_EXTRAS         =
	PPC_EXTRAS         =
	X86_LD          = cc -lbe -lroot -ltracker -ltranslation -lstdc++
	PPC_LD          = 
else
	OBJDRAW     = obj.x86
	X86_EXTRAS    = -O2 -D RELBUILD
	PPC_EXTRAS    = -O2 -d RELBUILD
	X86_LD          = cc -lbe -lroot -ltracker -ltranslation -lstdc++
	PPC_LD          = mwld -map $(APPNAME).xMAP
endif

X86_OPTIONS        =  -W
PPC_OPTIONS           =


LD                     =$($(PLATFORM)_LD)
OPTIONS           = $($(PLATFORM)_OPTIONS)
EXTRAS             = $($(PLATFORM)_EXTRAS)

INCLUDES        = -I/boot/system/develop/headers
CC              = cc
SHELL           = /bin/sh
CFLAGS          = $(OPTIONS) $(EXTRAS) $(INCLUDES) -std=c++03

PPC_HEADERS   =$(CC) $(CFLAGS) -make
X86_HEADERS   =$(CC) $(CFLAGS) -MM
HEADERS = $($(PLATFORM)_HEADERS)


ifeq ($OBJDRAW,)
	OBJDRAW     = .
endif

OBJS=$(addprefix $(OBJDRAW)/,$(SOURCES:.cpp=.o))

.PHONY: counter more checkfolder


## top level calls...
all:  $(OBJDRAW) compile more

run: all
	$(APPNAME)

debug: all
	/boot/system/apps/Debugger $(APPNAME)  &

clean:
	@echo Removing all Objects and Binaries
	rm -f $(APPNAME)rsrc/$(APPNAME).rsrc
	rm -f $(APPNAME)rsrc/Docs.zip
	rm -f $(APPNAME)rsrc/Skeleton.zip
	rm -f $(APPNAME).xSYM
	rm -f $(APPNAME).xMAP
	rm -f $(OBJDRAW)/*.o
	rm -f $(OBJDRAW)/*.d
	rm -f $(APPNAME)
	@echo All clean, type 'make all' to rebuild

compile: $(APPNAME)

$(APPNAME): $(OBJS) $(APPNAME)rsrc/$(APPNAME).rsrc
	@echo Linking...
	@$(LD) -o $(APPNAME) $(OBJS)
	@echo ...'xres' and 'mimeset'...
	@xres -o $(APPNAME) $(APPNAME)rsrc/$(APPNAME).rsrc
	@mimeset $(APPNAME)
	@echo $(APPNAME) executable created successfully

## the "?" should be a Semicolon
$(OBJDRAW)/%.o : %.cpp
	@echo Compiling $<...
	@$(CC) $(CFLAGS) -o $@ -c $<

Niue.res/Niue.r.out : Niue.res/TOKE.1.NIUE?tokens_cpp.txt Niue.res/TOKE.2.NIUE?tokens_be.txt Niue.res/TOKE.3.NIUE?tokens_const.txt
	cd Niue.res;mwbres Niue.r

#rebuild those resources..

Skeleton.zip:
	@zip -r $(APPNAME)rsrc/Skeleton.zip ../Skeleton

Docs.zip:
	@zip -j $(APPNAME)rsrc/Docs.zip ../Documentation/*

$(APPNAME)rsrc/$(APPNAME).rsrc: Skeleton.zip Docs.zip
	@rm -f Niue.rsrc
	@xres -o Niue.rsrc Niuersrc/Niuedef.rsrc
	@xres -o Niue.rsrc Niue.rsrc -a TOKE:1:tokens_cpp.txt Niuersrc/tokens_cpp.txt
	@xres -o Niue.rsrc Niue.rsrc -a TOKE:2:tokens_be.txt Niuersrc/tokens_be.txt
	@xres -o Niue.rsrc Niue.rsrc -a TOKE:3:tokens_const.txt Niuersrc/tokens_const.txt
	@xres -o Niue.rsrc Niue.rsrc -a SKEL:9:Skeleton.zip Niuersrc/Skeleton.zip
	@xres -o Niue.rsrc Niue.rsrc -a DOCS:8:Docs.zip Niuersrc/Docs.zip
#	@xres -o Niue.rsrc Niue.rsrc -a ICON:101:BEOS:L:STD_ICON Niuersrc/IconLarge
#	@xres -o Niue.rsrc Niue.rsrc -a MICN:101:BEOS:M:STD_ICON Niuersrc/IconMedium
	@mv Niue.rsrc Niuersrc/Niue.rsrc

checkfolder: $(OBJDRAW)/

#create a new folder
$(OBJDRAW):
	$(shell mkdir -p $(OBJDRAW))

ifeq ($(AUTODEPEND),TRUE)
$(OBJDRAW)/%.d: %.cpp
#        @echo Rebuilding dependancies for $<
		@echo Rebuilding dependancies for $(basename $(notdir $@)).cpp
#       @$(CC) $(CFLAGS) -make $(basename $(notdir $@)).cpp > /tmp/YateMakeFileBuilder
		@$(HEADERS) $(basename $(notdir $@)).cpp > /tmp/YateMakeFileBuilder
		@echo -e -n "\n$(OBJDRAW)/" | cat - /tmp/YateMakeFileBuilder >$@
#        $(SHELL) -ec '$(CC) -make $< \  #for gcc they tell me..
#                    | sed '\''s/\($*\)\.o[ :]*/\1 $@/g'\'' > $(OBJDRAW)/$@'
else
$(OBJDRAW)/%.d:
endif


-include $(addprefix $(OBJDRAW)/,$(SOURCES:.cpp=.d))
