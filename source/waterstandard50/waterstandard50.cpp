/***********************************************
**  文件名:     waterstand50.cpp
**  功能:       水表检定(标准表法)主界面-DN50，双天平、双控制板
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2015/12/25
**  专业组:     德鲁计量软件组
**  程序设计者: YS
**  程序员:     YS
**  版本历史:   2015/12 第一版
**  内容包含:
**  说明:		
**  更新记录:   

***********************************************/

#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtSql/QSqlTableModel>
#include <QtGui/QFileDialog>
#include <QtCore/QSignalMapper>
#include <QCloseEvent>
#include <math.h>

#include "waterstandard50.h"
#include "commondefine.h"
#include "algorithm.h"
#include "qtexdb.h"
#include "parasetdlg.h"
#include "readcomconfig.h"
#include "report.h"
#include "readstdmeter.h"

WaterStandardDlg50::WaterStandardDlg50(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"WaterStandardDlg50 thread:"<<QThread::currentThreadId();
	ui.setupUi(this);

	///////////////////////////////// 原showEvent()函数的内容 begin 
	//否则每次最小化再显示时，会调用showEvent函数，导致内容清空等现象
	ui.btnExhaust->hide();
	ui.btnGoOn->hide();

	if (!getPortSetIni(&m_portsetinfo)) //获取下位机端口号配置信息
	{
		QMessageBox::warning(this, tr("Warning"), tr("Warning:get port set info failed!"));
	}

	m_readComConfig = new ReadComConfig(); //读串口设置接口（必须在initBalanceCom前调用）

	m_tempObj = NULL;
	m_tempTimer = NULL;
	initTemperatureCom();	//初始化温度采集串口

	m_controlObj = NULL;
	initControlCom();		//初始化控制串口

	m_controlObj2 = NULL;
	initControlCom2();		//初始化控制串口2

	m_meterObj = NULL;      //热量表通讯
	m_recPtr = NULL;        //流量检测结果

	//计算类接口
	m_chkAlg = new CAlgorithm();

	//映射关系；初始化阀门状态	
	initValveStatus();      
	initRegulateStatus();

	m_exaustTimer = new QTimer(this); //排气定时器
	connect(m_exaustTimer, SIGNAL(timeout()), this, SLOT(slotExaustFinished()));

	m_stopFlag = true; //停止检测标志（退出界面后，不再检查天平容量）

	m_avgTFCount = 1; //计算平均温度用的累加计数器
	m_nowOrder = 0;   //当前进行的检定序号
	m_lastPortNO = 0; 

	m_nowParams = NULL;
	m_continueVerify = true; //连续检定
	m_resetZero = false;     //初值回零
	m_autopick = false;      //自动采集
	m_flowPointNum = 0;      //流量点个数
	m_maxMeterNum = 0;       //某规格表最多支持的检表个数
	m_oldMaxMeterNum = 0;
	m_validMeterNum = 0;     //实际检表个数
	m_exaustSecond = 45;     //默认排气时间45秒
	m_pickcode = PROTOCOL_VER_DELU; //采集代码 默认德鲁
	m_flowSC = 1.0;			 //流量安全系数，默认1.0
	m_adjErr = false;        //默认不调整误差
	m_writeNO = false;       //默认不写表号
	m_repeatverify = false;  //默认不重复检测
	m_meterStartValue = NULL;
	m_meterEndValue = NULL;
	m_meterTemper = NULL;
	m_meterDensity = NULL;
	m_meterStdValue = NULL;
	m_meterError = NULL;
	m_meterErr = NULL;
	m_oldMeterCoe = NULL;
	m_meterResult = NULL;
	m_stdStartVol = 0;
	m_stdEndVol = 0;
	m_timeStamp = "";
	m_nowDate = "";
	m_validDate = "";

	QSqlTableModel *model = new QSqlTableModel(this, g_defaultdb);  
	model->setTable("T_Meter_Standard");  
	model->select();  
	m_meterStdMapper = new QDataWidgetMapper(this);
	m_meterStdMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
	m_meterStdMapper->setModel(model);
	m_meterStdMapper->addMapping(ui.lnEditStandard, 1); //映射表"T_Meter_Standard"的第二个字段

	m_paraSetDlg = NULL;    //参数设置对话框
	m_paraSetReader = new ParaSetReader(); //读参数设置接口
	if (!readNowParaConfig()) //获取当前检定参数
	{
		qWarning()<<"读取参数配置文件失败!";
	}

	if (!isComAndPortNormal())
	{
		qWarning()<<"串口、端口设置错误!";
	}

// 	QTimer::singleShot(3000, this, SLOT(close()));

	///////////////////////////////// 原showEvent()函数的内容 end

// 	ui.lcdInTemper->display(50);
// 	ui.lcdOutTemper->display(50);

	/***************标准流量计***********************/
	m_mapInstWdg[FLOW_RATE_BIG]   = ui.lcdInstStdMeter_50;
	m_mapInstWdg[FLOW_RATE_MID_2] = ui.lcdInstStdMeter_25;
	m_mapInstWdg[FLOW_RATE_MID_1] = ui.lcdInstStdMeter_10;
	m_mapInstWdg[FLOW_RATE_SMALL] = ui.lcdInstStdMeter_3;

	m_mapAccumWdg[FLOW_RATE_BIG]   = ui.lcdAccumStdMeter_50;
	m_mapAccumWdg[FLOW_RATE_MID_2] = ui.lcdAccumStdMeter_25;
	m_mapAccumWdg[FLOW_RATE_MID_1] = ui.lcdAccumStdMeter_10;
	m_mapAccumWdg[FLOW_RATE_SMALL] = ui.lcdAccumStdMeter_3;

	m_stdMeterReader = NULL;
	m_stdMeterReader = new CStdMeterReader();
	connect(m_stdMeterReader, SIGNAL(signalReadInstReady(const flow_rate_wdg&, const float&)), this, SLOT(slotFreshInstFlow(const flow_rate_wdg&, const float&)));
	connect(m_stdMeterReader, SIGNAL(signalReadAccumReady(const flow_rate_wdg&, const float&)), this, SLOT(slotFreshAccumFlow(const flow_rate_wdg&, const float&)));
	connect(m_stdMeterReader, SIGNAL(signalReadTolInstReady(const float&)), this, SLOT(slotFreshTolInst(const float&)));
	connect(m_stdMeterReader, SIGNAL(signalReadTolAccumReady(const float&)), this, SLOT(slotFreshTolAccum(const float&)));
	m_stdMeterReader->startReadMeter();
	/***************标准流量计end********************/
}

void WaterStandardDlg50::slotFreshInstFlow(const flow_rate_wdg& idx, const float& value)
{
	m_mapInstWdg[idx]->display(value);
	switch (idx)
	{
	case FLOW_RATE_BIG:
		m_mapInstWdg[idx]->display(QString::number(value, 'f', 2));
		break;
	case FLOW_RATE_MID_2:
		m_mapInstWdg[idx]->display(QString::number(value, 'f', 2));
		break;
	case FLOW_RATE_MID_1:
		m_mapInstWdg[idx]->display(QString::number(value, 'f', 3));
		break;
	case FLOW_RATE_SMALL:
		m_mapInstWdg[idx]->display(QString::number(value, 'f', 4));
		break;
	default:
		break;
	}
}

void WaterStandardDlg50::slotFreshAccumFlow(const flow_rate_wdg& idx, const float& value)
{
	switch (idx)
	{
	case FLOW_RATE_BIG:
		m_mapAccumWdg[idx]->display(QString::number(value, 'f', 2));
		break;
	case FLOW_RATE_MID_2:
		m_mapAccumWdg[idx]->display(QString::number(value, 'f', 2));
		break;
	case FLOW_RATE_MID_1:
		m_mapAccumWdg[idx]->display(QString::number(value, 'f', 3));
		break;
	case FLOW_RATE_SMALL:
		m_mapAccumWdg[idx]->display(QString::number(value, 'f', 4));
		break;
	default:
		break;
	}
}

void WaterStandardDlg50::slotFreshTolInst(const float& value)
{
	ui.lcdFlowRate->display(QString::number(value, 'f', 4));
}

void WaterStandardDlg50::slotFreshTolAccum(const float& value)
{
	ui.lcdAccumStdMeter->display(QString::number(value, 'f', 4));
}

WaterStandardDlg50::~WaterStandardDlg50()
{
}

void WaterStandardDlg50::showEvent(QShowEvent * event)
{
	qDebug()<<"WaterStandardDlg50::showEvent";
}

void WaterStandardDlg50::closeEvent( QCloseEvent * event)
{
	qDebug()<<"^^^^^WaterStandardDlg50::closeEvent";
	int button = QMessageBox::question(this, tr("Question"), tr("Exit Really ?"), \
		QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	if (button == QMessageBox::No)
	{
		return event->ignore();
	}
	else
	{
		event->accept();
	}

	if (!m_stopFlag)
	{
		stopVerify();
	}
	openValve(m_portsetinfo.bigWaterOutNo);
	ui.labelHintPoint->clear();
	ui.labelHintProcess->setText(tr("release pipe pressure..."));
	openValve(m_portsetinfo.bigNo); //打开大流量点阀门，释放管路压力
 	wait(RELEASE_PRESS_TIME); //等待2秒，释放管路压力
	closeValve(m_portsetinfo.bigNo);
	ui.labelHintProcess->clear();
	ui.btnStart->setEnabled(true);

	if (m_paraSetReader) //读检定参数
	{
		delete m_paraSetReader;
		m_paraSetReader = NULL;
	}

	if (m_paraSetDlg)    //参数设置对话框
	{
		delete m_paraSetDlg;
		m_paraSetDlg = NULL;
	}

	if (m_readComConfig) //读串口设置
	{
		delete m_readComConfig;
		m_readComConfig = NULL;
	}

	if (m_tempThread.isRunning())  // 温度采集
	{
		m_tempThread.exit(); //否则日志中会有警告"QThread: Destroyed while thread is still running"
		if (m_tempObj)
		{
			delete m_tempObj;
			m_tempObj = NULL; 
		}	 		 		
	}	   

	if (m_tempTimer) //采集温度计时器
	{
		if (m_tempTimer->isActive())
		{
			m_tempTimer->stop();
		}
		delete m_tempTimer;
		m_tempTimer = NULL;
	}


	if (m_controlObj)  //阀门控制
	{
		delete m_controlObj;
		m_controlObj = NULL;

		m_valveThread.exit();
	}

	if (m_controlObj2)  //阀门控制2
	{
		delete m_controlObj2;
		m_controlObj2 = NULL;

		m_valveThread2.exit();
	}

	if (m_chkAlg)//计算类
	{
		delete m_chkAlg;
		m_chkAlg = NULL;
	}

	//热量表通讯
	if (m_meterObj)
	{
		delete []m_meterObj;
		m_meterObj = NULL;

		for (int i=0; i<m_oldMaxMeterNum; i++)
		{
			m_meterThread[i].exit();
		}
	}

	if (m_exaustTimer) //排气计时器
	{
		if (m_exaustTimer->isActive())
		{
			m_exaustTimer->stop();
		}
		delete m_exaustTimer;
		m_exaustTimer = NULL;
	}

	//计时器，用于动态显示调节阀开度
	if (m_regSmallTimer)
	{
		if (m_regSmallTimer->isActive())
		{
			m_regSmallTimer->stop();
		}
		delete m_regSmallTimer;
		m_regSmallTimer = NULL;
	}

	if (m_regMid1Timer)
	{
		if (m_regMid1Timer->isActive())
		{
			m_regMid1Timer->stop();
		}
		delete m_regMid1Timer;
		m_regMid1Timer = NULL;
	}

	if (m_regMid2Timer)
	{
		if (m_regMid2Timer->isActive())
		{
			m_regMid2Timer->stop();
		}
		delete m_regMid2Timer;
		m_regMid2Timer = NULL;
	}

	if (m_regBigTimer)
	{
		if (m_regBigTimer->isActive())
		{
			m_regBigTimer->stop();
		}
		delete m_regBigTimer;
		m_regBigTimer = NULL;
	}

 	if (m_stdMeterReader)
 	{
 		delete m_stdMeterReader;
 		m_stdMeterReader = NULL;
 	}

	emit signalClosed();
}

void WaterStandardDlg50::resizeEvent(QResizeEvent * event)
{
	qDebug()<<"resizeEvent...";

	int th = ui.tableWidget->size().height();
	int tw = ui.tableWidget->size().width();
	int hh = ui.tableWidget->horizontalHeader()->size().height();
	int vw = ui.tableWidget->verticalHeader()->size().width();
	int vSize = (int)((th-hh-10)/(m_maxMeterNum<=0 ? 12 : m_maxMeterNum));
	int hSize = (int)((tw-vw-10)/(COLUMN__FLOW_COUNT-1)); //隐藏了"密度"列
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(vSize);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(hSize);
}

/*
** 温度采集串口 上位机直接采集
** 周期请求
*/
void WaterStandardDlg50::initTemperatureCom()
{
	ComInfoStruct tempStruct = m_readComConfig->ReadTempConfig();
	m_tempObj = new TempComObject();
	m_tempObj->moveToThread(&m_tempThread);
	m_tempThread.start();
	m_tempObj->openTemperatureCom(&tempStruct);
	connect(m_tempObj, SIGNAL(temperatureIsReady(const QString &)), this, SLOT(slotFreshComTempValue(const QString &)));

	m_tempTimer = new QTimer();
	connect(m_tempTimer, SIGNAL(timeout()), this, SLOT(slotAskPipeTemperature()));

	m_tempTimer->start(TIMEOUT_PIPE_TEMPER); //周期请求温度
}

void WaterStandardDlg50::slotAskPipeTemperature()
{
	m_tempObj->writeTemperatureComBuffer();
}

//控制板通讯串口
void WaterStandardDlg50::initControlCom()
{
	ComInfoStruct valveStruct = m_readComConfig->ReadValveConfig();
	m_controlObj = new ControlComObject();
	m_controlObj->setProtocolVersion(m_portsetinfo.version);
	m_controlObj->moveToThread(&m_valveThread);
	m_valveThread.start();
	m_controlObj->openControlCom(&valveStruct);

	connect(m_controlObj, SIGNAL(controlRelayIsOk(const UINT8 &, const bool &)), this, SLOT(slotSetValveBtnStatus(const UINT8 &, const bool &)));
	connect(m_controlObj, SIGNAL(controlRegulateIsOk()), this, SLOT(slotSetRegulateOk()));
}

//控制板通讯串口2
void WaterStandardDlg50::initControlCom2()
{
	ComInfoStruct valveStruct2 = m_readComConfig->ReadValveConfig2();
	m_controlObj2 = new ControlComObject();
	m_controlObj2->setProtocolVersion(m_portsetinfo.version);
	m_controlObj2->moveToThread(&m_valveThread2);
	m_valveThread2.start();
	m_controlObj2->openControlCom(&valveStruct2);

	connect(m_controlObj2, SIGNAL(controlRelayIsOk(const UINT8 &, const bool &)), this, SLOT(slotSetValveBtnStatus(const UINT8 &, const bool &)));
	connect(m_controlObj2, SIGNAL(controlRegulateIsOk()), this, SLOT(slotSetRegulateOk()));
}

//热量表通讯串口
void WaterStandardDlg50::initMeterCom()
{
	if (m_meterObj)
	{
		delete []m_meterObj;
		m_meterObj = NULL;

		for (int i=0; i<m_oldMaxMeterNum; i++)
		{
			m_meterThread[i].exit();
		}
	}
	if (m_maxMeterNum <= 0)
	{
		return;
	}

	m_oldMaxMeterNum = m_maxMeterNum;
	m_meterThread = new ComThread[m_maxMeterNum];

	m_meterObj = new MeterComObject[m_maxMeterNum];
	int i=0;
	for (i=0; i<m_maxMeterNum; i++)
	{
		m_meterObj[i].moveToThread(&m_meterThread[i]);
		m_meterObj[i].setProtocolVersion(m_pickcode); //设置表协议类型
		m_meterThread[i].start();
		m_meterObj[i].openMeterCom(&m_readComConfig->ReadMeterConfigByNum(i+1));

		connect(&m_meterObj[i], SIGNAL(readMeterNoIsOK(const QString&, const QString&)), this, SLOT(slotSetMeterNumber(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterFlowIsOK(const QString&, const QString&)), this, SLOT(slotSetMeterFlow(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterBigCoeIsOK(const QString&, const QString&)), \
			this, SLOT(slotFreshBigCoe(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterMid2CoeIsOK(const QString&, const QString&)), \
			this, SLOT(slotFreshMid2Coe(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterMid1CoeIsOK(const QString&, const QString&)), \
			this, SLOT(slotFreshMid1Coe(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterSmallCoeIsOK(const QString&, const QString&)), \
			this, SLOT(slotFreshSmallCoe(const QString&, const QString&)));
	}
}

/*
** 端口号-阀门映射关系；初始化阀门状态（默认阀门初始状态全部为关闭,水泵初始状态为关闭）
** 需要改进得更加灵活
*/
void WaterStandardDlg50::initValveStatus()
{
	m_nowPortNo = 0;

	//端口号-阀门按钮 映射关系
	m_valveBtn[m_portsetinfo.bigNo] = ui.btnValveBig;
	m_valveBtn[m_portsetinfo.smallNo] = ui.btnValveSmall;
	m_valveBtn[m_portsetinfo.middle1No] = ui.btnValveMiddle1;
	m_valveBtn[m_portsetinfo.middle2No] = ui.btnValveMiddle2;
	m_valveBtn[m_portsetinfo.waterInNo] = ui.btnWaterIn;
	m_valveBtn[m_portsetinfo.bigWaterInNo] = ui.btnBigWaterIn;
	m_valveBtn[m_portsetinfo.bigWaterOutNo] = ui.btnBigWaterOut;
	m_valveBtn[m_portsetinfo.smallWaterInNo] = ui.btnSmallWaterIn;
	m_valveBtn[m_portsetinfo.smallWaterOutNo] = ui.btnSmallWaterOut;
	m_valveBtn[m_portsetinfo.pumpNo] = ui.btnWaterPump; //水泵

	//初始化：放水阀为打开，其他阀门为关闭状态
	m_valveStatus[m_portsetinfo.bigNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle1No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle2No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.waterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.smallWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.pumpNo] = VALVE_CLOSE; //水泵

	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigNo], m_valveStatus[m_portsetinfo.bigNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallNo], m_valveStatus[m_portsetinfo.smallNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.middle1No], m_valveStatus[m_portsetinfo.middle1No]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.middle2No], m_valveStatus[m_portsetinfo.middle2No]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.waterInNo], m_valveStatus[m_portsetinfo.waterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigWaterInNo], m_valveStatus[m_portsetinfo.bigWaterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigWaterOutNo], m_valveStatus[m_portsetinfo.bigWaterOutNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallWaterInNo], m_valveStatus[m_portsetinfo.smallWaterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallWaterOutNo], m_valveStatus[m_portsetinfo.smallWaterOutNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.pumpNo], m_valveStatus[m_portsetinfo.pumpNo]);
}

void WaterStandardDlg50::initRegulateStatus()
{
	//端口号-阀门按钮 映射关系
	m_RegLineEdit[m_portsetinfo.regSmallNo] = ui.lineEditOpeningSmall;
	m_RegLineEdit[m_portsetinfo.regMid1No] = ui.lineEditOpeningMid1;
	m_RegLineEdit[m_portsetinfo.regMid2No] = ui.lineEditOpeningMid2;
	m_RegLineEdit[m_portsetinfo.regBigNo] = ui.lineEditOpeningBig;

	m_RegSpinBox[m_portsetinfo.regSmallNo] = ui.spinBoxOpeningSmall;
	m_RegSpinBox[m_portsetinfo.regMid1No] = ui.spinBoxOpeningMid1;
	m_RegSpinBox[m_portsetinfo.regMid2No] = ui.spinBoxOpeningMid2;
	m_RegSpinBox[m_portsetinfo.regBigNo] = ui.spinBoxOpeningBig;

	//计时器，动态显示调节阀开度
	m_smallOpening = 0;
	m_regSmallTimer = new QTimer();
	connect(m_regSmallTimer, SIGNAL(timeout()), this, SLOT(slotFreshSmallRegOpening()));

	m_mid1Opening = 0;
	m_regMid1Timer = new QTimer();
	connect(m_regMid1Timer, SIGNAL(timeout()), this, SLOT(slotFreshMid1RegOpening()));

	m_mid2Opening = 0;
	m_regMid2Timer = new QTimer();
	connect(m_regMid2Timer, SIGNAL(timeout()), this, SLOT(slotFreshMid2RegOpening()));

	m_bigOpening = 0;
	m_regBigTimer = new QTimer();
	connect(m_regBigTimer, SIGNAL(timeout()), this, SLOT(slotFreshBigRegOpening()));
}

//在界面刷新入口温度和出口温度值
void WaterStandardDlg50::slotFreshComTempValue(const QString& tempStr)
{
	ui.lcdInTemper->display(tempStr.left(TEMPER_DATA_WIDTH));   //入口温度 PV
	ui.lcdOutTemper->display(tempStr.right(TEMPER_DATA_WIDTH)); //出口温度 SV
}

//检测串口、端口设置是否正确
int WaterStandardDlg50::isComAndPortNormal()
{
	return true;
}

//获取当前检定参数;初始化表格控件；显示关键参数；初始化热量表通讯串口
int WaterStandardDlg50::readNowParaConfig()
{
	if (NULL == m_paraSetReader)
	{
		return false;
	}

	if (!m_stopFlag)
	{
// 		QMessageBox::warning(this, tr("Warning"), tr("Verify...\nParaSet will work next time!"));
		return false;
	}

	m_state = STATE_INIT;

	m_nowParams = m_paraSetReader->getParams();
	m_continueVerify = m_nowParams->bo_converify; //连续检定
	m_resetZero = m_nowParams->bo_resetzero; //初值回零
	m_autopick = m_nowParams->bo_autopick;   //自动采集
	m_flowPointNum = m_nowParams->total_fp;  //有效流量点的个数 
	m_exaustSecond = m_nowParams->ex_time;   //排气时间
	m_standard = m_nowParams->m_stand;       //表规格
	m_model = m_nowParams->m_model;   //表型号
	m_maxMeterNum = m_nowParams->m_maxMeters;//不同表规格对应的最大检表数量
	m_pickcode = m_nowParams->m_pickcode; //采集代码
	m_numPrefix = getNumPrefixOfManufac(m_pickcode); //表号前缀
	m_flowSC = m_nowParams->sc_flow; //流量安全系数
	m_adjErr = m_nowParams->bo_adjerror; //是否调整误差
	m_writeNO = m_nowParams->bo_writenum;//是否写表号
	m_newMeterNO = m_nowParams->meterNo;

	initTableWidget();
	showNowKeyParaConfig();
	initMeterCom();

	ui.btnAllAdjError->setEnabled(false);
	ui.btnAllModifyNO->setEnabled(false);
	return true;
}

//初始化表格控件
void WaterStandardDlg50::initTableWidget()
{
	if (m_maxMeterNum <= 0)
	{
		return;
	}
	ui.tableWidget->setRowCount(m_maxMeterNum); //设置表格行数

	QSignalMapper *signalMapper1 = new QSignalMapper();
	QSignalMapper *signalMapper2 = new QSignalMapper();
	QSignalMapper *signalMapper3 = new QSignalMapper();
	QSignalMapper *signalMapper4 = new QSignalMapper();
	QSignalMapper *signalMapper5 = new QSignalMapper();

	QStringList vLabels;
	for (int i=0; i< ui.tableWidget->rowCount(); i++)
	{
		vLabels<<QString(QObject::tr("meterPosNo%1").arg(i+1));

		for (int j=0; j<ui.tableWidget->columnCount(); j++)
		{
			ui.tableWidget->setItem(i, j, new QTableWidgetItem(QString("")));
			ui.tableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);
		}

		//设为只读
		ui.tableWidget->item(i, COLUMN_FLOW_POINT)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_BAL_START)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_BAL_END)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_TEMPER)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_DENSITY)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_STD_VALUE)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_DISP_ERROR)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_STD_ERROR)->setFlags(Qt::NoItemFlags);

		//设置按钮
		QPushButton *btnModNo = new QPushButton(QObject::tr("\(%1\)").arg(i+1) + tr("ModifyNO"));
		ui.tableWidget->setCellWidget(i, COLUMN_MODIFY_METERNO, btnModNo);
		signalMapper1->setMapping(btnModNo, i);
		connect(btnModNo, SIGNAL(clicked()), signalMapper1, SLOT(map()));
		btnModNo->setEnabled(false);

		QPushButton *btnAdjErr = new QPushButton(QObject::tr("\(%1\)").arg(i+1) + tr("AdjustErr"));
		ui.tableWidget->setCellWidget(i, COLUMN_ADJUST_ERROR, btnAdjErr);
		signalMapper2->setMapping(btnAdjErr, i);
		connect(btnAdjErr, SIGNAL(clicked()), signalMapper2, SLOT(map()));
		btnAdjErr->setEnabled(false);

		QPushButton *btnReadData = new QPushButton(QObject::tr("\(%1\)").arg(i+1) + tr("ReadData"));
		ui.tableWidget->setCellWidget(i, COLUMN_READ_DATA, btnReadData);
		signalMapper3->setMapping(btnReadData, i);
		connect(btnReadData, SIGNAL(clicked()), signalMapper3, SLOT(map()));

		QPushButton *btnVerifySt = new QPushButton(QObject::tr("\(%1\)").arg(i+1) + tr("VerifySt"));
		ui.tableWidget->setCellWidget(i, COLUMN_VERIFY_STATUS, btnVerifySt);
		signalMapper4->setMapping(btnVerifySt, i);
		connect(btnVerifySt, SIGNAL(clicked()), signalMapper4, SLOT(map()));

		QPushButton *btnReadNO = new QPushButton(QObject::tr("\(%1\)").arg(i+1) + tr("ReadNO"));
		ui.tableWidget->setCellWidget(i, COLUMN_READ_NO, btnReadNO);
		signalMapper5->setMapping(btnReadNO, i);
		connect(btnReadNO, SIGNAL(clicked()), signalMapper5, SLOT(map()));
	}
	connect(signalMapper1, SIGNAL(mapped(const int &)),this, SLOT(slotModifyMeterNO(const int &)));
	connect(signalMapper2, SIGNAL(mapped(const int &)),this, SLOT(slotAdjustError(const int &)));
	connect(signalMapper3, SIGNAL(mapped(const int &)),this, SLOT(slotReadData(const int &)));
	connect(signalMapper4, SIGNAL(mapped(const int &)),this, SLOT(slotVerifyStatus(const int &)));
	connect(signalMapper5, SIGNAL(mapped(const int &)),this, SLOT(slotReadNO(const int &)));

	ui.tableWidget->setVerticalHeaderLabels(vLabels);
	ui.tableWidget->setFont(QFont("Times", 13, QFont::DemiBold, true));
	ui.tableWidget->hideColumn(COLUMN_DENSITY); //隐藏"密度"列
// 	ui.tableWidget->resizeColumnsToContents();
// 	ui.tableWidget->setColumnWidth(COLUMN_METER_NUMBER, 125);
}

//显示当前关键参数设置信息
void WaterStandardDlg50::showNowKeyParaConfig()
{
	if (NULL == m_nowParams)
	{
		return;
	}

	ui.cmbAutoPick->setCurrentIndex(m_nowParams->bo_autopick);
	ui.cmbContinue->setCurrentIndex(m_nowParams->bo_converify);
	ui.cmbResetZero->setCurrentIndex(m_nowParams->bo_resetzero);
	m_meterStdMapper->setCurrentIndex(m_nowParams->m_stand);
}

//检查数据采集是否正常，包括天平、温度、电磁流量计
int WaterStandardDlg50::isDataCollectNormal()
{
	return true;
}

/*
** 开始排气倒计时
*/
int WaterStandardDlg50::startExhaustCountDown()
{
	if (!isDataCollectNormal())
	{
		qWarning()<<"数据采集错误，请检查";
		QMessageBox::warning(this, tr("Warning"), tr("data acquisition error, please check!"));
		return false;
	}

	//打开4路电动调节阀
	openAllRegulator();
	ui.labelHintProcess->setText(tr("regulator is opening, please wait..."));
	ui.labelHintPoint->clear();
 	wait(5000); //等待电动调节阀调整到一定开度，用于排气

	m_controlObj->askSetDriverFreq(m_nowParams->fp_info[0].fp_freq);
	if (!openAllValveAndPump())
	{
		qWarning()<<"打开所有阀门和水泵 失败!";
		QMessageBox::warning(this, tr("Warning"), tr("exhaust air failed!"));
		return false;
	}
	m_stopFlag = false;
	m_exaustSecond = m_nowParams->ex_time;
	m_exaustTimer->start(CYCLE_TIME);//开始排气倒计时
	ui.labelHintProcess->setText(tr("Exhaust countdown: <font color=DarkGreen size=6><b>%1</b></font> second").arg(m_exaustSecond));
	ui.labelHintPoint->clear();
	qDebug()<<"排气倒计时:"<<m_exaustSecond<<"秒";

	return true;
}

/*
** 排气定时器响应函数
*/
void WaterStandardDlg50::slotExaustFinished()
{
	if (m_stopFlag)
	{
		return;
	}

	m_exaustSecond --;
	ui.labelHintProcess->setText(tr("Exhaust countdown: <font color=DarkGreen size=6><b>%1</b></font> second").arg(m_exaustSecond));
	qDebug()<<"排气倒计时:"<<m_exaustSecond<<"秒";
	if (m_exaustSecond > 1)
	{
		return;
	}

	m_exaustTimer->stop(); //停止排气计时
	ui.labelHintProcess->setText(tr("Exhaust countdown finished!"));
	ui.labelHintProcess->clear();
	if (!closeAllFlowPointValves()) //关闭所有流量点阀门 失败
	{
		if (!closeAllFlowPointValves()) //再尝试关闭一次
		{
			qWarning()<<"关闭所有流量点阀门失败，检定结束";
			return;
		}
	}

	if (setAllMeterVerifyStatus()) //设置检定状态成功
	{
		startVerify();
	}
}

/*
** 读取所有热表流量系数
*/
int WaterStandardDlg50::readAllMeterFlowCoe()
{
	qDebug()<<"readAllMeterFlowCoe ...";
	int idx = -1;
	for (int j=0; j<m_maxMeterNum; j++)
	{
		idx = isMeterPosValid(j+1);
		if (m_state == STATE_START_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_START)->setText("");
			if (idx >= 0)
			{
				m_meterStartValue[idx] = 0;
			}
		}
		else if (m_state == STATE_END_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_END)->setText("");
			if (idx >= 0)
			{
				m_meterEndValue[idx] = 0;
			}
		}
		m_meterObj[j].askReadMeterFlowCoe();
	}

	return true;
}

//设置所有热量表进入检定状态
int WaterStandardDlg50::setAllMeterVerifyStatus()
{
	ui.labelHintPoint->setText(tr("setting verify status ..."));
	on_btnAllVerifyStatus_clicked();
	wait(CYCLE_TIME);
	on_btnAllVerifyStatus_clicked();
	return true;
}

/*
** 打开所有阀门和水泵（关闭大、小天平进水阀，因为标准表法不需要使用天平）
*/
int WaterStandardDlg50::openAllValveAndPump()
{
	openValve(m_portsetinfo.bigWaterOutNo); //大天平放水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.bigWaterInNo);
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.smallWaterInNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.waterInNo);
	openWaterPump();//打开水泵
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.middle1No);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.middle2No);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.bigNo);

	return true;
}

//关闭所有阀门和水泵
int WaterStandardDlg50::closeAllValveAndPumpOpenOutValve()
{
	openValve(m_portsetinfo.bigWaterOutNo); //打开大天平放水阀
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.bigWaterInNo);  //关闭大天平进水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);//打开小天平放水阀
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.smallWaterInNo);//关闭小天平进水阀
	wait(CYCLE_TIME);
	closeWaterPump();    //退出时关闭水泵
	closeAllFlowPointValves();//关闭所有流量点阀门
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.waterInNo);//关闭进水阀

	return true;
}

//关闭所有流量点阀门
int WaterStandardDlg50::closeAllFlowPointValves()
{
	closeValve(m_portsetinfo.bigNo);
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.middle2No);
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.middle1No);
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.smallNo);

	return true;
}

/*
** 功能：判断天平重量是否达到要求的检定量；计算检定过程的平均温度和平均流量(m3/h)
** 脉冲数会很大，超出float的范围，所以要用double
*/
int WaterStandardDlg50::judgeTartgetVolAndCalcAvgTemperAndFlow(double initV, double verifyV)
{
	double targetV = initV + verifyV;
	ui.tableWidget->setEnabled(false);
	ui.btnAllReadNO->setEnabled(false);
	ui.btnAllReadData->setEnabled(false);
	ui.btnAllVerifyStatus->setEnabled(false);
	QDateTime startTime = QDateTime::currentDateTime();
	int second = 0;
	double nowFlow = m_paraSetReader->getFpBySeq(m_nowOrder).fp_verify;
	double nowVol = initV;
	while (!m_stopFlag && (nowVol < targetV))
	{
		qDebug()<<"当前流水量 ="<<nowVol<<", 小于目标体积 "<<targetV;
		qDebug()<<"m_stopFlag: "<<m_stopFlag;
		m_avgTFCount++;
		m_pipeInTemper += ui.lcdInTemper->value();
		m_pipeOutTemper += ui.lcdOutTemper->value();
		second = 3.6*(targetV - nowVol)/nowFlow;
		ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
			.arg(m_nowOrder).arg(nowFlow));
		ui.labelHintProcess->setText(tr("Verifying...Please wait for about <font color=DarkGreen size=6><b>%1</b></font> second").arg(second));
		wait(CYCLE_TIME);		
		nowVol = ui.lcdAccumStdMeter->value();//记录标准表体积(L)
	}

	m_pipeInTemper = m_pipeInTemper/m_avgTFCount;   //入口平均温度
	m_pipeOutTemper = m_pipeOutTemper/m_avgTFCount; //出口平均温度

	QDateTime endTime = QDateTime::currentDateTime();
	int tt = startTime.secsTo(endTime);
	if (NULL==m_paraSetReader || m_stopFlag)
	{
		return false;
	}
	m_realFlow = 3.6*(m_paraSetReader->getFpBySeq(m_nowOrder).fp_quantity + ui.lcdAccumStdMeter->value() - targetV)/tt;
	ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
		.arg(m_nowOrder).arg(nowFlow));
		ui.labelHintProcess->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: Verify Finished!").arg(m_nowOrder));
	//if (m_nowOrder == m_flowPointNum)
	//{
	//	ui.labelHintProcess->setText(tr("All flow points has verified!"));
	//	ui.btnNext->hide();
	//}
	int ret = !m_stopFlag && (ui.lcdAccumStdMeter->value() >= targetV);
	return ret;
}

//清空表格，第一列除外("表号"列)
void WaterStandardDlg50::clearTableContents()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		for (int j=1; j<ui.tableWidget->columnCount(); j++) //从第二列开始
		{
			if (ui.tableWidget->item(i,j) == 0)
			{
				continue;
			}
			ui.tableWidget->item(i,j)->setText("");
		}
	}
// 	ui.tableWidget->clearContents(); //清空表格
}

//点击"开始"按钮
void WaterStandardDlg50::on_btnStart_clicked()
{
	m_repeatverify = m_nowParams->bo_repeatverify; //重置重复检测选项

	m_timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"); //记录时间戳
	m_nowDate = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //当前日期'2014-08-07'
	m_validDate = QDateTime::currentDateTime().addYears(VALID_YEAR).addDays(-1).toString("yyyy-MM-dd"); //有效期

	ui.btnStart->setEnabled(false);
	ui.btnGoOn->hide();
	ui.labelHintPoint->clear();
	ui.labelHintProcess->clear();
	ui.tableWidget->setEnabled(true);
	ui.btnAllReadNO->setEnabled(true);
	ui.btnAllReadData->setEnabled(true);
	ui.btnAllVerifyStatus->setEnabled(true);
	ui.btnAllAdjError->setEnabled(false);
	ui.btnAllModifyNO->setEnabled(false);
	
	m_stopFlag = false;
	m_state = STATE_INIT;
	m_validMeterNum = 0;

	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->item(i,COLUMN_METER_NUMBER)->setText("");
		ui.tableWidget->item(i, COLUMN_METER_NUMBER)->setForeground(QBrush());
		ui.tableWidget->item(i, COLUMN_DISP_ERROR)->setForeground(QBrush());
		ui.tableWidget->cellWidget(i, COLUMN_ADJUST_ERROR)->setEnabled(false);
		ui.tableWidget->cellWidget(i, COLUMN_ADJUST_ERROR)->setStyleSheet(";");
		ui.tableWidget->cellWidget(i, COLUMN_MODIFY_METERNO)->setEnabled(false);
	}
	clearTableContents();

	if (!startExhaustCountDown())
	{
		return;
	}

	if (m_autopick) //自动读表
	{
		on_btnAllReadNO_clicked();
// 		sleep(m_exaustSecond*1000/2);
// 		setAllMeterVerifyStatus();
	}
	else //手动读表
	{
		ui.labelHintPoint->setText(tr("Please input meter number!"));
		ui.tableWidget->setCurrentCell(0, COLUMN_METER_NUMBER);
	}

	return;
}

/*
** 点击"排气"按钮
*/
void WaterStandardDlg50::on_btnExhaust_clicked()
{

}

//点击"继续"按钮
void WaterStandardDlg50::on_btnGoOn_clicked()
{
	ui.btnGoOn->hide();
	startVerify();
}

//点击"终止检测"按钮
void WaterStandardDlg50::on_btnStop_clicked()
{
	int button = QMessageBox::question(this, tr("Question"), tr("Stop Really ?"), \
		QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	if (button == QMessageBox::No)
	{
		return;
	}

	stopVerify();
}

void WaterStandardDlg50::on_btnExit_clicked()
{
	stopVerify();
	this->close();
}

//停止检定
void WaterStandardDlg50::stopVerify()
{
	ui.labelHintPoint->clear();
	if (!m_stopFlag)
	{
		ui.labelHintProcess->setText(tr("stopping verify...please wait a minute"));
		m_stopFlag = true;
		m_exaustTimer->stop();//停止排气定时器
		closeAllValveAndPumpOpenOutValve();
		ui.labelHintProcess->setText(tr("Verify has Stoped!"));
	}
	closeAllRegulator();

	m_state = STATE_INIT; //重置初始状态

	ui.tableWidget->setEnabled(true);
	ui.btnAllReadNO->setEnabled(true);
	ui.btnAllReadData->setEnabled(true);
	ui.btnAllVerifyStatus->setEnabled(true);
	ui.btnStart->setEnabled(true);
}

//开始检定
void WaterStandardDlg50::startVerify()
{
	//释放内存
	if (m_meterErr != NULL)
	{
		for (int m=0; m<m_validMeterNum; m++)
		{
			delete []m_meterErr[m];
		}
		delete []m_meterErr;
		m_meterErr = NULL;
	}
	if (m_oldMeterCoe != NULL)
	{
		for (int n=0; n<m_validMeterNum; n++)
		{
			delete []m_oldMeterCoe[n];
		}
		delete []m_oldMeterCoe;
		m_oldMeterCoe = NULL;
	}

	getValidMeterNum(); //获取实际检表的个数(根据获取到的表号个数)
	if (m_validMeterNum <= 0)
	{
		ui.labelHintPoint->setText(tr("Please input meter number!\n then click \"GoOn\" button!"));
		QMessageBox::warning(this, tr("Error"), tr("Error: meter count is zero !\nPlease input meter number, then click \"GoOn\" button!"));//请输入表号！
		ui.tableWidget->setCurrentCell(0, COLUMN_METER_NUMBER);
		ui.btnGoOn->show();
		return;
	}
	if (m_validMeterNum != m_maxMeterNum)
	{
		ui.labelHintPoint->clear();
		QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Question"), \
			tr("meter count maybe error ! read meter number again?\nclick \'Yes\' to read meter again;or click \'No\' to continue verify"), \
			QMessageBox::Yes|QMessageBox::No, this);
		messageBox->setDefaultButton(QMessageBox::No); //默认继续检定
		QTimer timer;
		connect(&timer, SIGNAL(timeout()), messageBox, SLOT(close()));
		timer.start(8000);
		if (messageBox->exec()==QMessageBox::Yes)
		{
			ui.labelHintPoint->setText(tr("Please input meter number!\n then click \"GoOn\" button!"));
			ui.btnGoOn->show();
			return;
		}
	}

	if (m_recPtr != NULL)
	{
		delete []m_recPtr;
		m_recPtr = NULL;
	}
	m_recPtr = new Water_Verify_Record_STR[m_validMeterNum];
	memset(m_recPtr, 0, sizeof(Water_Verify_Record_STR)*m_validMeterNum);

	m_state = STATE_INIT; //初始状态

	//表初值
	if (m_meterStartValue != NULL)
	{
		delete []m_meterStartValue;
		m_meterStartValue = NULL;
	}
	m_meterStartValue = new float[m_validMeterNum];
	memset(m_meterStartValue, 0, sizeof(float)*m_validMeterNum);

	//表终值
	if (m_meterEndValue != NULL)
	{
		delete []m_meterEndValue;
		m_meterEndValue = NULL;
	}
	m_meterEndValue = new float[m_validMeterNum];   
	memset(m_meterEndValue, 0, sizeof(float)*m_validMeterNum);
	
	//表温度
	if (m_meterTemper != NULL)
	{
		delete []m_meterTemper;
		m_meterTemper = NULL;
	}
	m_meterTemper = new float[m_validMeterNum];     
	memset(m_meterTemper, 0, sizeof(float)*m_validMeterNum);

	//表密度	
	if (m_meterDensity != NULL)
	{
		delete []m_meterDensity;
		m_meterDensity = NULL;
	}
	m_meterDensity = new float[m_validMeterNum];    
	memset(m_meterDensity, 0, sizeof(float)*m_validMeterNum);

	//被检表的标准值
	if (m_meterStdValue != NULL)
	{
		delete []m_meterStdValue;
		m_meterStdValue = NULL;
	}
	m_meterStdValue = new float[m_validMeterNum];   
	memset(m_meterStdValue, 0, sizeof(float)*m_validMeterNum);

    //被检表的误差(不同表、当前流量点)	
	if (m_meterError != NULL)
	{
		delete []m_meterError;
		m_meterError = NULL;
	}
	m_meterError = new float[m_validMeterNum];      
	memset(m_meterError, 0, sizeof(float)*m_validMeterNum);

	//被检表的误差(不同表、不同流量点)
	m_meterErr = new float*[m_validMeterNum];    
	for (int i=0; i<m_validMeterNum; i++)
	{
		m_meterErr[i] = new float[VERIFY_POINTS];
		memset(m_meterErr[i], 0, sizeof(float)*VERIFY_POINTS);
	}

	//各个有效表位的热量表的原来的各流量点系数
	m_oldMeterCoe = new MeterCoe_PTR[m_validMeterNum];
	for (int j=0; j<m_validMeterNum; j++)
	{
		m_oldMeterCoe[j] = new MeterCoe_STR;
		m_oldMeterCoe[j]->bigCoe = 1.0;
		m_oldMeterCoe[j]->mid2Coe = 1.0;
		m_oldMeterCoe[j]->mid1Coe = 1.0;
		m_oldMeterCoe[j]->smallCoe = 1.0;
	}

	//所有流量点是否都合格
	if (m_meterResult != NULL)
	{
		delete []m_meterResult;
		m_meterResult = NULL;
	}
	m_meterResult = new int[m_validMeterNum];
	for (int p=0; p<m_validMeterNum; p++)
	{
		m_meterResult[p] = 1;
	}

	 //读取原来的流量点系数
	if (m_adjErr)
	{
		ui.labelHintPoint->setText(tr("read meter now coe ..."));
		m_state = STATE_READ_COE;
		readAllMeterFlowCoe();
		wait(WAIT_COM_TIME); 
		m_state = STATE_INIT;
	}

	if (m_continueVerify) //连续检定
	{
		wait(BALANCE_STABLE_TIME); //等待3秒钟(等待水流稳定)
	}

	m_nowOrder = 1;
	prepareVerifyFlowPoint(m_nowOrder); //第一个流量点检定
}

//获取有效检表个数,并生成映射关系（被检表下标-表位号）
int WaterStandardDlg50::getValidMeterNum()
{
	//匹配表号是否为数字; 
	//前一个'\'是转义字符, "\\"就相当于'\', "\\d"相当于'\d', 匹配一个数字, '+'是数字的正闭包;
	//本模式也可以写成 "[1-9]+"
	QRegExp rx("\\d+");

	m_validMeterNum = 0; //先清零
	QString meterNum;//表号
	for (int i=0; i<m_maxMeterNum; i++)
	{
		if (NULL == ui.tableWidget->item(i, COLUMN_METER_NUMBER)) //"表号"单元格为空
		{
			continue;
		}
		meterNum = ui.tableWidget->item(i, COLUMN_METER_NUMBER)->text();
		if (!rx.exactMatch(meterNum))
		{
			continue;
		}

		m_meterPosMap[m_validMeterNum] = i+1; //表位号从1开始
		m_validMeterNum++;
	}
	return m_validMeterNum;
}

/*
** 判断表位号是否有效(该表位是否需要检表)
输入:meterPos(表位号)，从1开始
返回:被检表下标，从0开始
*/
int WaterStandardDlg50::isMeterPosValid(int meterPos)
{
	for (int i=0; i<m_validMeterNum; i++)
	{
		if (m_meterPosMap[i] == meterPos)
		{
			return i;
		}
	}
	return -1;
}

/*
** 准备单个流量点的检定，进行必要的检查
** 注意：order从1开始
*/
int WaterStandardDlg50::prepareVerifyFlowPoint(int order)
{
	if (order < 1 || order > m_flowPointNum || m_stopFlag)
	{
		return false;
	}

	if (!m_continueVerify) //非连续检定，每次检定开始之前都要判断天平容量
	{
		wait(BALANCE_STABLE_TIME);   //等待3秒钟，等待水流稳定
	}

	int i=0;
	if (m_resetZero) //初值回零
	{
			ui.labelHintPoint->setText(tr("Reset Zero"));
			while (i < RESET_ZERO_TIME && !m_stopFlag) //等待被检表初值回零
			{
				ui.labelHintProcess->setText(tr("please wait <font color=DarkGreen size=6><b>%1</b></font> seconds for reset zero").arg(RESET_ZERO_TIME-i));
				i++;
				wait(CYCLE_TIME); 
			}
	}
// 	else //初值不回零
// 	{
// 		if (order == 1) //第一次检定
// 		{
// 			setAllMeterVerifyStatus();
// 		}
// 	}

// 	clearTableContents();
	getMeterStartValue(); //获取表初值
	
	return true;
}

/************************************************************************/
/* 进行单个流量点的检定                                                               
/* 对于DN25以下的表, 之所以现今最流行的检测方法使质量法, 就是利用管道中的定常流
/* 的质量守恒原理, 
/* 即处于管道中的定常流动状态的液体,在任意管道界面上, 它的流进和流出液体质量相等.
/* 所以在进行标准表检定时, 必须将标准表的体积换算为对应温度下的质量.
/************************************************************************/
int WaterStandardDlg50::startVerifyFlowPoint(int order)
{
	if (m_stopFlag)
	{
		return false;
	}

	//判断初值是否全部读取成功
	bool ok;
	int row = 0;
	for (int p=0; p<m_validMeterNum; p++)
	{
		row = m_meterPosMap[p]-1;
		ui.tableWidget->item(row, COLUMN_METER_START)->text().toFloat(&ok);
		if (!ok || ui.tableWidget->item(row, COLUMN_METER_START)->text().isEmpty())
		{
			ui.labelHintProcess->setText(tr("please input start value of heat meter"));
			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START); //定位到需要输入初值的地方
			return false;
		}
	}

	m_pipeInTemper = ui.lcdInTemper->value();
	m_pipeOutTemper = ui.lcdOutTemper->value();
	m_realFlow = ui.lcdFlowRate->value();
	m_avgTFCount = 1;

	int regNO = m_paraSetReader->getFpBySeq(order).fp_regno; //order对应的调节阀端口号
	int opening = m_paraSetReader->getFpBySeq(order).fp_opening; //order对应的调节阀开度
	int portNo = m_paraSetReader->getFpBySeq(order).fp_valve;  //order对应的阀门端口号
	float verifyQuantity = m_paraSetReader->getFpBySeq(order).fp_quantity; //第order次检定对应的检定量
	float frequence = m_paraSetReader->getFpBySeq(order).fp_freq; //order对应的频率
	m_controlObj->askSetDriverFreq(frequence);
	while (m_RegSpinBox[regNO]->value() != m_RegLineEdit[regNO]->text().toInt()) //调节阀未到位
	{
		ui.labelHintProcess->setText(tr("please wait for regulator..."));
		ui.labelHintPoint->clear();
		wait(1000);
	}

	m_stdStartVol = ui.lcdAccumStdMeter->value();//记录标准表初始体积(L)
	qDebug() << "start volumn: " << m_stdStartVol;

	if (portNo == m_lastPortNO) //同一条管路连续跑
	{
		while (m_RegLineEdit[regNO]->text().toInt() > 0)
		{
			ui.labelHintProcess->setText(tr("regulator is closing, please wait..."));
			wait(1000);
		}
		setRegulatorOpening(regNO, opening);
		ui.labelHintProcess->setText(tr("regulator is opening, please wait..."));
		while (m_RegLineEdit[regNO]->text().toInt() < opening)
		{
			wait(1000);
		}
	}

	if (openValve(portNo)) //打开阀门，开始跑流量
	{
		m_lastPortNO = portNo;
		if (judgeTartgetVolAndCalcAvgTemperAndFlow(m_stdStartVol, verifyQuantity)) //跑完检定量并计算此过程的平均温度和平均流量
		{
			ui.tableWidget->setEnabled(true);
			ui.btnAllReadNO->setEnabled(true);
			ui.btnAllReadData->setEnabled(true);
			ui.btnAllVerifyStatus->setEnabled(true);
			setRegulatorOpening(regNO, 0); //关闭调节阀
			closeValve(portNo); //关闭order对应的阀门
			wait(BALANCE_STABLE_TIME); //等待3秒钟，让天平数值稳定

			m_stdEndVol = ui.lcdAccumStdMeter->value();//记录标准表最终体积(L)
			qDebug() << "end volumn: " << m_stdEndVol;
			float stdAvgDen = m_chkAlg->getDensityByQuery(m_pipeOutTemper);//标准表最终平均密度(kg/L)
			if(stdAvgDen<0)
			{
				return -1;
			}
			float stdMeterMass = stdAvgDen * (m_stdEndVol - m_stdStartVol);
			if (!m_resetZero && m_nowOrder>=2)
			{
				m_state = STATE_END_VALUE;
				makeStartValueByLastEndValue();
			}
			for (int m=0; m<m_validMeterNum; m++) //
			{
				m_meterTemper[m] = m_chkAlg->getMeterTempByPos(m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m]);//计算每个被检表的温度
				m_meterDensity[m] = m_chkAlg->getDensityByQuery(m_meterTemper[m]);//计算每个被检表的密度
				//计算每个被检表的体积标准值
				m_meterStdValue[m] = m_chkAlg->getStdVolByPos(stdMeterMass, m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m], STANDARD_METHOD);

				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_FLOW_POINT)->setText(QString::number(m_realFlow, 'f', 3));//流量点
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_METER_END)->setText("");//表终值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_START)->setText(QString::number(m_stdStartVol, 'f', 3));//初值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_END)->setText(QString::number(m_stdEndVol, 'f', 3));    //终值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_TEMPER)->setText(QString::number(m_meterTemper[m], 'f', 2));  //温度
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DENSITY)->setText(QString::number(m_meterDensity[m], 'f', 3));//密度
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_STD_VALUE)->setText(QString::number(m_meterStdValue[m], 'f', 3));//标准值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setText("");//示值误差
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setForeground(QBrush());//示值误差
			}
			getMeterEndValue();
		} //跑完检定量
	}

	return true;
}

/*
** 计算所有被检表的误差
*/
int WaterStandardDlg50::calcAllMeterError()
{
	int ret = 1;
	for (int i=0; i<m_validMeterNum; i++)
	{
		ret *= calcMeterError(i);
	}
	return ret; 
}

/*
** 计算某个被检表的误差
** 输入参数：
**     idx:被检表数组的索引
*/
int WaterStandardDlg50::calcMeterError(int idx)
{
	bool ok;
	int row = m_meterPosMap[idx] - 1;
	float endV = ui.tableWidget->item(row, COLUMN_METER_END)->text().toFloat(&ok);
	if (/*m_meterEndValue[idx] <= 0 ||*/ ui.tableWidget->item(row, COLUMN_METER_END)->text().isEmpty() || !ok)
	{
// 		ui.tableWidget->setCurrentCell(row, COLUMN_METER_END);
		return 0;
	}
	m_meterError[idx] = 100*(m_meterEndValue[idx] - m_meterStartValue[idx] - m_meterStdValue[idx])/m_meterStdValue[idx];//计算某个表的误差
	int valveIdx = m_paraSetReader->getFpBySeq(m_nowOrder).fp_valve_idx; //0:大 1:中二 2:中一 3:小
	m_meterErr[idx][valveIdx] = m_meterError[idx];
	ui.tableWidget->item(row, COLUMN_DISP_ERROR)->setText(QString::number(m_meterError[idx], 'f', ERR_PRECISION)); //示值误差
	float Q2 = m_nowParams->Q2BiQ1 * m_nowParams->Q3 / m_nowParams->Q3BiQ1;
	float stdError = m_flowSC*getWaterMeterStdError(Q2, m_nowParams->m_grade , m_meterTemper[idx], m_realFlow); //标准误差=规程要求误差*流量安全系数
	ui.tableWidget->item(row, COLUMN_STD_ERROR)->setText("±" + QString::number(stdError, 'f', ERR_PRECISION)); //标准误差
	if (fabs(m_meterError[idx]) > stdError)
	{
		ui.tableWidget->item(row, COLUMN_DISP_ERROR)->setForeground(QBrush(Qt::red));
		ui.tableWidget->item(row, COLUMN_METER_NUMBER)->setForeground(QBrush(Qt::red));
		ui.tableWidget->cellWidget(row, COLUMN_ADJUST_ERROR)->setStyleSheet("color: rgb(255, 0, 0);");
	}
	QString meterNoStr = m_numPrefix + QString("%1").arg(ui.tableWidget->item(row, 0)->text(), 8, '0');

	strncpy_s(m_recPtr[idx].timestamp, m_timeStamp.toAscii(), TIMESTAMP_LEN);
	m_recPtr[idx].flowPoint = m_realFlow;
	strcpy_s(m_recPtr[idx].meterNo, meterNoStr.toAscii());
	m_recPtr[idx].flowPointIdx = m_nowOrder; //
	m_recPtr[idx].methodFlag = STANDARD_METHOD; //标准表法
	m_recPtr[idx].meterValue0 = m_meterStartValue[idx];
	m_recPtr[idx].meterValue1 = m_meterEndValue[idx];
	m_recPtr[idx].stdMeterV0 = m_stdStartVol;
	m_recPtr[idx].stdMeterV1 = m_stdEndVol;
	m_recPtr[idx].pipeTemper = m_meterTemper[idx]; 
	m_recPtr[idx].density = m_meterDensity[idx];
	m_recPtr[idx].stdValue = m_meterStdValue[idx];
	m_recPtr[idx].dispError = m_meterError[idx];
	m_recPtr[idx].grade = m_nowParams->m_grade;
	m_recPtr[idx].stdError = stdError; //表的标准误差
	m_recPtr[idx].result = (fabs(m_recPtr[idx].dispError) <= fabs(m_recPtr[idx].stdError)) ? 1 : 0;
	m_meterResult[idx] *= m_recPtr[idx].result;
	m_recPtr[idx].meterPosNo = m_meterPosMap[idx];
	m_recPtr[idx].standard = m_standard;
	m_recPtr[idx].model = m_model;
	m_recPtr[idx].pickcode = m_pickcode; //采集代码
	m_recPtr[idx].manufactDept = m_nowParams->m_manufac;
	m_recPtr[idx].verifyDept = m_nowParams->m_vcomp;
	m_recPtr[idx].verifyPerson = m_nowParams->m_vperson;
	m_recPtr[idx].checkPerson = m_nowParams->m_cperson;
	strncpy_s(m_recPtr[idx].verifyDate, m_nowDate.toAscii(), DATE_LEN);
	strncpy_s(m_recPtr[idx].validDate, m_validDate.toAscii(), DATE_LEN);
	m_recPtr[idx].airPress = m_nowParams->m_airpress.toFloat();
	m_recPtr[idx].envTemper = m_nowParams->m_temper.toFloat();
	m_recPtr[idx].envHumidity = m_nowParams->m_humidity.toFloat();
	m_recPtr[idx].flowcoe = m_nowParams->sc_flow;
	m_recPtr[idx].deviceInfoId = m_readComConfig->getDeviceInfoID();
	m_recPtr[idx].waterPress = 0.6;
	m_recPtr[idx].Q3BiQ1 = m_nowParams->Q3BiQ1;
	m_recPtr[idx].Q3 = m_nowParams->Q3;
	m_recPtr[idx].Q2BiQ1 = m_nowParams->Q2BiQ1;

	return 1; 
}

/*
** 输完热量表终值后，计算检定结果
** 返回值：1-计算成功、读表数据全部成功
		   0-计算失败、读表数据有失败的
*/
int WaterStandardDlg50::calcVerifyResult()
{
	int ret = 0;
	ret = calcAllMeterError();

	if (ret) //读表流量都成功（终值）
	{
		saveAllVerifyRecords();
		ui.labelHintProcess->setText(tr("save database successfully!"));
		if (m_nowOrder>=m_flowPointNum) //最后一个流量点
		{
			stopVerify(); //停止检定
			if (m_adjErr) //选择调整误差
			{
				ui.btnAllAdjError->setEnabled(true);
				for (int i=0; i<m_validMeterNum; i++)
				{
					ui.tableWidget->cellWidget(m_meterPosMap[i]-1, COLUMN_ADJUST_ERROR)->setEnabled(true);
				}
			}
			if (m_writeNO) //选择写表号
			{
				ui.btnAllModifyNO->setEnabled(true);
				for (int n=0; n<m_validMeterNum; n++)
				{
					ui.tableWidget->cellWidget(m_meterPosMap[n]-1, COLUMN_MODIFY_METERNO)->setEnabled(true);
				}
				//如果不是新表，请将热量表调至"温差"状态！
				QMessageBox::information(this, tr("Hint"), tr("if it isn't new meter , please change meter to \"deltaT\" status !"));
				for (int j=0; j<m_validMeterNum; j++)
				{
					if (m_meterResult[j]) //所有流量点都合格才修改表号
					{
						m_mapMeterPosAndNewNO[m_meterPosMap[j]] = m_newMeterNO;
						slotModifyMeterNO(m_meterPosMap[j]-1);
						modifyFlowVerifyRec_MeterNO(m_numPrefix + QString::number(m_newMeterNO), m_timeStamp, m_meterPosMap[j]);
						m_newMeterNO++;
					}
				}
				saveStartMeterNO();
			}
			exportReport();
			if (m_repeatverify) //重复检测
			{
				if (m_adjErr) //调整误差
				{
					on_btnAllAdjError_clicked();
				}
				wait(WAIT_COM_TIME);
				on_btnStart_clicked();
				m_repeatverify = false;
			}
		}
		else //不是最后一个流量点
		{
			prepareVerifyFlowPoint(++m_nowOrder);
		}
	}
	else //有读表流量失败的（终值）
	{
		ui.labelHintProcess->setText(tr("please input end value of heat meter"));
	}

	return ret;
}

void WaterStandardDlg50::exportReport()
{
	QString sqlCondition = QString("F_TimeStamp=\'%1\' and F_MethodFlag = 1").arg(m_timeStamp);
	QString xlsname = QDateTime::fromString(m_timeStamp, "yyyy-MM-dd HH:mm:ss.zzz").toString("yyyy-MM-dd_hh-mm-ss") + ".xls";
	try
	{
		QString defaultPath = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\report\\water\\std\\";
		CReport rpt(sqlCondition);
		rpt.setIniName("rptconfig_water_std.ini");
		rpt.writeRpt();
		rpt.saveTo(defaultPath + xlsname);
		ui.labelHintProcess->setText(tr("Verify has Stoped!") + "\n" + tr("export excel file successful!"));
	}
	catch (QString e)
	{
		QMessageBox::warning(this, tr("Error"), e);
	}
}

//打开阀门
int WaterStandardDlg50::openValve(UINT8 portno)
{	
	if (NULL == m_controlObj)
	{
		return false;
	}
	if (portno <= 8)
	{
		m_controlObj->askControlRelay(portno, VALVE_OPEN);
	}
	else
	{
		m_controlObj2->askControlRelay(portno-8, VALVE_OPEN);
	}
	if (m_portsetinfo.version==OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(portno, VALVE_OPEN);
	}
	return true;
}

//关闭阀门
int WaterStandardDlg50::closeValve(UINT8 portno)
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	if (portno <= 8)
	{
		m_controlObj->askControlRelay(portno, VALVE_CLOSE);
	}
	else
	{
		m_controlObj2->askControlRelay(portno-8, VALVE_CLOSE);
	}
	if (m_portsetinfo.version==OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(portno, VALVE_CLOSE);
	}
	return true;
}

//操作阀门：打开或者关闭
int WaterStandardDlg50::operateValve(UINT8 portno)
{
	if (m_valveStatus[portno]==VALVE_OPEN) //阀门原来是打开状态
	{
		closeValve(portno);
	}
	else //阀门原来是关闭状态
	{
		openValve(portno);
	}
	return true;
}

//打开水泵
int WaterStandardDlg50::openWaterPump()
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_OPEN);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_OPEN);
	}
	return true;
}

//关闭水泵
int WaterStandardDlg50::closeWaterPump()
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_CLOSE);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_CLOSE);
	}
	return true;
}

//操作水泵 打开或者关闭
int WaterStandardDlg50::operateWaterPump()
{
	if (m_valveStatus[m_portsetinfo.pumpNo]==VALVE_OPEN) //水泵原来是打开状态
	{
		closeWaterPump();
	}
	else //水泵原来是关闭状态
	{
		openWaterPump();
	}
	return true;
}

//响应阀门状态设置成功
void WaterStandardDlg50::slotSetValveBtnStatus(const UINT8 &portno, const bool &status)
{
	m_valveStatus[portno] = status;
	setValveBtnBackColor(m_valveBtn[portno], m_valveStatus[portno]);
}

//响应调节阀调节成功
void WaterStandardDlg50::slotSetRegulateOk()
{
// 	setRegBtnBackColor(m_regBtn[m_nowRegIdx], true);
}


//自动读取表号成功 显示表号
void WaterStandardDlg50::slotSetMeterNumber(const QString& comName, const QString& meterNo)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	if (m_state == STATE_INIT)
	{
		ui.tableWidget->item(meterPos-1, COLUMN_METER_NUMBER)->setText(meterNo.right(8)); //表号
	}
}

/*
** 自动读取表流量成功 显示表流量
*/
void WaterStandardDlg50::slotSetMeterFlow(const QString& comName, const QString& flow)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}

	int idx = isMeterPosValid(meterPos);
	if (idx < 0) //该表位不检表，当然也不需要读表数据
	{
		return;
	}

	if (m_state == STATE_START_VALUE) //初值
	{
		ui.tableWidget->item(meterPos - 1, COLUMN_METER_START)->setText(flow);
	}
	else if (m_state == STATE_END_VALUE) //终值
	{
		ui.tableWidget->item(meterPos - 1, COLUMN_METER_END)->setText(flow);
	}
}

/*
** 自动读取表流量成功 刷新大流量点系数
*/
void WaterStandardDlg50::slotFreshBigCoe(const QString& comName, const QString& bigCoe)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}
	if (m_state == STATE_READ_COE)
	{
// 		m_meterErr[idx][0] = calcErrorValueOfCoe(bigCoe);
		m_oldMeterCoe[idx]->bigCoe = calcFloatValueOfCoe(bigCoe) ;
		qDebug()<<comName<<"表位 ="<<meterPos<<", 有效索引 ="<<idx<<", bigCoe ="<<m_oldMeterCoe[idx]->bigCoe;
	}
}

/*
** 自动读取表流量成功 刷新中二流量点系数
*/
void WaterStandardDlg50::slotFreshMid2Coe(const QString& comName, const QString& mid2Coe)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}

	if (m_state == STATE_READ_COE)
	{
// 		m_meterErr[idx][1] = calcErrorValueOfCoe(mid2Coe);
		m_oldMeterCoe[idx]->mid2Coe = calcFloatValueOfCoe(mid2Coe) ;
		qDebug()<<comName<<"表位 ="<<meterPos<<", 有效索引 ="<<idx<<", mid2Coe ="<<m_oldMeterCoe[idx]->mid2Coe;
	}
}

/*
** 自动读取表流量成功 刷新中一流量点系数
*/
void WaterStandardDlg50::slotFreshMid1Coe(const QString& comName, const QString& mid1Coe)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}

	if (m_state == STATE_READ_COE)
	{
// 		m_meterErr[idx][2] = calcErrorValueOfCoe(mid1Coe);
		m_oldMeterCoe[idx]->mid1Coe = calcFloatValueOfCoe(mid1Coe) ;
		qDebug()<<comName<<"表位 ="<<meterPos<<", 有效索引 ="<<idx<<", mid1Coe ="<<m_oldMeterCoe[idx]->mid1Coe;
	}
}

/*
** 自动读取表流量成功 刷新小流量点系数
*/
void WaterStandardDlg50::slotFreshSmallCoe(const QString& comName, const QString& smallCoe)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}

	if (m_state == STATE_READ_COE)
	{
// 		m_meterErr[idx][3] = calcErrorValueOfCoe(smallCoe);
		m_oldMeterCoe[idx]->smallCoe = calcFloatValueOfCoe(smallCoe) ;
		qDebug()<<comName<<"表位 ="<<meterPos<<", 有效索引 ="<<idx<<", smallCoe ="<<m_oldMeterCoe[idx]->smallCoe;
	}
}

//设置阀门按钮背景色
void WaterStandardDlg50::setValveBtnBackColor(QToolButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //阀门打开 绿色
	{
		btn->setStyleSheet("background-color:rgb(0,255,0);border:0px;border-radius:10px;");
// 		btn->setStyleSheet("border-image:url(:/weightmethod/images/btncheckon.png)");
	}
	else //阀门关闭 红色
	{
		btn->setStyleSheet("background-color:rgb(255,0,0);border:0px;border-radius:10px;");
// 		btn->setStyleSheet("border-image:url(:/weightmethod/images/btncheckoff.png)");
	}
}

//设置调节阀按钮背景色
void WaterStandardDlg50::setRegBtnBackColor(QPushButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //调节成功
	{
		btn->setStyleSheet("background:blue;border:0px;");
	}
	else //调节失败
	{
		btn->setStyleSheet("");
	}
}

//参数设置
void WaterStandardDlg50::on_btnParaSet_clicked()
{
	if (NULL == m_paraSetDlg)
	{
		m_paraSetDlg = new ParaSetDlg();
	}
	else
	{
		delete m_paraSetDlg;
		m_paraSetDlg = new ParaSetDlg();
	}
	connect(m_paraSetDlg, SIGNAL(saveSuccessSignal()), this, SLOT(readNowParaConfig()));
	m_paraSetDlg->showWaterPara();
	m_paraSetDlg->show();
}

/*
** 控制继电器开断
*/
void WaterStandardDlg50::on_btnWaterIn_clicked() //进水阀
{
	m_nowPortNo = m_portsetinfo.waterInNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnBigWaterIn_clicked() //大天平进水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterInNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnBigWaterOut_clicked() //大天平放水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterOutNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnSmallWaterIn_clicked() //小天平进水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterInNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnSmallWaterOut_clicked() //小天平放水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterOutNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnValveBig_clicked() //大流量阀
{
	m_nowPortNo = m_portsetinfo.bigNo;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnValveMiddle1_clicked() //中流一阀
{
	m_nowPortNo = m_portsetinfo.middle1No;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnValveMiddle2_clicked() //中流二阀
{
	m_nowPortNo = m_portsetinfo.middle2No;
	operateValve(m_nowPortNo);
}

void WaterStandardDlg50::on_btnValveSmall_clicked() //小流量阀
{
	m_nowPortNo = m_portsetinfo.smallNo;
	operateValve(m_nowPortNo);
}

/*
** 控制水泵开关
*/
void WaterStandardDlg50::on_btnWaterPump_clicked()
{
/*	if (ui.spinBoxFrequency->value() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("please input frequency of transducer"));//请设置变频器频率！
		ui.spinBoxFrequency->setFocus();
	}
 	m_controlObj->askControlRegulate(m_portsetinfo.pumpNo, ui.spinBoxFrequency->value());
*/
	m_nowPortNo = m_portsetinfo.pumpNo;
	operateWaterPump();
}

/*
** 设置变频器频率
*/
void WaterStandardDlg50::on_btnSetFreq_clicked()
{
	if (NULL == m_controlObj)
	{
		return;
	}
	m_controlObj->askSetDriverFreq(ui.spinBoxFreq->value());
}

//获取表初值
int WaterStandardDlg50::getMeterStartValue()
{
	if (m_stopFlag)
	{
		return false;
	}

	float nowFlow = m_paraSetReader->getFpBySeq(m_nowOrder).fp_verify;
	ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
		.arg(m_nowOrder).arg(nowFlow));
	m_state = STATE_START_VALUE;
	if (m_resetZero) //初值回零
	{
		memset(m_meterStartValue, 0, sizeof(float)*m_validMeterNum);
		for (int j=0; j<m_validMeterNum; j++)
		{
			ui.tableWidget->item(m_meterPosMap[j]-1, COLUMN_METER_START)->setText(""); //清空初值列
			ui.tableWidget->item(m_meterPosMap[j]-1, COLUMN_METER_START)->setText(QString("%1").arg(m_meterStartValue[j]));
		}
	}
	else //初值不回零
	{
		if (m_nowOrder <= 1)
		{
			if (m_autopick) //自动采集
			{
				wait(WAIT_COM_TIME); //需要等待，否则热表来不及响应通讯
				ui.labelHintProcess->setText(tr("please input start value of heat meter"));
				on_btnAllReadData_clicked();
//	 			wait(WAIT_COM_TIME); //等待串口返回数据
			}
			else //手动输入
			{
				ui.labelHintProcess->setText(tr("please input start value of heat meter"));
				ui.tableWidget->setCurrentCell(m_meterPosMap[0]-1, COLUMN_METER_START); //定位到第一个需要输入初值的地方
				return false;
			}
		}
		else //m_nowOrder >= 2
		{
// 			makeStartValueByLastEndValue();
			startVerifyFlowPoint(m_nowOrder);
		}
	}
	return true;
}

//获取表终值
int WaterStandardDlg50::getMeterEndValue()
{
	if (m_stopFlag)
	{
		return false;
	}

	ui.labelHintProcess->setText(tr("please input end value of heat meter"));
	m_state = STATE_END_VALUE;

	if (m_autopick) //自动采集
	{
		on_btnAllReadData_clicked();
		/*
		下面一行必须封掉，否则若有自动读表失败的，手动输入数据后，下一流量点跑完后，会出bug：
		QObject.cpp
		void QObject::installEventFilter(QObject *obj)
		qWarning("QObject::installEventFilter(): Cannot filter events for objects in a different thread.");
		*/
// 		sleep(WAIT_COM_TIME); //等待串口返回数据
	}
	else //手动输入
	{
// 		QMessageBox::information(this, tr("Hint"), tr("please input end value of heat meter"));//请输入热量表终值！
		ui.tableWidget->setCurrentCell(m_meterPosMap[0]-1, COLUMN_METER_END); //定位到第一个需要输入终值的地方
		return false;
	}
	return true;
}

//上一次的终值作为本次的初值
void WaterStandardDlg50::makeStartValueByLastEndValue()
{
// 	clearTableContents();
	for (int i=0; i<m_validMeterNum; i++)
	{
// 		m_meterStartValue[i] = m_meterEndValue[i];
		m_meterStartValue[i] = m_recPtr[i].meterValue1;
		ui.tableWidget->item(m_meterPosMap[i]-1, COLUMN_METER_START)->setText(QString("%1").arg(m_meterStartValue[i]));
	}
}
/*
** 响应处理用户输入完表初值、表终值
   输入参数：
      row：行数，从0开始
	  column：列数，从0开始
*/
void WaterStandardDlg50::on_tableWidget_cellChanged(int row, int column)
{
	if (!m_autopick && column==COLUMN_METER_NUMBER && m_state==STATE_INIT) //手动输入 表号列 初始状态
	{
		ui.tableWidget->setCurrentCell(row+1, column);
	}

	if (NULL==ui.tableWidget->item(row,  column) || NULL==m_meterStartValue || NULL==m_meterEndValue)
	{
		return;
	}

	int meterPos = row + 1; //表位号
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}

	bool ok;
	if (column==COLUMN_METER_START && m_state==STATE_START_VALUE) //表初值列 且 允许输入初值
	{
		m_meterStartValue[idx] = ui.tableWidget->item(row, column)->text().toFloat(&ok);
		if (!ok)
		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Error: please input right digits"));//输入错误！请输入正确的数字
// 			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START);
			return;
		}
		startVerifyFlowPoint(m_nowOrder);
	}

	if (column==COLUMN_METER_END && m_state==STATE_END_VALUE) //表终值列 且 允许输入终值
	{
		m_meterEndValue[idx] = ui.tableWidget->item(row, column)->text().toFloat(&ok);
		if (!ok)
		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Error: please input right digits"));//输入错误！请输入正确的数字
// 			ui.tableWidget->setCurrentCell(row, COLUMN_METER_END);
			return;
		}
// 		calcVerifyResult();
		if (calcVerifyResult()==0 && meterPos<m_meterPosMap[m_validMeterNum-1])//不是最后一个表终值,自动定位到下一个
		{
			ui.tableWidget->setCurrentCell(m_meterPosMap[idx+1]-1, column);
		}
	}
}

/*
** 保存所有被检表的检定记录
*/
int WaterStandardDlg50::saveAllVerifyRecords()
{
	int ret = insertWaterVerifyRec(m_recPtr, m_validMeterNum);
	if (ret != OPERATE_DB_OK)
	{
		QMessageBox::warning(this, tr("Error"), tr("Error:insert database failed!\n") + tr("Maybe network error!"));
	}
	return ret;
}

//请求读表号（所有表、广播地址读表）
void WaterStandardDlg50::on_btnAllReadNO_clicked()
{
	qDebug()<<"on_btnAllReadNO_clicked...";
	int idx = -1;
	for (int j=0; j<m_maxMeterNum; j++)
	{
		idx = isMeterPosValid(j+1);
		if (m_state == STATE_START_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_START)->setText("");
			if (idx >= 0)
			{
				m_meterStartValue[idx] = 0;
			}
		}
		else if (m_state == STATE_END_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_END)->setText("");
			if (idx >= 0)
			{
				m_meterEndValue[idx] = 0;
			}
		}
		slotReadNO(j);
	}
}

//请求读表数据（所有表、广播地址读表）
void WaterStandardDlg50::on_btnAllReadData_clicked()
{
	qDebug()<<"on_btnAllReadData_clicked...";
	int idx = -1;
	for (int j=0; j<m_maxMeterNum; j++)
	{
		idx = isMeterPosValid(j+1);
		if (m_state == STATE_START_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_START)->setText("");
			if (idx >= 0)
			{
				m_meterStartValue[idx] = 0;
			}
		}
		else if (m_state == STATE_END_VALUE)
		{
			ui.tableWidget->item(j, COLUMN_METER_END)->setText("");
			if (idx >= 0)
			{
				m_meterEndValue[idx] = 0;
			}
		}
		slotReadData(j);
	}
}

//设置检定状态（所有表）
void WaterStandardDlg50::on_btnAllVerifyStatus_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotVerifyStatus(i);
	}
}

//调整误差（所有表）
void WaterStandardDlg50::on_btnAllAdjError_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotAdjustError(i);
	}
}

//修改表号（所有表）
void WaterStandardDlg50::on_btnAllModifyNO_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotModifyMeterNO(i);
	}
}

/*
** 修改表号
** 输入参数：
	row:行号，由row可以知道当前热表对应的串口、表号、误差等等
   注意：表号为14位
*/
void WaterStandardDlg50::slotModifyMeterNO(const int &row)
{
	qDebug()<<"slotModifyMeterNO row ="<<row;
	int meterPos = row + 1; //表位号
	int idx = -1;
	idx = isMeterPosValid(meterPos);
	qDebug()<<"slotModifyMeterNO idx ="<<idx;
	if (idx < 0 || !m_writeNO)
	{
		return;
	}
	if (!m_meterResult[idx])//任一流量点检定结果不合格
	{
		return;
	}

	QString meterNo = m_recPtr[idx].meterNo;
	QString newMeterNo = m_numPrefix + QString("%1").arg(m_mapMeterPosAndNewNO[meterPos]);
	m_meterObj[row].askModifyMeterNO(meterNo, newMeterNo);
	qDebug()<<"slotModifyMeterNO: "<<meterNo<<", "<<newMeterNo;
	ui.tableWidget->item(row, COLUMN_METER_NUMBER)->setText(newMeterNo.right(8));
}

/*
** 调整误差
** 输入参数：
	row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void WaterStandardDlg50::slotAdjustError(const int &row)
{
	qDebug()<<"slotAdjustError row ="<<row;
	int idx = isMeterPosValid(row+1);
	if (idx < 0)
	{	
		return;
	}
	qDebug()<<"slotAdjustError idx ="<<idx;
	if (qAbs(m_meterErr[idx][0])>MAX_ERROR || qAbs(m_meterErr[idx][1])>MAX_ERROR || qAbs(m_meterErr[idx][2])>MAX_ERROR || qAbs(m_meterErr[idx][3])>MAX_ERROR)
	{
		qDebug()<<"slotAdjustError idx ="<<idx<<", 误差过大，不能修正";
		return;
	}
	QString meterNO = m_numPrefix + ui.tableWidget->item(row, COLUMN_METER_NUMBER)->text();
	m_meterObj[row].askModifyFlowCoe(meterNO, m_meterErr[idx][0], m_meterErr[idx][1], m_meterErr[idx][2], m_meterErr[idx][3], m_oldMeterCoe[idx]);
}

/*
** 读表数据
** 输入参数：
row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void WaterStandardDlg50::slotReadData(const int &row)
{
	qDebug()<<"slotReadData row ="<<row;
	m_meterObj[row].askReadMeterData();
}

/*
** 检定状态
** 输入参数：
row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void WaterStandardDlg50::slotVerifyStatus(const int &row)
{
	qDebug()<<"slotVerifyStatus row ="<<row;
	m_meterObj[row].askSetVerifyStatus(VTYPE_FLOW);
}

/*
** 读表号
** 输入参数：
row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void WaterStandardDlg50::slotReadNO(const int &row)
{
	qDebug()<<"slotReadNO row ="<<row;
	m_meterObj[row].askReadMeterNO();
}

/*
** 保存起始表号
*/
void WaterStandardDlg50::saveStartMeterNO()
{
	QString filename = getFullIniFileName("verifyparaset.ini");//配置文件的文件名
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");//解决向ini文件中写汉字乱码
	settings.setValue("Other/meternumber", m_newMeterNO);
}

/*
** 打开4路电动调节阀至设定的开度
** 注意：选中的管路，将调节阀开度调整到设定开度；
         未选中的管路，将将调节阀开度调整到50%，用于排气
*/
void WaterStandardDlg50::openAllRegulator()
{
	int regNO = 0;
	float opening = 0;
	int valve_idx = 0;
	QString last_valve_idx;
	for (int i=1; i<=m_flowPointNum; i++) //选中的管路，将调节阀开度调整到设定开度
	{
		regNO = m_paraSetReader->getFpBySeq(i).fp_regno;
		opening = m_paraSetReader->getFpBySeq(i).fp_opening;
		valve_idx = m_paraSetReader->getFpBySeq(i).fp_valve_idx;
		if (last_valve_idx.contains(QString("%1").arg(valve_idx))) //同一管路跑多个流量点
		{
			continue;
		}
		switch (valve_idx)
		{
		case 0: //大流量点
			ui.spinBoxOpeningBig->setValue(opening);
			on_btnRegulateBig_clicked();
			break;
		case 1: //中二流量点
			ui.spinBoxOpeningMid2->setValue(opening);
			on_btnRegulateMid2_clicked();
			break;
		case 2: //中一流量点
			ui.spinBoxOpeningMid1->setValue(opening);
			on_btnRegulateMid1_clicked();
			break;
		case 3: //小流量点
			ui.spinBoxOpeningSmall->setValue(opening);
			on_btnRegulateSmall_clicked();
			break;
		default:
			break;
		}
		last_valve_idx += QString("%1").arg(valve_idx);
	}

	//未选中的管路，将调节阀开度调整到50%，用于排气
	if (ui.spinBoxOpeningSmall->value()==0)
	{
		ui.spinBoxOpeningSmall->setValue(50);
		on_btnRegulateSmall_clicked();
	}
	if (ui.spinBoxOpeningMid1->value()==0)
	{
		ui.spinBoxOpeningMid1->setValue(50);
		on_btnRegulateMid1_clicked();
	}
	if (ui.spinBoxOpeningMid2->value()==0)
	{
		ui.spinBoxOpeningMid2->setValue(50);
		on_btnRegulateMid2_clicked();
	}
	if (ui.spinBoxOpeningBig->value()==0)
	{
		ui.spinBoxOpeningBig->setValue(50);
		on_btnRegulateBig_clicked();
	}
}

/*
** 关闭4路电动调节阀
*/
void WaterStandardDlg50::closeAllRegulator()
{
	if (ui.spinBoxOpeningSmall->value()!=0)
	{
		ui.spinBoxOpeningSmall->setValue(0);
		on_btnRegulateSmall_clicked();
	}
	if (ui.spinBoxOpeningMid1->value()!=0)
	{
		ui.spinBoxOpeningMid1->setValue(0);
		on_btnRegulateMid1_clicked();
	}
	if (ui.spinBoxOpeningMid2->value()!=0)
	{
		ui.spinBoxOpeningMid2->setValue(0);
		on_btnRegulateMid2_clicked();
	}
	if (ui.spinBoxOpeningBig->value()!=0)
	{
		ui.spinBoxOpeningBig->setValue(0);
		on_btnRegulateBig_clicked();
	}
}

void WaterStandardDlg50::setRegulatorOpening(int regNO, int opening)
{
	if (regNO == m_portsetinfo.regSmallNo) //小调节阀
	{
		ui.spinBoxOpeningSmall->setValue(opening);
		on_btnRegulateSmall_clicked();
	}
	else if (regNO == m_portsetinfo.regMid1No) //中一调节阀
	{
		ui.spinBoxOpeningMid1->setValue(opening);
		on_btnRegulateMid1_clicked();
	}
	else if (regNO == m_portsetinfo.regMid2No) //中二调节阀
	{
		ui.spinBoxOpeningMid2->setValue(opening);
		on_btnRegulateMid2_clicked();
	}
	else if (regNO == m_portsetinfo.regBigNo) //大调节阀
	{
		ui.spinBoxOpeningBig->setValue(opening);
		on_btnRegulateBig_clicked();
	}
}

//电动调节阀
void WaterStandardDlg50::on_btnRegulateSmall_clicked() //调节阀1-DN3
{
	m_smallOpening = ui.ThermoSmall->value();
	askControlRegulate(m_portsetinfo.regSmallNo, ui.spinBoxOpeningSmall->value());
	m_regSmallTimer->start(REGULATE_FRESH_TIME);
}

void WaterStandardDlg50::on_btnRegulateMid1_clicked() //调节阀2-DN10
{
	m_mid1Opening = ui.ThermoMid1->value();
	askControlRegulate(m_portsetinfo.regMid1No, ui.spinBoxOpeningMid1->value());
	m_regMid1Timer->start(REGULATE_FRESH_TIME);
}

void WaterStandardDlg50::on_btnRegulateMid2_clicked() //调节阀3-DN25
{
	m_mid2Opening = ui.ThermoMid2->value();
	askControlRegulate(m_portsetinfo.regMid2No, ui.spinBoxOpeningMid2->value());
	m_regMid2Timer->start(REGULATE_FRESH_TIME);
}

void WaterStandardDlg50::on_btnRegulateBig_clicked() //调节阀4-DN50
{
	m_bigOpening = ui.ThermoBig->value();
	askControlRegulate(m_portsetinfo.regBigNo, ui.spinBoxOpeningBig->value());
	m_regBigTimer->start(REGULATE_FRESH_TIME);
}

void WaterStandardDlg50::askControlRegulate(int regNO, int opening)
{
	if (regNO>=1 && regNO<=3)
	{
		m_controlObj->askControlRegulate(regNO, opening);
	}
	else
	{
		m_controlObj2->askControlRegulate(regNO-3, opening);
	}
}

void WaterStandardDlg50::on_lineEditOpeningSmall_textChanged(const QString & text)
{
	ui.ThermoSmall->setValue(text.toFloat());
}

void WaterStandardDlg50::on_lineEditOpeningMid1_textChanged(const QString & text)
{
	ui.ThermoMid1->setValue(text.toFloat());
}

void WaterStandardDlg50::on_lineEditOpeningMid2_textChanged(const QString & text)
{
	ui.ThermoMid2->setValue(text.toFloat());
}

void WaterStandardDlg50::on_lineEditOpeningBig_textChanged(const QString & text)
{
	ui.ThermoBig->setValue(text.toFloat());
}

void WaterStandardDlg50::slotFreshSmallRegOpening()
{
	if (ui.ThermoSmall->value() < ui.spinBoxOpeningSmall->value())
	{
		ui.lineEditOpeningSmall->setText(QString("%1").arg(m_smallOpening++));
	}
	else
	{
		ui.lineEditOpeningSmall->setText(QString("%1").arg(m_smallOpening--));
	}

	if (ui.lineEditOpeningSmall->text().toInt() == ui.spinBoxOpeningSmall->value())
	{
		m_regSmallTimer->stop();
	}
}

void WaterStandardDlg50::slotFreshMid1RegOpening()
{
	if (ui.ThermoMid1->value() < ui.spinBoxOpeningMid1->value())
	{
		ui.lineEditOpeningMid1->setText(QString("%1").arg(m_mid1Opening++));
	}
	else
	{
		ui.lineEditOpeningMid1->setText(QString("%1").arg(m_mid1Opening--));
	}

	if (ui.lineEditOpeningMid1->text().toInt() == ui.spinBoxOpeningMid1->value())
	{
		m_regMid1Timer->stop();
	}
}

void WaterStandardDlg50::slotFreshMid2RegOpening()
{
	if (ui.ThermoMid2->value() < ui.spinBoxOpeningMid2->value())
	{
		ui.lineEditOpeningMid2->setText(QString("%1").arg(m_mid2Opening++));
	}
	else
	{
		ui.lineEditOpeningMid2->setText(QString("%1").arg(m_mid2Opening--));
	}

	if (ui.lineEditOpeningMid2->text().toInt() == ui.spinBoxOpeningMid2->value())
	{
		m_regMid2Timer->stop();
	}
}

void WaterStandardDlg50::slotFreshBigRegOpening()
{
	if (ui.ThermoBig->value() < ui.spinBoxOpeningBig->value())
	{
		ui.lineEditOpeningBig->setText(QString("%1").arg(m_bigOpening++));
	}
	else
	{
		ui.lineEditOpeningBig->setText(QString("%1").arg(m_bigOpening--));
	}

	if (ui.lineEditOpeningBig->text().toInt() == ui.spinBoxOpeningBig->value())
	{
		m_regBigTimer->stop();
	}
}

void WaterStandardDlg50::on_lineEditStdMeter_textChanged(const QString &text)
{
	bool ok;
	float value = text.toFloat(&ok);
	if (ok)
	{
		ui.lcdAccumStdMeter->display(text);
	}
}
