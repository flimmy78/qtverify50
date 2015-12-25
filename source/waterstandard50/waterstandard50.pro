
TEMPLATE	= lib
TARGET    = waterstandard50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= WATERSTANDARD50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS +=	-lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport -lreadstdmeter \
				-lqwtd

DESTDIR = $(ADEHOME)\tmp\waterstandard50
#MOC_DIR = $(ADEHOME)/tmp/waterstandard50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/waterstandard50/obj
UI_DIR = ../include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/waterstandard50.h 
	
SOURCES	+= waterstandard50.cpp  

					 
FORMS	+= waterstandard50.ui
     
RESOURCES += waterstandard50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\waterstandard50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\waterstandard50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/waterstandard50_zh.ts language/waterstandard50_en.ts
		
