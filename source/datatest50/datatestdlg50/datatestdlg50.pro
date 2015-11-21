
TEMPLATE	= lib
TARGET    = datatestdlg50
CONFIG += qt dll debug thread

DEFINES	+= DATATESTDLG_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS += -lsystemsetdlg -lprotocol -lcomobject -lalgorithm -lqtexdb

DESTDIR = $(ADEHOME)\tmp\datatest50\datatestdlg50\obj
#MOC_DIR = $(ADEHOME)/tmp/datatest50/datatestdlg50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/datatest50/datatestdlg50/obj
UI_DIR = $(ADEHOME_INC)/include

INCLUDEPATH  	=    ./      \
									 include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= $$(ADEHOME_INC)/include/datatestdlg50.h \
	
SOURCES	+= source/datatestdlg50.cpp
			 
FORMS	+= ui/datatestdlg50.ui
     
RESOURCES += datatestdlg50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\datatestdlg50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\datatestdlg50_en.qm $(ADEHOME)\uif\i18n\en 
}

TRANSLATIONS =  \
		language/datatestdlg50_zh.ts language/datatestdlg50_en.ts
		
