TEMPLATE	= lib
TARGET    = waterweight50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= WATERWEIGHT50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS += -lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport -lqwtd -lreadstdmeter

DESTDIR = $(ADEHOME)\tmp\waterweight50
#MOC_DIR = $(ADEHOME)/tmp/waterweight50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/waterweight50/obj
UI_DIR = ../include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/waterweight50.h 
	
SOURCES	+= waterweight50.cpp  

					 
FORMS	+= waterweight50.ui
     
RESOURCES += waterweight50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\waterweight50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\waterweight50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/waterweight50_zh.ts language/waterweight50_en.ts
		
