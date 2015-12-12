/***********************************************
**  文件名:     SetPortFrm.cpp
**  功能:       将用户对各个设备的串口设置保存到 $ADEHOME/ini/comconfig.xml文件中;
					并在窗口加载时，将配置文件中的相应配置装载到窗口中
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2014/6/15
**  专业组:     德鲁计量软件组
**  程序设计者: 宋宝善
**  程序员:     宋宝善
**  版本历史:   2014/06 第一版
**  内容包含:
**  说明:
**  更新记录:	2015/11/21 增加设置双天平情况下的大、小天平进水阀、放水阀端口号-杨深
***********************************************/

#include <QSettings>
#include <QMessageBox>

#include "setportfrm.h"
#include "commondefine.h"
#include "algorithm.h"

SetPortFrm::SetPortFrm(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	gui.setupUi(this);

	QString filename = getFullIniFileName("portset.ini");
	PortSet = new QSettings(filename, QSettings::IniFormat);
	InstallIni();

	m_controlObj = NULL;
	m_controlObj2 = NULL;
	initControlCom();
	initControlCom2();
	initValveStatus();
}

SetPortFrm::~SetPortFrm()
{
}

void SetPortFrm::closeEvent(QCloseEvent * event)
{
	if (PortSet)
	{
		delete PortSet;
		PortSet = NULL;
	}

	if (m_controlObj)
	{
		delete m_controlObj;
		m_controlObj = NULL;
	}

	if (m_controlObj2)
	{
		delete m_controlObj2;
		m_controlObj2 = NULL;
	}
}

void SetPortFrm::on_btn_Exit_clicked()
{
	close();
}

void SetPortFrm::on_btn_Save_clicked()
{
	WriteIni();
	QMessageBox::information(this, tr("OK"), tr("Save Successful !"));
}

void SetPortFrm::initControlCom()
{
	ComInfoStruct valveStruct = m_readComConfig.ReadValveConfig();
	m_controlObj = new ControlComObject();
	m_controlObj->setProtocolVersion(gui.cBox_Version->currentIndex()); //设置协议版本
	m_controlObj->openControlCom(&valveStruct);
}

void SetPortFrm::initControlCom2()
{
	ComInfoStruct valveStruct2 = m_readComConfig.ReadValveConfig2();
	m_controlObj2 = new ControlComObject();
	m_controlObj2->setProtocolVersion(gui.cBox_Version->currentIndex()); //设置协议版本
	m_controlObj2->openControlCom(&valveStruct2);
}

void SetPortFrm::on_btnRoutine_1_clicked()
{
	m_valveStatus[1] = !m_valveStatus[1];
	m_controlObj->askControlRelay(1, m_valveStatus[1]);
}

void SetPortFrm::on_btnRoutine_2_clicked()
{
	m_valveStatus[2] = !m_valveStatus[2];
	m_controlObj->askControlRelay(2, m_valveStatus[2]);
}

void SetPortFrm::on_btnRoutine_3_clicked()
{
	m_valveStatus[3] = !m_valveStatus[3];
	m_controlObj->askControlRelay(3, m_valveStatus[3]);
}

void SetPortFrm::on_btnRoutine_4_clicked()
{
	m_valveStatus[4] = !m_valveStatus[4];
	m_controlObj->askControlRelay(4, m_valveStatus[4]);
}

void SetPortFrm::on_btnRoutine_5_clicked()
{
	m_valveStatus[5] = !m_valveStatus[5];
	m_controlObj->askControlRelay(5, m_valveStatus[5]);
}

void SetPortFrm::on_btnRoutine_6_clicked()
{
	m_valveStatus[6] = !m_valveStatus[6];
	m_controlObj->askControlRelay(6, m_valveStatus[6]);
}

void SetPortFrm::on_btnRoutine_7_clicked()
{
	m_valveStatus[7] = !m_valveStatus[7];
	m_controlObj->askControlRelay(7, m_valveStatus[7]);
}

void SetPortFrm::on_btnRoutine_8_clicked()
{
	m_valveStatus[8] = !m_valveStatus[8];
	m_controlObj->askControlRelay(8, m_valveStatus[8]);
}

void SetPortFrm::on_btnRoutine_9_clicked()
{
	m_valveStatus[9] = !m_valveStatus[9];
	m_controlObj2->askControlRelay(1, m_valveStatus[9]);
}

void SetPortFrm::on_btnRoutine_10_clicked()
{
	m_valveStatus[10] = !m_valveStatus[10];
	m_controlObj2->askControlRelay(2, m_valveStatus[10]);
}

void SetPortFrm::on_btnRoutine_11_clicked()
{
	m_valveStatus[11] = !m_valveStatus[11];
	m_controlObj2->askControlRelay(3, m_valveStatus[11]);
}

void SetPortFrm::on_btnRoutine_12_clicked()
{
	m_valveStatus[12] = !m_valveStatus[12];
	m_controlObj2->askControlRelay(4, m_valveStatus[12]);
}

void SetPortFrm::on_btnRoutine_13_clicked()
{
	m_valveStatus[13] = !m_valveStatus[13];
	m_controlObj2->askControlRelay(5, m_valveStatus[13]);
}

void SetPortFrm::on_btnRoutine_14_clicked()
{
	m_valveStatus[14] = !m_valveStatus[14];
	m_controlObj2->askControlRelay(6, m_valveStatus[14]);
}

void SetPortFrm::on_btnRoutine_15_clicked()
{
	m_valveStatus[15] = !m_valveStatus[15];
	m_controlObj2->askControlRelay(7, m_valveStatus[15]);
}

void SetPortFrm::on_btnRoutine_16_clicked()
{
	m_valveStatus[16] = !m_valveStatus[16];
	m_controlObj->askControlRelay(8, m_valveStatus[16]);
}

void SetPortFrm::initValveStatus()
{
	//端口号-阀门状态 全部阀门状态为关闭
	m_valveStatus[1] = VALVE_CLOSE;
	m_valveStatus[2] = VALVE_CLOSE;
	m_valveStatus[3] = VALVE_CLOSE;
	m_valveStatus[4] = VALVE_CLOSE;
	m_valveStatus[5] = VALVE_CLOSE;
	m_valveStatus[6] = VALVE_CLOSE;
	m_valveStatus[7] = VALVE_CLOSE;
	m_valveStatus[8] = VALVE_CLOSE;

	m_valveStatus[9] = VALVE_CLOSE;
	m_valveStatus[10] = VALVE_CLOSE;
	m_valveStatus[11] = VALVE_CLOSE;
	m_valveStatus[12] = VALVE_CLOSE;
	m_valveStatus[13] = VALVE_CLOSE;
	m_valveStatus[14] = VALVE_CLOSE;
	m_valveStatus[15] = VALVE_CLOSE;
	m_valveStatus[16] = VALVE_CLOSE;
}

/*
** 读取上次的端口配置
*/
void SetPortFrm::InstallIni()
{
	gui.cBox_Inlet->setCurrentIndex(PortSet->value("Relay/waterInNo").toInt() - 1);
	gui.cBox_Outlet->setCurrentIndex(PortSet->value("Relay/waterOutNo").toInt() - 1);
	gui.cBox_Rate_1->setCurrentIndex(PortSet->value("Relay/bigNo").toInt() - 1);
	gui.cBox_Rate_2->setCurrentIndex(PortSet->value("Relay/middle2No").toInt() - 1);
	gui.cBox_Rate_3->setCurrentIndex(PortSet->value("Relay/middle1No").toInt() - 1);
	gui.cBox_Rate_4->setCurrentIndex(PortSet->value("Relay/smallNo").toInt() - 1);
	gui.cBox_BigInlet->setCurrentIndex(PortSet->value("Relay/bigWaterInNo").toInt() - 1);
	gui.cBox_BigOutlet->setCurrentIndex(PortSet->value("Relay/bigWaterOutNo").toInt() - 1);
	gui.cBox_SmallInlet->setCurrentIndex(PortSet->value("Relay/smallWaterInNo").toInt() - 1);
	gui.cBox_SmallOutlet->setCurrentIndex(PortSet->value("Relay/smallWaterOutNo").toInt() - 1);

	gui.cBox_Pump->setCurrentIndex(PortSet->value("Regulate/pumpNo").toInt() - 1);
	gui.cBox_Regulate_Big->setCurrentIndex(PortSet->value("Regulate/bigNo").toInt() - 1);
	gui.cBox_Regulate_Mid2->setCurrentIndex(PortSet->value("Regulate/middle2No").toInt() - 1);
	gui.cBox_Regulate_Mid1->setCurrentIndex(PortSet->value("Regulate/middle1No").toInt() - 1);
	gui.cBox_Regulate_Small->setCurrentIndex(PortSet->value("Regulate/smallNo").toInt() - 1);

	gui.cBox_Version->setCurrentIndex(PortSet->value("CtrlBoard/version").toInt());
}

/*
** 保存端口配置
*/
void SetPortFrm::WriteIni()
{
	PortSet->setValue("Relay/waterInNo",gui.cBox_Inlet->currentIndex() + 1);
	PortSet->setValue("Relay/waterOutNo",gui.cBox_Outlet->currentIndex() + 1);
	PortSet->setValue("Relay/bigNo",gui.cBox_Rate_1->currentIndex() + 1);
	PortSet->setValue("Relay/middle2No",gui.cBox_Rate_2->currentIndex() + 1);
	PortSet->setValue("Relay/middle1No",gui.cBox_Rate_3->currentIndex() + 1);
	PortSet->setValue("Relay/smallNo",gui.cBox_Rate_4->currentIndex() + 1);
	PortSet->setValue("Relay/bigWaterInNo",gui.cBox_BigInlet->currentIndex() + 1);
	PortSet->setValue("Relay/bigWaterOutNo",gui.cBox_BigOutlet->currentIndex() + 1);
	PortSet->setValue("Relay/smallWaterInNo",gui.cBox_SmallInlet->currentIndex() + 1);
	PortSet->setValue("Relay/smallWaterOutNo",gui.cBox_SmallOutlet->currentIndex() + 1);

	PortSet->setValue("Regulate/pumpNo",gui.cBox_Pump->currentIndex() + 1);
	PortSet->setValue("Regulate/bigNo",gui.cBox_Regulate_Big->currentIndex() + 1);
	PortSet->setValue("Regulate/middle2No",gui.cBox_Regulate_Mid2->currentIndex() + 1);
	PortSet->setValue("Regulate/middle1No",gui.cBox_Regulate_Mid1->currentIndex() + 1);
	PortSet->setValue("Regulate/smallNo",gui.cBox_Regulate_Small->currentIndex() + 1);
	
	PortSet->setValue("CtrlBoard/version",gui.cBox_Version->currentIndex());
}