/***********************************************
**  �ļ���:     totalweight50.cpp
**  ����:       �����춨(������)������-DN50��˫��ƽ��˫���ư�
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2015/12/1
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2015/12 ��һ��
**  ���ݰ���:
**  ˵��:		
**  ���¼�¼:   2015-7-13 ��ֵ�����㣺��Ҫ�����ֵ�����жϳ�ֵ�Ƿ�ȫ����ȡ�ɹ�;
                          �ڱ����Ӧ��ÿ����һ����ֵ��������startVerifyFlowPoint���ڴ˺������жϳ�ֵ�Ƿ�ȫ����ȡ�ɹ���
						  �ڱ����Ӧ��ÿ����һ����ֵ��������calcVerifyResult���ڴ˺������ж���ֵ�Ƿ�ȫ����ȡ�ɹ���
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

TotalWeightDlg50::TotalWeightDlg50(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"TotalWeightDlg50 thread:"<<QThread::currentThreadId();
	ui.setupUi(this);
	
	//��ͬ�ȼ����������Ӧ�ı�׼���,��λ%
	m_gradeErrA[1] = 2.00f;
	m_gradeErrA[2] = 3.00f;
	m_gradeErrA[3] = 4.00f;

	m_gradeErrB[1] = 0.01f;
	m_gradeErrB[2] = 0.02f;
	m_gradeErrB[3] = 0.05f;

	m_mapNormalFlow[0] = 1.5f; //DN15�������� 1.5
	m_mapNormalFlow[1] = 2.5f; //DN20�������� 2.5
	m_mapNormalFlow[2] = 3.5f; //DN25�������� 3.5

	///////////////////////////////// ԭshowEvent()���������� begin 
	//����ÿ����С������ʾʱ�������showEvent����������������յ�����
	ui.btnReCalc->hide();
	ui.btnExhaust->hide();
	ui.btnGoOn->hide();

	if (!getPortSetIni(&m_portsetinfo)) //��ȡ��λ���˿ں�������Ϣ
	{
		QMessageBox::warning(this, tr("Warning"), tr("Warning:get port set info failed!"));
	}

	m_readComConfig = new ReadComConfig(); //���������ýӿڣ�������initBalanceComǰ���ã�
	m_readComConfig->getBalancePara(m_balMaxWht, m_balBottomWht); //��ȡ����ƽ��������ͻ�ˮ����
	m_readComConfig->getBalancePara2(m_balMaxWht2, m_balBottomWht2); //��ȡС��ƽ��������ͻ�ˮ����

	m_balanceObj = NULL;
	initBalanceCom();		//��ʼ����ƽ����

	m_balanceObj2 = NULL;
	initBalanceCom2();		//��ʼ����ƽ����2

	m_tempObj = NULL;
	m_tempTimer = NULL;
	initTemperatureCom();	//��ʼ���¶Ȳɼ�����

	m_stdTempObj = NULL;
	m_stdTempTimer = NULL;
	initStdTemperatureCom(); //��ʼ����׼�¶Ȳɼ�����

	m_controlObj = NULL;
	initControlCom();		//��ʼ�����ƴ���

	m_controlObj2 = NULL;
	initControlCom2();		//��ʼ�����ƴ���2

	m_meterObj = NULL;      //������ͨѶ
	m_recPtr = NULL;        //���������

	btnGroupEnergyUnit = new QButtonGroup(ui.groupBoxEnergyUnit); //������λ
	btnGroupEnergyUnit->addButton(ui.radioButtonMJ, UNIT_MJ);
	btnGroupEnergyUnit->addButton(ui.radioButtonKwh, UNIT_KWH);
	connect(btnGroupEnergyUnit, SIGNAL(buttonClicked(int)), this, SLOT(slot_btnGroupEnergyUnit_clicked(int)));
	ui.radioButtonKwh->setChecked(true); //Ĭ�ϵ�λ:kWh
	m_unit = UNIT_KWH;

	btnGroupInstallPos = new QButtonGroup(ui.groupBoxInstallPos); //��װλ��
	btnGroupInstallPos->addButton(ui.radioButtonPosIn, INSTALLPOS_IN);
	btnGroupInstallPos->addButton(ui.radioButtonPosOut, INSTALLPOS_OUT);
	connect(btnGroupInstallPos, SIGNAL(buttonClicked(int)), this, SLOT(slot_btnGroupInstallPos_clicked(int)));
	ui.radioButtonPosIn->setChecked(true); //Ĭ����ڰ�װ
	m_installPos = INSTALLPOS_IN;

	m_minDeltaT = 3.0; //��С�²�
	ui.lnEditMinDeltaT->setText(QString::number(m_minDeltaT));

	//����������
	m_totalcount = 0;
	m_startWeight = 0.0;
	m_endWeight = 0.0;
	memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	m_flowRateTimer = new QTimer();
	connect(m_flowRateTimer, SIGNAL(timeout()), this, SLOT(slotFreshFlowRate()));
	m_flowRateTimer->start(TIMEOUT_FLOW_SAMPLE);

	//������ӿ�
	m_chkAlg = new CAlgorithm();

	//ӳ���ϵ����ʼ������״̬	
	initValveStatus();      

	m_exaustTimer = new QTimer(this); //������ʱ��
	connect(m_exaustTimer, SIGNAL(timeout()), this, SLOT(slotExaustFinished()));

	m_stopFlag = true; //ֹͣ����־���˳�����󣬲��ټ����ƽ������

	m_avgTFCount = 1; //����ƽ���¶��õ��ۼӼ�����
	m_nowOrder = 0;  //��ǰ���еļ춨���

	m_nowParams = NULL;
	m_continueVerify = true; //�����춨
	m_resetZero = false;     //��ֵ����
	m_autopick = false;      //�Զ��ɼ�
	m_flowPointNum = 0;      //���������
	m_maxMeterNum = 0;       //ĳ�������֧�ֵļ�����
	m_oldMaxMeterNum = 0;
	m_validMeterNum = 0;     //ʵ�ʼ�����
	m_exaustSecond = 45;     //Ĭ������ʱ��45��
	m_pickcode = PROTOCOL_VER_DELU; //�ɼ����� Ĭ�ϵ�³
	m_totalSC = 1.0;			 //������ȫϵ����Ĭ��1.0
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
	m_meterStdMapper->addMapping(ui.lnEditStandard, 1); //ӳ���"T_Meter_Standard"�ĵڶ����ֶ�

	m_paraSetDlg = NULL;    //�������öԻ���
	m_paraSetReader = new ParaSetReader(); //���������ýӿ�
	if (!readNowParaConfig()) //��ȡ��ǰ�춨����
	{
		qWarning()<<"��ȡ���������ļ�ʧ��!";
	}

	if (!isComAndPortNormal())
	{
		qWarning()<<"���ڡ��˿����ô���!";
	}

	///////////////////////////////// ԭshowEvent()���������� end

	ui.lcdInTemper->display(50);
	ui.lcdOutTemper->display(40);
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
		m_stopFlag = true;
		closeAllValveAndPumpOpenOutValve();
	 	wait(CYCLE_TIME);
	}
	openValve(m_portsetinfo.bigWaterOutNo);
	ui.labelHintPoint->clear();
	ui.labelHintProcess->setText(tr("release pipe pressure..."));
	openValve(m_portsetinfo.bigNo); //�򿪴������㷧�ţ��ͷŹ�·ѹ��
 	wait(RELEASE_PRESS_TIME); //�ȴ�2�룬�ͷŹ�·ѹ��
	closeValve(m_portsetinfo.bigNo);
	ui.labelHintProcess->clear();
	ui.btnStart->setEnabled(true);

	if (m_paraSetReader) //���춨����
	{
		delete m_paraSetReader;
		m_paraSetReader = NULL;
	}

	if (m_paraSetDlg)    //�������öԻ���
	{
		delete m_paraSetDlg;
		m_paraSetDlg = NULL;
	}

	if (m_readComConfig) //����������
	{
		delete m_readComConfig;
		m_readComConfig = NULL;
	}

	if (m_tempObj)  // �¶Ȳɼ�
	{
		delete m_tempObj;
		m_tempObj = NULL;

		m_tempThread.exit(); //������־�л��о���"QThread: Destroyed while thread is still running"
	}

	if (m_tempTimer) //�ɼ��¶ȼ�ʱ��
	{
		if (m_tempTimer->isActive())
		{
			m_tempTimer->stop();
		}
		delete m_tempTimer;
		m_tempTimer = NULL;
	}

	if (m_stdTempTimer) //��׼�¶Ȳɼ���ʱ��, �������ڴ��ڶ���ͣ��
	{
		if (m_stdTempTimer->isActive())
		{
			m_stdTempTimer->stop();
		}
		delete m_stdTempTimer;
		m_stdTempTimer = NULL;
	}

	if (m_stdTempObj)  // ��׼�¶Ȳɼ�
	{
		delete m_stdTempObj;
		m_stdTempObj = NULL;

		m_stdTempThread.exit();
	}

	if (m_balanceObj)  //��ƽ�ɼ�
	{
		delete m_balanceObj;
		m_balanceObj = NULL;

		m_balanceThread.exit();
	}

	if (m_balanceObj2)  //��ƽ�ɼ�2
	{
		delete m_balanceObj2;
		m_balanceObj2 = NULL;

		m_balanceThread2.exit();
	}

	if (m_controlObj)  //���ſ���
	{
		delete m_controlObj;
		m_controlObj = NULL;

		m_valveThread.exit();
	}

	if (m_controlObj2)  //���ſ���2
	{
		delete m_controlObj2;
		m_controlObj2 = NULL;

		m_valveThread2.exit();
	}

	if (m_chkAlg)//������
	{
		delete m_chkAlg;
		m_chkAlg = NULL;
	}

	//������ͨѶ
	if (m_meterObj)
	{
		delete []m_meterObj;
		m_meterObj = NULL;

		for (int i=0; i<m_oldMaxMeterNum; i++)
		{
			m_meterThread[i].exit();
		}
	}

	if (m_exaustTimer) //������ʱ��
	{
		if (m_exaustTimer->isActive())
		{
			m_exaustTimer->stop();
		}
		delete m_exaustTimer;
		m_exaustTimer = NULL;
	}

	if (m_flowRateTimer) //�������ټ�ʱ��
	{
		if (m_flowRateTimer->isActive())
		{
			m_flowRateTimer->stop();
		}
		delete m_flowRateTimer;
		m_flowRateTimer = NULL;
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
	int hSize = (int)((tw-vw-10)/COLUMN_TOTAL_COUNT);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(vSize);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(hSize);
}

//��ƽ�ɼ����� ��λ��ֱ�Ӳɼ�
void TotalWeightDlg50::initBalanceCom()
{
	ComInfoStruct balanceStruct = m_readComConfig->ReadBalanceConfig();
	m_balanceObj = new BalanceComObject();
	int type = m_readComConfig->getBalanceType();
	m_balanceObj->setBalanceType(type);
	m_balanceObj->moveToThread(&m_balanceThread);
	m_balanceThread.start();
	m_balanceObj->openBalanceCom(&balanceStruct);

	//��ƽ��ֵ����λ��ֱ��ͨ����ƽ���ڲɼ�
	connect(m_balanceObj, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshBigBalanceValue(const float &)));
}

//��ƽ�ɼ�����2 ��λ��ֱ�Ӳɼ�
void TotalWeightDlg50::initBalanceCom2()
{
	ComInfoStruct balanceStruct2 = m_readComConfig->ReadBalanceConfig2();
	m_balanceObj2 = new BalanceComObject();
	int type = m_readComConfig->getBalanceType2();
	m_balanceObj2->setBalanceType(type);
	m_balanceObj2->moveToThread(&m_balanceThread2);
	m_balanceThread2.start();
	m_balanceObj2->openBalanceCom(&balanceStruct2);

	//��ƽ��ֵ����λ��ֱ��ͨ����ƽ���ڲɼ�
	connect(m_balanceObj2, SIGNAL(balanceValueIsReady(const float &)), this, SLOT(slotFreshSmallBalanceValue(const float &)));
}

/*
** �¶Ȳɼ����� ��λ��ֱ�Ӳɼ�
** ��������
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
	
	m_tempTimer->start(TIMEOUT_PIPE_TEMPER); //���������¶�
}

void TotalWeightDlg50::slotAskPipeTemperature()
{
	m_tempObj->writeTemperatureComBuffer();
}

/*
** ����һ�����̣߳��򿪱�׼�¶Ȳɼ�����
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

//���ư�ͨѶ����
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

//���ư�ͨѶ����2
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

//������ͨѶ����
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
		m_meterObj[i].setProtocolVersion(m_pickcode); //���ñ�Э������
		m_meterThread[i].start();
		m_meterObj[i].openMeterCom(&m_readComConfig->ReadMeterConfigByNum(i+1));
		
		connect(&m_meterObj[i], SIGNAL(readMeterNoIsOK(const QString&, const QString&)), this, SLOT(slotSetMeterNumber(const QString&, const QString&)));
		connect(&m_meterObj[i], SIGNAL(readMeterHeatIsOK(const QString&, const QString&)), this, SLOT(slotSetMeterEnergy(const QString&, const QString&)));
	}
}

/*
** �˿ں�-����ӳ���ϵ����ʼ������״̬��Ĭ�Ϸ��ų�ʼ״̬ȫ��Ϊ�ر�,ˮ�ó�ʼ״̬Ϊ�رգ�
** ��Ҫ�Ľ��ø������
*/
void TotalWeightDlg50::initValveStatus()
{
	m_nowPortNo = 0;

	//�˿ں�-���Ű�ť ӳ���ϵ
	m_valveBtn[m_portsetinfo.bigNo] = ui.btnValveBig;
	m_valveBtn[m_portsetinfo.smallNo] = ui.btnValveSmall;
	m_valveBtn[m_portsetinfo.middle1No] = ui.btnValveMiddle1;
	m_valveBtn[m_portsetinfo.middle2No] = ui.btnValveMiddle2;
	m_valveBtn[m_portsetinfo.waterInNo] = ui.btnWaterIn;
	m_valveBtn[m_portsetinfo.bigWaterInNo] = ui.btnBigWaterIn;
	m_valveBtn[m_portsetinfo.bigWaterOutNo] = ui.btnBigWaterOut;
	m_valveBtn[m_portsetinfo.smallWaterInNo] = ui.btnSmallWaterIn;
	m_valveBtn[m_portsetinfo.smallWaterOutNo] = ui.btnSmallWaterOut;
	m_valveBtn[m_portsetinfo.pumpNo] = ui.btnWaterPump; //ˮ��

	//��ʼ������ˮ��Ϊ�򿪣���������Ϊ�ر�״̬
	m_valveStatus[m_portsetinfo.bigNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle1No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.middle2No] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.waterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.bigWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.smallWaterInNo] = VALVE_CLOSE;
	m_valveStatus[m_portsetinfo.smallWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.pumpNo] = VALVE_CLOSE; //ˮ��

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

/*
** �������ܣ�
	�ڽ���ˢ�´���ƽ��ֵ
	��ֹ��ƽ���
*/
void TotalWeightDlg50::slotFreshBigBalanceValue(const float& balValue)
{
	QString wht = QString::number(balValue, 'f', 3);
	ui.lcdBigBalance->setText(wht);
	if (balValue > m_balMaxWht) //��ֹ��ƽ���
	{
		closeWaterPump(); //�ر�ˮ��
		closeValve(m_portsetinfo.waterInNo); //�رս�ˮ��
		openValve(m_portsetinfo.bigWaterOutNo); //�򿪴���ƽ��ˮ��
		openValve(m_portsetinfo.bigWaterInNo);  //�򿪴���ƽ��ˮ��
	}
}

/*
** �������ܣ�
	�ڽ���ˢ��С��ƽ��ֵ
	��ֹ��ƽ���
*/
void TotalWeightDlg50::slotFreshSmallBalanceValue(const float& balValue)
{
	QString wht = QString::number(balValue, 'f', 3);
	ui.lcdSmallBalance->setText(wht);
	if (balValue > m_balMaxWht2) //��ֹ��ƽ���
	{
		closeWaterPump(); //�ر�ˮ��
		closeValve(m_portsetinfo.waterInNo); //�رս�ˮ��
		openValve(m_portsetinfo.smallWaterOutNo); //��С��ƽ��ˮ��
		openValve(m_portsetinfo.smallWaterInNo);  //��С��ƽ��ˮ��
	}
}
//�ڽ���ˢ������¶Ⱥͳ����¶�ֵ
void TotalWeightDlg50::slotFreshComTempValue(const QString& tempStr)
{
	ui.lcdInTemper->display(tempStr.left(TEMPER_DATA_WIDTH));   //����¶� PV
	ui.lcdOutTemper->display(tempStr.right(TEMPER_DATA_WIDTH)); //�����¶� SV
}

//ˢ�±�׼�¶�
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

//�ɼ���׼�¶�
void TotalWeightDlg50::on_btnStdTempCollect_clicked()
{
	ui.lnEditInStdResist->clear();
	ui.lnEditOutStdResist->clear();
	ui.lnEditInStdTemp->clear();
	ui.lnEditOutStdTemp->clear();
	m_stdTempTimer->start(TIMEOUT_STD_TEMPER);
}

//ֹͣ�ɼ���׼�¶�
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
** ��������
*/
void TotalWeightDlg50::slotFreshFlowRate()
{
// 	qDebug()<<"TotalWeightDlg::slotFreshFlowRate thread:"<<QThread::currentThreadId();
	if (m_totalcount > 4294967290) //��ֹm_totalcount��� 32λ�޷���������Χ0~4294967295
	{
		m_totalcount = 0;
		m_startWeight = 0.0;
		m_endWeight = 0.0;
		memset(m_deltaWeight, 0, sizeof(float)*FLOW_SAMPLE_NUM);
	}
	if (m_totalcount == 0) //��¼��ƽ��ʼ����
	{
		m_startWeight = ui.lcdBigBalance->text().toFloat() + ui.lcdSmallBalance->text().toFloat();
		m_totalcount ++;
		return;
	}

	float flowValue = 0.0;
	float totalWeight = 0.0;
	m_endWeight = ui.lcdBigBalance->text().toFloat() + ui.lcdSmallBalance->text().toFloat();//ȡ��ǰ��ƽֵ, ��Ϊ��ǰ�������ֵ
	float delta_weight = m_endWeight - m_startWeight;
	m_deltaWeight[m_totalcount%FLOW_SAMPLE_NUM] = delta_weight;
// 	qWarning()<<"m_totalcount ="<<m_totalcount;
	for (int i=0; i<FLOW_SAMPLE_NUM; i++)
	{
		totalWeight += m_deltaWeight[i];
// 		qWarning()<<"totalWeight ="<<totalWeight<<", m_deltaWeight["<<i<<"] ="<<m_deltaWeight[i];
	}
	flowValue = 3.6*(totalWeight)*1000/(FLOW_SAMPLE_NUM*TIMEOUT_FLOW_SAMPLE);//���ۻ�ˮ��/��ʱ��  (��/Сʱ, t/h, m3/h)
//	flowValue = (totalWeight)*1000/(FLOW_SAMPLE_NUM*TIMEOUT_FLOW_SAMPLE);// kg/s
// 	qDebug()<<"flowValue ="<<flowValue;
	if (m_totalcount >= FLOW_SAMPLE_NUM)
	{
		ui.lcdFlowRate->display(QString::number(flowValue, 'f', 3)); //��ui.lcdFlowRate����ʾ����
	}
	m_totalcount ++;//�������ۼ�
	m_startWeight = m_endWeight;//����ǰֵ����, ��Ϊ�´�����ĳ�ֵ
}

//��⴮�ڡ��˿������Ƿ���ȷ
int TotalWeightDlg50::isComAndPortNormal()
{
	return true;
}

//��ȡ��ǰ�춨����;��ʼ�����ؼ�����ʾ�ؼ���������ʼ��������ͨѶ����
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
	m_continueVerify = m_nowParams->bo_converify; //�����춨
	m_resetZero = m_nowParams->bo_resetzero; //��ֵ����
	m_autopick = m_nowParams->bo_autopick;   //�Զ��ɼ�
	m_flowPointNum = m_nowParams->total_fp;  //��Ч������ĸ��� 
	m_exaustSecond = m_nowParams->ex_time;   //����ʱ��
	m_standard = m_nowParams->m_stand;       //����
	m_model = m_nowParams->m_model;          //���ͺ�
	m_maxMeterNum = m_nowParams->m_maxMeters;//��ͬ�����Ӧ�����������
	m_pickcode = m_nowParams->m_pickcode; //�ɼ�����
	m_numPrefix = getNumPrefixOfManufac(m_pickcode); //���ǰ׺
	m_totalSC = m_nowParams->sc_thermal;  //������ȫϵ��

	initTableWidget();
	showNowKeyParaConfig();
	initMeterCom();

	return true;
}

//��ʼ�����ؼ�
void TotalWeightDlg50::initTableWidget()
{
	if (m_maxMeterNum <= 0)
	{
		return;
	}
	ui.tableWidget->setRowCount(m_maxMeterNum); //���ñ������

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

		//��Ϊֻ��
		ui.tableWidget->item(i, COLUMN_FLOW_POINT)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_BAL_START)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_BAL_END)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_TEMPER)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_DENSITY)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_STD_VALUE)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_DISP_ERROR)->setFlags(Qt::NoItemFlags);
		ui.tableWidget->item(i, COLUMN_STD_ERROR)->setFlags(Qt::NoItemFlags);

		//���ð�ť
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
	ui.tableWidget->setFont(QFont("Times", 15, QFont::DemiBold, true));
// 	ui.tableWidget->resizeColumnsToContents();
// 	ui.tableWidget->setColumnWidth(COLUMN_METER_NUMBER, 125);
}

//��ʾ��ǰ�ؼ�����������Ϣ
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

//������ݲɼ��Ƿ�������������ƽ���¶ȡ����������
int TotalWeightDlg50::isDataCollectNormal()
{
	return true;
}

/*
** ��ʼ��������ʱ
*/
int TotalWeightDlg50::startExhaustCountDown()
{
	if (!isDataCollectNormal())
	{
		qWarning()<<"���ݲɼ���������";
		QMessageBox::warning(this, tr("Warning"), tr("data acquisition error, please check!"));
		return false;
	}
	m_controlObj->askSetDriverFreq(m_nowParams->fp_info[0].fp_freq);
	if (!openAllValveAndPump())
	{
		qWarning()<<"�����з��ź�ˮ�� ʧ��!";
		QMessageBox::warning(this, tr("Warning"), tr("exhaust air failed!"));
		return false;
	}
	m_stopFlag = false;
	m_exaustSecond = m_nowParams->ex_time;
	m_exaustTimer->start(CYCLE_TIME);//��ʼ��������ʱ
	ui.labelHintProcess->setText(tr("Exhaust countdown: <font color=DarkGreen size=6><b>%1</b></font> second").arg(m_exaustSecond));
	ui.labelHintPoint->clear();
	qDebug()<<"��������ʱ:"<<m_exaustSecond<<"��";

	return true;
}

/*
** ������ʱ����Ӧ����
*/
void TotalWeightDlg50::slotExaustFinished()
{
	if (m_stopFlag)
	{
		return;
	}

	m_exaustSecond --;
	ui.labelHintProcess->setText(tr("Exhaust countdown: <font color=DarkGreen size=6><b>%1</b></font> second").arg(m_exaustSecond));
	qDebug()<<"��������ʱ:"<<m_exaustSecond<<"��";
	if (m_exaustSecond > 1)
	{
		return;
	}

	m_exaustTimer->stop(); //ֹͣ������ʱ
	ui.labelHintProcess->setText(tr("Exhaust countdown finished!"));
	ui.labelHintProcess->clear();
	if (!closeAllFlowPointValves()) //�ر����������㷧�� ʧ��
	{
		if (!closeAllFlowPointValves()) //�ٳ��Թر�һ��
		{
			qWarning()<<"�ر����������㷧��ʧ�ܣ��춨����";
			return;
		}
	}

	if (!prepareBigBalanceInitWeight())//׼������ƽ��ʼ����
	{
		return;
	}

	if (!prepareSmallBalanceInitWeight())//׼��С��ƽ��ʼ����
	{
		return;
	}

	wait(BALANCE_STABLE_TIME); //�ȴ���ƽ��ֵ�ȶ�

	if (setAllMeterVerifyStatus()) //���ü춨״̬�ɹ�
	{
		startVerify();
	}
}

/*
** ���������󡢿�ʼ�춨ǰ��׼����ƽ,�ﵽҪ��ĳ�ʼ����
*/
int TotalWeightDlg50::prepareBigBalanceInitWeight()
{
	ui.labelHintPoint->setText(tr("prepare big balance init weight ...")); //׼������ƽ��ʼ����(��ˮ����)
	int ret = 0;
	//�жϴ���ƽ����,���С��Ҫ��Ļ�ˮ����(5kg)����رմ���ƽ��ˮ�����򿪴�������
	if (ui.lcdBigBalance->text().toFloat() < m_balBottomWht)
	{
		if (!closeValve(m_portsetinfo.bigWaterOutNo)) 
		{
			qWarning()<<"�رմ���ƽ��ˮ��ʧ��";
		}
		if (!openValve(m_portsetinfo.bigNo))
		{
			qWarning()<<"�򿪴�������ʧ��";
		}
		//�жϲ��ȴ���ƽ���������ڻ�ˮ����(5kg)
		if (isBigBalanceValueBigger(m_balBottomWht, true))
		{
			if (!closeValve(m_portsetinfo.bigNo))
			{
				qWarning()<<"�رմ�������ʧ��";
			}
			ret = 1;
		}
	}
	else //���ڻ�ˮ����
	{
		closeValve(m_portsetinfo.bigWaterOutNo); //�رմ���ƽ��ˮ��
		ret = 1;
	}

	return ret;
}

/*
** ���������󡢿�ʼ�춨ǰ��׼��С��ƽ,�ﵽҪ��ĳ�ʼ����
*/
int TotalWeightDlg50::prepareSmallBalanceInitWeight()
{
	ui.labelHintPoint->setText(tr("prepare small balance init weight ...")); //׼��С��ƽ��ʼ����(��ˮ����)
	int ret = 0;
	//�ж�С��ƽ����,���С��Ҫ��Ļ�ˮ����(5kg)����ر�С��ƽ��ˮ�������ж�������
	if (ui.lcdSmallBalance->text().toFloat() < m_balBottomWht2)
	{
		if (!closeValve(m_portsetinfo.smallWaterOutNo)) 
		{
			qWarning()<<"�ر�С��ƽ��ˮ��ʧ��";
		}
		if (!openValve(m_portsetinfo.middle2No))
		{
			qWarning()<<"���ж�������ʧ��";
		}
		//�жϲ��ȴ���ƽ���������ڻ�ˮ����(5kg)
		if (isSmallBalanceValueBigger(m_balBottomWht2, true))
		{
			if (!closeValve(m_portsetinfo.middle2No))
			{
				qWarning()<<"�ر��ж�������ʧ��";
			}
			ret = 1;
		}
	}
	else
	{
		closeValve(m_portsetinfo.smallWaterOutNo); //�ر�С��ƽ��ˮ��
		ret = 1;
	}

	return ret;
}

//�����������������춨״̬
int TotalWeightDlg50::setAllMeterVerifyStatus()
{
	ui.labelHintPoint->setText(tr("setting verify status ..."));
	on_btnAllVerifyStatus_clicked();
	wait(CYCLE_TIME);
	on_btnAllVerifyStatus_clicked();
	return true;
}

//�����з��ź�ˮ��
int TotalWeightDlg50::openAllValveAndPump()
{
	openValve(m_portsetinfo.bigWaterOutNo); //����ƽ��ˮ��
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.bigWaterInNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterInNo);
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.waterInNo);
	openWaterPump();//��ˮ��
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

//�ر����з��ź�ˮ��
int TotalWeightDlg50::closeAllValveAndPumpOpenOutValve()
{
	openValve(m_portsetinfo.bigWaterOutNo); //�򿪴���ƽ��ˮ��
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.bigWaterInNo);  //�򿪴���ƽ��ˮ��
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterOutNo);//��С��ƽ��ˮ��
	wait(CYCLE_TIME);
	openValve(m_portsetinfo.smallWaterInNo);//��С��ƽ��ˮ��
	wait(CYCLE_TIME);
	closeWaterPump();    //�˳�ʱ�ر�ˮ��
	closeAllFlowPointValves();//�ر����������㷧��
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.waterInNo);//�رս�ˮ��

	return true;
}

//�ر����������㷧��
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
** ��Ӧ�������ƽ�����ı仯
** ���������
	targetV: Ŀ������
	flg: true-Ҫ�����Ŀ������(Ĭ��)��false-Ҫ��С��Ŀ������
*/
int TotalWeightDlg50::isBigBalanceValueBigger(float targetV, bool flg)
{
	int ret = 0;
	if (flg) //Ҫ�����Ŀ������
	{
		while (!m_stopFlag && (ui.lcdBigBalance->text().toFloat() < targetV))
		{
			qDebug()<<"����ƽ���� ="<<ui.lcdBigBalance->text()<<", С��Ҫ������� "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdBigBalance->text().toFloat() >= targetV);
	}
	else //Ҫ��С��Ŀ������
	{
		while (!m_stopFlag && (ui.lcdBigBalance->text().toFloat() > targetV))
		{
			qDebug()<<"����ƽ���� ="<<ui.lcdBigBalance->text()<<", ����Ҫ������� "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdBigBalance->text().toFloat() <= targetV);
	}

	return ret;
}

/*
** ��Ӧ����С��ƽ�����ı仯
** ���������
	targetV: Ŀ������
	flg: true-Ҫ�����Ŀ������(Ĭ��)��false-Ҫ��С��Ŀ������
*/
int TotalWeightDlg50::isSmallBalanceValueBigger(float targetV, bool flg)
{
	int ret = 0;
	if (flg) //Ҫ�����Ŀ������
	{
		while (!m_stopFlag && (ui.lcdSmallBalance->text().toFloat() < targetV))
		{
			qDebug()<<"С��ƽ���� ="<<ui.lcdSmallBalance->text()<<", С��Ҫ������� "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdSmallBalance->text().toFloat() >= targetV);
	}
	else //Ҫ��С��Ŀ������
	{
		while (!m_stopFlag && (ui.lcdSmallBalance->text().toFloat() > targetV))
		{
			qDebug()<<"С��ƽ���� ="<<ui.lcdSmallBalance->text()<<", ����Ҫ������� "<<targetV;
			wait(CYCLE_TIME);
		}
		ret = !m_stopFlag && (ui.lcdSmallBalance->text().toFloat() <= targetV);
	}

	return ret;
}

/*
** ���ܣ��ж���ƽ�����Ƿ�ﵽҪ��ļ춨��������춨���̵�ƽ���¶Ⱥ�ƽ������(m3/h)
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
			qDebug()<<"����ƽ���� ="<<ui.lcdBigBalance->text()<<", С��Ҫ������� "<<targetV;
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
			qDebug()<<"С��ƽ���� ="<<ui.lcdSmallBalance->text()<<", С��Ҫ������� "<<targetV;
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

	m_pipeInTemper = m_pipeInTemper/m_avgTFCount;   //���ƽ���¶�
	m_pipeOutTemper = m_pipeOutTemper/m_avgTFCount; //����ƽ���¶�
	m_stdInTemper = m_stdInTemper/m_avgTFCount;     //��ڱ�׼�¶�ƽ��ֵ
	m_stdOutTemper = m_stdOutTemper/m_avgTFCount;   //���ڱ�׼�¶�ƽ��ֵ

// 	if (m_avgTFCount > FLOW_SAMPLE_NUM*2)
// 	{
// 		m_realFlow = m_realFlow/(m_avgTFCount-FLOW_SAMPLE_NUM*2+1); //ƽ������
// 	}
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

//��ձ�񣬵�һ�г���("���"��)
void TotalWeightDlg50::clearTableContents()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		for (int j=1; j<ui.tableWidget->columnCount(); j++) //�ӵڶ��п�ʼ
		{
			if (ui.tableWidget->item(i,j) == 0)
			{
				continue;
			}
			ui.tableWidget->item(i,j)->setText("");
		}
	}
// 	ui.tableWidget->clearContents(); //��ձ��
}

//���"��ʼ"��ť
void TotalWeightDlg50::on_btnStart_clicked()
{
	//�ж��Ƿ���������С�²�
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

	m_timeStamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"); //��¼ʱ���
	m_nowDate = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //��ǰ����'2014-08-07'
	m_validDate = QDateTime::currentDateTime().addYears(VALID_YEAR).addDays(-1).toString("yyyy-MM-dd"); //��Ч��

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
	
	if (m_autopick) //�Զ�����
	{
		on_btnAllReadNO_clicked();
	}
	else //�ֶ�����
	{
		ui.labelHintPoint->setText(tr("Please input meter number!"));
		ui.tableWidget->setCurrentCell(0, COLUMN_METER_NUMBER);
	}

	return;
}

/*
** ���"����"��ť
*/
void TotalWeightDlg50::on_btnExhaust_clicked()
{
}

//���"����"��ť �����Ż�ȡ�쳣
void TotalWeightDlg50::on_btnGoOn_clicked()
{
	ui.btnGoOn->hide();
	startVerify();
}

//���"���¼���"��ť
void TotalWeightDlg50::on_btnReCalc_clicked()
{
}

//���"��ֹ���"��ť
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
	this->close();
}

//ֹͣ�춨
void TotalWeightDlg50::stopVerify()
{
	ui.labelHintPoint->clear();
	if (!m_stopFlag)
	{
		ui.labelHintProcess->setText(tr("stopping verify...please wait a minute"));
		m_stopFlag = true; //���ټ����ƽ����
		m_exaustTimer->stop();//ֹͣ������ʱ��
		closeAllValveAndPumpOpenOutValve();
	}
	ui.labelHintProcess->setText(tr("Verify has Stoped!"));
	m_state = STATE_INIT; //���ó�ʼ״̬

	ui.tableWidget->setEnabled(true);
	ui.btnAllReadNO->setEnabled(true);
	ui.btnAllReadData->setEnabled(true);
	ui.btnAllVerifyStatus->setEnabled(true);
	ui.btnStart->setEnabled(true);
}

//��ʼ�춨
void TotalWeightDlg50::startVerify()
{
	//�ͷ��ڴ�
	if (m_meterErr != NULL)
	{
		for (int m=0; m<m_validMeterNum; m++)
		{
			delete []m_meterErr[m];
		}
		delete []m_meterErr;
		m_meterErr = NULL;
	}

	getValidMeterNum(); //��ȡʵ�ʼ��ĸ���(���ݻ�ȡ���ı�Ÿ���)
	if (m_validMeterNum <= 0)
	{
		ui.labelHintPoint->setText(tr("Please input meter number!\n then click \"GoOn\" button!"));
		QMessageBox::warning(this, tr("Error"), tr("Error: meter count is zero !\nPlease input meter number, then click \"GoOn\" button!"));//�������ţ�
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
		messageBox->setDefaultButton(QMessageBox::No); //Ĭ�ϼ����춨
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

	m_state = STATE_INIT; //��ʼ״̬

	//���ֵ
	if (m_meterStartValue != NULL)
	{
		delete []m_meterStartValue;
		m_meterStartValue = NULL;
	}
	m_meterStartValue = new float[m_validMeterNum];
	memset(m_meterStartValue, 0, sizeof(float)*m_validMeterNum);

	//����ֵ
	if (m_meterEndValue != NULL)
	{
		delete []m_meterEndValue;
		m_meterEndValue = NULL;
	}
	m_meterEndValue = new float[m_validMeterNum];   
	memset(m_meterEndValue, 0, sizeof(float)*m_validMeterNum);
	
	//���¶�
	if (m_meterTemper != NULL)
	{
		delete []m_meterTemper;
		m_meterTemper = NULL;
	}
	m_meterTemper = new float[m_validMeterNum];     
	memset(m_meterTemper, 0, sizeof(float)*m_validMeterNum);

	//���ܶ�	
	if (m_meterDensity != NULL)
	{
		delete []m_meterDensity;
		m_meterDensity = NULL;
	}
	m_meterDensity = new float[m_validMeterNum];    
	memset(m_meterDensity, 0, sizeof(float)*m_validMeterNum);

	//�����ı�׼ֵ
	if (m_meterStdValue != NULL)
	{
		delete []m_meterStdValue;
		m_meterStdValue = NULL;
	}
	m_meterStdValue = new float[m_validMeterNum];   
	memset(m_meterStdValue, 0, sizeof(float)*m_validMeterNum);

    //���������(��ͬ����ǰ������)	
	if (m_meterError != NULL)
	{
		delete []m_meterError;
		m_meterError = NULL;
	}
	m_meterError = new float[m_validMeterNum];      
	memset(m_meterError, 0, sizeof(float)*m_validMeterNum);

	//���������(��ͬ����ͬ������)
	m_meterErr = new float*[m_validMeterNum];    
	for (int i=0; i<m_validMeterNum; i++)
	{
		m_meterErr[i] = new float[VERIFY_POINTS];
		memset(m_meterErr[i], 0, sizeof(float)*VERIFY_POINTS);
	}

	//�����������Ƿ񶼺ϸ�
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
	if (m_continueVerify) //�����춨
	{
		if (!judgeBalanceCapacity(bigOK, smallOK)) //�ж���ƽ�����Ƿ��ܹ�����춨����
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

			while (!judgeBalanceCapacity(bigOK, smallOK)) //��С��ƽ����ͬʱ����춨����
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
			closeValve(m_portsetinfo.bigWaterOutNo); //������춨��������رշ�ˮ��
			closeValve(m_portsetinfo.smallWaterOutNo); //������춨��������رշ�ˮ��
			wait(BALANCE_STABLE_TIME); //�ȴ�3����(�ȴ�ˮ���ȶ�)
		}
	}

	m_nowOrder = 1;
	prepareVerifyFlowPoint(m_nowOrder); //��һ��������춨
}

//��ȡ��Ч������,������ӳ���ϵ��������±�-��λ�ţ�
int TotalWeightDlg50::getValidMeterNum()
{
	m_validMeterNum = 0; //������
	bool ok;
	for (int i=0; i<m_maxMeterNum; i++)
	{
		if (NULL == ui.tableWidget->item(i, COLUMN_METER_NUMBER)) //"���"��Ԫ��Ϊ��
		{
			continue;
		}
		ui.tableWidget->item(i, COLUMN_METER_NUMBER)->text().toInt(&ok, 10);
		if (!ok) //���ת��ʧ��(������)
		{
			continue;
		}

		m_meterPosMap[m_validMeterNum] = i+1; //��λ�Ŵ�1��ʼ
		m_validMeterNum++;
	}
	return m_validMeterNum;
}

/*
** �жϱ�λ���Ƿ���Ч(�ñ�λ�Ƿ���Ҫ���)
����:meterPos(��λ��)����1��ʼ
����:������±꣬��0��ʼ
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
** �ж���ƽ�����Ƿ�����춨������ͬʱ�жϴ�С��ƽ
** �����춨ʱ�������ܼ춨����
** ���������
		bigOK��1-���㣻0-�����㣨����ƽ��������춨������
		smallOK��1-���㣻0-�����㣨С��ƽ��������춨������
	����ֵ��
		true-���㣻false-������
*/
bool TotalWeightDlg50::judgeBalanceCapacity(int &bigOK, int &smallOK)
{
	bool ret = false;
	float bigQuantity = 0, smallQuantity = 0;
	int num = m_nowParams->total_fp; //��Ч������ĸ���
	for (int i=1; i<=num; i++)
	{
		if (m_paraSetReader->getFpBySeq(i).fp_valve_idx == 0) //�������㣬��Ӧ����ƽ
		{
			bigQuantity +=  m_paraSetReader->getFpBySeq(i).fp_quantity;
		}
		else //�������������㣬��ӦС��ƽ
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
** �ж���ƽ�����Ƿ�����춨����
** �������춨ʱ�����㵥�μ춨����
** ע�⣺order��1��ʼ
*/
int TotalWeightDlg50::judgeBalanceCapacitySingle(int order, int &bigBalance)
{
	if (order < 1 || order > VERIFY_POINTS)
	{
		return false;
	}
	bool ret = false;
	float quantity = m_nowParams->fp_info[order-1].fp_quantity;
	if (m_nowParams->fp_info[order-1].fp_valve_idx == 0) //�������㣬��Ӧ����ƽ
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
** ׼������������ļ춨�����б�Ҫ�ļ��
** ע�⣺order��1��ʼ
*/
int TotalWeightDlg50::prepareVerifyFlowPoint(int order)
{
	if (order < 1 || order > m_flowPointNum || m_stopFlag)
	{
		return false;
	}

	if (!m_continueVerify) //�������춨��ÿ�μ춨��ʼ֮ǰ��Ҫ�ж���ƽ����
	{
		int bigBalance = 0;
		if (!judgeBalanceCapacitySingle(order, bigBalance)) //��ƽ���������㱾�μ춨����
		{
			ui.labelHintPoint->setText(tr("prepare balance capacity ..."));
			if (bigBalance) //����ƽ
			{
				openValve(m_portsetinfo.bigWaterOutNo); //�򿪴���ƽ��ˮ��������ƽ��ˮ
			}
			else
			{
				openValve(m_portsetinfo.smallWaterOutNo); //��С��ƽ��ˮ����С��ƽ��ˮ
			}
			while (!judgeBalanceCapacitySingle(order, bigBalance)) //�ȴ���ƽ��ˮ��ֱ�����㱾�μ춨����
			{ 
				wait(CYCLE_TIME);
			}
			if (bigBalance)
			{
				closeValve(m_portsetinfo.bigWaterOutNo); //������춨��������رշ�ˮ��
			}
			else
			{
				closeValve(m_portsetinfo.smallWaterOutNo); //������춨��������رշ�ˮ��
			}
			wait(BALANCE_STABLE_TIME);   //�ȴ�3���ӣ��ȴ�ˮ���ȶ�
		}
	}

	int i=0;
	if (m_resetZero) //��ֵ����
	{
// 		if (m_autopick || order==1 ) //�Զ��ɼ������ǵ�һ���춨��,��Ҫ�ȴ��ȱ��ֵ����
// 		{
			ui.labelHintPoint->setText(tr("Reset Zero"));
			while (i < RESET_ZERO_TIME && !m_stopFlag) //�ȴ�������ֵ����
			{
				ui.labelHintProcess->setText(tr("please wait <font color=DarkGreen size=6><b>%1</b></font> seconds for reset zero").arg(RESET_ZERO_TIME-i));
				i++;
				wait(CYCLE_TIME); 
			}
// 		}
	}
/*	else //��ֵ������
	{
// 		if (order >= 2)
// 		{
			sleep(WATCH_DATA_TIME); //��3�룬��������Ա����һ�εļ춨���
// 		}
	}*/

// 	clearTableContents();
	getMeterStartValue(); //��ȡ���ֵ
	
	return true;
}

//���е���������ļ춨
int TotalWeightDlg50::startVerifyFlowPoint(int order)
{
	if (m_stopFlag)
	{
		return false;
	}

	//�жϳ�ֵ�Ƿ�ȫ����ȡ�ɹ�
	bool ok;
	int row = 0;
	for (int p=0; p<m_validMeterNum; p++)
	{
		row = m_meterPosMap[p] - 1;
		ui.tableWidget->item(row, COLUMN_METER_START)->text().toFloat(&ok);
		if (!ok || ui.tableWidget->item(row, COLUMN_METER_START)->text().isEmpty())
		{
// 			QMessageBox::information(this, tr("Hint"), tr("please input init value of heat meter"));//�������������ֵ��
			ui.labelHintProcess->setText(tr("please input start value of heat meter"));
			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START); //��λ����Ҫ�����ֵ�ĵط�
			return false;
		}
	}

	bool bigFlag = false;
	if (m_paraSetReader->getFpBySeq(order).fp_valve_idx == 0) //�������ִ�С��ƽ 0:����ƽ
	{
		bigFlag = true;
	}

	if (bigFlag)
	{
		m_balStartV = ui.lcdBigBalance->text().toFloat(); //��¼����ƽ��ֵ
	}
	else
	{
		m_balStartV = ui.lcdSmallBalance->text().toFloat(); //��¼С��ƽ��ֵ
	}
	m_pipeInTemper = ui.lcdInTemper->value();
	m_pipeOutTemper = ui.lcdOutTemper->value();
	m_realFlow = ui.lcdFlowRate->value();
	m_avgTFCount = 1;
	m_stdInTemper = ui.lnEditInStdTemp->text().toFloat();
	m_stdOutTemper = ui.lnEditOutStdTemp->text().toFloat();

	int portNo = m_paraSetReader->getFpBySeq(order).fp_valve;  //order��Ӧ�ķ��Ŷ˿ں�
	float verifyQuantity = m_paraSetReader->getFpBySeq(order).fp_quantity; //��order�μ춨��Ӧ�ļ춨��
	float frequence = m_paraSetReader->getFpBySeq(order).fp_freq; //order��Ӧ��Ƶ��
	m_controlObj->askSetDriverFreq(frequence);
	if (openValve(portNo)) //�򿪷��ţ���ʼ������
	{
		if (judgeBalanceAndCalcAvgTemperAndFlow(m_balStartV + verifyQuantity, bigFlag)) //����춨��������˹��̵�ƽ���¶Ⱥ�ƽ������
		{
			ui.tableWidget->setEnabled(true);
			ui.btnAllReadNO->setEnabled(true);
			ui.btnAllReadData->setEnabled(true);
			ui.btnAllVerifyStatus->setEnabled(true);
			closeValve(portNo); //�ر�order��Ӧ�ķ���
			wait(BALANCE_STABLE_TIME); //�ȴ�3���ӣ�����ƽ��ֵ�ȶ�
			if (bigFlag)
			{
				m_balEndV = ui.lcdBigBalance->text().toFloat(); //��¼����ƽ��ֵ
			}
			else
			{
				m_balEndV = ui.lcdSmallBalance->text().toFloat(); //��¼С��ƽ��ֵ
			}

			if (!m_resetZero && m_nowOrder>=2)
			{
				m_state = STATE_END_VALUE;
				makeStartValueByLastEndValue();
			}
			for (int m=0; m<m_validMeterNum; m++) //
			{
				m_meterTemper[m] = m_chkAlg->getMeterTempByPos(m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m]);//����ÿ���������¶�
				m_meterDensity[m] = m_chkAlg->getDensityByQuery(m_meterTemper[m]);//����ÿ���������ܶ�
				m_meterStdValue[m] = m_chkAlg->calcStdEnergyByEnthalpy(m_stdInTemper, m_stdOutTemper, m_balEndV-m_balStartV, m_unit); //����ÿ��������������׼ֵ

				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_FLOW_POINT)->setText(QString::number(m_realFlow, 'f', 3));//������
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_METER_END)->setText("");//����ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_START)->setText(QString::number(m_balStartV, 'f', 3));//��ƽ��ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_END)->setText(QString::number(m_balEndV, 'f', 3));    //��ƽ��ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_TEMPER)->setText(QString::number(m_meterTemper[m], 'f', 2));  //�¶�
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DENSITY)->setText(QString::number(m_meterDensity[m], 'f', 3));//�ܶ�
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_STD_VALUE)->setText(QString::number(m_meterStdValue[m], 'f', 3));//��׼ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setText("");//ʾֵ���
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setForeground(QBrush());//ʾֵ���
			}
			getMeterEndValue();
		} //����춨��
	}

	return true;
}

/*
** �������б��������
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
** ����ĳ�����������
** ���������
**     idx:��������������
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
	m_meterError[idx] = 100*(m_meterEndValue[idx] - m_meterStartValue[idx] - m_meterStdValue[idx])/m_meterStdValue[idx];//����ĳ��������
	int valveIdx = m_paraSetReader->getFpBySeq(m_nowOrder).fp_valve_idx; //0:�� 1:�ж� 2:��һ 3:С
	m_meterErr[idx][valveIdx] = m_meterError[idx];
	ui.tableWidget->item(row, COLUMN_DISP_ERROR)->setText(QString::number(m_meterError[idx], 'f', ERR_PRECISION)); //ʾֵ���
	float stdError = m_totalSC*(m_gradeErrA[m_nowParams->m_grade] + 4*m_minDeltaT/(m_stdInTemper-m_stdOutTemper) + m_gradeErrB[m_nowParams->m_grade]*m_mapNormalFlow[m_standard]/m_realFlow); //��׼���=���Ҫ�����*������ȫϵ��
	ui.tableWidget->item(row, COLUMN_STD_ERROR)->setText("��" + QString::number(stdError, 'f', ERR_PRECISION)); //��׼���
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
	m_recPtr[idx].methodFlag = WEIGHT_METHOD; //������
	m_recPtr[idx].meterValue0 = m_meterStartValue[idx];
	m_recPtr[idx].meterValue1 = m_meterEndValue[idx];
	m_recPtr[idx].balWeight0 = m_balStartV;
	m_recPtr[idx].balWeight1 = m_balEndV;
	m_recPtr[idx].pipeTemper = m_meterTemper[idx]; 
	m_recPtr[idx].density = m_meterDensity[idx];
	m_recPtr[idx].stdValue = m_meterStdValue[idx];
	m_recPtr[idx].dispError = m_meterError[idx];
	m_recPtr[idx].grade = m_nowParams->m_grade;
	m_recPtr[idx].stdError = stdError; //��ı�׼���
	m_recPtr[idx].result = (fabs(m_recPtr[idx].dispError) <= fabs(m_recPtr[idx].stdError)) ? 1 : 0;
	m_meterResult[idx] *= m_recPtr[idx].result;
	m_recPtr[idx].meterPosNo = m_meterPosMap[idx];
	m_recPtr[idx].standard = m_standard;
	m_recPtr[idx].model = m_model;
	m_recPtr[idx].pickcode = m_pickcode; //�ɼ�����
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

//������������ֵ�󣬼���춨���
int TotalWeightDlg50::calcVerifyResult()
{
	int ret = 0;
	ret = calcAllMeterError();

	if (ret) //�����������ɹ�����ֵ��
	{
		saveAllVerifyRecords();
		ui.labelHintProcess->setText(tr("save database successfully!"));
		if (m_nowOrder>=m_flowPointNum) //���һ��������
		{
			stopVerify(); //ֹͣ�춨
			exportReport();
		}
		else //�������һ��������
		{
			prepareVerifyFlowPoint(++m_nowOrder);
		}
	}
	else //�ж�������ʧ�ܵģ���ֵ��
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
		QString defaultPath = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\report\\total\\";
		CReport rpt(sqlCondition);
		rpt.setIniName("rptconfig_total.ini");
		rpt.writeRpt();
		rpt.saveTo(defaultPath + xlsname);
		ui.labelHintProcess->setText(tr("Verify has Stoped!") + "\n" + tr("export excel file successful!"));
	}
	catch (QString e)
	{
		QMessageBox::warning(this, tr("Error"), e);
	}
}

//�򿪷���
int TotalWeightDlg50::openValve(UINT8 portno)
{
	m_controlObj->askControlRelay(portno, VALVE_OPEN);
	if (m_portsetinfo.version==OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(portno, VALVE_OPEN);
	}
	return true;
}

//�رշ���
int TotalWeightDlg50::closeValve(UINT8 portno)
{
	m_controlObj->askControlRelay(portno, VALVE_CLOSE);
	if (m_portsetinfo.version==OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(portno, VALVE_CLOSE);
	}
	return true;
}

//�������ţ��򿪻��߹ر�
int TotalWeightDlg50::operateValve(UINT8 portno)
{
	if (m_valveStatus[portno]==VALVE_OPEN) //����ԭ���Ǵ�״̬
	{
		closeValve(portno);
	}
	else //����ԭ���ǹر�״̬
	{
		openValve(portno);
	}
	return true;
}

//��ˮ��
int TotalWeightDlg50::openWaterPump()
{
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_OPEN);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_OPEN);
	}
	return true;
}

//�ر�ˮ��
int TotalWeightDlg50::closeWaterPump()
{
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_CLOSE);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_CLOSE);
	}
	return true;
}

//����ˮ�� �򿪻��߹ر�
int TotalWeightDlg50::operateWaterPump()
{
	if (m_valveStatus[m_portsetinfo.pumpNo]==VALVE_OPEN) //ˮ��ԭ���Ǵ�״̬
	{
		closeWaterPump();
	}
	else //ˮ��ԭ���ǹر�״̬
	{
		openWaterPump();
	}
	return true;
}

//��Ӧ����״̬���óɹ�
void TotalWeightDlg50::slotSetValveBtnStatus(const UINT8 &portno, const bool &status)
{
	m_valveStatus[portno] = status;
	setValveBtnBackColor(m_valveBtn[portno], m_valveStatus[portno]);
}

//��Ӧ���ڷ����ڳɹ�
void TotalWeightDlg50::slotSetRegulateOk()
{
// 	setRegBtnBackColor(m_regBtn[m_nowRegIdx], true);
}


//�Զ���ȡ��ųɹ� ��ʾ���
void TotalWeightDlg50::slotSetMeterNumber(const QString& comName, const QString& meterNo)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}
	if (m_state == STATE_INIT)
	{
		ui.tableWidget->item(meterPos-1, COLUMN_METER_NUMBER)->setText(meterNo.right(8)); //���
	}
}

/*
** �Զ���ȡ�������ɹ� ��ʾ������
*/
void TotalWeightDlg50::slotSetMeterEnergy(const QString& comName, const QString& energy)
{
	int meterPos = m_readComConfig->getMeterPosByComName(comName);
	if (meterPos < 1)
	{
		return;
	}

	int idx = isMeterPosValid(meterPos);
  if (idx < 0) //�ñ�λ�������ȻҲ����Ҫ��������
	{
		return;
	}
	bool ok;
	float heat = energy.toFloat(&ok);
	if (m_state == STATE_START_VALUE) //��ֵ
	{
    ui.tableWidget->item(meterPos - 1, COLUMN_METER_START)->setText(energy);
	}
	else if (m_state == STATE_END_VALUE) //��ֵ
	{
  	ui.tableWidget->item(meterPos - 1, COLUMN_METER_END)->setText(energy);

	}
}

//���÷��Ű�ť����ɫ
void TotalWeightDlg50::setValveBtnBackColor(QToolButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //���Ŵ� ��ɫ
	{
		btn->setStyleSheet("background-color:rgb(0,255,0);border:0px;border-radius:10px;");
// 		btn->setStyleSheet("border-image:url(:/weightmethod/images/btncheckon.png)");
	}
	else //���Źر� ��ɫ
	{
		btn->setStyleSheet("background-color:rgb(255,0,0);border:0px;border-radius:10px;");
// 		btn->setStyleSheet("border-image:url(:/weightmethod/images/btncheckoff.png)");
	}
}

//���õ��ڷ���ť����ɫ
void TotalWeightDlg50::setRegBtnBackColor(QPushButton *btn, bool status)
{
	if (NULL == btn)
	{
		return;
	}
	if (status) //���ڳɹ�
	{
		btn->setStyleSheet("background:blue;border:0px;");
	}
	else //����ʧ��
	{
		btn->setStyleSheet("");
	}
}

//��������
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
** ���Ƽ̵�������
*/
void TotalWeightDlg50::on_btnWaterIn_clicked() //��ˮ��
{
	m_nowPortNo = m_portsetinfo.waterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnBigWaterIn_clicked() //����ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.bigWaterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnBigWaterOut_clicked() //����ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.bigWaterOutNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnSmallWaterIn_clicked() //С��ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.smallWaterInNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnSmallWaterOut_clicked() //С��ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.smallWaterOutNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveBig_clicked() //��������
{
	m_nowPortNo = m_portsetinfo.bigNo;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveMiddle1_clicked() //����һ��
{
	m_nowPortNo = m_portsetinfo.middle1No;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveMiddle2_clicked() //��������
{
	m_nowPortNo = m_portsetinfo.middle2No;
	operateValve(m_nowPortNo);
}

void TotalWeightDlg50::on_btnValveSmall_clicked() //С������
{
	m_nowPortNo = m_portsetinfo.smallNo;
	operateValve(m_nowPortNo);
}

/*
** ����ˮ�ÿ���
*/
void TotalWeightDlg50::on_btnWaterPump_clicked()
{
/*	if (ui.spinBoxFrequency->value() <= 0)
	{
		QMessageBox::warning(this, tr("Warning"), tr("please input frequency of transducer"));//�����ñ�Ƶ��Ƶ�ʣ�
		ui.spinBoxFrequency->setFocus();
	}
 	m_controlObj->askControlRegulate(m_portsetinfo.pumpNo, ui.spinBoxFrequency->value());
*/
	m_nowPortNo = m_portsetinfo.pumpNo;
	operateWaterPump();
}

/*
** ���ñ�Ƶ��Ƶ��
*/
void TotalWeightDlg50::on_btnSetFreq_clicked()
{
	m_controlObj->askSetDriverFreq(ui.spinBoxFreq->value());
}

//��ȡ���ֵ
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
	if (m_resetZero) //��ֵ����
	{
		memset(m_meterStartValue, 0, sizeof(float)*m_validMeterNum);
		for (int j=0; j<m_validMeterNum; j++)
		{
			ui.tableWidget->item(m_meterPosMap[j]-1, COLUMN_METER_START)->setText(""); //��ճ�ֵ��
			ui.tableWidget->item(m_meterPosMap[j]-1, COLUMN_METER_START)->setText(QString("%1").arg(m_meterStartValue[j]));
		}
	}
	else //��ֵ������
	{
		if (m_nowOrder <= 1)
		{
			if (m_autopick) //�Զ��ɼ�
			{
				wait(WAIT_COM_TIME); //��Ҫ�ȴ��������ȱ���������ӦͨѶ
				ui.labelHintProcess->setText(tr("please input start value of heat meter"));
				on_btnAllReadData_clicked();
//	 			sleep(WAIT_COM_TIME); //�ȴ����ڷ�������
			}
			else //�ֶ�����
			{
				ui.labelHintProcess->setText(tr("please input start value of heat meter"));
				ui.tableWidget->setCurrentCell(m_meterPosMap[0]-1, COLUMN_METER_START); //��λ����һ����Ҫ�����ֵ�ĵط�
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

//��ȡ����ֵ
int TotalWeightDlg50::getMeterEndValue()
{
	if (m_stopFlag)
	{
		return false;
	}

	ui.labelHintProcess->setText(tr("please input end value of heat meter"));	
	m_state = STATE_END_VALUE;

	if (m_autopick) //�Զ��ɼ�
	{
		on_btnAllReadData_clicked();
		/*
		����һ�б����������������Զ�����ʧ�ܵģ��ֶ��������ݺ���һ����������󣬻��bug��
		QObject.cpp
		void QObject::installEventFilter(QObject *obj)
		qWarning("QObject::installEventFilter(): Cannot filter events for objects in a different thread.");
		*/
// 		sleep(WAIT_COM_TIME); //�ȴ����ڷ�������
	}
	else //�ֶ�����
	{
		ui.tableWidget->setCurrentCell(m_meterPosMap[0]-1, COLUMN_METER_END); //��λ����һ����Ҫ������ֵ�ĵط�
		return false;
	}
	return true;
}

//��һ�ε���ֵ��Ϊ���εĳ�ֵ
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
** ��Ӧ�����û���������ֵ������ֵ
   ���������
      row����������0��ʼ
	  column����������0��ʼ
*/
void TotalWeightDlg50::on_tableWidget_cellChanged(int row, int column)
{
	if (!m_autopick && column==COLUMN_METER_NUMBER && m_state==STATE_INIT) //�ֶ����� ����� ��ʼ״̬
	{
		ui.tableWidget->setCurrentCell(row+1, column);
	}

	if (NULL==ui.tableWidget->item(row,  column) || NULL==m_meterStartValue || NULL==m_meterEndValue)
	{
		return;
	}

	int meterPos = row + 1; //��λ��
	int idx = isMeterPosValid(meterPos);
	if (idx < 0)
	{
		return;
	}

	bool ok;
	if (column==COLUMN_METER_START && m_state==STATE_START_VALUE) //���ֵ�� �� ���������ֵ
	{
		m_meterStartValue[idx] = ui.tableWidget->item(row, column)->text().toFloat(&ok);
		if (!ok)
		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Error: please input right digits"));//���������������ȷ������
// 			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START);
			return;
		}
		startVerifyFlowPoint(m_nowOrder);
	}

	if (column==COLUMN_METER_END && m_state==STATE_END_VALUE) //����ֵ�� �� ����������ֵ
	{
		m_meterEndValue[idx] = ui.tableWidget->item(row, column)->text().toFloat(&ok);
		if (!ok)
		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Error: please input right digits"));//���������������ȷ������
// 			ui.tableWidget->setCurrentCell(row, COLUMN_METER_END);
			return;
		}
// 		calcVerifyResult();
		if (/*!m_autopick &&*/calcVerifyResult()==0 && meterPos<m_meterPosMap[m_validMeterNum-1])//�ֶ����롢�������һ������ֵ,�Զ���λ����һ��
		{
			ui.tableWidget->setCurrentCell(m_meterPosMap[idx+1]-1, column);
		}
	}
}

/*
** �������б����ļ춨��¼
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

//�������ţ����б��㲥��ַ����
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

//����������ݣ����б��㲥��ַ����
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

//���ü춨״̬�����б�
void TotalWeightDlg50::on_btnAllVerifyStatus_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotVerifyStatus(i);
	}
}

/*
** ��ȡ���
** ���������
	row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void TotalWeightDlg50::slotReadNO(const int &row)
{
	qDebug()<<"slotReadNO row ="<<row;
	m_meterObj[row].askReadMeterNO();
}

/*
** ��ȡ������
** ���������
	row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void TotalWeightDlg50::slotReadData(const int &row)
{
	qDebug()<<"slotReadData row ="<<row;
	m_meterObj[row].askReadMeterData();
}

/*
** �춨״̬
** ���������
	row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void TotalWeightDlg50::slotVerifyStatus(const int &row)
{
	qDebug()<<"slotVerifyStatus row ="<<row;
	m_meterObj[row].askSetVerifyStatus(VTYPE_HEAT);
}