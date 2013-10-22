##Niue makefile v1

NAME := YourApp
TYPE := APP
SRCS := 	yourapp.cpp yourappview.cpp yourappwindow.cpp
LIBS := 	be
OPTIMIZE := FULL
RSRCS := yourapp.rsrc

#       Specify your RDEF files, if any. 
RDEFS := 

#       specify additional paths to directories following the standard 
#       libXXX.so or libXXX.a naming scheme.  You can specify full paths 
#       or paths relative to the makefile.  The paths included may not 
#       be recursive, so include all of the paths where libraries can 
#       be found.  Directories where source files are found are 
#       automatically included. 
LIBPATHS := 

#       additional paths to look for system headers 
#       these use the form: #include <header.h> 
#       source file directories are NOT auto-included here 
SYSTEM_INCLUDE_PATHS := 

#       additional paths to look for local headers 
#       thes use the form: #include "header.h" 
#       source file directories are automatically included 
LOCAL_INCLUDE_PATHS := 

#       specify any preprocessor symbols to be defined.  The symbols will not 
#       have their values set automatically; you must supply the value (if any) 
#       to use.  For example, setting DEFINES to "DEBUG=1" will cause the 
#       compiler option "-DDEBUG=1" to be used.  Setting DEFINES to "DEBUG" 
#       would pass "-DDEBUG" on the compiler's command line. 
DEFINES := 

#       specify special warning levels 
#       if unspecified default warnings will be used 
#       NONE = supress all warnings 
#       ALL = enable all warnings 
WARNINGS := 

#       specify whether image symbols will be created 
#       so that stack crawls in the debugger are meaningful 
#       if TRUE symbols will be created 
SYMBOLS := 

#       specify debug settings 
#       if TRUE will allow application to be run from a source-level 
#       debugger.  Note that this will disable all optimzation. 
DEBUGGER := 

#       specify additional compiler flags for all files 
COMPILER_FLAGS := 

#       specify additional linker flags 
LINKER_FLAGS := 

#       specify additional flex flags 
FLEX_FLAGS := 

#       specify additional bison flags 
BISON_FLAGS := 

#       specify the version of this particular item (follows setversion parameter structure)
#       for example:
#       -app 3 4 0 d 0 -short "340" -long "340 "`echo -n -e '©'`"1999 GNU GPL"
#       -app 1 0 1 f 0 -short "App 1.0.1" -long "Company App 1.0.1 written by FOO"
#       This may also be specified in a resource. 
APP_VERSION := 

#       (for TYPE == DRIVER only) Specify desired location of driver in the /dev 
#       hierarchy. Used by the driverinstall rule. E.g., DRIVER_PATH = video/usb will 
#       instruct the driverinstall rule to place a symlink to your driver's binary in 
#       ~/add-ons/kernel/drivers/dev/video/usb, so that your driver will appear at 
#       /dev/video/usb when loaded. Default is "misc". 
DRIVER_PATH := 

#       Specify if you want the object files to be somewhere besides the default location. 
OBJ_DIR := _objects

#       Specify a non default placement for the target 
TARGET_DIR := .

#       If you want to see the complete build line for every file, then set this to 'true', 
#       otherwise it will tell you at the end what the build flags were. 
CHATTY := 

#       Specify a directory for the 'install' target. 
INSTALL_DIR :=

#       Specify the name of this makefile. 
#       If you leave this blank, the makefile will not be considered as part of the 
#       dependenies for the project, and the project will not be rebuilt when the makefile 
#       is changed 
MAKEFILE := 

#       Specify TRUE if you want the install target to create links in the BeMenu 
MENU_LINKS :=

#       Related to MENU_LINKS, specify the name of the direcotry in the BeMenu 
#       you wish the link to go in. If the directory does not exist, it will be 
#       created. 
APP_MENU :=

#       If, for some reason, you don't want to use the dependencies (flex and yacc seem to choke 
#       on them), set this to false 
DODEPS := 

#	Set this variable if you have an svg text file you wish to use as 
#   your target's icon.
SVG_ICON := 

#	If you have some fancy custom build steps to do, specify them here
EXTRA_BUILD_STEPS := 

#	If you have some other files that should trigger a re-link, such as libs in the same
#	project that may get rebuilt, specify the full path to them here.
EXTRA_DEPS := 

include /boot/develop/etc/makefile-engine


