# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (x86) External Target" 0x0106
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=grove - Win32 Release
!MESSAGE No configuration specified.  Defaulting to grove - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "grove - Win32 Release" && "$(CFG)" != "grove - Win32 Debug" &&\
 "$(CFG)" != "spgrove - Win32 Release" && "$(CFG)" != "spgrove - Win32 Debug" &&\
 "$(CFG)" != "style - Win32 Release" && "$(CFG)" != "style - Win32 Debug" &&\
 "$(CFG)" != "jade - Win32 Release" && "$(CFG)" != "jade - Win32 Debug" &&\
 "$(CFG)" != "jadedist - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "jade.mak" CFG="grove - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "grove - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "grove - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "spgrove - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "spgrove - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "style - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "style - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "jade - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "jade - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "jadedist - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "jade - Win32 Debug"

!IF  "$(CFG)" == "grove - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "grove\Release"
# PROP BASE Intermediate_Dir "grove\Release"
# PROP BASE Target_Dir "grove"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "grove\Release"
# PROP Intermediate_Dir "grove\Release"
# PROP Target_Dir "grove"
OUTDIR=.\grove\Release
INTDIR=.\grove\Release

ALL : ".\bin\grove.dll"

CLEAN : 
	-@erase "$(INTDIR)\Node.obj"
	-@erase "$(OUTDIR)\grove.exp"
	-@erase "$(OUTDIR)\grove.lib"
	-@erase ".\bin\grove.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D "BUILD_LIBGROVE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /D "NDEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "BUILD_LIBGROVE" /Fp"$(INTDIR)/grove.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\grove\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/grove.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20200000 /subsystem:windows /dll /machine:I386 /out:"bin\grove.dll"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x20200000 /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/grove.pdb" /machine:I386 /out:"bin\grove.dll"\
 /implib:"$(OUTDIR)/grove.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Node.obj"

".\bin\grove.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "grove - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "grove\Debug"
# PROP BASE Intermediate_Dir "grove\Debug"
# PROP BASE Target_Dir "grove"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "grove\Debug"
# PROP Intermediate_Dir "grove\Debug"
# PROP Target_Dir "grove"
OUTDIR=.\grove\Debug
INTDIR=.\grove\Debug

ALL : ".\lib\UnicodeDebug\grove.dll"

CLEAN : 
	-@erase "$(INTDIR)\Node.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\grove.exp"
	-@erase "$(OUTDIR)\grove.lib"
	-@erase "$(OUTDIR)\grove.pdb"
	-@erase ".\lib\UnicodeDebug\grove.dll"
	-@erase ".\lib\UnicodeDebug\grove.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D "BUILD_LIBGROVE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /D "_DEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "BUILD_LIBGROVE" /Fp"$(INTDIR)/grove.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\grove\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/grove.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20200000 /subsystem:windows /dll /debug /machine:I386 /out:"lib\UnicodeDebug\grove.dll"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /base:0x20200000 /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/grove.pdb" /debug /machine:I386\
 /out:"lib\UnicodeDebug\grove.dll" /implib:"$(OUTDIR)/grove.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Node.obj"

".\lib\UnicodeDebug\grove.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spgrove - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "spgrove\Release"
# PROP BASE Intermediate_Dir "spgrove\Release"
# PROP BASE Target_Dir "spgrove"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "spgrove\Release"
# PROP Intermediate_Dir "spgrove\Release"
# PROP Target_Dir "spgrove"
OUTDIR=.\spgrove\Release
INTDIR=.\spgrove\Release

ALL : "grove - Win32 Release" ".\bin\spgrove.dll"

CLEAN : 
	-@erase "$(INTDIR)\GroveApp.obj"
	-@erase "$(INTDIR)\GroveBuilder.obj"
	-@erase "$(OUTDIR)\spgrove.exp"
	-@erase "$(OUTDIR)\spgrove.lib"
	-@erase ".\bin\spgrove.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "grove" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D SP_NAMESPACE=James_Clark_SP /D "BUILD_LIBSPGROVE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /I "grove" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 SP_NAMESPACE=James_Clark_SP /D "BUILD_LIBSPGROVE" /Fp"$(INTDIR)/spgrove.pch"\
 /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\spgrove\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spgrove.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20100000 /subsystem:windows /dll /machine:I386 /out:"bin/spgrove.dll"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20100000 /subsystem:windows\
 /dll /incremental:no /pdb:"$(OUTDIR)/spgrove.pdb" /machine:I386\
 /out:"bin/spgrove.dll" /implib:"$(OUTDIR)/spgrove.lib" 
LINK32_OBJS= \
	"$(INTDIR)\GroveApp.obj" \
	"$(INTDIR)\GroveBuilder.obj" \
	".\grove\Release\grove.lib"

".\bin\spgrove.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spgrove - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "spgrove\Debug"
# PROP BASE Intermediate_Dir "spgrove\Debug"
# PROP BASE Target_Dir "spgrove"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "spgrove\Debug"
# PROP Intermediate_Dir "spgrove\Debug"
# PROP Target_Dir "spgrove"
OUTDIR=.\spgrove\Debug
INTDIR=.\spgrove\Debug

ALL : "grove - Win32 Debug" ".\lib\UnicodeDebug\spgrove.dll"

CLEAN : 
	-@erase "$(INTDIR)\GroveApp.obj"
	-@erase "$(INTDIR)\GroveBuilder.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\spgrove.exp"
	-@erase "$(OUTDIR)\spgrove.lib"
	-@erase "$(OUTDIR)\spgrove.pdb"
	-@erase ".\lib\UnicodeDebug\spgrove.dll"
	-@erase ".\lib\UnicodeDebug\spgrove.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "grove" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "SP_MULTI_BYTE" /D GROVE_NAMESPACE=James_Clark_GROVE /D SP_NAMESPACE=James_Clark_SP /D "BUILD_LIBSPGROVE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "include" /I "grove" /D "_DEBUG"\
 /D "WIN32" /D "_WINDOWS" /D "SP_MULTI_BYTE" /D\
 GROVE_NAMESPACE=James_Clark_GROVE /D SP_NAMESPACE=James_Clark_SP /D\
 "BUILD_LIBSPGROVE" /Fp"$(INTDIR)/spgrove.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\spgrove\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spgrove.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20100000 /subsystem:windows /dll /debug /machine:I386 /out:"lib\UnicodeDebug\spgrove.dll"
LINK32_FLAGS=lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20100000 /subsystem:windows\
 /dll /incremental:yes /pdb:"$(OUTDIR)/spgrove.pdb" /debug /machine:I386\
 /out:"lib\UnicodeDebug\spgrove.dll" /implib:"$(OUTDIR)/spgrove.lib" 
LINK32_OBJS= \
	"$(INTDIR)\GroveApp.obj" \
	"$(INTDIR)\GroveBuilder.obj" \
	".\grove\Debug\grove.lib"

".\lib\UnicodeDebug\spgrove.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "style - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "style\Release"
# PROP BASE Intermediate_Dir "style\Release"
# PROP BASE Target_Dir "style"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "style\Release"
# PROP Intermediate_Dir "style\Release"
# PROP Target_Dir "style"
OUTDIR=.\style\Release
INTDIR=.\style\Release

ALL : "grove - Win32 Release" ".\bin\style.dll"

CLEAN : 
	-@erase "$(INTDIR)\Collector.obj"
	-@erase "$(INTDIR)\common_inst.obj"
	-@erase "$(INTDIR)\DssslSpecEventHandler.obj"
	-@erase "$(INTDIR)\ELObj.obj"
	-@erase "$(INTDIR)\ELObjMessageArg.obj"
	-@erase "$(INTDIR)\Expression.obj"
	-@erase "$(INTDIR)\FlowObj.obj"
	-@erase "$(INTDIR)\FOTBuilder.obj"
	-@erase "$(INTDIR)\InheritedC.obj"
	-@erase "$(INTDIR)\Insn.obj"
	-@erase "$(INTDIR)\Interpreter.obj"
	-@erase "$(INTDIR)\InterpreterMessages.obj"
	-@erase "$(INTDIR)\NumberCache.obj"
	-@erase "$(INTDIR)\primitive.obj"
	-@erase "$(INTDIR)\ProcessContext.obj"
	-@erase "$(INTDIR)\ProcessingMode.obj"
	-@erase "$(INTDIR)\Style.obj"
	-@erase "$(INTDIR)\style.pch"
	-@erase "$(INTDIR)\style_inst.obj"
	-@erase "$(INTDIR)\StyleEngine.obj"
	-@erase "$(INTDIR)\stylelib.obj"
	-@erase "$(OUTDIR)\style.exp"
	-@erase "$(OUTDIR)\style.lib"
	-@erase ".\bin\style.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D "SP_MULTI_BYTE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\style\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/style.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20000000 /subsystem:windows /dll /machine:I386 /out:"bin\style.dll"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20000000 /subsystem:windows\
 /dll /incremental:no /pdb:"$(OUTDIR)/style.pdb" /machine:I386\
 /out:"bin\style.dll" /implib:"$(OUTDIR)/style.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Collector.obj" \
	"$(INTDIR)\common_inst.obj" \
	"$(INTDIR)\DssslSpecEventHandler.obj" \
	"$(INTDIR)\ELObj.obj" \
	"$(INTDIR)\ELObjMessageArg.obj" \
	"$(INTDIR)\Expression.obj" \
	"$(INTDIR)\FlowObj.obj" \
	"$(INTDIR)\FOTBuilder.obj" \
	"$(INTDIR)\InheritedC.obj" \
	"$(INTDIR)\Insn.obj" \
	"$(INTDIR)\Interpreter.obj" \
	"$(INTDIR)\InterpreterMessages.obj" \
	"$(INTDIR)\NumberCache.obj" \
	"$(INTDIR)\primitive.obj" \
	"$(INTDIR)\ProcessContext.obj" \
	"$(INTDIR)\ProcessingMode.obj" \
	"$(INTDIR)\Style.obj" \
	"$(INTDIR)\style_inst.obj" \
	"$(INTDIR)\StyleEngine.obj" \
	"$(INTDIR)\stylelib.obj" \
	".\grove\Release\grove.lib"

".\bin\style.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "style\Debug"
# PROP BASE Intermediate_Dir "style\Debug"
# PROP BASE Target_Dir "style"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "style\Debug"
# PROP Intermediate_Dir "style\Debug"
# PROP Target_Dir "style"
OUTDIR=.\style\Debug
INTDIR=.\style\Debug

ALL : "grove - Win32 Debug" ".\lib\UnicodeDebug\style.dll"\
 "$(OUTDIR)\style.bsc"

CLEAN : 
	-@erase "$(INTDIR)\Collector.obj"
	-@erase "$(INTDIR)\Collector.sbr"
	-@erase "$(INTDIR)\common_inst.obj"
	-@erase "$(INTDIR)\common_inst.sbr"
	-@erase "$(INTDIR)\DssslSpecEventHandler.obj"
	-@erase "$(INTDIR)\DssslSpecEventHandler.sbr"
	-@erase "$(INTDIR)\ELObj.obj"
	-@erase "$(INTDIR)\ELObj.sbr"
	-@erase "$(INTDIR)\ELObjMessageArg.obj"
	-@erase "$(INTDIR)\ELObjMessageArg.sbr"
	-@erase "$(INTDIR)\Expression.obj"
	-@erase "$(INTDIR)\Expression.sbr"
	-@erase "$(INTDIR)\FlowObj.obj"
	-@erase "$(INTDIR)\FlowObj.sbr"
	-@erase "$(INTDIR)\FOTBuilder.obj"
	-@erase "$(INTDIR)\FOTBuilder.sbr"
	-@erase "$(INTDIR)\InheritedC.obj"
	-@erase "$(INTDIR)\InheritedC.sbr"
	-@erase "$(INTDIR)\Insn.obj"
	-@erase "$(INTDIR)\Insn.sbr"
	-@erase "$(INTDIR)\Interpreter.obj"
	-@erase "$(INTDIR)\Interpreter.sbr"
	-@erase "$(INTDIR)\InterpreterMessages.obj"
	-@erase "$(INTDIR)\InterpreterMessages.sbr"
	-@erase "$(INTDIR)\NumberCache.obj"
	-@erase "$(INTDIR)\NumberCache.sbr"
	-@erase "$(INTDIR)\primitive.obj"
	-@erase "$(INTDIR)\primitive.sbr"
	-@erase "$(INTDIR)\ProcessContext.obj"
	-@erase "$(INTDIR)\ProcessContext.sbr"
	-@erase "$(INTDIR)\ProcessingMode.obj"
	-@erase "$(INTDIR)\ProcessingMode.sbr"
	-@erase "$(INTDIR)\Style.obj"
	-@erase "$(INTDIR)\style.pch"
	-@erase "$(INTDIR)\Style.sbr"
	-@erase "$(INTDIR)\style_inst.obj"
	-@erase "$(INTDIR)\style_inst.sbr"
	-@erase "$(INTDIR)\StyleEngine.obj"
	-@erase "$(INTDIR)\StyleEngine.sbr"
	-@erase "$(INTDIR)\stylelib.obj"
	-@erase "$(INTDIR)\stylelib.sbr"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\style.bsc"
	-@erase "$(OUTDIR)\style.exp"
	-@erase "$(OUTDIR)\style.lib"
	-@erase "$(OUTDIR)\style.pdb"
	-@erase ".\lib\UnicodeDebug\style.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D "SP_MULTI_BYTE" /FR /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /YX /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\style\Debug/
CPP_SBRS=.\style\Debug/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/style.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Collector.sbr" \
	"$(INTDIR)\common_inst.sbr" \
	"$(INTDIR)\DssslSpecEventHandler.sbr" \
	"$(INTDIR)\ELObj.sbr" \
	"$(INTDIR)\ELObjMessageArg.sbr" \
	"$(INTDIR)\Expression.sbr" \
	"$(INTDIR)\FlowObj.sbr" \
	"$(INTDIR)\FOTBuilder.sbr" \
	"$(INTDIR)\InheritedC.sbr" \
	"$(INTDIR)\Insn.sbr" \
	"$(INTDIR)\Interpreter.sbr" \
	"$(INTDIR)\InterpreterMessages.sbr" \
	"$(INTDIR)\NumberCache.sbr" \
	"$(INTDIR)\primitive.sbr" \
	"$(INTDIR)\ProcessContext.sbr" \
	"$(INTDIR)\ProcessingMode.sbr" \
	"$(INTDIR)\Style.sbr" \
	"$(INTDIR)\style_inst.sbr" \
	"$(INTDIR)\StyleEngine.sbr" \
	"$(INTDIR)\stylelib.sbr"

"$(OUTDIR)\style.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20000000 /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"lib\UnicodeDebug\style.dll"
# SUBTRACT LINK32 /map
LINK32_FLAGS=lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /base:0x20000000 /subsystem:windows\
 /dll /incremental:no /pdb:"$(OUTDIR)/style.pdb" /debug /machine:I386\
 /out:"lib\UnicodeDebug\style.dll" /implib:"$(OUTDIR)/style.lib" 
LINK32_OBJS= \
	"$(INTDIR)\Collector.obj" \
	"$(INTDIR)\common_inst.obj" \
	"$(INTDIR)\DssslSpecEventHandler.obj" \
	"$(INTDIR)\ELObj.obj" \
	"$(INTDIR)\ELObjMessageArg.obj" \
	"$(INTDIR)\Expression.obj" \
	"$(INTDIR)\FlowObj.obj" \
	"$(INTDIR)\FOTBuilder.obj" \
	"$(INTDIR)\InheritedC.obj" \
	"$(INTDIR)\Insn.obj" \
	"$(INTDIR)\Interpreter.obj" \
	"$(INTDIR)\InterpreterMessages.obj" \
	"$(INTDIR)\NumberCache.obj" \
	"$(INTDIR)\primitive.obj" \
	"$(INTDIR)\ProcessContext.obj" \
	"$(INTDIR)\ProcessingMode.obj" \
	"$(INTDIR)\Style.obj" \
	"$(INTDIR)\style_inst.obj" \
	"$(INTDIR)\StyleEngine.obj" \
	"$(INTDIR)\stylelib.obj" \
	".\grove\Debug\grove.lib"

".\lib\UnicodeDebug\style.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jade - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jade\Release"
# PROP BASE Intermediate_Dir "jade\Release"
# PROP BASE Target_Dir "jade"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "jade\Release"
# PROP Intermediate_Dir "jade\Release"
# PROP Target_Dir "jade"
OUTDIR=.\jade\Release
INTDIR=.\jade\Release

ALL : "style - Win32 Release" "spgrove - Win32 Release" "grove - Win32 Release"\
 ".\bin\jade.exe"

CLEAN : 
	-@erase "$(INTDIR)\DssslApp.obj"
	-@erase "$(INTDIR)\HtmlFOTBuilder.obj"
	-@erase "$(INTDIR)\jade.obj"
	-@erase "$(INTDIR)\jade.res"
	-@erase "$(INTDIR)\RtfFOTBuilder.obj"
	-@erase "$(INTDIR)\SgmlFOTBuilder.obj"
	-@erase "$(INTDIR)\TeXFOTBuilder.obj"
	-@erase ".\bin\jade.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /I "style" /I "grove" /I "spgrove" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D "SP_MULTI_BYTE" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /I "style" /I "grove" /I\
 "spgrove" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP\
 /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/jade.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\jade\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/jade.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jade.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"bin/jade.exe"
# SUBTRACT LINK32 /profile
LINK32_FLAGS=lib\UnicodeRelease\sp110u.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/jade.pdb" /machine:I386 /out:"bin/jade.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DssslApp.obj" \
	"$(INTDIR)\HtmlFOTBuilder.obj" \
	"$(INTDIR)\jade.obj" \
	"$(INTDIR)\jade.res" \
	"$(INTDIR)\RtfFOTBuilder.obj" \
	"$(INTDIR)\SgmlFOTBuilder.obj" \
	"$(INTDIR)\TeXFOTBuilder.obj" \
	".\grove\Release\grove.lib" \
	".\spgrove\Release\spgrove.lib" \
	".\style\Release\style.lib"

".\bin\jade.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jade\Debug"
# PROP BASE Intermediate_Dir "jade\Debug"
# PROP BASE Target_Dir "jade"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "jade\Debug"
# PROP Intermediate_Dir "jade\Debug"
# PROP Target_Dir "jade"
OUTDIR=.\jade\Debug
INTDIR=.\jade\Debug

ALL : "style - Win32 Debug" "spgrove - Win32 Debug" "grove - Win32 Debug"\
 ".\lib\UnicodeDebug\jade.exe"

CLEAN : 
	-@erase "$(INTDIR)\DssslApp.obj"
	-@erase "$(INTDIR)\HtmlFOTBuilder.obj"
	-@erase "$(INTDIR)\jade.obj"
	-@erase "$(INTDIR)\jade.res"
	-@erase "$(INTDIR)\RtfFOTBuilder.obj"
	-@erase "$(INTDIR)\SgmlFOTBuilder.obj"
	-@erase "$(INTDIR)\TeXFOTBuilder.obj"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\jade.pdb"
	-@erase ".\lib\UnicodeDebug\jade.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "include" /I "style" /I "grove" /I "spgrove" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D "SP_MULTI_BYTE" /YX /c
CPP_PROJ=/nologo /MDd /W3 /GX /Zi /Od /I "include" /I "style" /I "grove" /I\
 "spgrove" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP\
 /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/jade.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\jade\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/jade.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/jade.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /debug /machine:I386 /out:"lib\UnicodeDebug\jade.exe"
LINK32_FLAGS=lib\UnicodeDebug\sp110ud.lib kernel32.lib user32.lib gdi32.lib\
 winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib\
 uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/jade.pdb" /debug /machine:I386 /out:"lib\UnicodeDebug\jade.exe"\
 
LINK32_OBJS= \
	"$(INTDIR)\DssslApp.obj" \
	"$(INTDIR)\HtmlFOTBuilder.obj" \
	"$(INTDIR)\jade.obj" \
	"$(INTDIR)\jade.res" \
	"$(INTDIR)\RtfFOTBuilder.obj" \
	"$(INTDIR)\SgmlFOTBuilder.obj" \
	"$(INTDIR)\TeXFOTBuilder.obj" \
	".\grove\Debug\grove.lib" \
	".\spgrove\Debug\spgrove.lib" \
	".\style\Debug\style.lib"

".\lib\UnicodeDebug\jade.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "jadedist - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jadedist\Release"
# PROP BASE Intermediate_Dir "jadedist\Release"
# PROP BASE Target_Dir "jadedist"
# PROP BASE Cmd_Line "NMAKE /f jadedist.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "jadedist\jadedist.exe"
# PROP BASE Bsc_Name "jadedist\jadedist.bsc"
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "jadedist\Release"
# PROP Intermediate_Dir "jadedist\Release"
# PROP Target_Dir "jadedist"
# PROP Cmd_Line "cd .. & zip  -q -@ <jadedist\files.txt jadedist\jade.zip & zip -q jadedist\jade.zip jadedist\files.txt & zip -q -j -@ <jadedist\bin-files.txt jadedist\jadew.zip"
# PROP Rebuild_Opt ""
# PROP Target_File "jadedist\jade.zip"
# PROP Bsc_Name ""
OUTDIR=.\jadedist\Release
INTDIR=.\jadedist\Release

ALL : "jade - Win32 Release" 

CLEAN : 
	-@erase 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

!ENDIF 

################################################################################
# Begin Target

# Name "grove - Win32 Release"
# Name "grove - Win32 Debug"

!IF  "$(CFG)" == "grove - Win32 Release"

!ELSEIF  "$(CFG)" == "grove - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\grove\Node.cxx

!IF  "$(CFG)" == "grove - Win32 Release"

DEP_CPP_NODE_=\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\config.h"\
	

"$(INTDIR)\Node.obj" : $(SOURCE) $(DEP_CPP_NODE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "grove - Win32 Debug"

DEP_CPP_NODE_=\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\config.h"\
	

"$(INTDIR)\Node.obj" : $(SOURCE) $(DEP_CPP_NODE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "spgrove - Win32 Release"
# Name "spgrove - Win32 Debug"

!IF  "$(CFG)" == "spgrove - Win32 Release"

!ELSEIF  "$(CFG)" == "spgrove - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\spgrove\GroveBuilder.cxx
DEP_CPP_GROVE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\spgrove\grove_inst.cxx"\
	".\spgrove\GroveBuilder.h"\
	".\spgrove\threads.h"\
	

"$(INTDIR)\GroveBuilder.obj" : $(SOURCE) $(DEP_CPP_GROVE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\spgrove\GroveApp.cxx
DEP_CPP_GROVEA=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityApp.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\EventsWanted.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\MessageBuilder.h"\
	"..\sp\include\MessageReporter.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\ParserOptions.h"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\CmdLineApp.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExtendEntityManager.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\ParserApp.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\spgrove\GroveApp.h"\
	".\spgrove\GroveBuilder.h"\
	".\spgrove\threads.h"\
	

"$(INTDIR)\GroveApp.obj" : $(SOURCE) $(DEP_CPP_GROVEA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "grove"

!IF  "$(CFG)" == "spgrove - Win32 Release"

"grove - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Release" 

!ELSEIF  "$(CFG)" == "spgrove - Win32 Debug"

"grove - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "style - Win32 Release"
# Name "style - Win32 Debug"

!IF  "$(CFG)" == "style - Win32 Release"

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

!ENDIF 

################################################################################
# Begin Project Dependency

# Project_Dep_Name "grove"

!IF  "$(CFG)" == "style - Win32 Release"

"grove - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Release" 

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

"grove - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Source File

SOURCE=.\style\stylelib.cxx
DEP_CPP_STYLE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yc"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D "WIN32"\
 /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yc"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\stylelib.obj" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\style.pch" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yc"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yc"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\stylelib.obj" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\stylelib.sbr" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\style.pch" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\StyleEngine.cxx
DEP_CPP_STYLEE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\StyleEngine.obj" : $(SOURCE) $(DEP_CPP_STYLEE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StyleEngine.obj" : $(SOURCE) $(DEP_CPP_STYLEE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\StyleEngine.sbr" : $(SOURCE) $(DEP_CPP_STYLEE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\Style.cxx
DEP_CPP_STYLE_=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\Style.obj" : $(SOURCE) $(DEP_CPP_STYLE_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Style.obj" : $(SOURCE) $(DEP_CPP_STYLE_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\Style.sbr" : $(SOURCE) $(DEP_CPP_STYLE_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\ProcessingMode.cxx
DEP_CPP_PROCE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\ProcessingMode.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\ProcessingMode.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\ProcessingMode.sbr" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\ProcessContext.cxx

!IF  "$(CFG)" == "style - Win32 Release"

DEP_CPP_PROCES=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	
NODEP_CPP_PROCES=\
	".\style\}"\
	
# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\ProcessContext.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

DEP_CPP_PROCES=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	
# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\ProcessContext.obj" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\ProcessContext.sbr" : $(SOURCE) $(DEP_CPP_PROCES) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\primitive.cxx
DEP_CPP_PRIMI=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\primitive.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\primitive.obj" : $(SOURCE) $(DEP_CPP_PRIMI) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\primitive.obj" : $(SOURCE) $(DEP_CPP_PRIMI) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\primitive.sbr" : $(SOURCE) $(DEP_CPP_PRIMI) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\NumberCache.cxx
DEP_CPP_NUMBE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\NumberCache.obj" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\NumberCache.obj" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\NumberCache.sbr" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\InterpreterMessages.cxx
DEP_CPP_INTER=\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\Vector.cxx"\
	".\include\Boolean.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Owner.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\Vector.h"\
	".\include\xnew.h"\
	".\style\InterpreterMessages.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# SUBTRACT CPP /YX

"$(INTDIR)\InterpreterMessages.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# SUBTRACT CPP /YX

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\InterpreterMessages.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\InterpreterMessages.sbr" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\Interpreter.cxx
DEP_CPP_INTERP=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\Interpreter.obj" : $(SOURCE) $(DEP_CPP_INTERP) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Interpreter.obj" : $(SOURCE) $(DEP_CPP_INTERP) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\Interpreter.sbr" : $(SOURCE) $(DEP_CPP_INTERP) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\Insn.cxx
DEP_CPP_INSN_=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\Insn.obj" : $(SOURCE) $(DEP_CPP_INSN_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Insn.obj" : $(SOURCE) $(DEP_CPP_INSN_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\Insn.sbr" : $(SOURCE) $(DEP_CPP_INSN_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\FOTBuilder.cxx
DEP_CPP_FOTBU=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\FOTBuilder.obj" : $(SOURCE) $(DEP_CPP_FOTBU) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\FOTBuilder.obj" : $(SOURCE) $(DEP_CPP_FOTBU) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\FOTBuilder.sbr" : $(SOURCE) $(DEP_CPP_FOTBU) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\Expression.cxx
DEP_CPP_EXPRE=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\Expression.obj" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Expression.obj" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\Expression.sbr" : $(SOURCE) $(DEP_CPP_EXPRE) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\ELObjMessageArg.cxx
DEP_CPP_ELOBJ=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\ELObjMessageArg.obj" : $(SOURCE) $(DEP_CPP_ELOBJ) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\ELObjMessageArg.obj" : $(SOURCE) $(DEP_CPP_ELOBJ) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\ELObjMessageArg.sbr" : $(SOURCE) $(DEP_CPP_ELOBJ) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\ELObj.cxx
DEP_CPP_ELOBJ_=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\ELObj.obj" : $(SOURCE) $(DEP_CPP_ELOBJ_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\ELObj.obj" : $(SOURCE) $(DEP_CPP_ELOBJ_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\ELObj.sbr" : $(SOURCE) $(DEP_CPP_ELOBJ_) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\DssslSpecEventHandler.cxx
DEP_CPP_DSSSL=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\InternalInputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\DssslSpecEventHandler.obj" : $(SOURCE) $(DEP_CPP_DSSSL) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\DssslSpecEventHandler.obj" : $(SOURCE) $(DEP_CPP_DSSSL) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\DssslSpecEventHandler.sbr" : $(SOURCE) $(DEP_CPP_DSSSL) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\Collector.cxx
DEP_CPP_COLLE=\
	".\include\Boolean.h"\
	".\include\config.h"\
	".\include\macros.h"\
	".\style\Collector.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# SUBTRACT CPP /YX

"$(INTDIR)\Collector.obj" : $(SOURCE) $(DEP_CPP_COLLE) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# SUBTRACT CPP /YX

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Collector.obj" : $(SOURCE) $(DEP_CPP_COLLE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Collector.sbr" : $(SOURCE) $(DEP_CPP_COLLE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\FlowObj.cxx
DEP_CPP_FLOWO=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FlowObj_inst.cxx"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\FlowObj.obj" : $(SOURCE) $(DEP_CPP_FLOWO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\FlowObj.obj" : $(SOURCE) $(DEP_CPP_FLOWO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\FlowObj.sbr" : $(SOURCE) $(DEP_CPP_FLOWO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\InheritedC.cxx
DEP_CPP_INHER=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\InheritedC.obj" : $(SOURCE) $(DEP_CPP_INHER) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\InheritedC.obj" : $(SOURCE) $(DEP_CPP_INHER) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\InheritedC.sbr" : $(SOURCE) $(DEP_CPP_INHER) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\style_inst.cxx
DEP_CPP_STYLE_I=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\style_inst.obj" : $(SOURCE) $(DEP_CPP_STYLE_I) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\style_inst.obj" : $(SOURCE) $(DEP_CPP_STYLE_I) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\style_inst.sbr" : $(SOURCE) $(DEP_CPP_STYLE_I) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\common_inst.cxx
DEP_CPP_COMMO=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\IQueue.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\ArcEngine.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\InputSource.h"\
	".\include\IQueue.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\style\Collector.h"\
	".\style\dsssl_ns.h"\
	".\style\DssslSpecEventHandler.h"\
	".\style\ELObj.h"\
	".\style\ELObjMessageArg.h"\
	".\style\EvalContext.h"\
	".\style\Expression.h"\
	".\style\FOTBuilder.h"\
	".\style\Insn.h"\
	".\style\Insn2.h"\
	".\style\Interpreter.h"\
	".\style\InterpreterMessages.h"\
	".\style\NumberCache.h"\
	".\style\ProcessContext.h"\
	".\style\ProcessingMode.h"\
	".\style\SosofoObj.h"\
	".\style\Style.h"\
	".\style\style_pch.h"\
	".\style\StyleEngine.h"\
	".\style\stylelib.h"\
	".\style\VM.h"\
	

!IF  "$(CFG)" == "style - Win32 Release"

# ADD CPP /Yu"stylelib.h"

"$(INTDIR)\common_inst.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "grove" /I "include" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# ADD CPP /Yu"stylelib.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "grove" /I "include" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D DSSSL_NAMESPACE=James_Clark_DSSSL /D\
 SP_NAMESPACE=James_Clark_SP /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /FR"$(INTDIR)/" /Fp"$(INTDIR)/style.pch" /Yu"stylelib.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\common_inst.obj" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

"$(INTDIR)\common_inst.sbr" : $(SOURCE) $(DEP_CPP_COMMO) "$(INTDIR)"\
 "$(INTDIR)\style.pch"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\InterpreterMessages.msg

!IF  "$(CFG)" == "style - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\style_inst.m4

!IF  "$(CFG)" == "style - Win32 Release"

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\FlowObj_inst.m4

!IF  "$(CFG)" == "style - Win32 Release"

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\style\common_inst.m4

!IF  "$(CFG)" == "style - Win32 Release"

!ELSEIF  "$(CFG)" == "style - Win32 Debug"

!ENDIF 

# End Source File
# End Target
################################################################################
# Begin Target

# Name "jade - Win32 Release"
# Name "jade - Win32 Debug"

!IF  "$(CFG)" == "jade - Win32 Release"

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\jade\SgmlFOTBuilder.cxx
DEP_CPP_SGMLF=\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\Vector.cxx"\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\ExternalId.h"\
	".\include\IList.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\SubstTable.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\xnew.h"\
	".\jade\SgmlFOTBuilder.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\SgmlFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_SGMLF) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\RtfFOTBuilder.cxx

!IF  "$(CFG)" == "jade - Win32 Release"

DEP_CPP_RTFFO=\
	"..\sp\include\constant.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\InputSource.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StorageManager.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\jade\RtfFOTBuilder.h"\
	".\jade\RtfFOTBuilder_inst.cxx"\
	

"$(INTDIR)\RtfFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_RTFFO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

DEP_CPP_RTFFO=\
	"..\sp\include\constant.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\ExtendEntityManager.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\InputSource.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StorageManager.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\SubstTable.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\jade\RtfFOTBuilder.h"\
	".\jade\RtfFOTBuilder_inst.cxx"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\RtfFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_RTFFO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\jade.cxx
DEP_CPP_JADE_=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityApp.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\EventsWanted.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\MessageBuilder.h"\
	"..\sp\include\MessageReporter.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\ParserOptions.h"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\CmdLineApp.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrnoMessageArg.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExtendEntityManager.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\ParserApp.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\sptchar.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\jade\DssslApp.h"\
	".\jade\HtmlFOTBuilder.h"\
	".\jade\jade_version.h"\
	".\jade\JadeMessages.h"\
	".\jade\RtfFOTBuilder.h"\
	".\jade\SgmlFOTBuilder.h"\
	".\jade\TeXFOTBuilder.h"\
	".\spgrove\GroveApp.h"\
	".\spgrove\GroveBuilder.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\jade.obj" : $(SOURCE) $(DEP_CPP_JADE_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\DssslApp.cxx
DEP_CPP_DSSSLA=\
	"..\sp\include\Allocator.h"\
	"..\sp\include\Attributed.h"\
	"..\sp\include\CharsetDecl.h"\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\constant.h"\
	"..\sp\include\ContentToken.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\ElementType.h"\
	"..\sp\include\EntityApp.h"\
	"..\sp\include\EntityCatalog.h"\
	"..\sp\include\EntityDecl.h"\
	"..\sp\include\EntityManager.h"\
	"..\sp\include\EventsWanted.h"\
	"..\sp\include\HashTable.cxx"\
	"..\sp\include\HashTableItemBase.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\ISet.cxx"\
	"..\sp\include\Lpd.h"\
	"..\sp\include\Markup.h"\
	"..\sp\include\MessageBuilder.h"\
	"..\sp\include\MessageReporter.h"\
	"..\sp\include\Mode.h"\
	"..\sp\include\NamedResource.h"\
	"..\sp\include\NamedResourceTable.h"\
	"..\sp\include\Notation.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\OwnerTable.cxx"\
	"..\sp\include\ParserOptions.h"\
	"..\sp\include\PointerTable.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\RangeMap.cxx"\
	"..\sp\include\RangeMap.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\Sd.h"\
	"..\sp\include\SdText.h"\
	"..\sp\include\ShortReferenceMap.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\UnivCharsetDesc.h"\
	"..\sp\include\Vector.cxx"\
	"..\sp\include\XcharMap.cxx"\
	".\grove\Node.h"\
	".\include\Attribute.h"\
	".\include\Boolean.h"\
	".\include\CharsetInfo.h"\
	".\include\CmdLineApp.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\Dtd.h"\
	".\include\Entity.h"\
	".\include\ErrorCountEventHandler.h"\
	".\include\Event.h"\
	".\include\ExtendEntityManager.h"\
	".\include\ExternalId.h"\
	".\include\Hash.h"\
	".\include\HashTable.h"\
	".\include\HashTableItemBase.h"\
	".\include\IList.h"\
	".\include\ISet.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Named.h"\
	".\include\NamedTable.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\OwnerTable.h"\
	".\include\ParserApp.h"\
	".\include\PointerTable.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\SgmlParser.h"\
	".\include\sptchar.h"\
	".\include\StorageManager.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Syntax.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\XcharMap.h"\
	".\include\xnew.h"\
	".\jade\DssslApp.h"\
	".\jade\DssslAppMessages.h"\
	".\spgrove\GroveApp.h"\
	".\spgrove\GroveBuilder.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	".\style\StyleEngine.h"\
	

"$(INTDIR)\DssslApp.obj" : $(SOURCE) $(DEP_CPP_DSSSLA) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\fot.dtd

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\HtmlFOTBuilder.cxx

!IF  "$(CFG)" == "jade - Win32 Release"

DEP_CPP_HTMLF=\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\MessageBuilder.h"\
	"..\sp\include\MessageReporter.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\Vector.cxx"\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\CmdLineApp.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\ErrnoMessageArg.h"\
	".\include\ExternalId.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\xnew.h"\
	".\jade\HtmlFOTBuilder.h"\
	".\jade\HtmlFOTBuilder_inst.cxx"\
	".\jade\HtmlMessages.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\HtmlFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_HTMLF) "$(INTDIR)"
   $(CPP) /nologo /MD /W3 /GX /O2 /I "include" /I "style" /I "grove" /I\
 "spgrove" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP\
 /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/jade.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

DEP_CPP_HTMLF=\
	"..\sp\include\CodingSystem.h"\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\IListIterBase.h"\
	"..\sp\include\MessageBuilder.h"\
	"..\sp\include\MessageReporter.h"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\Vector.cxx"\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\CmdLineApp.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\ErrnoMessageArg.h"\
	".\include\ExternalId.h"\
	".\include\IList.h"\
	".\include\IListIter.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\macros.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\NCVector.h"\
	".\include\OutputCharStream.h"\
	".\include\Owner.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\StringResource.h"\
	".\include\SubstTable.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\xnew.h"\
	".\jade\HtmlFOTBuilder.h"\
	".\jade\HtmlFOTBuilder_inst.cxx"\
	".\jade\HtmlMessages.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\HtmlFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_HTMLF) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /GX /Zi /Od /I "include" /I "style" /I "grove" /I\
 "spgrove" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D SP_NAMESPACE=James_Clark_SP\
 /D DSSSL_NAMESPACE=James_Clark_DSSSL /D GROVE_NAMESPACE=James_Clark_GROVE /D\
 "SP_MULTI_BYTE" /Fp"$(INTDIR)/jade.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\HtmlFOTBuilder_inst.m4

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\jade.rc
DEP_RSC_JADE_R=\
	".\jade\DssslAppMessages.rc"\
	".\jade\HtmlMessages.rc"\
	".\jade\JadeMessages.rc"\
	".\style\InterpreterMessages.rc"\
	

!IF  "$(CFG)" == "jade - Win32 Release"

# ADD BASE RSC /l 0x809 /i "jade"
# ADD RSC /l 0x809 /i "jade" /i "style"

"$(INTDIR)\jade.res" : $(SOURCE) $(DEP_RSC_JADE_R) "$(INTDIR)"
   $(RSC) /l 0x809 /fo"$(INTDIR)/jade.res" /i "jade" /i "style" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# ADD BASE RSC /l 0x809 /i "jade"
# ADD RSC /l 0x809 /i "jade" /i "style"

"$(INTDIR)\jade.res" : $(SOURCE) $(DEP_RSC_JADE_R) "$(INTDIR)"
   $(RSC) /l 0x809 /fo"$(INTDIR)/jade.res" /i "jade" /i "style" /d "_DEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\JadeMessages.msg

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\jade_version.h

!IF  "$(CFG)" == "jade - Win32 Release"

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\DssslAppMessages.msg

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\HtmlMessages.msg

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\RtfFOTBuilder_inst.m4

!IF  "$(CFG)" == "jade - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jade\TeXFOTBuilder.cxx
DEP_CPP_TEXFO=\
	"..\sp\include\CopyOwner.cxx"\
	"..\sp\include\IListBase.h"\
	"..\sp\include\Owner.cxx"\
	"..\sp\include\Ptr.cxx"\
	"..\sp\include\rtti.h"\
	"..\sp\include\StringOf.cxx"\
	"..\sp\include\SubstTable.cxx"\
	"..\sp\include\TypeId.h"\
	"..\sp\include\Vector.cxx"\
	".\grove\Node.h"\
	".\include\Boolean.h"\
	".\include\config.h"\
	".\include\CopyOwner.h"\
	".\include\ExternalId.h"\
	".\include\IList.h"\
	".\include\Link.h"\
	".\include\Location.h"\
	".\include\Message.h"\
	".\include\MessageArg.h"\
	".\include\Owner.h"\
	".\include\Ptr.h"\
	".\include\Resource.h"\
	".\include\StringC.h"\
	".\include\StringOf.h"\
	".\include\SubstTable.h"\
	".\include\Text.h"\
	".\include\Vector.h"\
	".\include\xnew.h"\
	".\jade\TeXFOTBuilder.h"\
	".\style\dsssl_ns.h"\
	".\style\FOTBuilder.h"\
	

"$(INTDIR)\TeXFOTBuilder.obj" : $(SOURCE) $(DEP_CPP_TEXFO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Project Dependency

# Project_Dep_Name "grove"

!IF  "$(CFG)" == "jade - Win32 Release"

"grove - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Release" 

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

"grove - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="grove - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "spgrove"

!IF  "$(CFG)" == "jade - Win32 Release"

"spgrove - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="spgrove - Win32 Release" 

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

"spgrove - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="spgrove - Win32 Debug" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Project Dependency

# Project_Dep_Name "style"

!IF  "$(CFG)" == "jade - Win32 Release"

"style - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="style - Win32 Release" 

!ELSEIF  "$(CFG)" == "jade - Win32 Debug"

"style - Win32 Debug" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="style - Win32 Debug" 

!ENDIF 

# End Project Dependency
# End Target
################################################################################
# Begin Target

# Name "jadedist - Win32 Release"

!IF  "$(CFG)" == "jadedist - Win32 Release"

".\jadedist\jade.zip" : 
   CD E:\users\jjc\projects\SP\jadedist
   cd .. & zip  -q -@ <jadedist\files.txt jadedist\jade.zip & zip -q\
 jadedist\jade.zip jadedist\files.txt & zip -q -j -@ <jadedist\bin-files.txt\
 jadedist\jadew.zip

!ENDIF 

################################################################################
# Begin Project Dependency

# Project_Dep_Name "jade"

!IF  "$(CFG)" == "jadedist - Win32 Release"

"jade - Win32 Release" : 
   $(MAKE) /$(MAKEFLAGS) /F ".\jade.mak" CFG="jade - Win32 Release" 

!ENDIF 

# End Project Dependency
################################################################################
# Begin Source File

SOURCE=".\jadedist\bin-files.txt"
# End Source File
################################################################################
# Begin Source File

SOURCE=.\jadedist\files.txt
# End Source File
# End Target
# End Project
################################################################################
