/***********************************************
**  文件名:     datatestdlg50.cpp
**  功能:       采集与控制测试程序，针对DN50，双天平、双控制板
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2015/11/23
**  专业组:     德鲁计量软件组
**  程序设计者: YS
**  程序员:     YS
**  版本历史:   2015/11 第一版
**  内容包含:
**  说明:		
**  更新记录:   
***********************************************/

#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

#include "datatestdlg50.h"
#include "commondefine.h"
#include "parasetdlg.h"
#include "readcomconfig.h"
#include "algorithm.h"

DataTestDlg50::DataTestDlg50(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
}

DataTestDlg50::~DataTestDlg50()
{
}

void DataTestDlg50::closeEvent( QCloseEvent * event)
{
	qDebug()<<"^^^^^DataTestDlg::closeEvent";

// 	m_controlObj->askControlRelay(m_portsetinfo.waterOutNo, VALVE_OPEN); //退出时自动打开放水阀

	if (m_paraSetReader) //读检定参数
	{
		delete m_paraSetReader;
		m_paraSetReader = NULL;
	}

	if (m_paraSetDlg) //参数设置对话框
	{
		delete m_paraSetDlg;
		m_paraSetDlg = NULL;
	}

	if (m_readComConfig)  //读串口设置
	{
		delete m_readComConfig;
		m_readComConfig = NULL;
	}

	if (m_controlObj)  //阀门控制
	{
		closePump();//退出时, 关闭水泵
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

	if (m_meterObj)  //热量表串口通讯
	{
		delete m_meterObj;
		m_meterObj = NULL;

		m_meterThread.exit();
	}
/*--------------------------------------------------------------*/
	if (m_tempTimer) //读温度计时器
	{
		if (m_tempTimer->isActive())
		{
			m_tempTimer->stop();
		}
		delete m_tempTimer;
		m_tempTimer = NULL;
	}

	if (m_tempThread.isRunning())
	{
		m_tempThread.exit();
		if (m_tempObj)  // 温度采集
		{
			delete m_tempObj;
			m_tempObj = NULL;
		}
	}
/*--------------------------------------------------------------*/
	if (m_stdTempTimer) //读标准温度计时器
	{
		if (m_stdTempTimer->isActive())
		{
			m_stdTempTimer->stop();
		}
		delete m_stdTempTimer;
		m_stdTempTimer = NULL;
	}
	if (m_stdTempThread.isRunning())
	{
		m_stdTempThread.exit();
		if (m_stdTempObj)  // 标准温度采集
		{
			delete m_stdTempObj;
			m_stdTempObj = NULL;

			m_stdTempThread.exit();
		}
	}	
/*--------------------------------------------------------------*/
	if (m_flowRateTimer) //计算流量计时器
	{
		if (m_flowRateTimer->isActive())
		{
			m_flowRateTimer->stop();
		}
		delete m_flowRateTimer;
		m_flowRateTimer = NULL;
	}

	if (m_oldCoe)
	{
		delete m_oldCoe;
		m_oldCoe = NULL;
	}
/*****************************************************/

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
}

void DataTestDlg50::showEvent(QShowEvent *event)
{
	qDebug()<<"DataTestDlg showEvent thread:"<<QThread::currentThreadId();
	//获取下位机端口配置信息
	if (!getPortSetIni(&m_portsetinfo))
	{
		QMessageBox::warning(this, tr("Warning"), tr("get port set info failed!"));//获取下位机端口配置信息失败!请重新设置！
	}

	initValveStatus();      //初始化阀门状态
	initRegulateStatus();   //初始化调节阀状态

	m_paraSetDlg = NULL;
	m_paraSetReader = new ParaSetReader(); //读参数设置接口

	m_readComConfig = NULL;
	m_readComConfig = new ReadComConfig();
	m_readComConfig->getBalancePara(m_balMaxWht, m_balBottomWht); //获取大天平最大容量和回水底量
	m_readComConfig->getBalancePara2(m_balMaxWht2, m_balBottomWht2); //获取小天平最大容量和回水底量

	m_tempObj = NULL;
	m_tempTimer = NULL;
	initTemperatureCom();	 //初始化管路温度采集串口

	m_stdTempObj = NULL;
	m_stdTempTimer = NULL;
	initStdTemperatureCom(); //初始化标准温度采集串口

	m_controlObj = NULL;
	initControlCom();		//初始化控制串口

	m_controlObj2 = NULL;
	initControlCom2();		//初始化控制串口2

	m_balanceObj = NULL;
	initBalanceCom();		//初始化天平串口

	m_balanceObj2 = NULL;
	initBalanceCom2();		//初始化天平串口2

	m_meterObj = NULL;
	initComOfHeatMeter();	//初始化热量表串口

	//计算流速用
	m_totalcount = 0;
	m_startWeight = 0.0;
	m_endWeight = 0.0;
	memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	m_flowRateTimer = new QTimer();
	connect(m_flowRateTimer, SIGNAL(timeout()), this, SLOT(slotFreshFlowRate()));
	m_flowRateTimer->start(TIMEOUT_FLOW_SAMPLE);

	m_oldCoe = new MeterCoe_STR;
	memset(m_oldCoe, 0, sizeof(MeterCoe_STR));

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

void DataTestDlg50::slotFreshInstFlow(const flow_rate_wdg& idx, const float& value)
{
	m_mapInstWdg[idx]->display(value);
}

void DataTestDlg50::slotFreshAccumFlow(const flow_rate_wdg& idx, const float& value)
{
	m_mapAccumWdg[idx]->display(value);
}

void DataTestDlg50::slotFreshTolInst(const float& value)
{
	ui.lcdStdMeterFlowRate->display(value);
}

void DataTestDlg50::slotFreshTolAccum(const float& value)
{
	ui.lcdAccumStdMeter->display(value);
}

/*
** 开辟一个新线程，打开温度采集串口
*/
void DataTestDlg50::initTemperatureCom()
{
	ComInfoStruct tempStruct = m_readComConfig->ReadTempConfig();
	m_tempObj = new TempComObject();
	m_tempObj->moveToThread(&m_tempThread);
	m_tempThread.start();
	m_tempObj->openTemperatureCom(&tempStruct);
	connect(m_tempObj, SIGNAL(temperatureIsReady(const QString &)), this, SLOT(slotFreshComTempValue(const QString &)));

	m_tempTimer = new QTimer();
	//connect(m_tempTimer, SIGNAL(timeout()), m_tempObj, SLOT(writeTemperatureComBuffer()));
	connect(m_tempTimer, SIGNAL(timeout()), this, SLOT(slotAskPipeTemperature()));
// 	connect(m_tempTimer, SIGNAL(timeout()), this, SLOT(slotFreshFlow()));
	
	m_tempTimer->start(TIMEOUT_PIPE_TEMPER); //周期请求温度
}

void DataTestDlg50::slotAskPipeTemperature()
{
	m_tempObj->writeTemperatureComBuffer();
}

/*
** 开辟一个新线程，打开标准温度采集串口
*/
void DataTestDlg50::initStdTemperatureCom()
{
	ComInfoStruct tempStruct = m_readComConfig->ReadStdTempConfig();
	m_stdTempObj = new StdTempComObject();
	QSettings stdconfig(getFullIniFileName("stdplasensor.ini"), QSettings::IniFormat);
	m_stdTempObj->moveToThread(&m_stdTempThread);
	m_stdTempThread.start();
	m_stdTempObj->openTemperatureCom(&tempStruct); //先打开串口，然后再设置标准温度计的协议版本
	m_stdTempObj->setStdTempVersion(stdconfig.value("in_use/model").toInt());
	connect(m_stdTempObj, SIGNAL(temperatureIsReady(const QString &)), this, SLOT(slotFreshStdTempValue(const QString &)));

	m_stdTempCommand = stdTempR1;
	m_stdTempTimer = new QTimer();
	connect(m_stdTempTimer, SIGNAL(timeout()), this, SLOT(slotAskStdTemperature()));
	
 	m_stdTempTimer->start(TIMEOUT_STD_TEMPER);
}

void DataTestDlg50::slotAskStdTemperature()
{
	m_stdTempObj->writeStdTempComBuffer(m_stdTempCommand);
}

//天平采集串口 上位机直接采集
void DataTestDlg50::initBalanceCom()
{
	ComInfoStruct balanceStruct = m_readComConfig->ReadBalanceConfig();
	m_balanceObj = new BalanceComObject();
	m_balanceObj->setBalanceType(m_readComConfig->getBalanceType());
	m_balanceObj->moveToThread(&m_balanceThread);
	m_balanceThread.start();
	m_balanceObj->openBalanceCom(&balanceStruct);

	//天平数值由上位机直接通过天平串口采集
	connect(m_balanceObj, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshBigBalanceValue(const float &)));
}

//天平采集串口 上位机直接采集2
void DataTestDlg50::initBalanceCom2()
{
	ComInfoStruct balanceStruct2 = m_readComConfig->ReadBalanceConfig2();
	m_balanceObj2 = new BalanceComObject();
	m_balanceObj2->setBalanceType(m_readComConfig->getBalanceType2());
	m_balanceObj2->moveToThread(&m_balanceThread2);
	m_balanceThread2.start();
	m_balanceObj2->openBalanceCom(&balanceStruct2);

	//天平数值由上位机直接通过天平串口采集
	connect(m_balanceObj2, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshSmallBalanceValue(const float &)));
}

/*
** 开辟一个新线程，打开控制板通讯串口
*/
void DataTestDlg50::initControlCom()
{
	ComInfoStruct valveStruct = m_readComConfig->ReadValveConfig();
	m_controlObj = new ControlComObject();
	m_controlObj->setProtocolVersion(m_portsetinfo.version); //设置协议版本
	m_controlObj->moveToThread(&m_valveThread);
	m_valveThread.start();
	m_controlObj->openControlCom(&valveStruct);

	connect(m_controlObj, SIGNAL(controlRelayIsOk(const UINT8 &, const bool &)), this, SLOT(slotSetValveBtnStatus(const UINT8 &, const bool &)));
	connect(m_controlObj, SIGNAL(controlRegulateIsOk()), this, SLOT(slotSetRegulateOk()));
	//天平数值从控制板获取
// 	connect(m_controlObj, SIGNAL(controlGetBalanceValueIsOk(const float&)), this, SLOT(slotFreshBalanceValue(const float &)));
}

/*
** 开辟一个新线程，打开控制板通讯串口2
*/
void DataTestDlg50::initControlCom2()
{
	ComInfoStruct valveStruct2 = m_readComConfig->ReadValveConfig2();
	m_controlObj2 = new ControlComObject();
	m_controlObj2->setProtocolVersion(m_portsetinfo.version); //设置协议版本
	m_controlObj2->moveToThread(&m_valveThread2);
	m_valveThread2.start();
	m_controlObj2->openControlCom(&valveStruct2);

	connect(m_controlObj2, SIGNAL(controlRelayIsOk(const UINT8 &, const bool &)), this, SLOT(slotSetValveBtnStatus(const UINT8 &, const bool &)));
	connect(m_controlObj2, SIGNAL(controlRegulateIsOk()), this, SLOT(slotSetRegulateOk()));
}

//热量表串口通讯
void DataTestDlg50::initComOfHeatMeter()
{
	m_meterObj = new MeterComObject();
	int version = m_paraSetReader->getParams()->m_pickcode;
	m_meterObj->setProtocolVersion(version);//设置协议版本号
	m_meterObj->moveToThread(&m_meterThread);
	m_meterThread.start();

	connect(m_meterObj, SIGNAL(readMeterNoIsOK(const QString&, const QString&)), this, SLOT(slotFreshMeterNo(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterFlowIsOK(const QString&, const QString&)), this, SLOT(slotFreshMeterFlow(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterHeatIsOK(const QString&, const QString&)), this, SLOT(slotFreshMeterHeat(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterDateIsOK(const QString&, const QString&)), this, SLOT(slotFreshMeterDate(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterTemperIsOK(const QString&, const QString&, const QString&)), \
		this, SLOT(slotFreshMeterTemper(const QString&, const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterBigCoeIsOK(const QString&, const QString&)), \
		this, SLOT(slotFreshBigCoe(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterMid2CoeIsOK(const QString&, const QString&)), \
		this, SLOT(slotFreshMid2Coe(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterMid1CoeIsOK(const QString&, const QString&)), \
		this, SLOT(slotFreshMid1Coe(const QString&, const QString&)));
	connect(m_meterObj, SIGNAL(readMeterSmallCoeIsOK(const QString&, const QString&)), \
		this, SLOT(slotFreshSmallCoe(const QString&, const QString&)));

	QStringList cfgList = m_readComConfig->ReadIndexByName(meter(1));
	ui.portNameComboBox->setCurrentIndex(cfgList.at(0).toInt());
	ui.baudRateComboBox->setCurrentIndex(cfgList.at(1).toInt());
	ui.dataBitsComboBox->setCurrentIndex(cfgList.at(2).toInt());
	ui.parityComboBox->setCurrentIndex(cfgList.at(3).toInt());
	ui.stopBitsComboBox->setCurrentIndex(cfgList.at(4).toInt());

	ui.btnOpenCom->setEnabled(true);
	ui.btnCloseCom->setEnabled(false);
	setMeterComboxEnabled(true);
	setMeterOperBtnEnabled(false);
	clearMeterDispInfo();
}

//初始化阀门状态
void DataTestDlg50::initValveStatus()
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

	//端口号-阀门状态 全部阀门状态为关闭
	m_valveStatus[m_portsetinfo.bigNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle1No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle2No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.waterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.smallWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallWaterOutNo] = VALVE_OPEN;
	//端口号-水泵状态 初始为关闭
	m_valveStatus[m_portsetinfo.pumpNo] = VALVE_CLOSE;

	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigNo], m_valveStatus[m_portsetinfo.bigNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallNo], m_valveStatus[m_portsetinfo.smallNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.middle1No], m_valveStatus[m_portsetinfo.middle1No]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.middle2No], m_valveStatus[m_portsetinfo.middle2No]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.waterInNo], m_valveStatus[m_portsetinfo.waterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigWaterInNo], m_valveStatus[m_portsetinfo.bigWaterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.bigWaterOutNo], m_valveStatus[m_portsetinfo.bigWaterOutNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallWaterInNo], m_valveStatus[m_portsetinfo.smallWaterInNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.smallWaterOutNo], m_valveStatus[m_portsetinfo.smallWaterOutNo]);
	setValveBtnBackColor(m_valveBtn[m_portsetinfo.pumpNo], m_valveStatus[m_portsetinfo.pumpNo]); //水泵
}

//初始化调节阀状态
void DataTestDlg50::initRegulateStatus()
{
	m_nowRegNo = 0;

	//端口号-调节阀按钮 映射关系
	m_regBtn[m_portsetinfo.regSmallNo] = ui.btnRegulateSmall;
	m_regBtn[m_portsetinfo.regMid1No] = ui.btnRegulateMid1;
	m_regBtn[m_portsetinfo.regMid2No] = ui.btnRegulateMid2;
	m_regBtn[m_portsetinfo.regBigNo] = ui.btnRegulateBig;

	//调节阀初始状态
	m_regStatus[m_portsetinfo.regSmallNo] = REG_SUCCESS;
	m_regStatus[m_portsetinfo.regMid1No] = REG_SUCCESS;
	m_regStatus[m_portsetinfo.regMid2No] = REG_SUCCESS;
	m_regStatus[m_portsetinfo.regBigNo] = REG_SUCCESS;

	setRegBtnBackColor(m_regBtn[m_portsetinfo.regSmallNo], m_regStatus[m_portsetinfo.regSmallNo]);
	setRegBtnBackColor(m_regBtn[m_portsetinfo.regMid1No], m_regStatus[m_portsetinfo.regMid1No]);
	setRegBtnBackColor(m_regBtn[m_portsetinfo.regMid2No], m_regStatus[m_portsetinfo.regMid2No]);
	setRegBtnBackColor(m_regBtn[m_portsetinfo.regBigNo], m_regStatus[m_portsetinfo.regBigNo]);

	//端口号-调节阀开度显示行 映射关系
	m_regLineEdit[m_portsetinfo.regSmallNo] = ui.lineEditOpeningSmall;
	m_regLineEdit[m_portsetinfo.regMid1No] = ui.lineEditOpeningMid1;
	m_regLineEdit[m_portsetinfo.regMid2No] = ui.lineEditOpeningMid2;
	m_regLineEdit[m_portsetinfo.regBigNo] = ui.lineEditOpeningBig;

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

//打开热量表通讯串口
void DataTestDlg50::on_btnOpenCom_clicked()
{
	ComInfoStruct comStruct;
	comStruct.portName = ui.portNameComboBox->currentText();
	comStruct.baudRate = ui.baudRateComboBox->currentText().toInt();
	comStruct.dataBit = ui.dataBitsComboBox->currentText().toInt();
	comStruct.parity = ui.parityComboBox->currentIndex();
	comStruct.stopBit = ui.stopBitsComboBox->currentIndex();
	if (!m_meterObj->openMeterCom(&comStruct))
	{
		return; //打开热表通讯串口失败
	}

	ui.btnOpenCom->setEnabled(false);
	ui.btnCloseCom->setEnabled(true);
	setMeterOperBtnEnabled(true);
	setMeterComboxEnabled(false);
}

//关闭热量表通讯串口
void DataTestDlg50::on_btnCloseCom_clicked()
{
	m_meterObj->closeMeterCom();

	ui.btnOpenCom->setEnabled(true);
	ui.btnCloseCom->setEnabled(false);
	setMeterOperBtnEnabled(false);
	setMeterComboxEnabled(true);
}

/*
** 控制继电器开断
*/
void DataTestDlg50::operateRelay()
{
	if (m_nowPortNo <= 8) //1#控制板
	{
		m_controlObj->askControlRelay(m_nowPortNo, !m_valveStatus[m_nowPortNo]);
	}
	else //2#控制板
	{
		m_controlObj2->askControlRelay(m_nowPortNo-8, !m_valveStatus[m_nowPortNo]);
	}

	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, !m_valveStatus[m_nowPortNo]);
	}
}

void DataTestDlg50::on_btnWaterIn_clicked() //进水阀
{
	m_nowPortNo = m_portsetinfo.waterInNo;
	operateRelay();
}

void DataTestDlg50::on_btnBigWaterIn_clicked() //大天平进水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterInNo;
	operateRelay();
}

void DataTestDlg50::on_btnBigWaterOut_clicked() //大天平放水阀
{
	m_nowPortNo = m_portsetinfo.bigWaterOutNo;
	operateRelay();
}

void DataTestDlg50::on_btnSmallWaterIn_clicked() //小天平进水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterInNo;
	operateRelay();
}

void DataTestDlg50::on_btnSmallWaterOut_clicked() //小天平放水阀
{
	m_nowPortNo = m_portsetinfo.smallWaterOutNo;
	operateRelay();
}

void DataTestDlg50::on_btnValveBig_clicked() //大流量阀
{
	m_nowPortNo = m_portsetinfo.bigNo;
	operateRelay();

	m_nowRegNo = m_portsetinfo.regSmallNo;
}

void DataTestDlg50::on_btnValveMiddle1_clicked() //中流一阀
{
	m_nowPortNo = m_portsetinfo.middle1No;
	operateRelay();

	m_nowRegNo = m_portsetinfo.regMid1No;
}

void DataTestDlg50::on_btnValveMiddle2_clicked() //中流二阀
{
	m_nowPortNo = m_portsetinfo.middle2No;
	operateRelay();
}

void DataTestDlg50::on_btnValveSmall_clicked() //小流量阀
{
	m_nowPortNo = m_portsetinfo.smallNo;
	operateRelay();
}

/*
** 控制水泵开关
*/
void DataTestDlg50::on_btnWaterPump_clicked() //水泵
{
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, !m_valveStatus[m_nowPortNo]);

	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, !m_valveStatus[m_nowPortNo]);
	}
}

void DataTestDlg50::openPump()
{
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, true);

	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, true);
	}
}

void DataTestDlg50::closePump()
{
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, false);

	if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
	{
		slotSetValveBtnStatus(m_nowPortNo, false);
	}
}

//设置频率
void DataTestDlg50::on_btnSetFreq_clicked()
{
	m_controlObj->askSetDriverFreq(ui.spinBoxFreq->value());
}

//电动调节阀
void DataTestDlg50::on_btnRegulateSmall_clicked() //调节阀1-DN3
{
	m_nowRegNo = m_portsetinfo.regSmallNo;
	m_smallOpening = ui.ThermoSmall->value();
	setRegulate(m_nowRegNo, ui.spinBoxOpeningSmall->value());
	m_regSmallTimer->start(REGULATE_FRESH_TIME);
}

void DataTestDlg50::on_btnRegulateMid1_clicked() //调节阀2-DN10
{
	m_nowRegNo = m_portsetinfo.regMid1No;
	m_mid1Opening = ui.ThermoMid1->value();
	setRegulate(m_nowRegNo, ui.spinBoxOpeningMid1->value());
	m_regMid1Timer->start(REGULATE_FRESH_TIME);
}

void DataTestDlg50::on_btnRegulateMid2_clicked() //调节阀3-DN25
{
	m_nowRegNo = m_portsetinfo.regMid2No;
	m_mid2Opening = ui.ThermoMid2->value();
	setRegulate(m_nowRegNo, ui.spinBoxOpeningMid2->value());
	m_regMid2Timer->start(REGULATE_FRESH_TIME);
}

void DataTestDlg50::on_btnRegulateBig_clicked() //调节阀4-DN50
{
	m_nowRegNo = m_portsetinfo.regBigNo;
	m_bigOpening = ui.ThermoBig->value();
	setRegulate(m_nowRegNo, ui.spinBoxOpeningBig->value());
	m_regBigTimer->start(REGULATE_FRESH_TIME);
}

void DataTestDlg50::setRegulate(int regNO, int opening)
{
	setRegBtnBackColor(m_regBtn[regNO], false); //初始化调节阀背景色
	if (regNO>=1 && regNO<=3)
	{
		m_controlObj->askControlRegulate(regNO, opening);
	}
	else
	{
		m_controlObj2->askControlRegulate(regNO-3, opening);
	}
}

void DataTestDlg50::on_lineEditOpeningSmall_textChanged(const QString & text)
{
	ui.ThermoSmall->setValue(text.toFloat());
}

void DataTestDlg50::on_lineEditOpeningMid1_textChanged(const QString & text)
{
	ui.ThermoMid1->setValue(text.toFloat());
}

void DataTestDlg50::on_lineEditOpeningMid2_textChanged(const QString & text)
{
	ui.ThermoMid2->setValue(text.toFloat());
}

void DataTestDlg50::on_lineEditOpeningBig_textChanged(const QString & text)
{
	ui.ThermoBig->setValue(text.toFloat());
}

void DataTestDlg50::slotFreshSmallRegOpening()
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

void DataTestDlg50::slotFreshMid1RegOpening()
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

void DataTestDlg50::slotFreshMid2RegOpening()
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

void DataTestDlg50::slotFreshBigRegOpening()
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

//参数设置
void DataTestDlg50::on_btnParaSet_clicked()
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
	m_paraSetDlg->show();
}

void DataTestDlg50::on_btnExit_clicked()
{
	this->close();
}

//采集标准温度
void DataTestDlg50::on_btnStdTempCollect_clicked()
{
	m_stdTempTimer->start(TIMEOUT_STD_TEMPER);
}

//停止采集标准温度
void DataTestDlg50::on_btnStdTempStop_clicked()
{
	m_stdTempTimer->stop();
}

void DataTestDlg50::on_lnEditInStdResist_textChanged(const QString & text)
{
	float resis = text.toFloat();
	float temp = calcTemperByResis(resis);
	ui.lnEditInStdTemp->setText(QString::number(temp));
}

void DataTestDlg50::on_lnEditOutStdResist_textChanged(const QString & text)
{
	float resis = text.toFloat();
	float temp = calcTemperByResis(resis);
	ui.lnEditOutStdTemp->setText(QString::number(temp));
}

//刷新温度
void DataTestDlg50::slotFreshComTempValue(const QString& tempStr)
{
	ui.lnEditTempIn->setText(tempStr.left(TEMPER_DATA_WIDTH));   //入口温度 PV
	ui.lnEditTempOut->setText(tempStr.right(TEMPER_DATA_WIDTH)); //出口温度 SV
}

//刷新标准温度
void DataTestDlg50::slotFreshStdTempValue(const QString& stdTempStr)
{
// 	qDebug()<<"stdTempStr ="<<stdTempStr<<"; m_stdTempCommand ="<<m_stdTempCommand;
	switch (m_stdTempCommand)
	{
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

//刷新大天平数值
void DataTestDlg50::slotFreshBigBalanceValue(const float& balValue)
{
	ui.lnEditBigBalance->setText(QString::number(balValue, 'f', 3));

	if (balValue > m_balMaxWht) //防止天平溢出
	{
		m_controlObj->askControlRelay(m_portsetinfo.bigWaterOutNo, VALVE_OPEN);// 打开大天平放水阀	
		m_controlObj->askControlRelay(m_portsetinfo.bigWaterInNo, VALVE_OPEN);// 打开大天平进水阀
		m_controlObj->askControlRelay(m_portsetinfo.waterInNo, VALVE_CLOSE);// 关闭进水阀
		if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
		{
			slotSetValveBtnStatus(m_portsetinfo.bigWaterOutNo, VALVE_OPEN);
			slotSetValveBtnStatus(m_portsetinfo.bigWaterInNo, VALVE_OPEN);
			slotSetValveBtnStatus(m_portsetinfo.waterInNo, VALVE_CLOSE);
		}
		closePump();
	}
}

//刷新小天平数值
void DataTestDlg50::slotFreshSmallBalanceValue(const float& balValue)
{
	ui.lnEditSmallBalance->setText(QString::number(balValue, 'f', 3));

	if (balValue > m_balMaxWht2) //防止天平溢出
	{
		m_controlObj->askControlRelay(m_portsetinfo.smallWaterOutNo, VALVE_OPEN);// 打开小天平放水阀	
		m_controlObj->askControlRelay(m_portsetinfo.smallWaterInNo, VALVE_OPEN);// 打开小天平进水阀
		m_controlObj->askControlRelay(m_portsetinfo.waterInNo, VALVE_CLOSE);// 关闭进水阀
		if (m_portsetinfo.version == OLD_CTRL_VERSION) //老控制板 无反馈
		{
			slotSetValveBtnStatus(m_portsetinfo.smallWaterOutNo, VALVE_OPEN);
			slotSetValveBtnStatus(m_portsetinfo.smallWaterInNo, VALVE_OPEN);
			slotSetValveBtnStatus(m_portsetinfo.smallWaterInNo, VALVE_CLOSE);
		}
		closePump();
	}
}

//响应阀门状态设置成功
void DataTestDlg50::slotSetValveBtnStatus(const UINT8 &portno, const bool &status)
{
	m_valveStatus[portno] = status;
	setValveBtnBackColor(m_valveBtn[portno], m_valveStatus[portno]);
}

//响应调节阀调节成功
void DataTestDlg50::slotSetRegulateOk()
{
	setRegBtnBackColor(m_regBtn[m_nowRegNo], REG_SUCCESS);
}

//设置阀门按钮背景色
void DataTestDlg50::setValveBtnBackColor(QPushButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //阀门打开 绿色
	{
		btn->setStyleSheet("background-color:rgb(0,255,0);"); 
	}
	else //阀门关闭 红色
	{
		btn->setStyleSheet("background-color:rgb(255,0,0);border:0px;border-image: url(:/datatestdlg/images/close.png);"); 
	}
}

//设置调节阀按钮背景色
void DataTestDlg50::setRegBtnBackColor(QPushButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //调节成功
	{
// 		btn->setStyleSheet("background:rgb(0,255,0);border-image: url(:/datatestdlg/images/success.png);");
	}
	else //调节失败
	{
// 		btn->setStyleSheet("background:rgb(255,0,0);border-image: url(:/datatestdlg/images/failed.png);");
	}
}


/*
** 计算流速(每1秒采样一次天平变化值，计算前10秒的平均流速)
*/
void DataTestDlg50::slotFreshFlowRate()
{
	if (m_totalcount > 4294967290) //防止m_totalcount溢出 32位无符号整数范围0~4294967295
	{
		m_totalcount = 0;
		m_startWeight = 0.0;
		m_endWeight = 0.0;
		memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	}
	if (m_totalcount == 0) //记录天平初始重量
	{
		m_startWeight = ui.lnEditBigBalance->text().replace(" ", 0).toFloat();
		m_totalcount ++;
		return;
	}

	float flowValue = 0.0;
	float totalWeight = 0.0;
	m_endWeight = ui.lnEditBigBalance->text().replace(" ", 0).toFloat();//取当前天平值, 作为当前运算的终值
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
	ui.lnEditFlowRate->setText(QString::number(flowValue, 'f', 3)); //在ui.lnEditFlowRate中显示流速
	m_totalcount ++;//计数器累加
	m_startWeight = m_endWeight;//将当前值保存, 作为下次运算的初值
}

void DataTestDlg50::clearMeterDispInfo()
{
	ui.lnEditMeterNo->clear();
	ui.lnEditMeterTempIn->clear();
	ui.lnEditMeterTempOut->clear();
	ui.lnEditMeterTotalFlow->clear();
	ui.lnEditMeterHeat->clear();
	ui.dateEditMeter->setDate(QDate(2000,1,1));
	ui.lnEditBigOldError->clear();
	ui.lnEditBigOldCoe->clear();
	ui.lnEditBigOrgCoe->clear();
	ui.lnEditMid2OldError->clear();
	ui.lnEditMid2OldCoe->clear();
	ui.lnEditMid2OrgCoe->clear();
	ui.lnEditMid1OldError->clear();
	ui.lnEditMid1OldCoe->clear();
	ui.lnEditMid1OrgCoe->clear();
	ui.lnEditSmallOldError->clear();
	ui.lnEditSmallOldCoe->clear();
	ui.lnEditSmallOrgCoe->clear();
}

//设置检定状态
void DataTestDlg50::on_btnSetVerifyStatus_clicked()
{
	qDebug()<<"进入检定状态...";
	m_meterObj->askSetVerifyStatus(); //设置进入检定状态
}

//退出检定状态
void DataTestDlg50::on_btnExitVerifyStatus_clicked()
{
	qDebug()<<"退出检定状态...";
	m_meterObj->askExitVerifyStatus(); //设置退出检定状态
}

//读表数据
void DataTestDlg50::on_btnReadMeterData_clicked()
{
	clearMeterDispInfo();
	qDebug()<<"读表 开始...";
 	m_meterObj->askReadMeterData(); //请求读表
}

//修改表号
void DataTestDlg50::on_btnModifyMeterNo_clicked()
{
	QString oldMeterNo = ui.lnEditMeterNo->text();
	QString newMeterNo = ui.lnEditNewMeterNo->text();
	if (oldMeterNo.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("MeterNO. is empty! please read meter first!"));
		return;
	}
	if (newMeterNo.isEmpty() || newMeterNo.size()!=2*CJ188_ADDR_LEN)
	{
		QMessageBox::warning(this, tr("Warning"), tr("new NO. is error!\nplease input 14 bits meter number!"));
		ui.lnEditNewMeterNo->setFocus();
		return;
	}

	m_meterObj->askModifyMeterNO(oldMeterNo, newMeterNo); //请求修改表号
}

//修改流量系数
void DataTestDlg50::on_btnModifyFlowCoe_clicked()
{
	QString meterNO = ui.lnEditMeterNo->text();
	if (meterNO.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("MeterNO. is empty! please read meter first!"));
		return;
	}
	bool ok;
	float bigErr = ui.lnEditBigNewError->text().toFloat(&ok);
	if (!ok || ui.lnEditBigNewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float mid2Err = ui.lnEditMid2NewError->text().toFloat();
	if (!ok || ui.lnEditMid2NewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float mid1Err = ui.lnEditMid1NewError->text().toFloat();
	if (!ok || ui.lnEditMid1NewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float smallErr = ui.lnEditSmallNewError->text().toFloat();
	if (!ok || ui.lnEditSmallNewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	m_meterObj->askModifyFlowCoe(meterNO, bigErr, mid2Err, mid1Err, smallErr);
}

//二次修改流量系数
void DataTestDlg50::on_btn2ModifyFlowCoe_clicked()
{
	QString meterNO = ui.lnEditMeterNo->text();
	if (meterNO.isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("MeterNO. is empty! please read meter first!"));
		return;
	}
	bool ok;
	float bigErr = ui.lnEditBigNewError->text().toFloat(&ok);
	if (!ok || ui.lnEditBigNewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float mid2Err = ui.lnEditMid2NewError->text().toFloat(&ok);
	if (!ok || ui.lnEditMid2NewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float mid1Err = ui.lnEditMid1NewError->text().toFloat(&ok);
	if (!ok || ui.lnEditMid1NewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}
	float smallErr = ui.lnEditSmallNewError->text().toFloat(&ok);
	if (!ok || ui.lnEditSmallNewError->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("err is error! please input err first!"));
		return;
	}

	m_oldCoe->bigCoe = ui.lnEditBigOldCoe->text().toFloat(&ok);
	if (!ok || ui.lnEditBigOldCoe->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("oldCoe is empty! please read meter first!"));
		return;
	}
	m_oldCoe->mid2Coe = ui.lnEditMid2OldCoe->text().toFloat(&ok);
	if (!ok || ui.lnEditMid2OldCoe->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("oldCoe is empty! please read meter first!"));
		return;
	}
	m_oldCoe->mid1Coe = ui.lnEditMid1OldCoe->text().toFloat(&ok);
	if (!ok || ui.lnEditMid1OldCoe->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("oldCoe is empty! please read meter first!"));
		return;
	}
	m_oldCoe->smallCoe = ui.lnEditSmallOldCoe->text().toFloat(&ok);
	if (!ok || ui.lnEditSmallOldCoe->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Warning"), tr("oldCoe is empty! please read meter first!"));
		return;
	}
	m_meterObj->askModifyFlowCoe(meterNO, bigErr, mid2Err, mid1Err, smallErr, m_oldCoe);
}

//设置口径
void DataTestDlg50::on_btnSetStandard_clicked()
{
	qDebug()<<"设置口径 ...";
	UINT8 std = ui.cmbStandard->currentIndex() + 1;
	m_meterObj->askSetStandard(std); //设置口径
}

//设置系统时间
void DataTestDlg50::on_btnSetTime_clicked()
{
	qDebug()<<"设置系统时间 ...";
	m_meterObj->askSetSystemTime(); //设置系统时间
}

//设置一级地址
void DataTestDlg50::on_btnSetAddr1_clicked()
{
	qDebug()<<"设置一级地址 ...";
	QString curAddr1 = ui.lnEditCurAddr1->text();
	QString newAddr1 = ui.lnEditAddress1->text();
	bool ok1;
	bool ok2;
	UINT8 curAddr = curAddr1.toUInt(&ok1, 10);
	UINT8 newAddr = newAddr1.toUInt(&ok2, 10);

	if (curAddr1.isEmpty() || !ok1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("curAddr1 is error! please input 0-255!"));
		ui.lnEditCurAddr1->setFocus();
		return;
	}
	if (newAddr1.isEmpty() || !ok2)
	{
		QMessageBox::warning(this, tr("Warning"), tr("newAddr1 is error! please input 0-255!"));
		ui.lnEditAddress1->setFocus();
		return;
	}

	m_meterObj->askSetAddress1(curAddr1, newAddr1); //设置一级地址
}

//设置二级地址
void DataTestDlg50::on_btnSetAddr2_clicked()
{
	qDebug()<<"设置二级地址 ...";
	QString curAddr1 = ui.lnEditCurAddr1->text();
	QString newAddr2 = ui.lnEditAddress2->text();

	bool ok1;
	UINT8 curAddr = curAddr1.toUInt(&ok1, 10);
	if (curAddr1.isEmpty() || !ok1)
	{
		QMessageBox::warning(this, tr("Warning"), tr("curAddr1 is error! please input 0-255!"));
		ui.lnEditCurAddr1->setFocus();
		return;
	}

	m_meterObj->askSetAddress2(curAddr1, newAddr2); //设置二级地址
}

//响应读取表号成功
void DataTestDlg50::slotFreshMeterNo(const QString& comName, const QString& meterNo)
{
	ui.lnEditMeterNo->setText(meterNo);
	qDebug()<<"读取表号 成功...";
}

//响应读取表流量成功
void DataTestDlg50::slotFreshMeterFlow(const QString& comName, const QString& flow)
{
	ui.lnEditMeterTotalFlow->setText(flow);
	qDebug()<<"读取表流量 成功...";
}

//响应读取表热量成功
void DataTestDlg50::slotFreshMeterHeat(const QString& comName, const QString& heat)
{
	ui.lnEditMeterHeat->setText(heat);
	qDebug()<<"读取表热量 成功...";
}

//响应读取表日期成功
void DataTestDlg50::slotFreshMeterDate(const QString& comName, const QString& date)
{
	ui.dateEditMeter->setDate(QDate::fromString(date, "yyyyMMdd"));
	qDebug()<<"读取表日期 成功...";
}

//响应读取表进出水温度成功
void DataTestDlg50::slotFreshMeterTemper(const QString& comName, const QString& tempIn, const QString& tempOut)
{
	ui.lnEditMeterTempIn->setText(tempIn);
	ui.lnEditMeterTempOut->setText(tempOut);
	qDebug()<<"读取表进出水温度 成功...";
}

void DataTestDlg50::slotFreshBigCoe(const QString& comName, const QString& bigCoe)
{
	ui.lnEditBigOrgCoe->setText(bigCoe);
	float bigCoeV = calcFloatValueOfCoe(bigCoe);
	ui.lnEditBigOldCoe->setText(QString::number(bigCoeV, 'f', COE_PRECISION));
	float bigErr = calcErrorValueOfCoe(bigCoe);
	ui.lnEditBigOldError->setText(QString::number(bigErr, 'f', COE_PRECISION));
	qDebug()<<"读取大流量系数 成功...";
}

void DataTestDlg50::slotFreshMid2Coe(const QString& comName, const QString& mid2Coe)
{
	ui.lnEditMid2OrgCoe->setText(mid2Coe);
	float mid2CoeV = calcFloatValueOfCoe(mid2Coe);
	ui.lnEditMid2OldCoe->setText(QString::number(mid2CoeV, 'f', COE_PRECISION));
	float mid2Err = calcErrorValueOfCoe(mid2Coe);
	ui.lnEditMid2OldError->setText(QString::number(mid2Err, 'f', COE_PRECISION));
	qDebug()<<"读取中流2流量系数 成功...";
}

void DataTestDlg50::slotFreshMid1Coe(const QString& comName, const QString& mid1Coe)
{
	ui.lnEditMid1OrgCoe->setText(mid1Coe);
	float mid1CoeV = calcFloatValueOfCoe(mid1Coe);
	ui.lnEditMid1OldCoe->setText(QString::number(mid1CoeV, 'f', COE_PRECISION));
	float mid1Err = calcErrorValueOfCoe(mid1Coe);
	ui.lnEditMid1OldError->setText(QString::number(mid1Err, 'f', COE_PRECISION));
	qDebug()<<"读取中流1流量系数 成功...";
}

void DataTestDlg50::slotFreshSmallCoe(const QString& comName, const QString& smallCoe)
{
	ui.lnEditSmallOrgCoe->setText(smallCoe);
	float smallCoeV = calcFloatValueOfCoe(smallCoe);
	ui.lnEditSmallOldCoe->setText(QString::number(smallCoeV, 'f', COE_PRECISION));
	float smallErr = calcErrorValueOfCoe(smallCoe);
	ui.lnEditSmallOldError->setText(QString::number(smallErr, 'f', COE_PRECISION));
	qDebug()<<"读取小流量系数 成功...";
}

void DataTestDlg50::setMeterComboxEnabled(bool flag)
{
	ui.portNameComboBox->setEnabled(flag);
	ui.baudRateComboBox->setEnabled(flag);
	ui.dataBitsComboBox->setEnabled(flag);
	ui.parityComboBox->setEnabled(flag);
	ui.stopBitsComboBox->setEnabled(flag);
}

void DataTestDlg50::setMeterOperBtnEnabled(bool flag)
{
	ui.btnSetVerifyStatus->setEnabled(flag);
	ui.btnExitVerifyStatus->setEnabled(flag);
	ui.btnReadMeterData->setEnabled(flag);
	ui.btnModifyMeterNo->setEnabled(flag);
	ui.btnModifyFlowCoe->setEnabled(flag);
	ui.btn2ModifyFlowCoe->setEnabled(flag);
	ui.btnSetStandard->setEnabled(flag);
	ui.cmbStandard->setEnabled(flag);
	ui.btnSetAddr1->setEnabled(flag);
	ui.btnSetAddr2->setEnabled(flag);
	ui.btnSetTime->setEnabled(flag);
}

