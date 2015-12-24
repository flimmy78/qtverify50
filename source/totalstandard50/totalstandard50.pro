
TEMPLATE	= lib
TARGET    = totalstandard50
CONFIG += qt dll debug thread
QT += sql

DEFINES	+= TOTALSTANDARD50_DLL QT_THREAD_SUPPORT

QMAKE_LIBDIR +=  ./           \
        	     $(ADEHOME)/lib \
	             $(ADEHOME)/bin 
	            
LIBS +=	-lprotocol -lsystemsetdlg -lcomobject -lqtexdb -lalgorithm -lreport -lqwtd -lreadstdmeter

DESTDIR = $(ADEHOME)\tmp\totalstandard50
#MOC_DIR = $(ADEHOME)/tmp/totalstandard50/moc
OBJECTS_DIR = $(ADEHOME)/tmp/totalstandard50/obj
UI_DIR = $(ADEHOME_INC)/include

INCLUDEPATH  	=    ./      \
									 ../include \ 
									 $$(ADEHOME_INC)/include	\
									 $$(ADEHOME_INC)/include/qextserial

HEADERS	+= ../include/totalstandard50.h 
	
SOURCES	+= totalstandard50.cpp  

					 
FORMS	+= totalstandard50.ui
     
RESOURCES += totalstandard50.qrc     
         
win32{
	MY_DEST_LIB_VAR = $${DESTDIR} $${TARGET}.lib
	MY_DEST_LIB = $$join( MY_DEST_LIB_VAR, "\\" )
	MY_DEST_DLL_VAR = $${DESTDIR} $${TARGET}.dll
	MY_DEST_DLL = $$join( MY_DEST_DLL_VAR, "\\" )

	QMAKE_POST_LINK = copy $${MY_DEST_LIB} $$(ADEHOME)\lib \
                  & copy $${MY_DEST_DLL} $$(ADEHOME)\dll \
                  & copy .\language\totalstandard50_zh.qm $(ADEHOME)\uif\i18n\zh \
                  & copy .\language\totalstandard50_en.qm $(ADEHOME)\uif\i18n\en 

}

TRANSLATIONS =  \
		language/totalstandard50_zh.ts language/totalstandard50_en.ts
		
