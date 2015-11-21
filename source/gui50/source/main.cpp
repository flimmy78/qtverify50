#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QDebug>
#include <QtGui/QSplashScreen>
#include <QtCore/QThread>
#include <QtGui/QDesktopWidget>
#include <QSettings>
#include <QDir>

#include "mainform.h"
#include "logger.h"
#include "logindialog.h"
#include "qtexdb.h"
#include "algorithm.h"
#include "md5encode.h"
#include "register.h"

MainForm *g_mainform;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString adehome = QProcessEnvironment::systemEnvironment().value("ADEHOME");

	//���ط����ļ�
	QString lang = "zh"; //Ĭ����ʾ����
	if (argc == 2) 
	{
		lang = QString::fromLocal8Bit(argv[1]);
	}
	QString file_name = getFullIniFileName("tr_qtverify.ini");
	QFile file(file_name );
	if( file.open(QIODevice::ReadOnly | QIODevice::Text) ) 
	{
		QTranslator *translator = NULL;
		QTextStream text(&file);
		QString line ;
		while ( !text.atEnd() ) 
		{
			line = text.readLine().simplified();
			if ( line.isEmpty() || line.startsWith("#") ) 
			{
				continue;
			}
			QString i18nName = adehome + "\\uif\\i18n\\" + lang + "\\";
			line = line + "_" + lang + ".qm";
			i18nName.append(line);//.append(QString("_%1.qm").arg(lang));
			translator = new QTranslator( 0 );
			if (!translator->load( i18nName ))
			{
				qDebug()<<"load translator file"<<line<<"failed!";
			}
			app.installTranslator( translator );
		}
		file.close();
	}
	else
	{
		qDebug("no i18n ini file.\n");
	}

	//�ж��Ƿ���Ȩ�û�
	QFile license(adehome + "\\ini\\license");
	license.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&license);
	QString regCode = in.readLine();
	license.close();
	bool licensOK = isLicenseOK(regCode);
	int validDays = 0;
	if (!licensOK) //����Ȩ�û�
	{
		//�ж��Ƿ���������
		QDateTime regDate = getProbationStartDate();
		validDays = PROBATION_DAYS - regDate.daysTo(QDateTime::currentDateTime());
		if (!regDate.isValid() || validDays <= 0) //����Ȩ�û������������ѹ���
		{
			RegisterDlg *reg = new RegisterDlg(qGetVolumeInfo());
			reg->show();
			qDebug()<<"please register first";
// 			QObject::connect(reg, SIGNAL(signalRegisterSuccess()), app, SLOT(exec()));
			return app.exec();
		}
	}

	//������ʽ��
	QFile qss(":/qtverify/qss/default.qss");
	qss.open(QFile::ReadOnly);
	app.setStyleSheet(qss.readAll());
	qss.close();

	//QSplashScreen
	Qt::Alignment align = Qt::AlignCenter | Qt::AlignBottom;
	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap(":/qtverify/images/facility.png"));
	splash->show();

	//�ַ�������
	QTextCodec *codec = QTextCodec::codecForName("system");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);

	//ע��MessageHandler
	qInstallMsgHandler(myMessageOutput);

	//��ӡ��־���ļ���
// 	qDebug()<<"This is a debug message";
// 	qWarning()<<"This is a warning message";
// 	qCritical()<<"This is a critical message";
// 	qFatal("file< %s >, line< %d >, This is a fatal message", __FILE__, __LINE__);
// 	qFatal("This is a fatal message");
	
	qDebug()<<"qtverify main thread:"<<QThread::currentThreadId();

	splash->showMessage(QObject::tr("connect database ..."), align, Qt::blue);
	wait(200);
	QString strLibPath(adehome + QDir::separator()+ "bin");   
	app.addLibraryPath(strLibPath); 
	startdb(); //�������ݿ�

	splash->showMessage(QObject::tr("setting up the mainwindow ..."), align, Qt::blue);
	wait(200);
	g_mainform = new MainForm(licensOK, validDays);
	
// 	LoginDialog login;
// 	if (login.exec() == QDialog::Accepted)
// 	{
		g_mainform->resize(1280, 800);  
		g_mainform->move((app.desktop()->width()-g_mainform->width())/2, (app.desktop()->height()-g_mainform->height())/2);  
		g_mainform->showMaximized();
		splash->finish(g_mainform);
		delete splash;
		app.exec();
// 	}

	closedb();
	return 0;
}
