/***********************************************
**  �ļ���:     mainform.cpp
**  ����:       �춨̨��λ����������
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2015/11/21
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2015/11 ��һ��
**  ���ݰ���:
**  ˵��:		���˫��ƽ���Ƶļ춨̨
**  ���¼�¼:   
***********************************************/

#include <QtGui/QMessageBox>
#include <QDesktopWidget>
#include <QAxObject>
#include <QProcess>
#include <QtCore>

#include "mainform.h"
#include "setcomdlg.h"
#include "datatestdlg50.h"
#include "setportfrm.h"
#include "masterslaveset.h"
#include "algorithm.h"
#include "flowweight50.h"
// #include "flowstandard.h"
// #include "totalweight.h"
// #include "totalstandard.h"
#include "calcverify.h"
#include "cmbverify.h"
#include "stdmtrparaset.h"
#include "stdmtrcoecorrect.h"
#include "tvercomp.h"
#include "tverparam.h"
#include "stdplasensor.h"
#include "chkplasensor.h"
#include "flow_result.h"
#include "platinum_result.h"
#include "calculator_result.h"
#include "cmb_result.h"
#include "total_result.h"
#include "scancodedlg.h"
#include "register.h"
#include "md5encode.h"
#include "adjustratedlg.h"
#include "usermanagedlg.h"
#include "logindialog.h"

MainForm::MainForm(bool licenseOK, int validDays, QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	qDebug()<<"MainForm thread:"<<QThread::currentThreadId();

	ui.setupUi(this);
	QString adehome = QProcessEnvironment::systemEnvironment().value("ADEHOME");
	QString logofile = adehome.replace("\\", "\/") + "\/uif\/pixmap\/adelogo.png";
	ui.label->setStyleSheet(QString::fromUtf8("border-image: url(%1);").arg(logofile));

	m_flowResultDlg = NULL;
	m_alg = new CAlgorithm();

	m_userManageDlg = NULL;
	m_registerDlg = NULL;
	m_scanCodeDlg = NULL;
	m_setcom = NULL;
	m_datatestdlg = NULL;
	m_adjustRateDlg = NULL;
	m_portSet = NULL;
	m_masterslave = NULL;
 	m_flowWeightDlg = NULL;
// 	m_flowStandardDlg = NULL;
// 	m_totalWeightDlg = NULL;
// 	m_totalStandardDlg = NULL;
	m_calcDlg = NULL;  
	m_cmbVerifyDlg = NULL;
	m_tvercompDlg = NULL;
	m_tverparaDlg = NULL;
	m_stdParaSet = NULL;
	m_stdCoeCorrect = NULL;
	m_stdPtParaDlg = NULL;
	m_chkPtParaDlg = NULL;
	m_PlaResultDlg = NULL;
	m_CalcResultDlg = NULL;
	m_CmbResultDlg = NULL;
	m_TotalResultDlg = NULL;

	m_comProcess = new QProcess(this);
	QObject::connect(m_comProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

	QLabel *permanent = new QLabel(this);
	permanent->setFrameStyle(QFrame::NoFrame | QFrame::Sunken);
	permanent->setText(tr("<a href=\"http://www.sdm.com.cn\">Yantai Aerospace Delu Energy-saving technology Co.,Ltd</a>"));
	permanent->setTextFormat(Qt::RichText);
	permanent->setOpenExternalLinks(true);
	ui.statusBar->addPermanentWidget(permanent);

	m_probationinfo = new QLabel(this);
	m_probationinfo->setFrameStyle(QFrame::NoFrame | QFrame::Sunken);
	m_probationinfo->setAlignment(Qt::AlignLeft);
	if (licenseOK) //��ʽ��
	{
		m_probationinfo->setText(tr("Official Version"));
	}
	else //���ð�
	{
		m_probationinfo->setText(tr("Trial Version! Probation period is %1 days").arg(validDays));
	}
	m_probationinfo->setTextFormat(Qt::RichText);
	m_probationinfo->setOpenExternalLinks(true);
	ui.statusBar->addPermanentWidget(m_probationinfo);

}

MainForm::~MainForm()
{
}

void MainForm::closeEvent( QCloseEvent * event)
{
	int button = QMessageBox::question(this, tr("Question"), tr("Quit Really ?"), \
		QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	if (button == QMessageBox::No)
	{
		event->ignore();
	}
	else if (button == QMessageBox::Yes)
	{
		event->accept();
		qDebug()<<"^^^^^MainForm::closeEvent";

		if (m_flowResultDlg)
		{
			delete m_flowResultDlg;
			m_flowResultDlg = NULL;
		}

		if (m_alg)
		{
			delete m_alg;
			m_alg = NULL;
		}

		if (m_userManageDlg)
		{
			delete m_userManageDlg;
			m_userManageDlg = NULL;
		}

		if (m_registerDlg)
		{
			delete m_registerDlg;
			m_registerDlg = NULL;
		}

		if (m_scanCodeDlg)
		{
			delete m_scanCodeDlg;
			m_scanCodeDlg = NULL;
		}

		if (m_setcom)
		{
			delete m_setcom;
			m_setcom = NULL;
		}

		if (m_portSet)
		{
			delete m_portSet;
			m_portSet = NULL;
		}

		if (m_datatestdlg)
		{
			delete m_datatestdlg;
			m_datatestdlg = NULL;
		}

		if (m_adjustRateDlg)
		{
			delete m_adjustRateDlg;
			m_adjustRateDlg = NULL;
		}

		if (m_masterslave)
		{
			delete m_masterslave;
			m_masterslave = NULL;
		}

		if (m_comProcess)
		{
			m_comProcess->close();
			delete m_comProcess;
			m_comProcess = NULL;
		}

		if (m_flowWeightDlg)
		{
			delete m_flowWeightDlg;
			m_flowWeightDlg = NULL;
		}

// 		if (m_flowStandardDlg)
// 		{
// 			delete m_flowStandardDlg;
// 			m_flowStandardDlg = NULL;
// 		}
// 
// 		if (m_totalWeightDlg)
// 		{
// 			delete m_totalWeightDlg;
// 			m_totalWeightDlg = NULL;
// 		}
// 
// 		if (m_totalStandardDlg)
// 		{
// 			delete m_totalStandardDlg;
// 			m_totalStandardDlg = NULL;
// 		}

		if (m_calcDlg)
		{
			delete m_calcDlg;
			m_calcDlg = NULL;
		}

		if (m_cmbVerifyDlg)
		{
			delete m_cmbVerifyDlg;
			m_cmbVerifyDlg = NULL;
		}

		if (m_tvercompDlg)
		{
			delete m_tvercompDlg;
			m_tvercompDlg = NULL;
		}	

		if (m_tverparaDlg)
		{
			delete m_tverparaDlg;
			m_tverparaDlg = NULL;
		}	

		if (m_stdParaSet)
		{
			delete m_stdParaSet;
			m_stdParaSet = NULL;
		}

		if (m_stdCoeCorrect)
		{
			delete m_stdCoeCorrect;
			m_stdCoeCorrect = NULL;
		}

		if (m_stdPtParaDlg)
		{
			delete m_stdPtParaDlg;
			m_stdPtParaDlg = NULL;
		}

		if (m_chkPtParaDlg)
		{
			delete m_chkPtParaDlg;
			m_chkPtParaDlg = NULL;
		}

		if (m_PlaResultDlg)
		{
			delete m_PlaResultDlg;
			m_PlaResultDlg = NULL;
		}

		if (m_CalcResultDlg)
		{
			delete m_CalcResultDlg;
			m_CalcResultDlg = NULL;
		}

		if (m_CmbResultDlg)
		{
			delete m_CmbResultDlg;
			m_CmbResultDlg = NULL;
		}

		if (m_TotalResultDlg)
		{
			delete m_TotalResultDlg;
			m_TotalResultDlg = NULL;
		}
	}
}

void MainForm::on_actionScanCode_triggered()
{
	if (NULL == m_scanCodeDlg)
	{
		m_scanCodeDlg = new ScanCodeDlg();
	}
	else //Ŀ����ִ��ScanCodeDlg�Ĺ��캯��
	{
		delete m_scanCodeDlg;
		m_scanCodeDlg = NULL;
		m_scanCodeDlg = new ScanCodeDlg();
	}
	m_scanCodeDlg->show();
}

void MainForm::on_actionComSet_triggered()
{
	if (NULL == m_setcom)
	{
		m_setcom = new SetComDlg();
	}
	m_setcom->show();
}

void MainForm::on_actionPortSet_triggered()
{
	if (NULL == m_portSet)
	{
		m_portSet = new SetPortFrm();
	}
	m_portSet->show();
}

//�ɼ�����Ʋ��Գ���
void MainForm::on_actionDataTest_triggered()
{
	if (NULL == m_datatestdlg)
	{
		m_datatestdlg = new DataTestDlg50();
	}
	else //Ŀ����ִ��DataTestDlg�Ĺ��캯��
	{
		delete m_datatestdlg;
		m_datatestdlg = NULL;
		m_datatestdlg = new DataTestDlg50();
	}
	int ah = QApplication::desktop()->availableGeometry().height();
	int aw = QApplication::desktop()->availableGeometry().width();
// 	int sh = QApplication::desktop()->screenGeometry().height();
// 	int sw = QApplication::desktop()->screenGeometry().width();
// 	m_datatestdlg->resize(800, 800);
	int wh = m_datatestdlg->height();
	int ww = m_datatestdlg->width();
	int x = (aw - ww)/2;
	int y = (ah - wh - 100)/2;
	m_datatestdlg->move(x, y);  
//	m_datatestdlg->move((QApplication::desktop()->width()-m_datatestdlg->width())/2, (QApplication::desktop()->height()-m_datatestdlg->height())/2);  
	m_datatestdlg->show();
}

void MainForm::on_actionAdjustFlowRate_triggered()
{
	if (NULL == m_adjustRateDlg)
	{
		m_adjustRateDlg = new AdjustRateDlg();
	}
	m_adjustRateDlg->show();
}

//���ô��ڵ��Թ���
void MainForm::on_actionComDebuger_triggered()
{
	QStringList cmdlist;
	cmdlist<<"zh";
	m_comProcess->start("delucom", cmdlist);
}

//��׼������趨
void MainForm::on_actionStdMtrParaSet_triggered()
{
	if (NULL == m_stdParaSet)
	{
		m_stdParaSet = new StdMtrParaSet();
	}
	else //Ŀ����ִ��StdParaSet�Ĺ��캯��
	{
		delete m_stdParaSet;
		m_stdParaSet = NULL;
		m_stdParaSet = new StdMtrParaSet();
	}
	m_stdParaSet->move((QApplication::desktop()->availableGeometry().width()-m_stdParaSet->width())/2, \
		(QApplication::desktop()->availableGeometry().height()-m_stdParaSet->height()-100)/2);  
	m_stdParaSet->show();
}

//��׼��ϵ������
void MainForm::on_actionStdMtrCoeCorrect_triggered()
{
	if (NULL == m_stdCoeCorrect)
	{
		m_stdCoeCorrect = new StdMtrCoeCorrect();
	}
	else //Ŀ����ִ��StdCoeCorrect�Ĺ��캯��
	{
		delete m_stdCoeCorrect;
		m_stdCoeCorrect = NULL;
		m_stdCoeCorrect = new StdMtrCoeCorrect();
	}
	m_stdCoeCorrect->show();
}

//��׼����������趨
void MainForm::on_actionStdPtParaSet_triggered()
{
	if (NULL == m_stdPtParaDlg)
	{
		m_stdPtParaDlg = new stdplasensorDlg();
	}
	else //Ŀ����ִ��stdplasensorDlg�Ĺ��캯��
	{
		delete m_stdPtParaDlg;
		m_stdPtParaDlg = NULL;
		m_stdPtParaDlg = new stdplasensorDlg();
	}
	m_stdPtParaDlg->show();
}

//���첬��������趨
void MainForm::on_actionPtParaSet_triggered()
{
	if (NULL == m_chkPtParaDlg)
	{
		m_chkPtParaDlg = new chkplasensorDlg();
	}
	else //Ŀ����ִ��chkplasensorDlg�Ĺ��캯��
	{
		delete m_chkPtParaDlg;
		m_chkPtParaDlg = NULL;
		m_chkPtParaDlg = new chkplasensorDlg();
	}
	m_chkPtParaDlg->show();
}

//����-�ӻ�����
void MainForm::on_actionMasterSlaveSet_triggered()
{
	if (NULL == m_masterslave)
	{
		m_masterslave = new CMasterSlave();
	}
	m_masterslave->show();
}

//�����춨(������)
void MainForm::on_actionFlowWeight_triggered()
{
	if (NULL == m_flowWeightDlg)
	{
		m_flowWeightDlg = new FlowWeightDlg50();
	}
	else //Ŀ����ִ��FlowWeightDlg50�Ĺ��캯��
	{
		delete m_flowWeightDlg;
		m_flowWeightDlg = NULL;
		m_flowWeightDlg = new FlowWeightDlg50();
	}
	m_flowWeightDlg->showMaximized();
}
/*
//�����춨(��׼��)
void MainForm::on_actionFlowStandard_triggered()
{
	if (NULL == m_flowStandardDlg)
	{
		m_flowStandardDlg = new FlowStandardDlg();
	}
	else //Ŀ����ִ��FlowStandardDlg�Ĺ��캯��
	{
		delete m_flowStandardDlg;
		m_flowStandardDlg = NULL;
		m_flowStandardDlg = new FlowStandardDlg();
	}
	m_flowStandardDlg->showMaximized();
}*/

//������춨(�ȽϷ�)
void MainForm::on_actionPtCompare_triggered()
{
	if (NULL == m_tvercompDlg)
	{
		m_tvercompDlg = new tvercompDlg();
	}
	else //Ŀ����ִ��tvercompDlg�Ĺ��캯��
	{
		delete m_tvercompDlg;
		m_tvercompDlg = NULL;
		m_tvercompDlg = new tvercompDlg();
	}
	m_tvercompDlg->show();
}

//������춨(������)
void MainForm::on_actionPtPara_triggered()
{
	if (NULL == m_tverparaDlg)
	{
		m_tverparaDlg = new tverparamDlg();
	}
	else //Ŀ����ִ��tverparamDlg�Ĺ��캯��
	{
		delete m_tverparaDlg;
		m_tverparaDlg = NULL;
		m_tverparaDlg = new tverparamDlg();
	}
	m_tverparaDlg->show();
}

//�������춨
void MainForm::on_actionCalculator_triggered()
{
	if (NULL == m_calcDlg)
	{
		m_calcDlg = new CalcDlg();
	}
	else //Ŀ����ִ��CalcDlg�Ĺ��캯��
	{
		delete m_calcDlg;
		m_calcDlg = NULL;
		m_calcDlg = new CalcDlg();
	}
	m_calcDlg->show();
}

//�¶�/��������ϼ춨
void MainForm::on_actionCombine_triggered()
{
	if (NULL == m_cmbVerifyDlg)
	{
		m_cmbVerifyDlg = new CmbVerifyDlg();
	}
	else //Ŀ����ִ��CalcDlg�Ĺ��캯��
	{
		delete m_cmbVerifyDlg;
		m_cmbVerifyDlg = NULL;
		m_cmbVerifyDlg = new CmbVerifyDlg();
	}
	m_cmbVerifyDlg->showMaximized();
}
/*
//�����춨����������
void MainForm::on_actionTotalWeight_triggered()
{
	if (NULL == m_totalWeightDlg)
	{
		m_totalWeightDlg = new TotalWeightDlg();
	}
	else //Ŀ����ִ��TotalWeightDlg�Ĺ��캯��
	{
		delete m_totalWeightDlg;
		m_totalWeightDlg = NULL;
		m_totalWeightDlg = new TotalWeightDlg();
	}
	m_totalWeightDlg->showMaximized();
}

//�����춨����׼����
void MainForm::on_actionTotalStandard_triggered()
{
	if (NULL == m_totalStandardDlg)
	{
		m_totalStandardDlg = new TotalStandardDlg();
	}
	else //Ŀ����ִ��TotalStandardDlg�Ĺ��캯��
	{
		delete m_totalStandardDlg;
		m_totalStandardDlg = NULL;
		m_totalStandardDlg = new TotalStandardDlg();
	}
	m_totalStandardDlg->showMaximized();
}*/

//��ѯ�����춨����������������ͱ�׼����
void MainForm::on_actionFlowResult_triggered()
{
	if (NULL != m_flowResultDlg)
	{	
		delete m_flowResultDlg;
		m_flowResultDlg = NULL;
	}
	m_flowResultDlg = new FlowResultDlg();
	m_flowResultDlg->show();
}

//��ѯ�����춨����������������ͱ�׼����
void MainForm::on_actionTotalResult_triggered()
{
	if (NULL != m_TotalResultDlg)
	{	
		delete m_TotalResultDlg;
		m_TotalResultDlg = NULL;
	}
	m_TotalResultDlg = new TotalResultDlg();
	m_TotalResultDlg->show();
}

//��ѯ�¶ȴ������춨���
void MainForm::on_actionPtResult_triggered()
{
	if (NULL != m_PlaResultDlg)
	{	
		delete m_PlaResultDlg;
		m_PlaResultDlg = NULL;
	}
	m_PlaResultDlg = new PlaResultDlg();
	m_PlaResultDlg->show();
}

//��ѯ�������춨���
void MainForm::on_actionCalculatorResult_triggered()
{
	if (NULL != m_CalcResultDlg)
	{	
		delete m_CalcResultDlg;
		m_CalcResultDlg = NULL;
	}
	m_CalcResultDlg = new CalcResultDlg();
	m_CalcResultDlg->show();
}

//��ѯ�¶�/��������ϼ춨���
void MainForm::on_actionCombineResult_triggered()
{
	if (NULL != m_CmbResultDlg)
	{	
		delete m_CmbResultDlg;
		m_CmbResultDlg = NULL;
	}
	m_CmbResultDlg = new CmbResultDlg();
	m_CmbResultDlg->show();
}

void MainForm::on_actionQueryExcel_triggered()
{
	QAxObject *excel = NULL;
	excel = new QAxObject("Excel.Application");
	if (!excel)
	{ 
		QMessageBox::critical(this, tr("Error"), tr("Excel object lose!"));
		return;
	}
	QAxObject *workbooks = NULL;
	QAxObject *workbook = NULL;
	excel->dynamicCall("SetVisible(bool)", false);
	workbooks = excel->querySubObject("WorkBooks");
	QString xlsFile = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\dat\\test.xlsx";
	workbook = workbooks->querySubObject("Open(QString, QVariant)", xlsFile);
	if (NULL==workbook)
	{
		return;
	}
	QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//�򿪵�һ��sheet
	QAxObject * usedrange = worksheet->querySubObject("UsedRange");//��ȡ��sheet��ʹ�÷�Χ����
	QAxObject * rows = usedrange->querySubObject("Rows");
	QAxObject * columns = usedrange->querySubObject("Columns");
	int intRowStart = usedrange->property("Row").toInt();
	int intColStart = usedrange->property("Column").toInt();
	int intCols = columns->property("Count").toInt();
	int intRows = rows->property("Count").toInt();

	for (int i = intRowStart; i < intRowStart + intRows; i++) //�� 
	{
		for (int j = intColStart; j < intColStart + intCols; j++) //�� 
		{
			QAxObject * range = worksheet->querySubObject("Cells(int,int)", i, j ); //��ȡ��Ԫ��  
			qDebug("row %d, col %d, value is %d\n", i, j, range->property("Value").toInt()); //************������!!!!!! 
		} 
	}
}

//�˳�
void MainForm::on_actionExit_triggered()
{
	this->close();
}

void MainForm::on_actionUserManage_triggered()
{
	LoginDialog login;
	if (login.exec() != QDialog::Accepted)
	{
		return;
	}
	if (login.getCurRoleID() != 0) //�ǳ����û�
	{
		QMessageBox::warning(this, tr("Error"), tr("No right for user manage!"));
		return;
	}
	if (NULL == m_userManageDlg)
	{
		m_userManageDlg = new UserManageDlg();
	}
	else //Ŀ����ִ��UserManageDlg�Ĺ��캯��
	{
		delete m_userManageDlg;
		m_userManageDlg = NULL;
		m_userManageDlg = new UserManageDlg();
	}
	m_userManageDlg->show();
}

void MainForm::on_actionAbout_triggered()
{
	QMessageBox::aboutQt(this);
}

void MainForm::on_actionRegister_triggered()
{
	if (NULL == m_registerDlg)
	{
		m_registerDlg = new RegisterDlg(qGetVolumeInfo());
	}
	else //Ŀ����ִ��RegisterDlg�Ĺ��캯��
	{
		delete m_registerDlg;
		m_registerDlg = NULL;
		m_registerDlg = new RegisterDlg(qGetVolumeInfo());
	}
	connect(m_registerDlg, SIGNAL(signalRegisterSuccess()), this, SLOT(slotRegisterSuccess()));
	m_registerDlg->show();
}

//��ʾ���
void MainForm::on_actionDefault_triggered()
{
	QFile qss(":/qtverify/qss/default.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm::on_actionClassic_triggered()
{
	QFile qss(":/qtverify/qss/classic.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm::on_actionFashion_triggered()
{
	QFile qss(":/qtverify/qss/fashion.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm::chaneLanguage(QString lang)
{
	QString file_name = getFullIniFileName("tr_qtverify.ini");
	QFile file(file_name );
	if( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) 
	{
		qDebug("no i18n ini file.\n");
		return;
	}
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
		QString i18nName = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\uif\\i18n\\" + lang + "\\";
		line = line + "_" + lang + ".qm";
		i18nName.append(line);//.append(QString("_%1.qm").arg(lang));
		translator = new QTranslator( 0 );
		if (!translator->load( i18nName ))
		{
			qDebug()<<"load translator file"<<line<<"failed!";
			continue;
		}
		qApp->installTranslator( translator );
		ui.retranslateUi(this);
	}
	file.close();
	delete []translator;
}

void MainForm::on_actionEnglish_triggered()
{
	chaneLanguage("en");
}

void MainForm::on_actionChinese_triggered()
{
	chaneLanguage("zh");
}

void MainForm::processError(QProcess::ProcessError error)
{
	switch(error)
	{
	case QProcess::FailedToStart:
		QMessageBox::information(0, tr("Hint"), tr("Failed To Start"));
		break;
// 	case QProcess::Crashed:
// 		QMessageBox::information(0, tr("Hint"), tr("Crashed"));
// 		break;
	case QProcess::Timedout:
		QMessageBox::information(0, tr("Hint"), tr("Timed Out"));
		break;
	case QProcess::WriteError:
		QMessageBox::information(0, tr("Hint"), tr("Write Error"));
		break;
	case QProcess::ReadError:
		QMessageBox::information(0, tr("Hint"), tr("Read Error"));
		break;
	case QProcess::UnknownError:
		QMessageBox::information(0, tr("Hint"), tr("Unknown Error"));
		break;
	default:
// 		QMessageBox::information(0, tr("default"), tr("default"));
		break;
	}
}

void MainForm::slotRegisterSuccess()
{
	qDebug()<<ui.statusBar->currentMessage();
	m_probationinfo->setText(tr("Official Version"));
}