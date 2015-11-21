/***********************************************
**  �ļ���:     main.cpp
**  ����:       �ɼ�����Ʋ���-������
								���DN50װ��
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2015/11/21
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2015/11 ��һ��
**  ���ݰ���:
**  ˵��:
**  ���¼�¼:
***********************************************/

#include <QtGui/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QtCore/QTextCodec>

#include "qtexdb.h"
#include "logger.h"
#include "datatestdlg50.h"

int main( int argc, char ** argv )
{
	QApplication app( argc, argv );
	QString lang = "zh";//Ĭ����ʾ����
	if (argc == 2)
	{
		lang = QString::fromAscii(argv[1]);
	}

	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName("GB2312"));

	//ע��MessageHandler
	qInstallMsgHandler(myMessageOutput);

	QTranslator translator(0);
	QString adehome = QProcessEnvironment::systemEnvironment().value("ADEHOME");
	if (!adehome.isEmpty()) 
	{
		QString filename = adehome + "\\uif\\i18n\\" + lang + "\\datatestdlg50_" + lang + ".qm";
		bool loadok = translator.load(filename, "");
		if (!loadok)
		{
			qDebug()<<"load translator file \""<<filename<<"\" failed!";
		}
		app.installTranslator(&translator);
	}
	qDebug()<<"datatest50 main thread:"<<QThread::currentThreadId();
// 	qApp->setStyleSheet("QLineEdit { background-color: yellow }");
	startdb();

	DataTestDlg50 w;
	w.show();
	app.exec();

	closedb();
	return 0;
}