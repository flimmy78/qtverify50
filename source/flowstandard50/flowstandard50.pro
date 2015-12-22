TEMPLATE	= lib
TARGET    = flowstandard50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= FLOWSTANDARD50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS +=	-lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport -lreadstdmeter

DESTDIR = $(ADEHOME)\tmp\flowstandard50
#MOC_DIR = $(ADEHOME)/tmp/flowstandard50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/flowstandard50/obj
UI_DIR = ../include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/flowstandard50.h 
	
SOURCES	+= flowstandard50.cpp  

					 
FORMS	+= flowstandard50.ui
     
RESOURCES += flowstandard50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\flowstandard50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\flowstandard50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/flowstandard50_zh.ts language/flowstandard50_en.ts
		
