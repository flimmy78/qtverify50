
TEMPLATE	= lib
TARGET    = totalweight50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= TOTALWEIGHT50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS +=	-lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport

DESTDIR = $(ADEHOME)\tmp\totalweight50
#MOC_DIR = $(ADEHOME)/tmp/totalweight50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/totalweight50/obj
UI_DIR = ../include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/totalweight50.h 
	
SOURCES	+= totalweight50.cpp  

					 
FORMS	+= totalweight50.ui
     
RESOURCES += totalweight50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\totalweight50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\totalweight50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/totalweight50_zh.ts language/totalweight50_en.ts
		
