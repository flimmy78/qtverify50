/***********************************************
**  �ļ���:     flowstandard50.cpp
**  ����:       �����춨(��׼����������-DN50��˫��ƽ��˫���ư�
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2015/11/27
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2015/11 ��һ��
**  ���ݰ���:
**  ˵��:		
**  ���¼�¼:   
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

#include "flowstandard50.h"
#include "commondefine.h"
#include "algorithm.h"
#include "qtexdb.h"
#include "parasetdlg.h"
#include "readcomconfig.h"
#include "report.h"

FlowStandardDlg50::FlowStandardDlg50(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"FlowStandardDlg50 thread:"<<QThread::currentThreadId();
	ui.setupUi(this);
	m_stdParam = new QSettings(getFullIniFileName("stdmtrparaset.ini"), QSettings::IniFormat);

	//��ͬ�ȼ����������Ӧ�ı�׼���,��λ%
	m_gradeErrA[1] = 1.00f;
	m_gradeErrA[2] = 2.00f;
	m_gradeErrA[3] = 3.00f;

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
	m_state = STATE_INIT;

	if (!getPortSetIni(&m_portsetinfo)) //��ȡ��λ���˿ں�������Ϣ
	{
		QMessageBox::warning(this, tr("Warning"), tr("Warning:get port set info failed!"));
	}

	m_readComConfig = new ReadComConfig(); //���������ýӿڣ�������initBalanceComǰ���ã�

	m_instantFlowCom = NULL;
	m_instSTDMeterTimer = NULL;
	initInstStdCom();//��ʼ��˲ʱ��������

	m_accumulateFlowCom = NULL;
	m_accumSTDMeterTimer = NULL;
	initAccumStdCom();//��ʼ���ۻ���������

	m_tempObj = NULL;
	m_tempTimer = NULL;
	initTemperatureCom();	//��ʼ���¶Ȳɼ�����

	m_controlObj = NULL;
	initControlCom();		//��ʼ�����ƴ���

	m_meterObj = NULL;      //������ͨѶ
	m_recPtr = NULL;        //���������

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
	m_flowSC = 1.0;			 //������ȫϵ����Ĭ��1.0
	m_adjErr = false;        //Ĭ�ϲ��������
	m_writeNO = false;       //Ĭ�ϲ�д���
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
	m_StdStartMass = 0;
	m_StdEndMass = 0;
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
}

FlowStandardDlg50::~FlowStandardDlg50()
{
}

void FlowStandardDlg50::showEvent(QShowEvent * event)
{
	qDebug()<<"FlowStandardDlg::showEvent";
}

void FlowStandardDlg50::closeEvent( QCloseEvent * event)
{
	qDebug()<<"^^^^^FlowStandardDlg::closeEvent";
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

	m_stopFlag = true;
	closeAllValveAndPumpOpenOutValve();
	wait(CYCLE_TIME);
	openWaterOutValve();
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


	if (m_tempTimer) //�ɼ��¶ȼ�ʱ��
	{
		if (m_tempTimer->isActive())
		{
			m_tempTimer->stop();
		}
		delete m_tempTimer;
		m_tempTimer = NULL;
	}

	if (m_tempThread.isRunning())  // �¶Ȳɼ�
	{
		m_tempThread.exit(); //������־�л��о���"QThread: Destroyed while thread is still running"
		if (m_tempObj)
		{
			delete m_tempObj;
			m_tempObj = NULL; 
		}	 		 		
	}	   

	if (m_controlObj)  //���ſ���
	{
		delete m_controlObj;
		m_controlObj = NULL;

		m_valveThread.exit();
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

	if (m_instSTDMeterTimer)
	{
		if (m_instSTDMeterTimer->isActive())
		{
			m_instSTDMeterTimer->stop();
		}

		delete m_instSTDMeterTimer;
		m_instSTDMeterTimer = NULL;
	}

	if (m_instantFlowCom)
	{
		m_instantFlowThread.exit();
		delete m_instantFlowCom;
		m_instantFlowCom = NULL;
	}

	if (m_accumSTDMeterTimer)
	{
		if (m_accumSTDMeterTimer->isActive())
		{
			m_accumSTDMeterTimer->stop();
		}
		delete m_accumSTDMeterTimer;
		m_accumSTDMeterTimer = NULL;
	}

	if (m_accumulateFlowCom)
	{
		m_accumFlowThread.exit();
		delete m_accumulateFlowCom;
		m_accumulateFlowCom = NULL;
	}
	
	if (m_stdParam)//ͣ��˲ʱ���ۻ�������timer��������������ļ�����
	{
		delete m_stdParam;
		m_stdParam = NULL;
	}

	emit signalClosed();
}

void FlowStandardDlg50::resizeEvent(QResizeEvent * event)
{
	qDebug()<<"resizeEvent...";

	int th = ui.tableWidget->size().height();
	int tw = ui.tableWidget->size().width();
	int hh = ui.tableWidget->horizontalHeader()->size().height();
	int vw = ui.tableWidget->verticalHeader()->size().width();
	int vSize = (int)((th-hh-10)/(m_maxMeterNum<=0 ? 12 : m_maxMeterNum));
	int hSize = (int)((tw-vw-10)/COLUMN__FLOW_COUNT);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(vSize);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(hSize);
}

//˲ʱ�����ɼ�����, ��λ��ֱ�Ӳɼ�
void FlowStandardDlg50::initInstStdCom()
{
	ComInfoStruct InstStdCom = m_readComConfig->ReadInstStdConfig();
	m_instantFlowCom = new lcModRtuComObject();
	m_instantFlowCom->moveToThread(&m_instantFlowThread);
	m_instantFlowThread.start();
	m_instantFlowCom->openLcModCom(&InstStdCom);

	connect(m_instantFlowCom, SIGNAL(lcModValueIsReady(const QByteArray &)), this, SLOT(slotGetInstStdMeterPulse(const QByteArray &)));

	if (m_instSTDMeterTimer == NULL)
		m_instSTDMeterTimer = new QTimer();

	connect(m_instSTDMeterTimer, SIGNAL(timeout()), this, SLOT(slotAskInstPulse()));
	m_instSTDMeterTimer->start(TIMEOUT_STD_INST);
}
//�ۼ������ɼ�����, ��λ��ֱ�Ӳɼ�
void FlowStandardDlg50::initAccumStdCom()
{
	ComInfoStruct AccumStdCom = m_readComConfig->ReadAccumStdConfig();
	m_accumulateFlowCom = new lcModRtuComObject();
	m_accumulateFlowCom->moveToThread(&m_accumFlowThread);
	m_accumFlowThread.start();
	m_accumulateFlowCom->openLcModCom(&AccumStdCom);

	connect(m_accumulateFlowCom, SIGNAL(lcModValueIsReady(const QByteArray &)), this, SLOT(slotGetAccumStdMeterPulse(const QByteArray &)));

	if (m_accumSTDMeterTimer == NULL)
		m_accumSTDMeterTimer = new QTimer();

	connect(m_accumSTDMeterTimer, SIGNAL(timeout()), this, SLOT(slotAskAccumPulse()));
	m_accumSTDMeterTimer->start(TIMEOUT_STD_ACCUM);
}

void FlowStandardDlg50::slotAskInstPulse()
{
	bool ok;
	uchar address = (uchar)m_stdParam->value("DevNo./InstDevNo").toString().toInt(&ok, 16);
	m_instantFlowCom->ask901712RoutesCmd(address);
}

void FlowStandardDlg50::slotAskAccumPulse()
{
	bool ok;
	uchar address = (uchar)m_stdParam->value("DevNo./AccumDevNo").toString().toInt(&ok, 16);
	m_accumulateFlowCom->ask9150A16RoutesCmd(address);
}

/*
** �¶Ȳɼ����� ��λ��ֱ�Ӳɼ�
** ��������
*/
void FlowStandardDlg50::initTemperatureCom()
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

void FlowStandardDlg50::slotAskPipeTemperature()
{
	m_tempObj->writeTemperatureComBuffer();
}

//���ư�ͨѶ����
void FlowStandardDlg50::initControlCom()
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


//������ͨѶ����
void FlowStandardDlg50::initMeterCom()
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
** �˿ں�-����ӳ���ϵ����ʼ������״̬��Ĭ�Ϸ��ų�ʼ״̬ȫ��Ϊ�ر�,ˮ�ó�ʼ״̬Ϊ�رգ�
** ��Ҫ�Ľ��ø������
*/
void FlowStandardDlg50::initValveStatus()
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
	m_valveStatus[m_portsetinfo.bigWaterInNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.bigWaterOutNo] = VALVE_OPEN;
	m_valveStatus[m_portsetinfo.smallWaterInNo] = VALVE_OPEN;
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

//�ڽ���ˢ������¶Ⱥͳ����¶�ֵ
void FlowStandardDlg50::slotFreshComTempValue(const QString& tempStr)
{
	ui.lcdInTemper->display(tempStr.left(TEMPER_DATA_WIDTH));   //����¶� PV
	ui.lcdOutTemper->display(tempStr.right(TEMPER_DATA_WIDTH)); //�����¶� SV
}

//��⴮�ڡ��˿������Ƿ���ȷ
int FlowStandardDlg50::isComAndPortNormal()
{
	return true;
}

//��ȡ��ǰ�춨����;��ʼ�����ؼ�����ʾ�ؼ���������ʼ��������ͨѶ����
int FlowStandardDlg50::readNowParaConfig()
{
	if (NULL == m_paraSetReader)
	{
		return false;
	}

	if (!m_stopFlag)
	{
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
	m_model = m_nowParams->m_model;   //���ͺ�
	m_maxMeterNum = m_nowParams->m_maxMeters;//��ͬ�����Ӧ�����������
	m_pickcode = m_nowParams->m_pickcode; //�ɼ�����
	m_numPrefix = getNumPrefixOfManufac(m_pickcode); //���ǰ׺
	m_flowSC = m_nowParams->sc_flow; //������ȫϵ��
	m_adjErr = m_nowParams->bo_adjerror; //�Ƿ�������
	m_writeNO = m_nowParams->bo_writenum;//�Ƿ�д���
	m_newMeterNO = m_nowParams->meterNo;

	initTableWidget();
	showNowKeyParaConfig();
	initMeterCom();

	ui.btnAllAdjError->setEnabled(false);
	ui.btnAllModifyNO->setEnabled(false);
	return true;
}

//��ʼ�����ؼ�
void FlowStandardDlg50::initTableWidget()
{
	if (m_maxMeterNum <= 0)
	{
		return;
	}
	ui.tableWidget->setRowCount(m_maxMeterNum); //���ñ������

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
		QPushButton *btnModNo = new QPushButton(QObject::tr("(%1)").arg(i+1) + tr("ModifyNO"));
		ui.tableWidget->setCellWidget(i, COLUMN_MODIFY_METERNO, btnModNo);
		signalMapper1->setMapping(btnModNo, i);
		connect(btnModNo, SIGNAL(clicked()), signalMapper1, SLOT(map()));
		btnModNo->setEnabled(false);

		QPushButton *btnAdjErr = new QPushButton(QObject::tr("(%1)").arg(i+1) + tr("AdjustErr"));
		ui.tableWidget->setCellWidget(i, COLUMN_ADJUST_ERROR, btnAdjErr);
		signalMapper2->setMapping(btnAdjErr, i);
		connect(btnAdjErr, SIGNAL(clicked()), signalMapper2, SLOT(map()));
		btnAdjErr->setEnabled(false);

		QPushButton *btnReadData = new QPushButton(QObject::tr("(%1)").arg(i+1) + tr("ReadData"));
		ui.tableWidget->setCellWidget(i, COLUMN_READ_DATA, btnReadData);
		signalMapper3->setMapping(btnReadData, i);
		connect(btnReadData, SIGNAL(clicked()), signalMapper3, SLOT(map()));

		QPushButton *btnVerifySt = new QPushButton(QObject::tr("(%1)").arg(i+1) + tr("VerifySt"));
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
	ui.tableWidget->setFont(QFont("Times", 15, QFont::DemiBold, true));
// 	ui.tableWidget->resizeColumnsToContents();
}

//��ʾ��ǰ�ؼ�����������Ϣ
void FlowStandardDlg50::showNowKeyParaConfig()
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
int FlowStandardDlg50::isDataCollectNormal()
{
	return true;
}

/*
** ��ʼ��������ʱ
*/
int FlowStandardDlg50::startExhaustCountDown()
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
void FlowStandardDlg50::slotExaustFinished()
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

	if (setAllMeterVerifyStatus()) //���ü춨״̬�ɹ�
	{
		startVerify();
	}
}

/*
** ��ȡ�����ȱ�����ϵ��
*/
int FlowStandardDlg50::readAllMeterFlowCoe()
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

//�����������������춨״̬
int FlowStandardDlg50::setAllMeterVerifyStatus()
{
	ui.labelHintPoint->setText(tr("setting verify status ..."));
	on_btnAllVerifyStatus_clicked();
	wait(CYCLE_TIME);
	on_btnAllVerifyStatus_clicked();
	return true;
}


//�����з��ź�ˮ��
int FlowStandardDlg50::openAllValveAndPump()
{
	openWaterOutValve();
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
int FlowStandardDlg50::closeAllValveAndPumpOpenOutValve()
{
	openWaterOutValve(); //�˳�ʱ�򿪷�ˮ��
	wait(CYCLE_TIME);
	closeWaterPump();    //�˳�ʱ�ر�ˮ��
	closeAllFlowPointValves();//�ر����������㷧��
	wait(CYCLE_TIME);
	closeValve(m_portsetinfo.waterInNo);//�رս�ˮ��

	return true;
}

//�ر����������㷧��
int FlowStandardDlg50::closeAllFlowPointValves()
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

//�رշ�ˮ����
int FlowStandardDlg50::closeWaterOutValve()
{
	closeValve(m_portsetinfo.waterOutNo);
	return true;
}

//�򿪷�ˮ����
int FlowStandardDlg50::openWaterOutValve()
{
	openValve(m_portsetinfo.waterOutNo);
	return true;
}

/*
** ���ܣ��ж���ƽ�����Ƿ�ﵽҪ��ļ춨��������춨���̵�ƽ���¶Ⱥ�ƽ������(m3/h)
** ��������ܴ󣬳���float�ķ�Χ������Ҫ��double
*/
int FlowStandardDlg50::judgeTartgetVolAndCalcAvgTemperAndFlow(double initV, double verifyV)
{
	double targetV       = initV + verifyV;
	ui.tableWidget->setEnabled(false);
	ui.btnAllReadNO->setEnabled(false);
	ui.btnAllReadData->setEnabled(false);
	ui.btnAllVerifyStatus->setEnabled(false);
	QDateTime startTime = QDateTime::currentDateTime();
	int second          = 0;
	double nowFlow       = m_paraSetReader->getFpBySeq(m_nowOrder).fp_verify;
	double nowVol        = initV;
	flow_rate_wdg wdgIdx = (flow_rate_wdg)m_paraSetReader->getBigSmallBySeq(m_nowOrder);//��ȡ����������С������ 
	while (!m_stopFlag && (nowVol < targetV))
	{
		qDebug()<<"��ǰ��ˮ�� ="<<nowVol<<", С��Ŀ����� "<<targetV;
		qDebug()<<"m_stopFlag: "<<m_stopFlag;
		m_avgTFCount++;
		m_pipeInTemper += ui.lcdInTemper->value();
		m_pipeOutTemper += ui.lcdOutTemper->value();
		second = 3.6*(targetV - nowVol)/nowFlow;
		ui.labelHintPoint->setText(tr("NO. <font color=DarkGreen size=6><b>%1</b></font> flow point: <font color=DarkGreen size=6><b>%2</b></font> m3/h")\
			.arg(m_nowOrder).arg(nowFlow));
		ui.labelHintProcess->setText(tr("Verifying...Please wait for about <font color=DarkGreen size=6><b>%1</b></font> second").arg(second));
		wait(CYCLE_TIME);		
		nowVol = getAccumFLowVolume(wdgIdx);//��¼��׼�����(L)
	}

	m_pipeInTemper = m_pipeInTemper/m_avgTFCount;   //���ƽ���¶�
	m_pipeOutTemper = m_pipeOutTemper/m_avgTFCount; //����ƽ���¶�

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

//��ձ�񣬵�һ�г���("���"��)
void FlowStandardDlg50::clearTableContents()
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
void FlowStandardDlg50::on_btnStart_clicked()
{
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

	if (m_autopick) //�Զ�����
	{
		on_btnAllReadNO_clicked();
// 		sleep(m_exaustSecond*1000/2);
// 		setAllMeterVerifyStatus();
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
void FlowStandardDlg50::on_btnExhaust_clicked()
{

}

//���"����"��ť
void FlowStandardDlg50::on_btnGoOn_clicked()
{
	ui.btnGoOn->hide();
	startVerify();
}

//���"���¼���"��ť
void FlowStandardDlg50::on_btnReCalc_clicked()
{
}

//���"��ֹ���"��ť
void FlowStandardDlg50::on_btnStop_clicked()
{
	int button = QMessageBox::question(this, tr("Question"), tr("Stop Really ?"), \
		QMessageBox::Yes|QMessageBox::Default, QMessageBox::No|QMessageBox::Escape);
	if (button == QMessageBox::No)
	{
		return;
	}

	stopVerify();
}

void FlowStandardDlg50::on_btnExit_clicked()
{
	stopVerify();
	this->close();
}

//ֹͣ�춨
void FlowStandardDlg50::stopVerify()
{
	ui.labelHintPoint->clear();
	if (!m_stopFlag)
	{
		ui.labelHintProcess->setText(tr("stopping verify...please wait a minute"));
		m_exaustTimer->stop();//ֹͣ������ʱ��
		closeAllValveAndPumpOpenOutValve();
	}
	m_stopFlag = true; //���ټ����ƽ����
	ui.labelHintProcess->setText(tr("Verify has Stoped!"));
	m_state = STATE_INIT; //���ó�ʼ״̬

	ui.tableWidget->setEnabled(true);
	ui.btnAllReadNO->setEnabled(true);
	ui.btnAllReadData->setEnabled(true);
	ui.btnAllVerifyStatus->setEnabled(true);
	ui.btnStart->setEnabled(true);
}

//��ʼ�춨
void FlowStandardDlg50::startVerify()
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
	if (m_oldMeterCoe != NULL)
	{
		for (int n=0; n<m_validMeterNum; n++)
		{
			delete []m_oldMeterCoe[n];
		}
		delete []m_oldMeterCoe;
		m_oldMeterCoe = NULL;
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
	m_recPtr = new Flow_Verify_Record_STR[m_validMeterNum];
	memset(m_recPtr, 0, sizeof(Flow_Verify_Record_STR)*m_validMeterNum);

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

	//������Ч��λ���������ԭ���ĸ�������ϵ��
	m_oldMeterCoe = new MeterCoe_PTR[m_validMeterNum];
	for (int j=0; j<m_validMeterNum; j++)
	{
		m_oldMeterCoe[j] = new MeterCoe_STR;
		m_oldMeterCoe[j]->bigCoe = 1.0;
		m_oldMeterCoe[j]->mid2Coe = 1.0;
		m_oldMeterCoe[j]->mid1Coe = 1.0;
		m_oldMeterCoe[j]->smallCoe = 1.0;
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

	 //��ȡԭ����������ϵ��
	if (m_adjErr)
	{
		ui.labelHintPoint->setText(tr("read meter now coe ..."));
		m_state = STATE_READ_COE;
		readAllMeterFlowCoe();
		wait(WAIT_COM_TIME);
		m_state = STATE_INIT;
	}

	if (m_continueVerify) //�����춨
	{
		wait(BALANCE_STABLE_TIME); //�ȴ�3����(�ȴ�ˮ���ȶ�)
	}

	m_nowOrder = 1;
	prepareVerifyFlowPoint(m_nowOrder); //��һ��������춨
}

//��ȡ��Ч������,������ӳ���ϵ��������±�-��λ�ţ�
int FlowStandardDlg50::getValidMeterNum()
{

	//ƥ�����Ƿ�Ϊ����; 
	//ǰһ��'\'��ת���ַ�, "\\"���൱��'\', "\\d"�൱��'\d', ƥ��һ������, '+'�����ֵ����հ�;
	//��ģʽҲ����д�� "[1-9]+"
	QRegExp rx("\\d+");

	m_validMeterNum = 0; //������
	QString meterNum;//���
	for (int i=0; i<m_maxMeterNum; i++)
	{
		if (NULL == ui.tableWidget->item(i, COLUMN_METER_NUMBER)) //"���"��Ԫ��Ϊ��
		{
			continue;
		}
		meterNum = ui.tableWidget->item(i, COLUMN_METER_NUMBER)->text();
		if (!rx.exactMatch(meterNum))
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
int FlowStandardDlg50::isMeterPosValid(int meterPos)
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
** ׼������������ļ춨�����б�Ҫ�ļ��
** ע�⣺order��1��ʼ
*/
int FlowStandardDlg50::prepareVerifyFlowPoint(int order)
{
	if (order < 1 || order > m_flowPointNum || m_stopFlag)
	{
		return false;
	}
		
	if (!m_continueVerify) //�������춨��ÿ�μ춨��ʼ֮ǰ��Ҫ�ж���ƽ����
	{
		wait(BALANCE_STABLE_TIME);   //�ȴ�3���ӣ��ȴ�ˮ���ȶ�
	}

	int i=0;
	if (m_resetZero) //��ֵ����
	{
		ui.labelHintPoint->setText(tr("Reset Zero"));
		while (i < RESET_ZERO_TIME && !m_stopFlag) //�ȴ�������ֵ����
		{
			ui.labelHintProcess->setText(tr("please wait <font color=DarkGreen size=4><b>%1</b></font> seconds for reset zero").arg(RESET_ZERO_TIME-i));
			i++;
			wait(CYCLE_TIME); 
		}
	}
	getMeterStartValue(); //��ȡ���ֵ

	return true;
}

/************************************************************************/
/* ���е���������ļ춨                                                               
/* ����DN25���µı�, ֮�����ֽ������еļ�ⷽ��ʹ������, �������ùܵ��еĶ�����
/* �������غ�ԭ��, 
/* �����ڹܵ��еĶ�������״̬��Һ��,������ܵ�������, ��������������Һ���������.
/* �����ڽ��б�׼��춨ʱ, ���뽫��׼����������Ϊ��Ӧ�¶��µ�����.
/************************************************************************/
int FlowStandardDlg50::startVerifyFlowPoint(int order)
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
		row = m_meterPosMap[p]-1;
		ui.tableWidget->item(row, COLUMN_METER_START)->text().toFloat(&ok);
		if (!ok || ui.tableWidget->item(row, COLUMN_METER_START)->text().isEmpty())
		{
// 			QMessageBox::information(this, tr("Hint"), tr("please input start value of heat meter"));//�������������ֵ��
			ui.labelHintProcess->setText(tr("please input start value of heat meter"));
			ui.tableWidget->setCurrentCell(row, COLUMN_METER_START); //��λ����Ҫ�����ֵ�ĵط�
			return false;
		}
	}

	m_pipeInTemper = ui.lcdInTemper->value();
	m_pipeOutTemper = ui.lcdOutTemper->value();
	m_realFlow = ui.lcdFlowRate->value();
	m_avgTFCount = 1;

	int portNo = m_paraSetReader->getFpBySeq(order).fp_valve;  //order��Ӧ�ķ��Ŷ˿ں�	
	float verifyQuantity = m_paraSetReader->getFpBySeq(order).fp_quantity; //��order�μ춨��Ӧ�ļ춨��
	float frequence = m_paraSetReader->getFpBySeq(order).fp_freq; //order��Ӧ��Ƶ��
	m_controlObj->askSetDriverFreq(frequence);

	flow_rate_wdg wdgIdx = (flow_rate_wdg)m_paraSetReader->getBigSmallBySeq(order);//��ȡ����������С������ 
	m_stdStartVol = getAccumFLowVolume(wdgIdx);//��¼��׼���ʼ���(L)
	//m_stdStartVol = ui.lcdAccumStdMeter->value();
	qDebug() << "start volumn: " << m_stdStartVol;
	float stdStartT = m_pipeOutTemper;//��׼���ʼ�¶�, �ֲɼ���·���ڵ�ƽ���¶�.(��׼ȷ��Ҫ����ȷ������)
	float stdStartDen = m_chkAlg->getDensityByQuery(stdStartT);//��׼���ʼƽ���ܶ�(kg/L)
	if (stdStartDen<0)
	{
		return -1;
	}
	m_StdStartMass = stdStartDen*m_stdStartVol;

	if (openValve(portNo)) //�򿪷��ţ���ʼ������
	{
		if (judgeTartgetVolAndCalcAvgTemperAndFlow(m_stdStartVol, verifyQuantity)) //����춨��������˹��̵�ƽ���¶Ⱥ�ƽ������
		{
			ui.tableWidget->setEnabled(true);
			ui.btnAllReadNO->setEnabled(true);
			ui.btnAllReadData->setEnabled(true);
			ui.btnAllVerifyStatus->setEnabled(true);
			closeValve(portNo); //�ر�order��Ӧ�ķ���
			wait(BALANCE_STABLE_TIME); //�ȴ�3���ӣ�����ƽ��ֵ�ȶ�

			m_stdEndVol = getAccumFLowVolume(wdgIdx);//��¼��׼���������(L)
			//m_stdEndVol = ui.lcdAccumStdMeter->value();
			float stdEndT = m_pipeOutTemper;//��׼�������¶�, �ֲɼ���·���ڵ�ƽ���¶�.(��׼ȷ��Ҫ����ȷ������)
			float stdEndDen = m_chkAlg->getDensityByQuery(stdEndT);//��׼������ƽ���ܶ�(kg/L)
			qDebug() << "end volumn: " << m_stdEndVol;
			if(stdEndDen<0)
			{
				return -1;
			}
			m_StdEndMass = m_stdEndVol*stdEndDen;
			if (!m_resetZero && m_nowOrder>=2)
			{
				m_state = STATE_END_VALUE;
				makeStartValueByLastEndValue();
			}
			for (int m=0; m<m_validMeterNum; m++) //
			{
				m_meterTemper[m] = m_chkAlg->getMeterTempByPos(m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m]);//����ÿ���������¶�
				m_meterDensity[m] = m_chkAlg->getDensityByQuery(m_meterTemper[m]);//����ÿ���������ܶ�
				m_meterStdValue[m] = m_chkAlg->getStdVolByPos((m_StdEndMass-m_StdStartMass), m_pipeInTemper, m_pipeOutTemper, m_meterPosMap[m]); //����ÿ�������������׼ֵ

				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_FLOW_POINT)->setText(QString::number(m_realFlow, 'f', 3));//������
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_METER_END)->setText("");//����ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_START)->setText(QString::number(m_StdStartMass, 'f', 3));//��ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_BAL_END)->setText(QString::number(m_StdEndMass, 'f', 3));    //��ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_TEMPER)->setText(QString::number(m_meterTemper[m], 'f', 2));  //�¶�
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DENSITY)->setText(QString::number(m_meterDensity[m], 'f', 3));//�ܶ�
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_STD_VALUE)->setText(QString::number(m_meterStdValue[m], 'f', 3));//��׼ֵ
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setText("");//ʾֵ���
				ui.tableWidget->item(m_meterPosMap[m]-1, COLUMN_DISP_ERROR)->setForeground(QBrush());//ʾֵ���
			}
			getMeterEndValue();
			}//����춨��
		}

	return true;
}

/*
** �������б��������
*/
int FlowStandardDlg50::calcAllMeterError()
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
int FlowStandardDlg50::calcMeterError(int idx)
{
	bool ok;
	int row = m_meterPosMap[idx] - 1;
	ui.tableWidget->item(row, COLUMN_METER_END)->text().toFloat(&ok);
	if (/*m_meterEndValue[idx] <= 0 ||*/ ui.tableWidget->item(row, COLUMN_METER_END)->text().isEmpty() || !ok)
	{
//		ui.tableWidget->setCurrentCell(row, COLUMN_METER_END);
		return 0;
	}
	m_meterError[idx] = 100*(m_meterEndValue[idx] - m_meterStartValue[idx] - m_meterStdValue[idx])/m_meterStdValue[idx];//����ĳ��������
	int valveIdx = m_paraSetReader->getFpBySeq(m_nowOrder).fp_valve_idx; //0:�� 1:�ж� 2:��һ 3:С
	m_meterErr[idx][valveIdx] = m_meterError[idx];
	ui.tableWidget->item(row, COLUMN_DISP_ERROR)->setText(QString::number(m_meterError[idx], 'f', ERR_PRECISION)); //ʾֵ���
	float stdError = m_flowSC*(m_gradeErrA[m_nowParams->m_grade] + m_gradeErrB[m_nowParams->m_grade]*m_mapNormalFlow[m_standard]/m_realFlow); //��׼���=���Ҫ�����*������ȫϵ��
	ui.tableWidget->item(row, COLUMN_STD_ERROR)->setText("��" + QString::number(stdError, 'f', ERR_PRECISION)); //��׼���
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
	m_recPtr[idx].methodFlag = STANDARD_METHOD; //��׼��
	m_recPtr[idx].meterValue0 = m_meterStartValue[idx];
	m_recPtr[idx].meterValue1 = m_meterEndValue[idx];
	m_recPtr[idx].balWeight0 = m_StdStartMass;
	m_recPtr[idx].balWeight1 = m_StdEndMass;
	m_recPtr[idx].stdMeterV0 = m_stdStartVol;
	m_recPtr[idx].stdMeterV1 = m_stdEndVol;
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
	m_recPtr[idx].flowcoe = m_nowParams->sc_flow;
	m_recPtr[idx].deviceInfoId = m_readComConfig->getDeviceInfoID();

	return 1; 
}

//������������ֵ�󣬼���춨���
int FlowStandardDlg50::calcVerifyResult()
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
			if (m_adjErr) //ѡ��������
			{
				ui.btnAllAdjError->setEnabled(true);
				for (int i=0; i<m_validMeterNum; i++)
				{
					ui.tableWidget->cellWidget(m_meterPosMap[i]-1, COLUMN_ADJUST_ERROR)->setEnabled(true);
				}
			}
			if (m_writeNO) //ѡ��д���
			{
				ui.btnAllModifyNO->setEnabled(true);
				for (int n=0; n<m_validMeterNum; n++)
				{
					ui.tableWidget->cellWidget(m_meterPosMap[n]-1, COLUMN_MODIFY_METERNO)->setEnabled(true);
				}
				//��������±��뽫���������"�²�"״̬��
				QMessageBox::information(this, tr("Hint"), tr("if it isn't new meter , please change meter to \"deltaT\" status !"));
				for (int j=0; j<m_validMeterNum; j++)
				{
					if (m_meterResult[j]) //���������㶼�ϸ���޸ı��
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

void FlowStandardDlg50::exportReport()
{
	QString sqlCondition = QString("F_TimeStamp=\'%1\' and F_MethodFlag = 1").arg(m_timeStamp);
	QString xlsname = QDateTime::fromString(m_timeStamp, "yyyy-MM-dd HH:mm:ss.zzz").toString("yyyy-MM-dd_hh-mm-ss") + ".xls";
	try
	{
		QString defaultPath = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\report\\flow\\std\\";
		CReport rpt(sqlCondition);
		rpt.setIniName("rptconfig_flow_std.ini");
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
int FlowStandardDlg50::openValve(UINT8 portno)
{	
	if (NULL == m_controlObj)
	{
		return false;
	}

	if (portno == m_portsetinfo.waterOutNo)
	{
		m_controlObj->askControlRelay(portno, VALVE_CLOSE);
	}
	else
		m_controlObj->askControlRelay(portno, VALVE_OPEN);

	if (m_portsetinfo.version==OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(portno, VALVE_OPEN);
	}
	return true;
}

//�رշ���
int FlowStandardDlg50::closeValve(UINT8 portno)
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	if (portno == m_portsetinfo.waterOutNo)
	{
		m_controlObj->askControlRelay(portno, VALVE_OPEN);
	}
	else
		m_controlObj->askControlRelay(portno, VALVE_CLOSE);

	if (m_portsetinfo.version==OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(portno, VALVE_CLOSE);
	}
	return true;
}

//�������ţ��򿪻��߹ر�
int FlowStandardDlg50::operateValve(UINT8 portno)
{
	m_valveStatus[portno] ? closeValve(portno) : openValve(portno);
	return true;
}

//��ˮ��
int FlowStandardDlg50::openWaterPump()
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_OPEN);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_OPEN);
	}
	return true;
}

//�ر�ˮ��
int FlowStandardDlg50::closeWaterPump()
{
	if (NULL == m_controlObj)
	{
		return false;
	}
	m_nowPortNo = m_portsetinfo.pumpNo;
	m_controlObj->askControlWaterPump(m_nowPortNo, VALVE_CLOSE);
	if (m_portsetinfo.version == OLD_CTRL_VERSION) //�Ͽ��ư� �޷���
	{
		slotSetValveBtnStatus(m_nowPortNo, VALVE_CLOSE);
	}
	return true;
}

//����ˮ�� �򿪻��߹ر�
int FlowStandardDlg50::operateWaterPump()
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
void FlowStandardDlg50::slotSetValveBtnStatus(const UINT8 &portno, const bool &status)
{
	m_valveStatus[portno] = status;
	setValveBtnBackColor(m_valveBtn[portno], m_valveStatus[portno]);
}

//��Ӧ���ڷ����ڳɹ�
void FlowStandardDlg50::slotSetRegulateOk()
{
	// 	setRegBtnBackColor(m_regBtn[m_nowRegIdx], true);
}


//�Զ���ȡ��ųɹ� ��ʾ���
void FlowStandardDlg50::slotSetMeterNumber(const QString& comName, const QString& meterNo)
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
void FlowStandardDlg50::slotSetMeterFlow(const QString& comName, const QString& flow)
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

	if (m_state == STATE_START_VALUE) //��ֵ
	{
		ui.tableWidget->item(meterPos - 1, COLUMN_METER_START)->setText(flow);
	}
	else if (m_state == STATE_END_VALUE) //��ֵ
	{
		ui.tableWidget->item(meterPos - 1, COLUMN_METER_END)->setText(flow);
	}
}

/*
** �Զ���ȡ�������ɹ� ˢ�´�������ϵ��
*/
void FlowStandardDlg50::slotFreshBigCoe(const QString& comName, const QString& bigCoe)
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
		qDebug()<<comName<<"��λ ="<<meterPos<<", ��Ч���� ="<<idx<<", bigCoe ="<<m_oldMeterCoe[idx]->bigCoe;
	}
}

/*
** �Զ���ȡ�������ɹ� ˢ���ж�������ϵ��
*/
void FlowStandardDlg50::slotFreshMid2Coe(const QString& comName, const QString& mid2Coe)
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
		qDebug()<<comName<<"��λ ="<<meterPos<<", ��Ч���� ="<<idx<<", mid2Coe ="<<m_oldMeterCoe[idx]->mid2Coe;
	}
}

/*
** �Զ���ȡ�������ɹ� ˢ����һ������ϵ��
*/
void FlowStandardDlg50::slotFreshMid1Coe(const QString& comName, const QString& mid1Coe)
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
		//	m_meterErr[idx][2] = calcErrorValueOfCoe(mid1Coe);
		m_oldMeterCoe[idx]->mid1Coe = calcFloatValueOfCoe(mid1Coe) ;
		qDebug()<<comName<<"��λ ="<<meterPos<<", ��Ч���� ="<<idx<<", mid1Coe ="<<m_oldMeterCoe[idx]->mid1Coe;
	}
}

/*
** �Զ���ȡ�������ɹ� ˢ��С������ϵ��
*/
void FlowStandardDlg50::slotFreshSmallCoe(const QString& comName, const QString& smallCoe)
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
		//	m_meterErr[idx][3] = calcErrorValueOfCoe(smallCoe);
		m_oldMeterCoe[idx]->smallCoe = calcFloatValueOfCoe(smallCoe) ;
		qDebug()<<comName<<"��λ ="<<meterPos<<", ��Ч���� ="<<idx<<", smallCoe ="<<m_oldMeterCoe[idx]->smallCoe;
	}
}

//���÷��Ű�ť����ɫ
void FlowStandardDlg50::setValveBtnBackColor(QToolButton *btn, bool status)
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
void FlowStandardDlg50::setRegBtnBackColor(QPushButton *btn, bool status)
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
void FlowStandardDlg50::on_btnParaSet_clicked()
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
void FlowStandardDlg50::on_btnWaterIn_clicked() //��ˮ��
{
	m_nowPortNo = m_portsetinfo.waterInNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnBigWaterIn_clicked() //����ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.bigWaterInNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnBigWaterOut_clicked() //����ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.bigWaterOutNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnSmallWaterIn_clicked() //С��ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.smallWaterInNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnSmallWaterOut_clicked() //С��ƽ��ˮ��
{
	m_nowPortNo = m_portsetinfo.smallWaterOutNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnValveBig_clicked() //��������
{
	m_nowPortNo = m_portsetinfo.bigNo;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnValveMiddle1_clicked() //����һ��
{
	m_nowPortNo = m_portsetinfo.middle1No;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnValveMiddle2_clicked() //��������
{
	m_nowPortNo = m_portsetinfo.middle2No;
	operateValve(m_nowPortNo);
}

void FlowStandardDlg50::on_btnValveSmall_clicked() //С������
{
	m_nowPortNo = m_portsetinfo.smallNo;
	operateValve(m_nowPortNo);
}

/*
** ����ˮ�ÿ���
*/
void FlowStandardDlg50::on_btnWaterPump_clicked()
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
void FlowStandardDlg50::on_btnSetFreq_clicked()
{
	if (NULL == m_controlObj)
	{
		return;
	}
	m_controlObj->askSetDriverFreq(ui.spinBoxFreq->value());
}

//��ȡ���ֵ
int FlowStandardDlg50::getMeterStartValue()
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
int FlowStandardDlg50::getMeterEndValue()
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
// 		QMessageBox::information(this, tr("Hint"), tr("please input end value of heat meter"));//��������������ֵ��
		ui.tableWidget->setCurrentCell(m_meterPosMap[0]-1, COLUMN_METER_END); //��λ����һ����Ҫ������ֵ�ĵط�
		return false;
	}
	return true;
}

//��һ�ε���ֵ��Ϊ���εĳ�ֵ
void FlowStandardDlg50::makeStartValueByLastEndValue()
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
void FlowStandardDlg50::on_tableWidget_cellChanged(int row, int column)
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
			return;
		}
		startVerifyFlowPoint(m_nowOrder);
	}

	if (column==COLUMN_METER_END && m_state==STATE_END_VALUE) //����ֵ�� �� ����������ֵ
	{
		m_meterEndValue[idx] = ui.tableWidget->item(row, column)->text().toFloat(&ok);
		if (!ok)
		{
// 			QMessageBox::warning(this, tr("Warning"), tr("Error: please input digits"));//�����������������
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
int FlowStandardDlg50::saveAllVerifyRecords()
{
	int ret = insertFlowVerifyRec(m_recPtr, m_validMeterNum);
	if (ret != OPERATE_DB_OK)
	{
		QMessageBox::warning(this, tr("Error"), tr("Error:insert database failed!\n") + tr("Maybe network error!"));
	}
	return ret;
}

//�������ţ����б��㲥��ַ����
void FlowStandardDlg50::on_btnAllReadNO_clicked()
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
void FlowStandardDlg50::on_btnAllReadData_clicked()
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
void FlowStandardDlg50::on_btnAllVerifyStatus_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotVerifyStatus(i);
	}
}

//���������б�
void FlowStandardDlg50::on_btnAllAdjError_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotAdjustError(i);
	}
}

//�޸ı�ţ����б�
void FlowStandardDlg50::on_btnAllModifyNO_clicked()
{
	for (int i=0; i<m_maxMeterNum; i++)
	{
		slotModifyMeterNO(i);
	}
}

/*
** �޸ı��
** ���������
row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
ע�⣺���Ϊ14λ
*/
void FlowStandardDlg50::slotModifyMeterNO(const int &row)
{
	qDebug()<<"slotModifyMeterNO row ="<<row;
	int meterPos = row + 1; //��λ��
	int idx = -1;
	idx = isMeterPosValid(meterPos);
	qDebug()<<"slotModifyMeterNO idx ="<<idx;
	if (idx < 0 || !m_writeNO)
	{
		return;
	}
	if (!m_meterResult[idx])//��һ������춨������ϸ�
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
** �������
** ���������
	row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void FlowStandardDlg50::slotAdjustError(const int &row)
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
		qDebug()<<"slotAdjustError idx ="<<idx<<", �����󣬲�������";
		return;
	}
	QString meterNO = m_numPrefix + ui.tableWidget->item(row, COLUMN_METER_NUMBER)->text();
	m_meterObj[row].askModifyFlowCoe(meterNO, m_meterErr[idx][0], m_meterErr[idx][1], m_meterErr[idx][2], m_meterErr[idx][3], m_oldMeterCoe[idx]);
}

/*
** ��ȡ���
** ���������
row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void FlowStandardDlg50::slotReadNO(const int &row)
{
	qDebug()<<"slotReadNO row ="<<row;
	m_meterObj[row].askReadMeterNO();
}

/*
** ��ȡ���
** ���������
row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void FlowStandardDlg50::slotReadData(const int &row)
{
	qDebug()<<"slotReadMeter row ="<<row;
	m_meterObj[row].askReadMeterData();
}

/*
** �춨״̬
** ���������
row:�кţ���row����֪����ǰ�ȱ��Ӧ�Ĵ��ڡ���š����ȵ�
*/
void FlowStandardDlg50::slotVerifyStatus(const int &row)
{
	qDebug()<<"slotVerifyStatus row ="<<row;
	m_meterObj[row].askSetVerifyStatus(VTYPE_FLOW);
}

/*
** ������ʼ���
*/
void FlowStandardDlg50::saveStartMeterNO()
{
	QString filename = getFullIniFileName("verifyparaset.ini");//�����ļ����ļ���
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");//�����ini�ļ���д��������
	settings.setValue("Other/meternumber", m_newMeterNO);
}

void FlowStandardDlg50::slotGetInstStdMeterPulse(const QByteArray & valueArray)
{
	m_instStdCurrent = valueArray;
	freshInstStdMeter();
	float instValue = 0.0;//˲ʱ����

	instValue = ui.lcdInstStdMeter_3->value()+ui.lcdInstStdMeter_10->value()+ui.lcdInstStdMeter_25->value()+ui.lcdInstStdMeter_50->value();
	ui.lcdFlowRate->display(instValue);
}

float FlowStandardDlg50::getFlowValueByValve(flow_rate_wdg wdgIdx, flow_type fType)
{
	int route		= getRouteByWdg(wdgIdx, fType);//��׼��ӵ�����ģ���ͨ����
	int count		= 0;//����/�����ź�	
	float pulse		= 0.0f;//���嵱��
	float upperFlow = 0.0f;//��������
	float retValue  = 0.0f;//˲ʱ����

	switch (fType)
	{
	case INST_FLOW_VALUE:		
		if (m_instRouteIsRead.contains(route))//���ǰ���Ѿ���ȡ����ͨ��, ��ֱ�ӷ���0	
			return 0.0f;
		else
			m_instRouteIsRead.append(route);//���ǰ��û�ж�ȡ����ͨ��, ��������

		retValue = getInstFlowRate(wdgIdx);
		break;
	case ACCUM_FLOW_VALUE:
		if (m_accumRouteIsRead.contains(route))//���ǰ���Ѿ���ȡ����ͨ��, ��ֱ�ӷ���0	
			return 0.0f;
		else
			m_accumRouteIsRead.append(route);//���ǰ��û�ж�ȡ����ͨ��, ��������

		retValue = getAccumFLowVolume(wdgIdx);
		break;
	default:
		break;
	}	
	return retValue;
}

int FlowStandardDlg50::getRouteByWdg(flow_rate_wdg wdgIdx, flow_type fType)
{
	int route = -1;
	if (m_stdParam == NULL)
	{
		return route;
	}
	m_stdParam->beginReadArray("Route");
	m_stdParam->setArrayIndex(wdgIdx);
	switch(fType)
	{
	case INST_FLOW_VALUE:
		route = m_stdParam->value("InstRoute").toInt();
		break;
	case ACCUM_FLOW_VALUE:
		route = m_stdParam->value("AccumRoute").toInt();
		break;
	default:
		break;
	}	
	m_stdParam->endArray();
	return route;
}

float FlowStandardDlg50::getStdUpperFlow(flow_rate_wdg wdgIdx)
{
	float upperFlow = 0.0f;

	m_stdParam->beginReadArray("FlowRate");
	m_stdParam->setArrayIndex(wdgIdx);
	upperFlow = m_stdParam->value("UpperFlow").toFloat();
	m_stdParam->endArray();
	return upperFlow;
}

float FlowStandardDlg50::getStdPulse(flow_rate_wdg wdgIdx)
{
	float pulse = 0.0f;

	m_stdParam->beginReadArray("Pulse");
	m_stdParam->setArrayIndex(wdgIdx);
	pulse = m_stdParam->value("Pulse").toFloat();
	m_stdParam->endArray();
	return pulse;
}

void FlowStandardDlg50::slotGetAccumStdMeterPulse(const QByteArray & valueArray)
{
	m_accumStdPulse = valueArray;
	freshAccumStdMeter();
	float accumValue = 0.0;//�ۻ�����

	accumValue = ui.lcdAccumStdMeter_3->value()+ui.lcdAccumStdMeter_10->value()+ui.lcdAccumStdMeter_25->value()+ui.lcdAccumStdMeter_50->value();
	ui.lcdAccumStdMeter->display(accumValue);	
}

void FlowStandardDlg50::freshInstStdMeter()
{
	ui.lcdInstStdMeter_50->display(getInstFlowRate(FLOW_RATE_BIG));
	ui.lcdInstStdMeter_25->display(getInstFlowRate(FLOW_RATE_MID_2));
	ui.lcdInstStdMeter_10->display(getInstFlowRate(FLOW_RATE_MID_1));
	ui.lcdInstStdMeter_3->display(getInstFlowRate(FLOW_RATE_SMALL));
}

void FlowStandardDlg50::freshAccumStdMeter()
{
	ui.lcdAccumStdMeter_50->display(getAccumFLowVolume(FLOW_RATE_BIG));
	ui.lcdAccumStdMeter_25->display(getAccumFLowVolume(FLOW_RATE_MID_2));
	ui.lcdAccumStdMeter_10->display(getAccumFLowVolume(FLOW_RATE_MID_1));
	ui.lcdAccumStdMeter_3->display(getAccumFLowVolume(FLOW_RATE_SMALL));
}

float FlowStandardDlg50::getInstFlowRate(flow_rate_wdg idx)
{
	int route = getRouteByWdg(idx, INST_FLOW_VALUE);
	if (route < 0)
	{
		return 0.0;
	}
	int count = get9017RouteI(route, m_instStdCurrent);
	float upperFlow = getStdUpperFlow(idx);
	return getInstStdValue(count, upperFlow);
}

float FlowStandardDlg50::getAccumFLowVolume(flow_rate_wdg idx)
{
	int route = getRouteByWdg(idx, ACCUM_FLOW_VALUE);
	if (route < 0)
	{
		return 0.0;
	}
	int count = get9150ARouteI(route, m_accumStdPulse);
	float pulse = getStdPulse(idx);
	return count*pulse;
}