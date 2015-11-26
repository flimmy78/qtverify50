TEMPLATE	= app
TARGET    = datatest50

CONFIG += qt warn_on debug
RC_FILE = datatest50.rc

INCLUDEPATH	 +=   ../include	\
									$$(ADEHOME_INC)/include  \
                 	$$(ADEHOME_INC)/include/qextserial

QMAKE_LIBDIR += \
              $$(ADEHOME)/lib \
              $$(ADEHOME)/bin
           
LIBS	+= -ldatatestdlg50 -lqtexdb

SOURCES	+= main.cpp

win32:debug{
	CONFIG += console
}

win32{
DEFINES += WIN32 _AFXDLL
DEFINES -= _USRDLL
DESTDIR = $(ADEHOME)\tmp\datatest50\obj
}

MOC_DIR = $(ADEHOME)/tmp/datatest50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/datatest50/obj
UI_DIR = $(ADEHOME)/tmp/datatest50/ui


win32{
	MY_DEST_EXE_VAR = $${DESTDIR} $${TARGET}.exe
	MY_DEST_EXE = $$join( MY_DEST_EXE_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_EXE} $$(ADEHOME)\bin
}


