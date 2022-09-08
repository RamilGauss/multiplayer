TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= qt warn_on

INCLUDEPATH += /usr/lib/glib-2.0/include \
	       /usr/include/glib-2.0

HEADERS += ArrUIDobject.h \
  AutoCreateVar.h \
  BL_AppFile.h \
  BL_ConfigFile.h \
  BL_Conv.h \
  BL_CycleBuffer.h \
  BL_Debug.h \
  BL_Log.h \
  BL_New.h \
  BL_Thread.h \
  BL_Time.h \
  BL_IOStream.h \
  BL_IOStreamFile.h \
  BL_IOStreamMem.h \
  ErrorReg.h \
  FileError.h \
  FileShare.h \
  GCS.h \
  GTime.h \
  hArray.h \
  HiTimer.h \
  IImageNotify.h \
  ILogConsole.h \
  ImageFile.h \
  List_DualLink.h \
  List_DualLink_p.h \
  List_SingleLink.h \
  List_SingleLink_p.h \
  ListMem_SingleLink.h \
  ListMem_SingleLink_p.h \
  LogFile.h \
  LogManager.h \
  LogStd.h \
  RWLock.h \
  TimerLog.h \
  TObject.h \
  ToStr.h \
  TypeDef.h \
  UIDobject.h \
  WatchValue.h 

SOURCES		= \
	ArrUIDobject.cpp \
	BL_AppFile.cpp \
	BL_ConfigFile.cpp \
	BL_Conv.cpp \
	BL_Debug.cpp \
	BL_Time.cpp \
  BL_IOStream.cpp \
  BL_IOStreamFile.cpp \
  BL_IOStreamMem.cpp \
	BL_Log.cpp \
	ErrNotify_File.cpp \
	ErrorDesc.cpp \
	ErrorReg.cpp \
	errSTD.cpp \
	errSND.cpp \
	errStr.cpp \
	FileShare.cpp \
 	FileError.cpp \
	GCS.cpp \
	hArray.cpp \
	HiTimer.cpp \
	ILogConsole.cpp \
	ImageFile.cpp \
	LogFile.cpp \
	LogStd.cpp \
	LogManager.cpp \
	ToStr.cpp \
	UIDobject.cpp \
 	BL_Thread.cpp \
	WatchValue.cpp \
  TimerLog.cpp 
  

DEFINES += TD_CROSSPLATFORM

unix {    
  OBJECTS_DIR = ../Temp/GBaseLib
  INCLUDEPATH += /usr/lib/glib-2.0/include
  INCLUDEPATH += /usr/include/glib-2.0	  
	LIBS += -lasound
}

TYPE_CONFIG = $$[typeConfig]

contains( TYPE_CONFIG, Debug ) {
message( "GBaselib - debug" )
  CONFIG	+= debug
  DEFINES += _DEBUG 
  TARGET = /usr/lib/VKBR/GBaseLibd
}

contains( TYPE_CONFIG, Release ) {
message( "GBaselib - release" )
CONFIG	+= release
TARGET = /usr/lib/VKBR/GBaseLib
}
