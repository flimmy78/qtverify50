
TEMPLATE	= lib
TARGET    = flowweight50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= FLOWWEIGHT50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS += -lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport -lqwtd

DESTDIR = $(ADEHOME)\tmp\flowweight50
#MOC_DIR = $(ADEHOME)/tmp/flowweight50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/flowweight50/obj
UI_DIR = ../include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/flowweight50.h 
	
SOURCES	+= flowweight50.cpp  

					 
FORMS	+= flowweight50.ui
     
RESOURCES += flowweight50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\flowweight50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\flowweight50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/flowweight50_zh.ts language/flowweight50_en.ts
		
