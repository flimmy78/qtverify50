/***********************************************
**  文件名:     totalweight50.cpp
**  功能:       总量检定(质量法)主界面-DN50、双天平、双控制板
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2015/12/1
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

#include "totalweight50.h"
#include "commondefine.h"
#include "algorithm.h"
#include "qtexdb.h"
#include "parasetdlg.h"
#include "readcomconfig.h"
#include "report.h"
#include "readstdmeter.h"

TotalWeightDlg50::TotalWeightDlg50(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"TotalWeightDlg50 thread:"<<QThread::currentThreadId();
	ui.setupUi(this);
	
	//不同等级的热量表对应的标准误差,单位%
	m_gradeErrA[1] = 2.00f;
	m_gradeErrA[2] = 3.00f;
	m_gradeErrA[3] = 4.00f;

	m_gradeErrB[1] = 0.01f;
	m_gradeErrB[2] = 0.02f;
	m_gradeErrB[3] = 0.05f;

	m_mapNormalFlow[0] = 1.5f; //DN15常用流量 1.5
	m_mapNormalFlow[1] = 2.5f; //DN20常用流量 2.5
	m_mapNormalFlow[2] = 3.5f; //DN25常用流量 3.5

	///////////////////////////////// 原showEvent()函数的内容 begin 
	//否则每次最小化再显示时，会调用showEvent函数，导致内容清空等现象
	ui.btnExhaust->hide();
	ui.btnGoOn->hide();

	if (!getPortSetIni(&m_portsetinfo)) //获取下位机端口号配置信息
	{
		QMessageBox::warning(this, tr("Warning"), tr("Warning:get port set info failed!"));
	}

	m_readComConfig = new ReadComConfig(); //读串口设置接口（必须在initBalanceCom前调用）
	m_readComConfig->getBalancePara(m_balMaxWht, m_balBottomWht); //获取大天平最大容量和回水底量
	m_readComConfig->getBalancePara2(m_balMaxWht2, m_balBottomWht2); //获取小天平最大容量和回水底量

	m_balanceObj = NULL;
	initBalanceCom();		//初始化天平串口

	m_balanceObj2 = NULL;
	initBalanceCom2();		//初始化天平串口2

	m_tempObj = NULL;
	m_tempTimer = NULL;
	initTemperatureCom();	//初始化温度采集串口

	m_stdTempObj = NULL;
	m_stdTempTimer = NULL;
	initStdTemperatureCom(); //初始化标准温度采集串口

	m_controlObj = NULL;
	initControlCom();		//初始化控制串口

	m_controlObj2 = NULL;
	initControlCom2();		//初始化控制串口2

	m_meterObj = NULL;      //热量表通讯
	m_recPtr = NULL;        //流量检测结果

	btnGroupEnergyUnit = new QButtonGroup(ui.groupBoxEnergyUnit); //能量单位
	btnGroupEnergyUnit->addButton(ui.radioButtonMJ, UNIT_MJ);
	btnGroupEnergyUnit->addButton(ui.radioButtonKwh, UNIT_KWH);
	connect(btnGroupEnergyUnit, SIGNAL(buttonClicked(int)), this, SLOT(slot_btnGroupEnergyUnit_clicked(int)));
	ui.radioButtonKwh->setChecked(true); //默认单位:kWh
	m_unit = UNIT_KWH;

	btnGroupInstallPos = new QButtonGroup(ui.groupBoxInstallPos); //安装位置
	btnGroupInstallPos->addButton(ui.radioButtonPosIn, INSTALLPOS_IN);
	btnGroupInstallPos->addButton(ui.radioButtonPosOut, INSTALLPOS_OUT);
	connect(btnGroupInstallPos, SIGNAL(buttonClicked(int)), this, SLOT(slot_btnGroupInstallPos_clicked(int)));
	ui.radioButtonPosIn->setChecked(true); //默认入口安装
	m_installPos = INSTALLPOS_IN;

	m_minDeltaT = 3.0; //最小温差
	ui.lnEditMinDeltaT->setText(QString::number(m_minDeltaT));

	//计算流速用
	m_totalcount = 0;
	m_startWeight = 0.0;
	m_endWeight = 0.0;
	memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	m_flowRateTimer = new QTimer();
	connect(m_flowRateTimer, SIGNAL(timeout()), this, SLOT(slotFreshFlowRate()));
	m_flowRateTimer->start(TIMEOUT_FLOW_SAMPLE);

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
	m_totalSC = 1.0;			 //总量安全系数，默认1.0
	m_meterStartValue = NULL;
	m_meterEndValue = NULL;
	m_meterTemper = NULL;
	m_meterDensity = NULL;
	m_meterStdValue = NULL;
	m_meterError = NULL;
	m_meterErr = NULL;
	m_meterResult = NULL;
	m_balStartV = 0;
	m_balEndV = 0;
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

	ui.lcdInTemper->display(50);
	ui.lcdOutTemper->display(40);
	ui.lnEditInStdTemp->setText("25");
	ui.lnEditOutStdTemp->setText("20");

	m_stdMeterReader = NULL;
	m_stdMeterReader = new CStdMeterReader();
	connect(m_stdMeterReader, SIGNAL(signalReadTolInstReady(const float&)), this, SLOT(slotFreshTolInst(const float&)));
	m_stdMeterReader->startReadInstMeter();
}

void TotalWeightDlg50::slotFreshTolInst(const float& value)
{
	ui.lcdFlowRateStd->display(value);
}

TotalWeightDlg50::~TotalWeightDlg50()
{
}

void TotalWeightDlg50::showEvent(QShowEvent * event)
{
	qDebug()<<"TotalWeightDlg50::showEvent";
}

void TotalWeightDlg50::closeEvent(QCloseEvent * event)
{
	qDebug()<<"^^^^^TotalWeightDlg50::closeEvent";
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

	if (m_tempObj)  // 温度采集
	{
		delete m_tempObj;
		m_tempObj = NULL;

		m_tempThread.exit(); //否则日志中会有警告"QThread: Destroyed while thread is still running"
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

	if (m_stdTempTimer) //标准温度采集计时器, 必须先于串口对象停掉
	{
		if (m_stdTempTimer->isActive())
		{
			m_stdTempTimer->stop();
		}
		delete m_stdTempTimer;
		m_stdTempTimer = NULL;
	}

	if (m_stdTempObj)  // 标准温度采集
	{
		delete m_stdTempObj;
		m_stdTempObj = NULL;

		m_stdTempThread.exit();
	}

	if (m_balanceObj)  //天平采集
	{
		delete m_balanceObj;
		m_balanceObj = NULL;

		m_balanceThread.exit();
	}

	if (m_balanceObj2)  //天平采集2
	{
		delete m_balanceObj2;
		m_balanceObj2 = NULL;

		m_balanceThread2.exit();
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

	if (m_flowRateTimer) //计算流速计时器
	{
		if (m_flowRateTimer->isActive())
		{
			m_flowRateTimer->stop();
		}
		delete m_flowRateTimer;
		m_flowRateTimer = NULL;
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

void TotalWeightDlg50::resizeEvent(QResizeEvent * event)
{
	qDebug()<<"resizeEvent...";

	int th = ui.tableWidget->size().height();
	int tw = ui.tableWidget->size().width();
	int hh = ui.tableWidget->horizontalHeader()->size().height();
	int vw = ui.tableWidget->verticalHeader()->size().width();
	int vSize = (int)((th-hh-10)/(m_maxMeterNum<=0 ? 12 : m_maxMeterNum));
	int hSize = (int)((tw-vw-10)/(COLUMN_TOTAL_COUNT-1)); //隐藏了"密度"列
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(vSize);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(hSize);
}

//天平采集串口 上位机直接采集
void TotalWeightDlg50::initBalanceCom()
{
	ComInfoStruct balanceStruct = m_readComConfig->ReadBalanceConfig();
	m_balanceObj = new BalanceComObject();
	int type = m_readComConfig->getBalanceType();
	m_balanceObj->setBalanceType(type);
	m_balanceObj->moveToThread(&m_balanceThread);
	m_balanceThread.start();
	m_balanceObj->openBalanceCom(&balanceStruct);

	//天平数值由上位机直接通过天平串口采集
	connect(m_balanceObj, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshBigBalanceValue(const float &)));
}

//天平采集串口2 上位机直接采集
void TotalWeightDlg50::initBalanceCom2()
{
	ComInfoStruct balanceStruct2 = m_readComConfig->ReadBalanceConfig2();
	m_balanceObj2 = new BalanceComObject();
	int type = m_readComConfig->getBalanceType2();
	m_balanceObj2->setBalanceType(type);
	m_balanceObj2->moveToThread(&m_balanceThread2);
	m_balanceThread2.start();
	m_balanceObj2->openBalanceCom(&balanceStruct2);

	//天平数值由上位机直接通过天平串口采集
	connect(m_balanceObj2, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshSmallBalanceValue(const float &)));
}

/*
** 温度采集串口 上位机直接采集
** 周期请求
*/
void TotalWeightDlg50::initTemperatureCom()
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

void TotalWeightDlg50::slotAskPipeTemperature()
{
	m_tempObj->writeTemperatureComBuffer();
}

/*
** 开辟一个新线程，打开标准温度采集串口
*/
void TotalWeightDlg50::initStdTemperatureCom()
{
	ComInfoStruct tempStruct = m_readComConfig->ReadStdTempConfig();
	m_stdTempObj = new StdTempComObject();
	QSettings stdconfig(getFullIniFileName("stdplasensor.ini"), QSettings::IniFormat);
	m_stdTempObj->moveToThread(&m_stdTempThread);
	m_stdTempThread.start();
	m_stdTempObj->openTemperatureCom(&tempStruct);
	m_stdTempObj->setStdTempVersion(stdconfig.value("in_use/model").toInt());
	connect(m_stdTempObj, SIGNAL(temperatureIsReady(const QString &)), this, SLOT(slotFreshStdTempValue(const QString &)));

	m_stdTempCommand = stdTempR1;
	m_stdTempTimer = new QTimer();
	connect(m_stdTempTimer, SIGNAL(timeout()), this, SLOT(slotAskStdTemperature()));
	
 	m_stdTempTimer->start(TIMEOUT_STD_TEMPER);
}

void TotalWeightDlg50::slotAskStdTemperature()
{
	m_stdTempObj->writeStdTempComBuffer(m_stdTempCommand);
}

//控制板通讯串口
void TotalWeightDlg50::initControlCom()
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
void TotalWeightDlg50::initControlCom2()
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
void TotalWeightDlg50::initMeterCom()
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
		connect(&m_meterObj[i], SIGNAL(readMeterHeatIsOK(const QString&, const QString&)), this, SLOT(slotSetMeterEnergy(const QString&, const QString&)));
	}
}

/*
** 端口号-阀门映射关系；初始化阀门状态（默认阀门初始状态全部为关闭,水泵初始状态为关闭）
** 需要改进得更加灵活
*/
void TotalWeightDlg50::initValveStatus()
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

void TotalWeightDlg50::initRegulateStatus()
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

/*
** 函数功能：
	在界面刷新大天平数值
	防止天平溢出
*/
void TotalWeightDlg50::slotFreshBigBalanceValue(const float& balValue)
{
	QString wht = QString::number(balValue, 'f', 3);
	ui.lcdBigBalance->setText(wht);
	if (balValue > m_balMaxWht) //防止天平溢出
	{
		closeWaterPump(); //关闭水泵
		closeValve(m_portsetinfo.waterInNo); //关闭进水阀
		openValve(m_portsetinfo.bigWaterOutNo); //打开大天平放水阀
		openValve(m_portsetinfo.bigWaterInNo);  //打开大天平进水阀
	}
}

/*
** 函数功能：
	在界面刷新小天平数值
	防止天平溢出
*/
void TotalWeightDlg50::slotFreshSmallBalanceValue(const float& balValue)
{
	QString wht = QString::number(balValue, 'f', 3);
	ui.lcdSmallBalance->setText(wht);
	if (balValue > m_balMaxWht2) //防止天平溢出
	{
		closeWaterPump(); //关闭水泵
		closeValve(m_portsetinfo.waterInNo); //关闭进水阀
		openValve(m_portsetinfo.smallWaterOutNo); //打开小天平放水阀
		openValve(m_portsetinfo.smallWaterInNo);  //打开小天平进水阀
	}
}

//在界面刷新入口温度和出口温度值
void TotalWeightDlg50::slotFreshComTempValue(const QString& tempStr)
{
	ui.lcdInTemper->display(tempStr.left(TEMPER_DATA_WIDTH));   //入口温度 PV
	ui.lcdOutTemper->display(tempStr.right(TEMPER_DATA_WIDTH)); //出口温度 SV
}

//刷新标准温度
void TotalWeightDlg50::slotFreshStdTempValue(const QString& stdTempStr)
{
// 	qDebug()<<"stdTempStr ="<<stdTempStr<<"; m_stdTempCommand ="<<m_stdTempCommand;
	switch (m_stdTempCommand)
	{
// 	case stdTempT1: 
// 		ui.lnEditOutStdResist->setText(stdTempStr);
// 		break;
// 	case stdTempT2: 
// 		ui.lnEditInStdTemp->setText(stdTempStr);
// 		break;
	case stdTempR1: 
		ui.lnEditInStdResist->setText(stdTempStr);
		m_stdTempCommand = stdTempR2;
		break;
	case stdTempR2: 
		ui.lnEditOutStdResist->setText(stdTempStr);
		m_stdTempCommand = stdTempR1;
		break;
	default:
		break;
	}
}

//采集标准温度
void TotalWeightDlg50::on_btnStdTempCollect_clicked()
{
	ui.lnEditInStdResist->clear();
	ui.lnEditOutStdResist->clear();
	ui.lnEditInStdTemp->clear();
	ui.lnEditOutStdTemp->clear();
	m_stdTempTimer->start(TIMEOUT_STD_TEMPER);
}

//停止采集标准温度
void TotalWeightDlg50::on_btnStdTempStop_clicked()
{
	m_stdTempTimer->stop();
}

void TotalWeightDlg50::on_lnEditInStdResist_textChanged(const QString & text)
{
	float resis = text.toFloat();
	float temp = calcTemperByResis(resis);
	ui.lnEditInStdTemp->setText(QString::number(temp));
}

void TotalWeightDlg50::on_lnEditOutStdResist_textChanged(const QString & text)
{
	float resis = text.toFloat();
	float temp = calcTemperByResis(resis);
	ui.lnEditOutStdTemp->setText(QString::number(temp));
}

void TotalWeightDlg50::slot_btnGroupEnergyUnit_clicked(int id)
{
	m_unit = btnGroupEnergyUnit->checkedId();
	if (m_unit==UNIT_KWH) 
	{
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_METER_START, new QTableWidgetItem(QObject::tr("MeterValue0(kWh)")));
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_METER_END, new QTableWidgetItem(QObject::tr("MeterValue1(kWh)")));
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_STD_VALUE, new QTableWidgetItem(QObject::tr("StdValue(kWh)")));
	}
	else
	{
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_METER_START, new QTableWidgetItem(QObject::tr("MeterValue0(MJ)")));
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_METER_END, new QTableWidgetItem(QObject::tr("MeterValue1(MJ)")));
		ui.tableWidget->setHorizontalHeaderItem(COLUMN_STD_VALUE, new QTableWidgetItem(QObject::tr("StdValue(MJ)")));
	}
}

void TotalWeightDlg50::slot_btnGroupInstallPos_clicked(int id)
{
	m_installPos = btnGroupInstallPos->checkedId();
}

/*
** 计算流速
*/
void TotalWeightDlg50::slotFreshFlowRate()
{
// 	qDebug()<<"TotalWeightDlg::slotFreshFlowRate thread:"<<QThread::currentThreadId();
	if (m_totalcount > 4294967290) //防止m_totalcount溢出 32位无符号整数范围0~4294967295
	{
		m_totalcount = 0;
		m_startWeight = 0.0;
		m_endWeight = 0.0;
		memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	}
	if (m_totalcount == 0) //记录天平初始重量
	{
		m_startWeight = ui.lcdBigBalance->text().toFloat() + ui.lcdSmallBalance->text().toFloat();
		m_totalcount ++;
		return;
	}

	float flowValue = 0.0;
	float totalWeight = 0.0;
	m_endWeight = ui.lcdBigBalance->text().toFloat() + ui.lcdSmallBalance->text().toFloat();//取当前天平值, 作为当前运算的终值
	float delta_weight = m_endWeight - m_startWeight;
	m_deltaWeight[m_totalcount%FLOW_SAMPLE_NUM] = delta_weight;
// 	qWarning()<<"m_totalcount ="<<m_totalcount;
	for (int i=0; i<FLOW_SAMPLE_NUM; i++)
	{
		totalWeight += m_deltaWeight[i];
// 		qWarning()<<"totalWeight ="<<totalWeight<<", m_deltaWeight["<<i<<"] ="<<m_deltaWeight[i];
	}
	flowValue = 3.6*(totalWeight)*1000/(FLOW_SAMPLE_NUM*TIMEOUT_FLOW_SAMPLE);//总累积水量/总时间  (吨/小时, t/h, m3/h)
//	flowValue = (totalWeight)*1000/(FLOW_SAMPLE_NUM*TIMEOUT_FLOW_SAMPLE);// kg/s
// 	qDebug()<<"flowValue ="<<flowValue;
	if (m_totalcount >= FLOW_SAMPLE_NUM)
	{
		ui.lcdFlowRate->display(QString::number(flowValue, 'f', 3)); //在ui.lcdFlowRate中显示流速
	}
	m_totalcount ++;//计数器累加
	m_startWeight = m_endWeight;//将当前值保存, 作为下次运算的初值
}

//检测串口、端口设置是否正确
int TotalWeightDlg50::isComAndPortNormal()
{
	return true;
}

//获取当前检定参数;初始化表格控件；显示关键参数；初始化热量表通讯串口
int TotalWeightDlg50::readNowParaConfig()
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
	m_model = m_nowParams->m_model;          //表型号
	m_maxMeterNum = m_nowParams->m_maxMeters;//不同表规格对应的最大检表数量
	m_pickcode = m_nowParams->m_pickcode; //采集代码
	m_numPrefix = getNumPrefixOfManufac(m_pickcode); //表号前缀
	m_totalSC = m_nowParams->sc_thermal;  //总量安全系数

	initTableWidget();
	showNowKeyParaConfig();
	initMeterCom();

	return true;
}

//初始化表格控件
void TotalWeightDlg50::initTableWidget()
{
	if (m_maxMeterNum <= 0)
	{
		return;
	}
	ui.tableWidget->setRowCount(m_maxMeterNum); //设置表格行数

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
void TotalWeightDlg50::showNowKeyParaConfig()
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
int TotalWeightDlg50::isDataCollectNormal()
{
	return true;
}

/*
** 开始排气倒计时
*/
int TotalWeightDlg50::startExhaustCountDown()
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
void TotalWeightDlg50::slotExaustFinished()
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

	//准备天平初始重量 begin
	bool hasBigBalance = false; //是否需要准备大天平
	bool hasSmallBalance = false; //是否需要准备小天平
	for (int i=1; i<=m_flowPointNum; i++)
	{
		if (m_paraSetReader->getFpBySeq(i).fp_valve_idx == 0)
		{
			hasBigBalance = true;
		}
		if (m_paraSetReader->getFpBySeq(i).fp_valve_idx >= 1)
		{
			hasSmallBalance = true;
		}
	}
	if (hasBigBalance) //需要准备大天平
	{
		if (!prepareBigBalanceInitWeight())//准备大天平初始重量
		{
			return;
		}
	}
	if (hasSmallBalance) //需要准备小天平
	{
		if (!prepareSmallBalanceInitWeight())//准备小天平初始重量
		{
			return;
		}
	}
	//准备天平初始重量 end
	wait(BALANCE_STABLE_TIME); //等待天平数值稳定

	if (setAllMeterVerifyStatus()) //设置检定状态成功
	{
		startVerify();
	}
}

/*
** 排气结束后、开始检定前，准备大天平,达到要求的初始重量
** 此时状态：水泵开启、进水阀开启、四个流量点阀门关闭、大小天平的进水和放水阀打开
*/
int TotalWeightDlg50::prepareBigBalanceInitWeight()
{
	ui.labelHintPoint->setText(tr("prepare big balance init weight ...")); //准备大天平初始重量(回水底量)
	int ret = 0;
	//判断大天平重量,如果小于要求的回水底量(5kg)，则关闭大天平放水阀，打开大流量阀
	if (ui.lcdBigBalance->text().toFloat() < m_balBottomWht)
	{
		if (!closeValve(m_portsetinfo.bigWaterOutNo)) 
		{
			qWarning()<<"关闭大天平放水阀失败";
		}
		if (!openValve(m_portsetinfo.bigNo))
		{
			qWarning()<<"打开大流量阀失败";
		}
		//判断并等待天平重量，大于回水底量(5kg)
		if (isBigBalanceValueBigger(m_balBottomWht, true))
		{
			if (!closeValve(m_portsetinfo.bigNo))
			{
				qWarning()<<"关闭大流量阀失败";
			}
			ret = 1;
		}
	}
	else //大于回水底量
	{
		closeValve(m_portsetinfo.bigWaterOutNo); //关闭大天平放水阀
		ret = 1;
	}

	return ret;
}

/*
** 排气结束后、开始检定前，准备小天平,达到要求的初始重量
*/
int TotalWeightDlg50::prepareSmallBalanceInitWeight()
{
	ui.labelHintPoint->setText(tr("prepare small balance init weight ...")); //准备小天平初始重量(回水底量)
	int ret = 0;
	//判断小天平重量,如果小于要求的回水底量(5kg)，则关闭小天平放水阀，打开中二流量阀
	if (ui.lcdSmallBalance->text().toFloat() < m_balBottomWht2)
	{
		if (!closeValve(m_portsetinfo.smallWaterOutNo)) 
		{
			qWarning()<<"关闭小天平放水阀失败";
		}
		if (!openValve(m_portsetinfo.middle2No))
		{
			qWarning()<<"打开中二流量阀失败";
		}
		//判断并等待天平重量，大于回水底量(5kg)
		if (isSmallBalanceValueBigger(m_balBottomWht2, true))
		{
			if (!closeValve(m_portsetinfo.middle2No))
			{
				qWarning()<<"关闭中二流量阀失败";
			}
			ret = 1;
		}
	}
	else
	{
		closeValve(m_portsetinfo.smallWaterOutNo); //关闭小天平放水阀
		ret = 1;
	}

	return ret;
}

//设置所有热量表进入检定状态
int TotalWeightDlg50::setAllMeterVerifyStatus()
{
	ui.labelHintPoint->setText(tr("setting verify status ..."));
	on_btnAllVerifyStatus_clicked();
	wait(CYCLE_TIME);
	on_btnAllVerifyStatus_clicked();
	return true;
}

//打开所有阀门和水泵
int TotalWeightDlg50::openAllValveAndPump()
{
	openValve(m_portsetinfo.bigWaterOutNo); //大天平放水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.bigWaterInNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterInNo);
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
int TotalWeightDlg50::closeAllValveAndPumpOpenOutValve()
{
	openValve(m_portsetinfo.bigWaterOutNo); //打开大天平放水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.bigWaterInNo);  //打开大天平进水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);//打开小天平放水阀
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterInNo);//打开小天平进水阀
	wait(CYCLE_TIME);
	closeWaterPump();    //退出时关闭水泵
	closeAllFlowPointValves();//关闭所有流量点阀门
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.waterInNo);//关闭进水阀

	return true;
}

//关闭所有流量点阀门
int TotalWeightDlg50::closeAllFlowPointValves()
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
** 响应处理大天平质量的变化
** 输入参数：
	targetV: 目标重量
	flg: true-要求大于目标重量(默认)；false-要求小于目标重量
*/
int TotalWeightDlg50::isBigBalanceValueBigger(float targetV, bool flg)
{
	int ret = 0;
	if (flg) //要求大于目标重量
	{
		while (!m_stopFlag && (ui.lcdBigBalance->text().toFloat() < targetV))
		{
			qDebug()<<"大天平重量 ="<<ui.lcdBigBalance->text()<<", 小于要求的重量 "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdBigBalance->text().toFloat() >= targetV);
	}
	else //要求小于目标重量
	{
		while (!m_stopFlag && (ui.lcdBigBalance->text().toFloat() > targetV))
		{
			qDebug()<<"大天平重量 ="<<ui.lcdBigBalance->text()<<", 大于要求的重量 "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdBigBalance->text().toFloat() <= targetV);
	}

	return ret;
}

/*
** 响应处理小天平质量的变化
** 输入参数：
	targetV: 目标重量
	flg: true-要求大于目标重量(默认)；false-要求小于目标重量
*/
int TotalWeightDlg50::isSmallBalanceValueBigger(float targetV, bool flg)
{
	int ret = 0;
	if (flg) //要求大于目标重量
	{
		while (!m_stopFlag && (ui.lcdSmallBalance->text().toFloat() < targetV))
		{
			qDebug()<<"小天平重量 ="<<ui.lcdSmallBalance->text()<<", 小于要求的重量 "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdSmallBalance->text().toFloat() >= targetV);
	}
	else //要求小于目标重量
	{
		while (!m_stopFlag && (ui.lcdSmallBalance->text().toFloat() > targetV))
		{
			qDebug()<<"小天平重量 ="<<ui.lcdSmallBalance->text()<<", 大于要求的重量 "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdSmallBalance->text().toFloat() <= targetV);
	}

	return ret;
}

/*
** 功能：判断天平重量是否达到要求的检定量；计算检定过程的平均温度和平均流量(m3/h)
*/
int TotalWeightDlg50::judgeBalanceAndCalcAvgTemperAndFlow(float targetV, bool bigFlag)
{
	ui.tableWidget->setEnabled(false);
	ui.btnAllReadNO->setEnabled(false);
	ui.btnAllReadData->setEnabled(false);
	ui.btnAllVerifyStatus->setEnabled(false);
	QDateTime startTime = QDateTime::currentDateTime();
	int second = 0;
	float nowFlow = m_paraSetReader->getFpBySeq(m_nowOrder).fp_verify;

	if (bigFlag)
	{
		while (!m_stopFlag && (ui.lcdBigBalance->text().toFloat() < targetV))
		{
			qDebug()<<"大天平重量 ="<<ui.lcdBigBalance->text()<<", 小于要求的重量 "<<targetV;
			m_avgTFCount++;
			m_pipeInTemper += ui.lcdInTemper->value();
			m_pipeOutTemper += ui.lcdOutTemper->value();
			m_stdInTemper += ui.lnEditInStdTemp->text().toFloat();
			m_stdOutTemper += ui.lnEditOutStdTemp->text().toFloat();
			second = 3.6*(targetV - ui.lcdBigBalance->text().toFloat())/nowFlow;
			ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
				.arg(m_nowOrder).arg(nowFlow));
			ui.labelHintProcess->setText(tr("Verifying...Please wait for about <font color=DarkGreen size=6><b>%1</b></font> second").arg(second));
			wait(CYCLE_TIME);
		}	
	}
	else
	{
		while (!m_stopFlag && (ui.lcdSmallBalance->text().toFloat() < targetV))
		{
			qDebug()<<"小天平重量 ="<<ui.lcdSmallBalance->text()<<", 小于要求的重量 "<<targetV;
			m_avgTFCount++;
			m_pipeInTemper += ui.lcdInTemper->value();
			m_pipeOutTemper += ui.lcdOutTemper->value();
			m_stdInTemper += ui.lnEditInStdTemp->text().toFloat();
			m_stdOutTemper += ui.lnEditOutStdTemp->text().toFloat();
			second = 3.6*(targetV - ui.lcdSmallBalance->text().toFloat())/nowFlow;
			ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
				.arg(m_nowOrder).arg(nowFlow));
			ui.labelHintProcess->setText(tr("Verifying...Please wait for about <font color=DarkGreen size=6><b>%1</b></font> second").arg(second));
			wait(CYCLE_TIME);
		}	
	}

	m_pipeInTemper = m_pipeInTemper/m_avgTFCount;   //入口平均温度
	m_pipeOutTemper = m_pipeOutTemper/m_avgTFCount; //出口平均温度
	m_stdInTemper = m_stdInTemper/m_avgTFCount;     //入口标准温度平均值
	m_stdOutTemper = m_stdOutTemper/m_avgTFCount;   //出口标准温度平均值
	QDateTime endTime = QDateTime::currentDateTime();
	int tt = startTime.secsTo(endTime);
	if (NULL==m_paraSetReader || m_stopFlag)
	{
		return false;
	}
	if (bigFlag)
	{
		m_realFlow = 3.6*(m_paraSetReader->getFpBySeq(m_nowOrder).fp_quantity + ui.lcdBigBalance->text().toFloat() - targetV)/tt;
	}
	else
	{
		m_realFlow = 3.6*(m_paraSetReader->getFpBySeq(m_nowOrder).fp_quantity + ui.lcdSmallBalance->text().toFloat() - targetV)/tt;
	}
	ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
		.arg(m_nowOrder).arg(nowFlow));
	ui.labelHintProcess->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: Verify Finished!").arg(m_nowOrder));
	
	int ret = false;
	if (bigFlag)
	{
		ret = !m_stopFlag && (ui.lcdBigBalance->text().toFloat() >= targetV);
	}
	else
	{
		ret = !m_stopFlag && (ui.lcdSmallBalance->text().toFloat() >= targetV);
	}
	return ret;
}

//清空表格，第一列除外("表号"列)
void TotalWeightDlg50::clearTableContents()
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
void TotalWeightDlg50::on_btnStart_clicked()
{
	//判断是否输入了最小温差
	bool ok;
	m_minDeltaT = ui.lnEditMinDeltaT->text().toFloat(&ok);
	if (!ok || ui.lnEditMinDeltaT->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("please input minimum delta temperature!"));
		ui.lnEditMinDeltaT->setFocus();
		return;
	}
	bool ok1, ok2;
	float stdInTemp = ui.lnEditInStdTemp->text().toFloat(&ok1);
	float stdOutTemp = ui.lnEditOutStdTemp->text().toFloat(&ok2);
	if ( !ok1 || !ok2 || (stdInTemp-stdOutTemp)<=0 )
	{
		QMessageBox::warning(this, tr("Warning"), tr("std temperature is error, please check!"));
		return;
	}

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
	
	m_stopFlag = false;
	m_state = STATE_INIT;
	m_validMeterNum = 0;

	for (int i=0; i<ui.tableWidget->rowCount(); i++)
	{
		ui.tableWidget->item(i,COLUMN_METER_NUMBER)->setText("");
		ui.tableWidget->item(i, COLUMN_METER_NUMBER)->setForeground(QBrush());
		ui.tableWidget->item(i, COLUMN_DISP_ERROR)->setForeground(QBrush());
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
void TotalWeightDlg50::on_btnExhaust_clicked()
{
}

//点击"继续"按钮 处理表号获取异常
void TotalWeightDlg50::on_btnGoOn_clicked()
{
	ui.btnGoOn->hide();
	startVerify();
}

//点击"终止检测"按钮
void TotalWeightDlg50::on_btnStop_clicked()
{
	int button = QMessageBox::question(this, tr("Question"), tr("Stop Really ?"), \
		QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	if (button == QMessageBox::No)
	{
		return;
	}

	stopVerify();
}

void TotalWeightDlg50::on_btnExit_clicked()
{
	stopVerify();
	this->close();
}

//停止检定
void TotalWeightDlg50::stopVerify()
{
	ui.labelHintPoint->clear();
	if (!m_stopFlag)
	{
		ui.labelHintProcess->setText(tr("stopping verify...please wait a minute"));
		m_stopFlag = true; //不再检查天平质量
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
void TotalWeightDlg50::startVerify()
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
	m_recPtr = new Total_Verify_Record_STR[m_validMeterNum];
	memset(m_recPtr, 0, sizeof(Total_Verify_Record_STR)*m_validMeterNum);

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

  int bigOK = 0, smallOK = 0;
	int bigNum = 0, smallNum = 0;
	if (m_continueVerify) //连续检定
	{
		if (!judgeBalanceCapacity(bigOK, smallOK)) //判断天平容量是否能够满足检定用量
		{
			ui.labelHintPoint->setText(tr("prepare balance capacity ..."));
			if (!bigOK)
			{
				openValve(m_portsetinfo.bigWaterOutNo);
			}
			if (!smallOK)
			{
				openValve(m_portsetinfo.smallWaterOutNo);
			}

			while (!judgeBalanceCapacity(bigOK, smallOK)) //大、小天平不能同时满足检定容量
			{ 
				if (bigOK && bigNum<=0)
				{
					closeValve(m_portsetinfo.bigWaterOutNo);
					bigNum++;
				}
				if (smallOK && smallNum<=0)
				{
					closeValve(m_portsetinfo.smallWaterOutNo);
					smallNum++;
				}
				wait(CYCLE_TIME);
			}
			closeValve(m_portsetinfo.bigWaterOutNo); //若满足检定用量，则关闭放水阀
			closeValve(m_portsetinfo.smallWaterOutNo); //若满足检定用量，则关闭放水阀
			wait(BALANCE_STABLE_TIME); //等待3秒钟(等待水流稳定)
		}
	}

	m_nowOrder = 1;
	prepareVerifyFlowPoint(m_nowOrder); //第一个流量点检定
}

//获取有效检表个数,并生成映射关系（被检表下标-表位号）
int TotalWeightDlg50::getValidMeterNum()
{
	m_validMeterNum = 0; //先清零
	bool ok;
	for (int i=0; i<m_maxMeterNum; i++)
	{
		if (NULL == ui.tableWidget->item(i, COLUMN_METER_NUMBER)) //"表号"单元格为空
		{
			continue;
		}
		ui.tableWidget->item(i, COLUMN_METER_NUMBER)->text().toInt(&ok, 10);
		if (!ok) //表号转换失败(非数字)
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
int TotalWeightDlg50::isMeterPosValid(int meterPos)
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
** 判断天平容量是否满足检定用量，同时判断大、小天平
** 连续检定时：满足总检定用量
** 输出参数：
		bigOK：1-满足；0-不满足（大天平容量满足检定用量）
		smallOK：1-满足；0-不满足（小天平容量满足检定用量）
	返回值：
		true-满足；false-不满足
*/
bool TotalWeightDlg50::judgeBalanceCapacity(int &bigOK, int &smallOK)
{
	bool ret = false;
	float bigQuantity = 0, smallQuantity = 0;
	int num = m_nowParams->total_fp; //有效流量点的个数
	for (int i=1; i<=num; i++)
	{
		if (m_paraSetReader->getFpBySeq(i).fp_valve_idx == 0) //大流量点，对应大天平
		{
			bigQuantity +=  m_paraSetReader->getFpBySeq(i).fp_quantity;
		}
		else //其他三个流量点，对应小天平
		{
			smallQuantity += m_paraSetReader->getFpBySeq(i).fp_quantity;
		}		
// 		totalQuantity += m_nowParams->fp_info[i].fp_quantity;
	}
	bigOK = (ui.lcdBigBalance->text().toFloat() + bigQuantity) < (m_balMaxWht - m_balBottomWht);
	smallOK = (ui.lcdSmallBalance->text().toFloat() + smallQuantity) < (m_balMaxWht2 - m_balBottomWht2);
	ret = bigOK * smallOK;
	return ret;
}

/*
** 判断天平容量是否满足检定用量
** 不连续检定时：满足单次检定用量
** 注意：order从1开始
*/
int TotalWeightDlg50::judgeBalanceCapacitySingle(int order, int &bigBalance)
{
	if (order < 1 || order > VERIFY_POINTS)
	{
		return false;
	}
	bool ret = false;
	float quantity = m_nowParams->fp_info[order-1].fp_quantity;
	if (m_nowParams->fp_info[order-1].fp_valve_idx == 0) //大流量点，对应大天平
	{
		bigBalance = 1;
		ret = (ui.lcdBigBalance->text().toFloat() + quantity) < (m_balMaxWht - m_balBottomWht);
	}
	else
	{
		bigBalance = 0;
		ret = (ui.lcdSmallBalance->text().toFloat() + quantity) < (m_balMaxWht2 - m_balBottomWht2);
	}
	return ret;
}

/*
** 准备单个流量点的检定，进行必要的检查
** 注意：order从1开始
*/
int TotalWeightDlg50::prepareVerifyFlowPoint(int order)
{
	if (order < 1 || order > m_flowPointNum || m_stopFlag)
	{
		return false;
	}

	if (!m_continueVerify) //非连续检定，每次检定开始之前都要判断天平容量
	{
		int bigBalance = 0;
		if (!judgeBalanceCapacitySingle(order, bigBalance)) //天平容量不满足本次检定用量
		{
			ui.labelHintPoint->setText(tr("prepare balance capacity ..."));
			if (bigBalance) //大天平
			{
				openValve(m_portsetinfo.bigWaterOutNo); //打开大天平放水阀，大天平放水
			}
			else
			{
				openValve(m_portsetinfo.smallWaterOutNo); //打开小天平放水阀，小天平放水
			}
			while (!judgeBalanceCapacitySingle(order, bigBalance)) //等待天平放水，直至满足本次检定用量
			{ 
				wait(CYCLE_TIME);
			}
			if (bigBalance)
			{
				closeValve(m_portsetinfo.bigWaterOutNo); //若满足检定用量，则关闭放水阀
			}
			else
			{
				closeValve(m_portsetinfo.smallWaterOutNo); //若满足检定用量，则关闭放水阀
			}
			wait(BALANCE_STABLE_TIME);   //等待3秒钟，等待水流稳定
		}
	}

	int i=0;
	if (m_resetZero) //初值回零
	{
// 		if (m_autopick || order==1 ) //自动采集或者是第一个检定点,需要等待热表初值回零
// 		{
			ui.labelHintPoint->setText(tr("Reset Zero"));
			while (i < RESET_ZERO_TIME && !m_stopFlag) //等待被检表初值回零
			{
				ui.labelHintProcess->setText(tr("please wait <font color=DarkGreen size=6><b>%1</b></font> seconds for reset zero").arg(RESET_ZERO_TIME-i));
				i++;
				wait(CYCLE_TIME); 
			}
// 		}
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

/*
** 进行单个流量点的检定
** order:检定次序，从1开始
*/
int TotalWeightDlg50::startVerifyFlowPoint(int order)
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
		row = m_meterPosMap[p] - 1;
		ui.tableWidget->item(row, COLUMN_METER_START)->text().toFloat(&ok);
		if (!ok || ui.tableWidget->item(row, COLUMN_METER_START)->text().isEmpty())
		{
			ui.labelHintProcess->setText(tr("please input start value of heat meter"));
			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START); //定位到需要输入初值的地方
			return false;
		}
	}

	bool bigFlag = false;
	if (m_paraSetReader->getFpBySeq(order).fp_valve_idx == 0) //用来区分大、小天平 0:大天平
	{
		bigFlag = true;
	}

	if (bigFlag)
	{
		m_balStartV = ui.lcdBigBalance->text().toFloat(); //记录大天平初值
	}
	else
	{
		m_balStartV = ui.lcdSmallBalance->text().toFloat(); //记录小天平初值
	}
	m_pipeInTemper = ui.lcdInTemper->value();
	m_pipeOutTemper = ui.lcdOutTemper->value();
	m_realFlow = ui.lcdFlowRate->value();
	m_avgTFCount = 1;
	m_stdInTemper = ui.lnEditInStdTemp->text().toFloat();
	m_stdOutTemper = ui.lnEditOutStdTemp->text().toFloat();

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
		if (judgeBalanceAndCalcAvgTemperAndFlow(m_balStartV + verifyQuantity, bigFlag)) //跑完检定量并计算此过程的平均温度和平均流量
		{
			ui.tableWidget->setEnabled(true);
			ui.btnAllReadNO->setEnabled(true);
			ui.btnAllReadData->setEnabled(true);
			ui.btnAllVerifyStatus->setEnabled(true);
			setRegulatorOpening(regNO, 0); //关闭调节阀
			closeValve(portNo); //关闭order对应的阀门
			wait(BALANCE_STABLE_TIME); //等待3秒钟，让天平数值稳定
			if (bigFlag)
			{
				m_balEndV = ui.lcdBigBalance->text().toFloat(); //记录大天平终值
			}
			else
			{
				m_balEndV = ui.lcdSmallBalance->text().toFloat(); //记录小天平终值
			}

			if (!m_resetZero && m_nowOrder>=2)
			{
				m_state = STATE_END_VALUE;
				makeStartValueByLastEndValue();
			}
			for (int m=0; m<m_validMeterNum; m++) //
			{
				m_meterTemper[m] = m_chkAlg->getMeterTempByPos(m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m]);//计算每个被检表的温度
				m_meterDensity[m] = m_chkAlg->getDensityByQuery(m_meterTemper[m]);//计算每个被检表的密度
				 //计算每个被检表的热量标准值
				m_meterStdValue[m] = m_chkAlg->calcStdEnergyByEnthalpy(m_stdInTemper, m_stdOutTemper, m_balEndV-m_balStartV, m_unit, WEIGHT_METHOD, bigFlag);

				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_FLOW_POINT)->setText(QString::number(m_realFlow, 'f', 3));//流量点
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_METER_END)->setText("");//表终值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_START)->setText(QString::number(m_balStartV, 'f', 3));//天平初值
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_END)->setText(QString::number(m_balEndV, 'f', 3));    //天平终值
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
int TotalWeightDlg50::calcAllMeterError()
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
int TotalWeightDlg50::calcMeterError(int idx)
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
	float stdError = m_totalSC*(m_gradeErrA[m_nowParams->m_grade] + 4*m_minDeltaT/(m_stdInTemper-m_stdOutTemper) + m_gradeErrB[m_nowParams->m_grade]*m_mapNormalFlow[m_standard]/m_realFlow); //标准误差=规程要求误差*总量安全系数
	ui.tableWidget->item(row, COLUMN_STD_ERROR)->setText("±" + QString::number(stdError, 'f', ERR_PRECISION)); //标准误差
	if (fabs(m_meterError[idx]) > stdError)
	{
		ui.tableWidget->item(row, COLUMN_DISP_ERROR)->setForeground(QBrush(Qt::red));
		ui.tableWidget->item(row, COLUMN_METER_NUMBER)->setForeground(QBrush(Qt::red));
	}
	QString meterNoStr = m_numPrefix + QString("%1").arg(ui.tableWidget->item(row, 0)->text(), 8, '0');

	strncpy_s(m_recPtr[idx].timestamp, m_timeStamp.toAscii(), TIMESTAMP_LEN);
	m_recPtr[idx].flowPoint = m_realFlow;
	strcpy_s(m_recPtr[idx].meterNo, meterNoStr.toAscii());
	m_recPtr[idx].flowPointIdx = m_nowOrder; //
	m_recPtr[idx].methodFlag = WEIGHT_METHOD; //质量法
	m_recPtr[idx].meterValue0 = m_meterStartValue[idx];
	m_recPtr[idx].meterValue1 = m_meterEndValue[idx];
	m_recPtr[idx].balWeight0 = m_balStartV;
	m_recPtr[idx].balWeight1 = m_balEndV;
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
	m_recPtr[idx].totalcoe = m_nowParams->sc_thermal;
	m_recPtr[idx].inSlotTemper = ui.lnEditInStdTemp->text().toFloat();
	m_recPtr[idx].outSlotTemper = ui.lnEditOutStdTemp->text().toFloat();
	m_recPtr[idx].deviceInfoId = m_readComConfig->getDeviceInfoID();

	return 1; 
}

/*
** 输完热量表终值后，计算检定结果
** 返回值：1-计算成功、读表数据全部成功
		   0-计算失败、读表数据有失败的
*/
int TotalWeightDlg50::calcVerifyResult()
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
			exportReport();
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

void TotalWeightDlg50::exportReport()
{
	QString sqlCondition = QString("F_TimeStamp=\'%1\' and F_MethodFlag = 0").arg(m_timeStamp);
	QString xlsname = QDateTime::fromString(m_timeStamp, "yyyy-MM-dd HH:mm:ss.zzz").toString("yyyy-MM-dd_hh-mm-ss") + ".xls";
	try
	{
		QString defaultPath = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\report\\total\\mass\\";
		CReport rpt(sqlCondition);
		rpt.setIniName("rptconfig_total_mass.ini");
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
int TotalWeightDlg50::openValve(UINT8 portno)
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
int TotalWeightDlg50::closeValve(UINT8 portno)
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
int TotalWeightDlg50::operateValve(UINT8 portno)
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
int TotalWeightDlg50::openWaterPump()
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
int TotalWeightDlg50::closeWaterPump()
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
int TotalWeightDlg50::operateWaterPump()
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
void TotalWeightDlg50::slotSetValveBtnStatus(const UINT8 &portno, const bool &status)
{
	m_valveStatus[portno] = status;
	setValveBtnBackColor(m_valveBtn[portno], m_valveStatus[portno]);
}

//响应调节阀调节成功
void TotalWeightDlg50::slotSetRegulateOk()
{
// 	setRegBtnBackColor(m_regBtn[m_nowRegIdx], true);
}


//自动读取表号成功 显示表号
void TotalWeightDlg50::slotSetMeterNumber(const QString& comName, const QString& meterNo)
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
** 自动读取表热量成功 显示表热量
*/
void TotalWeightDlg50::slotSetMeterEnergy(const QString& comName, const QString& energy)
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
	bool ok;
	float heat = energy.toFloat(&ok);
	if (m_state == STATE_START_VALUE) //初值
	{
    ui.tableWidget->item(meterPos - 1, COLUMN_METER_START)->setText(energy);
	}
	else if (m_state == STATE_END_VALUE) //终值
	{
  	ui.tableWidget->item(meterPos - 1, COLUMN_METER_END)->setText(energy);

	}
}

//设置阀门按钮背景色
void TotalWeightDlg50::setValveBtnBackColor(QToolButton *btn, bool status)
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
void TotalWeightDlg50::setRegBtnBackColor(QPushButton *btn, bool status)
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
void TotalWeightDlg50::on_btnParaSet_clicked()
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
	m_paraSetDlg->show();
}

/*
** 控制继电器开断
*/
void TotalWeightDlg50::on_btnWaterIn_clicked() //进水阀
{
	m_nowPortNo = m_portsetinfo.waterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnBigWaterIn_clicked() //大天平进水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnBigWaterOut_clicked() //大天平放水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterOutNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnSmallWaterIn_clicked() //小天平进水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnSmallWaterOut_clicked() //小天平放水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterOutNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveBig_clicked() //大流量阀
{
	m_nowPortNo = m_portsetinfo.bigNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveMiddle1_clicked() //中流一阀
{
	m_nowPortNo = m_portsetinfo.middle1No;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveMiddle2_clicked() //中流二阀
{
	m_nowPortNo = m_portsetinfo.middle2No;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveSmall_clicked() //小流量阀
{
	m_nowPortNo = m_portsetinfo.smallNo;
	operateValve(m_nowPortNo);
}

/*
** 控制水泵开关
*/
void TotalWeightDlg50::on_btnWaterPump_clicked()
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
void TotalWeightDlg50::on_btnSetFreq_clicked()
{
	if (NULL == m_controlObj)
	{
		return;
	}
	m_controlObj->askSetDriverFreq(ui.spinBoxFreq->value());
}

//获取表初值
int TotalWeightDlg50::getMeterStartValue()
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
int TotalWeightDlg50::getMeterEndValue()
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
void TotalWeightDlg50::makeStartValueByLastEndValue()
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
void TotalWeightDlg50::on_tableWidget_cellChanged(int row, int column)
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
int TotalWeightDlg50::saveAllVerifyRecords()
{
 	int ret = insertTotalVerifyRec(m_recPtr, m_validMeterNum);
	if (ret != OPERATE_DB_OK)
	{
		QMessageBox::warning(this, tr("Error"), tr("Error:insert database failed!\n") + tr("Maybe network error!"));
	}
	return ret;
}

//请求读表号（所有表、广播地址读表）
void TotalWeightDlg50::on_btnAllReadNO_clicked()
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
void TotalWeightDlg50::on_btnAllReadData_clicked()
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
void TotalWeightDlg50::on_btnAllVerifyStatus_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotVerifyStatus(i);
	}
}

/*
** 读取表号
** 输入参数：
	row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void TotalWeightDlg50::slotReadNO(const int &row)
{
	qDebug()<<"slotReadNO row ="<<row;
	m_meterObj[row].askReadMeterNO();
}

/*
** 读取表数据
** 输入参数：
	row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void TotalWeightDlg50::slotReadData(const int &row)
{
	qDebug()<<"slotReadData row ="<<row;
	m_meterObj[row].askReadMeterData();
}

/*
** 检定状态
** 输入参数：
	row:行号，由row可以知道当前热表对应的串口、表号、误差等等
*/
void TotalWeightDlg50::slotVerifyStatus(const int &row)
{
	qDebug()<<"slotVerifyStatus row ="<<row;
	m_meterObj[row].askSetVerifyStatus(VTYPE_HEAT);
}

/*
** 打开4路电动调节阀至设定的开度
** 注意：选中的管路，将调节阀开度调整到设定开度；
         未选中的管路，将将调节阀开度调整到50%，用于排气
*/
void TotalWeightDlg50::openAllRegulator()
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
void TotalWeightDlg50::closeAllRegulator()
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

void TotalWeightDlg50::setRegulatorOpening(int regNO, int opening)
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
void TotalWeightDlg50::on_btnRegulateSmall_clicked() //调节阀1-DN3
{
	m_smallOpening = ui.ThermoSmall->value();
	askControlRegulate(m_portsetinfo.regSmallNo, ui.spinBoxOpeningSmall->value());
	m_regSmallTimer->start(REGULATE_FRESH_TIME);
}

void TotalWeightDlg50::on_btnRegulateMid1_clicked() //调节阀2-DN10
{
	m_mid1Opening = ui.ThermoMid1->value();
	askControlRegulate(m_portsetinfo.regMid1No, ui.spinBoxOpeningMid1->value());
	m_regMid1Timer->start(REGULATE_FRESH_TIME);
}

void TotalWeightDlg50::on_btnRegulateMid2_clicked() //调节阀3-DN25
{
	m_mid2Opening = ui.ThermoMid2->value();
	askControlRegulate(m_portsetinfo.regMid2No, ui.spinBoxOpeningMid2->value());
	m_regMid2Timer->start(REGULATE_FRESH_TIME);
}

void TotalWeightDlg50::on_btnRegulateBig_clicked() //调节阀4-DN50
{
	m_bigOpening = ui.ThermoBig->value();
	askControlRegulate(m_portsetinfo.regBigNo, ui.spinBoxOpeningBig->value());
	m_regBigTimer->start(REGULATE_FRESH_TIME);
}

void TotalWeightDlg50::askControlRegulate(int regNO, int opening)
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

void TotalWeightDlg50::on_lineEditOpeningSmall_textChanged(const QString & text)
{
	ui.ThermoSmall->setValue(text.toFloat());
}

void TotalWeightDlg50::on_lineEditOpeningMid1_textChanged(const QString & text)
{
	ui.ThermoMid1->setValue(text.toFloat());
}

void TotalWeightDlg50::on_lineEditOpeningMid2_textChanged(const QString & text)
{
	ui.ThermoMid2->setValue(text.toFloat());
}

void TotalWeightDlg50::on_lineEditOpeningBig_textChanged(const QString & text)
{
	ui.ThermoBig->setValue(text.toFloat());
}

void TotalWeightDlg50::slotFreshSmallRegOpening()
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

void TotalWeightDlg50::slotFreshMid1RegOpening()
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

void TotalWeightDlg50::slotFreshMid2RegOpening()
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

void TotalWeightDlg50::slotFreshBigRegOpening()
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
