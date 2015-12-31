/***********************************************
**  文件名:     mainform.cpp
**  功能:       检定台上位机主界面框架
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2015/11/21
**  专业组:     德鲁计量软件组
**  程序设计者: YS
**  程序员:     YS
**  版本历史:   2015/11 第一版
**  内容包含:
**  说明:		针对双天平配制的检定台
**  更新记录:   
***********************************************/

#include <QtGui/QMessageBox>
#include <QDesktopWidget>
#include <QAxObject>
#include <QProcess>
#include <QtCore>

#include "mainform50.h"
#include "setcomdlg.h"
#include "datatestdlg50.h"
#include "setportfrm.h"
#include "masterslaveset.h"
#include "algorithm.h"
#include "flowweight50.h"
#include "flowstandard50.h"
#include "totalweight50.h"
#include "totalstandard50.h"
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
#include "water_result.h"
#include "scancodedlg.h"
#include "register.h"
#include "md5encode.h"
#include "adjustratedlg.h"
#include "usermanagedlg.h"
#include "logindialog.h"
#include "waterweight50.h"
#include "waterstandard50.h"

MainForm50::MainForm50(bool licenseOK, int validDays, QWidget *parent, Qt::WFlags flags)
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
 	m_flowStandardDlg = NULL;
	m_totalWeightDlg = NULL;
 	m_totalStandardDlg = NULL;
	m_waterWeight50 = NULL; //水表检定（质量法）
	m_waterStandard50 = NULL; //水表检定（质量法）
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
	m_WaterResultDlg = NULL;

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
	if (licenseOK) //正式版
	{
		m_probationinfo->setText(tr("Official Version"));
	}
	else //试用版
	{
		m_probationinfo->setText(tr("Trial Version! Probation period is %1 days").arg(validDays));
	}
	m_probationinfo->setTextFormat(Qt::RichText);
	m_probationinfo->setOpenExternalLinks(true);
	ui.statusBar->addPermanentWidget(m_probationinfo);

	ui.actionScanCode->setVisible(false);
	ui.actionAdjustFlowRate->setVisible(false);
}

MainForm50::~MainForm50()
{
}

void MainForm50::closeEvent( QCloseEvent * event)
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

		if (m_flowStandardDlg)
		{
			delete m_flowStandardDlg;
			m_flowStandardDlg = NULL;
		}

		if (m_totalWeightDlg)
		{
			delete m_totalWeightDlg;
			m_totalWeightDlg = NULL;
		}

		if (m_totalStandardDlg)
		{
			delete m_totalStandardDlg;
			m_totalStandardDlg = NULL;
		}

		if (m_waterWeight50)
		{
			delete m_waterWeight50;
			m_waterWeight50 = NULL;
		}

		if (m_waterStandard50)
		{
			delete m_waterStandard50;
			m_waterStandard50 = NULL;
		}

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

		if (m_WaterResultDlg)
		{
			delete m_WaterResultDlg;
			m_WaterResultDlg = NULL;
		}
	}
}

void MainForm50::on_actionScanCode_triggered()
{
	if (NULL == m_scanCodeDlg)
	{
		m_scanCodeDlg = new ScanCodeDlg();
	}
	else //目的是执行ScanCodeDlg的构造函数
	{
		delete m_scanCodeDlg;
		m_scanCodeDlg = NULL;
		m_scanCodeDlg = new ScanCodeDlg();
	}
	m_scanCodeDlg->show();
}

void MainForm50::on_actionComSet_triggered()
{
	if (NULL == m_setcom)
	{
		m_setcom = new SetComDlg();
	}
	else //目的是执行SetComDlg的构造函数
	{
		delete m_setcom;
		m_setcom = NULL;
		m_setcom = new SetComDlg();
	}
	m_setcom->show();
}

void MainForm50::on_actionPortSet_triggered()
{
	if (NULL == m_portSet)
	{
		m_portSet = new SetPortFrm();
	}
	else //目的是执行SetPortFrm的构造函数
	{
		delete m_portSet;
		m_portSet = NULL;
		m_portSet = new SetPortFrm();
	}
	m_portSet->show();
}

//采集与控制测试程序
void MainForm50::on_actionDataTest_triggered()
{
	if (NULL == m_datatestdlg)
	{
		m_datatestdlg = new DataTestDlg50();
		connect(m_datatestdlg, SIGNAL(signalClosed()), this, SLOT(slotOnDataTestDlg50Closed()));
		m_datatestdlg->show();
	}
}

void MainForm50::slotOnDataTestDlg50Closed()
{
	if (m_datatestdlg)
	{
		delete m_datatestdlg;
		m_datatestdlg = NULL;
	}	
}

void MainForm50::on_actionAdjustFlowRate_triggered()
{
	if (NULL == m_adjustRateDlg)
	{
		m_adjustRateDlg = new AdjustRateDlg();
	}
	m_adjustRateDlg->show();
}

//调用串口调试工具
void MainForm50::on_actionComDebuger_triggered()
{
	QStringList cmdlist;
	cmdlist<<"zh";
	m_comProcess->start("delucom", cmdlist);
}

//标准表参数设定
void MainForm50::on_actionStdMtrParaSet_triggered()
{
	if (NULL == m_stdParaSet)
	{
		m_stdParaSet = new StdMtrParaSet();
	}
	else //目的是执行StdParaSet的构造函数
	{
		delete m_stdParaSet;
		m_stdParaSet = NULL;
		m_stdParaSet = new StdMtrParaSet();
	}
	m_stdParaSet->move((QApplication::desktop()->availableGeometry().width()-m_stdParaSet->width())/2, \
		(QApplication::desktop()->availableGeometry().height()-m_stdParaSet->height()-100)/2);  
	m_stdParaSet->show();
}

//标准表系数修正
void MainForm50::on_actionStdMtrCoeCorrect_triggered()
{
	if (m_stdCoeCorrect == NULL)
	{
		m_stdCoeCorrect = new StdMtrCoeCorrect();
		connect(m_stdCoeCorrect, SIGNAL(signalClosed()), this, SLOT(slotOnStdMtrCoeCorrectClosed()));
		m_stdCoeCorrect->show();
	}
}

void MainForm50::slotOnStdMtrCoeCorrectClosed()
{
	if (m_stdCoeCorrect)
	{
		delete m_stdCoeCorrect;
		m_stdCoeCorrect = NULL;
	}
}

//标准铂电阻参数设定
void MainForm50::on_actionStdPtParaSet_triggered()
{
	if (NULL == m_stdPtParaDlg)
	{
		m_stdPtParaDlg = new stdplasensorDlg();
	}
	else //目的是执行stdplasensorDlg的构造函数
	{
		delete m_stdPtParaDlg;
		m_stdPtParaDlg = NULL;
		m_stdPtParaDlg = new stdplasensorDlg();
	}
	m_stdPtParaDlg->show();
}

//被检铂电阻参数设定
void MainForm50::on_actionPtParaSet_triggered()
{
	if (NULL == m_chkPtParaDlg)
	{
		m_chkPtParaDlg = new chkplasensorDlg();
	}
	else //目的是执行chkplasensorDlg的构造函数
	{
		delete m_chkPtParaDlg;
		m_chkPtParaDlg = NULL;
		m_chkPtParaDlg = new chkplasensorDlg();
	}
	m_chkPtParaDlg->show();
}

//主机-从机设置
void MainForm50::on_actionMasterSlaveSet_triggered()
{
	if (NULL == m_masterslave)
	{
		m_masterslave = new CMasterSlave();
	}
	m_masterslave->show();
}

//流量检定(质量法)
void MainForm50::on_actionFlowWeight_triggered()
{
	if (NULL == m_flowWeightDlg)
	{
		m_flowWeightDlg = new FlowWeightDlg50();
	}
	else //目的是执行FlowWeightDlg50的构造函数
	{
		delete m_flowWeightDlg;
		m_flowWeightDlg = NULL;
		m_flowWeightDlg = new FlowWeightDlg50();
	}
	m_flowWeightDlg->showMaximized();
}

//流量检定(标准表法)
void MainForm50::on_actionFlowStandard_triggered()
{
	if (NULL == m_flowStandardDlg)
	{
		m_flowStandardDlg = new FlowStandardDlg50();
	}
	else //目的是执行FlowStandardDlg50的构造函数
	{
		delete m_flowStandardDlg;
		m_flowStandardDlg = NULL;
		m_flowStandardDlg = new FlowStandardDlg50();
	}
	m_flowStandardDlg->showMaximized();
}

//铂电阻检定(比较法)
void MainForm50::on_actionPtCompare_triggered()
{
	if (NULL == m_tvercompDlg)
	{
		m_tvercompDlg = new tvercompDlg();
	}
	else //目的是执行tvercompDlg的构造函数
	{
		delete m_tvercompDlg;
		m_tvercompDlg = NULL;
		m_tvercompDlg = new tvercompDlg();
	}
	m_tvercompDlg->show();
}

//铂电阻检定(参数法)
void MainForm50::on_actionPtPara_triggered()
{
	if (NULL == m_tverparaDlg)
	{
		m_tverparaDlg = new tverparamDlg();
	}
	else //目的是执行tverparamDlg的构造函数
	{
		delete m_tverparaDlg;
		m_tverparaDlg = NULL;
		m_tverparaDlg = new tverparamDlg();
	}
	m_tverparaDlg->show();
}

//计算器检定
void MainForm50::on_actionCalculator_triggered()
{
	if (NULL == m_calcDlg)
	{
		m_calcDlg = new CalcDlg();
	}
	else //目的是执行CalcDlg的构造函数
	{
		delete m_calcDlg;
		m_calcDlg = NULL;
		m_calcDlg = new CalcDlg();
	}
	m_calcDlg->show();
}

//温度/计算器组合检定
void MainForm50::on_actionCombine_triggered()
{
	if (NULL == m_cmbVerifyDlg)
	{
		m_cmbVerifyDlg = new CmbVerifyDlg();
	}
	else //目的是执行CalcDlg的构造函数
	{
		delete m_cmbVerifyDlg;
		m_cmbVerifyDlg = NULL;
		m_cmbVerifyDlg = new CmbVerifyDlg();
	}
	m_cmbVerifyDlg->showMaximized();
}

//水表检定（质量法）
void MainForm50::on_actionWaterWeight_triggered()
{
	if (NULL == m_waterWeight50)
	{
		m_waterWeight50 = new WaterWeightDlg50();
	}
	else //目的是执行WaterWeightDlg50的构造函数
	{
		delete m_waterWeight50;
		m_waterWeight50 = NULL;
		m_waterWeight50 = new WaterWeightDlg50();
	}
	m_waterWeight50->showMaximized();
}

//水表检定（质量法）
void MainForm50::on_actionWaterStandard_triggered()
{
	if (NULL == m_waterStandard50)
	{
		m_waterStandard50 = new WaterStandardDlg50();
	}
	else //目的是执行WaterStandardDlg50的构造函数
	{
		delete m_waterStandard50;
		m_waterStandard50 = NULL;
		m_waterStandard50 = new WaterStandardDlg50();
	}
	m_waterStandard50->showMaximized();
}

//总量检定（质量法）
void MainForm50::on_actionTotalWeight_triggered()
{
	if (NULL == m_totalWeightDlg)
	{
		m_totalWeightDlg = new TotalWeightDlg50();
	}
	else //目的是执行TotalWeightDlg50的构造函数
	{
		delete m_totalWeightDlg;
		m_totalWeightDlg = NULL;
		m_totalWeightDlg = new TotalWeightDlg50();
	}
	m_totalWeightDlg->showMaximized();
}

//总量检定（标准表法）
void MainForm50::on_actionTotalStandard_triggered()
{
	if (NULL == m_totalStandardDlg)
	{
		m_totalStandardDlg = new TotalStandardDlg50();
	}
	else //目的是执行TotalStandardDlg的构造函数
	{
		delete m_totalStandardDlg;
		m_totalStandardDlg = NULL;
		m_totalStandardDlg = new TotalStandardDlg50();
	}
	m_totalStandardDlg->showMaximized();
}

//查询流量检定结果（包括质量法和标准表法）
void MainForm50::on_actionFlowResult_triggered()
{
	if (NULL != m_flowResultDlg)
	{	
		delete m_flowResultDlg;
		m_flowResultDlg = NULL;
	}
	m_flowResultDlg = new FlowResultDlg();
	m_flowResultDlg->show();
}

//查询总量检定结果（包括质量法和标准表法）
void MainForm50::on_actionTotalResult_triggered()
{
	if (NULL != m_TotalResultDlg)
	{	
		delete m_TotalResultDlg;
		m_TotalResultDlg = NULL;
	}
	m_TotalResultDlg = new TotalResultDlg();
	m_TotalResultDlg->show();
}

//查询水表检定结果（包括质量法和标准表法）
void MainForm50::on_actionWaterResult_triggered()
{
	if (NULL != m_WaterResultDlg)
	{	
		delete m_WaterResultDlg;
		m_WaterResultDlg = NULL;
	}
	m_WaterResultDlg = new WaterResultDlg();
	m_WaterResultDlg->show();
}

//查询温度传感器检定结果
void MainForm50::on_actionPtResult_triggered()
{
	if (NULL != m_PlaResultDlg)
	{	
		delete m_PlaResultDlg;
		m_PlaResultDlg = NULL;
	}
	m_PlaResultDlg = new PlaResultDlg();
	m_PlaResultDlg->show();
}

//查询计算器检定结果
void MainForm50::on_actionCalculatorResult_triggered()
{
	if (NULL != m_CalcResultDlg)
	{	
		delete m_CalcResultDlg;
		m_CalcResultDlg = NULL;
	}
	m_CalcResultDlg = new CalcResultDlg();
	m_CalcResultDlg->show();
}

//查询温度/计算器组合检定结果
void MainForm50::on_actionCombineResult_triggered()
{
	if (NULL != m_CmbResultDlg)
	{	
		delete m_CmbResultDlg;
		m_CmbResultDlg = NULL;
	}
	m_CmbResultDlg = new CmbResultDlg();
	m_CmbResultDlg->show();
}

void MainForm50::on_actionQueryExcel_triggered()
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
	QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1);//打开第一个sheet
	QAxObject * usedrange = worksheet->querySubObject("UsedRange");//获取该sheet的使用范围对象
	QAxObject * rows = usedrange->querySubObject("Rows");
	QAxObject * columns = usedrange->querySubObject("Columns");
	int intRowStart = usedrange->property("Row").toInt();
	int intColStart = usedrange->property("Column").toInt();
	int intCols = columns->property("Count").toInt();
	int intRows = rows->property("Count").toInt();

	for (int i = intRowStart; i < intRowStart + intRows; i++) //行 
	{
		for (int j = intColStart; j < intColStart + intCols; j++) //列 
		{
			QAxObject * range = worksheet->querySubObject("Cells(int,int)", i, j ); //获取单元格  
			qDebug("row %d, col %d, value is %d\n", i, j, range->property("Value").toInt()); //************出问题!!!!!! 
		} 
	}
}

//退出
void MainForm50::on_actionExit_triggered()
{
	this->close();
}

void MainForm50::on_actionUserManage_triggered()
{
	LoginDialog login;
	if (login.exec() != QDialog::Accepted)
	{
		return;
	}
	if (login.getCurRoleID() != 0) //非超级用户
	{
		QMessageBox::warning(this, tr("Error"), tr("No right for user manage!"));
		return;
	}
	if (NULL == m_userManageDlg)
	{
		m_userManageDlg = new UserManageDlg();
	}
	else //目的是执行UserManageDlg的构造函数
	{
		delete m_userManageDlg;
		m_userManageDlg = NULL;
		m_userManageDlg = new UserManageDlg();
	}
	m_userManageDlg->show();
}

void MainForm50::on_actionAbout_triggered()
{
	QMessageBox::aboutQt(this);
}

void MainForm50::on_actionRegister_triggered()
{
	if (NULL == m_registerDlg)
	{
		m_registerDlg = new RegisterDlg(qGetVolumeInfo());
	}
	else //目的是执行RegisterDlg的构造函数
	{
		delete m_registerDlg;
		m_registerDlg = NULL;
		m_registerDlg = new RegisterDlg(qGetVolumeInfo());
	}
	connect(m_registerDlg, SIGNAL(signalRegisterSuccess()), this, SLOT(slotRegisterSuccess()));
	m_registerDlg->show();
}

//显示风格
void MainForm50::on_actionDefault_triggered()
{
	QFile qss(":/qtverify/qss/default.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm50::on_actionClassic_triggered()
{
	QFile qss(":/qtverify/qss/classic.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm50::on_actionFashion_triggered()
{
	QFile qss(":/qtverify/qss/fashion.qss");
	qss.open(QFile::ReadOnly);
	this->setStyleSheet(qss.readAll());
	qss.close();
}

void MainForm50::chaneLanguage(QString lang)
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

void MainForm50::on_actionEnglish_triggered()
{
	chaneLanguage("en");
}

void MainForm50::on_actionChinese_triggered()
{
	chaneLanguage("zh");
}

void MainForm50::processError(QProcess::ProcessError error)
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

void MainForm50::slotRegisterSuccess()
{
	qDebug()<<ui.statusBar->currentMessage();
	m_probationinfo->setText(tr("Official Version"));
}