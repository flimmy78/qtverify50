/***********************************************
**  文件名:     stdparaset.cpp
**  功能:       标准表参数设定
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2015/3/5
**  专业组:     德鲁计量软件组
**  程序设计者: YS
**  程序员:     YS
**  版本历史:   2015/03 第一版
**  内容包含:
**  说明:
**  更新记录:
***********************************************/

#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QTextCodec>
#include <QThread>
#include <QtGui/QMessageBox>
#include <QtCore/QProcessEnvironment>

#include "algorithm.h"
#include "stdmtrparaset.h"

StdMtrParaSet::StdMtrParaSet(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"StdParaSet thread:"<<QThread::currentThreadId();
	ui.setupUi(this);	
}

void StdMtrParaSet::showEvent(QShowEvent *)
{
	initWdgVec();
	m_stdParam = new QSettings(getFullIniFileName("stdmtrparaset.ini"),QSettings::IniFormat);
	installStdMeter();
	installRoute();
}

StdMtrParaSet::~StdMtrParaSet()
{
	qDebug()<<"!!! StdParaSet destructor";
}

void StdMtrParaSet::closeEvent(QCloseEvent *)
{
	if (m_stdParam)
	{
		delete m_stdParam;
		m_stdParam = NULL;
	}
}

void StdMtrParaSet::initWdgVec()
{
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_1);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_2);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_3);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_4);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_5);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_6);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_7);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_8);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_9);
	m_vecflowPointDN3.append(ui.lineEdit_DN3_FLowPoint_10);
	   
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_1);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_2);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_3);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_4);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_5);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_6);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_7);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_8);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_9);
	m_vecflowPointDN5.append(ui.lineEdit_DN10_FLowPoint_10);
	   
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_1);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_2);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_3);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_4);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_5);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_6);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_7);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_8);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_9);
	m_vecflowPointDN10.append(ui.lineEdit_DN25_FLowPoint_10);
	   
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_1);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_2);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_3);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_4);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_5);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_6);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_7);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_8);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_9);
	m_vecflowPointDN25.append(ui.lineEdit_DN50_FLowPoint_10);
	
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_1);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_2);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_3);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_4);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_5);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_6);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_7);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_8);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_9);
	m_vecKCoeDN3.append(ui.lineEdit_DN3_KCoe_10);

	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_1);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_2);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_3);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_4);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_5);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_6);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_7);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_8);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_9);
	m_vecKCoeDN10.append(ui.lineEdit_DN10_KCoe_10);
	   									
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_1);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_2);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_3);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_4);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_5);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_6);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_7);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_8);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_9);
	m_vecKCoeDN25.append(ui.lineEdit_DN25_KCoe_10);
	
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_1);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_2);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_3);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_4);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_5);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_6);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_7);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_8);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_9);
	m_vecKCoeDN5.append(ui.lineEdit_DN50_KCoe_10);

	m_vecPulse.append(ui.lineEdit_DN3_Pulse);
	m_vecPulse.append(ui.lineEdit_DN10_Pulse);
	m_vecPulse.append(ui.lineEdit_DN25_Pulse);
	m_vecPulse.append(ui.lineEdit_DN50_Pulse);
	
	m_vecUpperFlow.append(ui.lineEdit_DN3_UpperFlow);
	m_vecUpperFlow.append(ui.lineEdit_DN10_UpperFlow);
	m_vecUpperFlow.append(ui.lineEdit_DN25_UpperFlow);
	m_vecUpperFlow.append(ui.lineEdit_DN50_UpperFlow);

	
	m_vecAdjust.append(ui.lineEdit_DN3_adjust);
	m_vecAdjust.append(ui.lineEdit_DN10_adjust);
	m_vecAdjust.append(ui.lineEdit_DN25_adjust);
	m_vecAdjust.append(ui.lineEdit_DN50_adjust);

	m_vecRouteDN.append(ui.comboBox_diam_0);
	m_vecRouteDN.append(ui.comboBox_diam_1);
	m_vecRouteDN.append(ui.comboBox_diam_2);
	m_vecRouteDN.append(ui.comboBox_diam_3);
	
	m_vecInstRoute.append(ui.cBox_instroute_0);
	m_vecInstRoute.append(ui.cBox_instroute_1);
	m_vecInstRoute.append(ui.cBox_instroute_2);
	m_vecInstRoute.append(ui.cBox_instroute_3);
	
	m_vecAccumRoute.append(ui.cBox_accumroute_0);
	m_vecAccumRoute.append(ui.cBox_accumroute_1);
	m_vecAccumRoute.append(ui.cBox_accumroute_2);
	m_vecAccumRoute.append(ui.cBox_accumroute_3);
}

void StdMtrParaSet::installStdMeter()
{
	installFlowKCoe();
	installPulse();
	installFlowRate();
}

void StdMtrParaSet::installRoute()
{
	m_stdParam->beginReadArray("Route");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecRouteDN.at(i)->setCurrentIndex(m_stdParam->value("Diameter").toInt()+1);
		m_vecInstRoute.at(i)->setCurrentIndex(m_stdParam->value("InstRoute").toInt()+1);
		m_vecAccumRoute.at(i)->setCurrentIndex(m_stdParam->value("AccumRoute").toInt()+1);
	}
	m_stdParam->endArray();

	m_stdParam->beginGroup("DevNo.");
	ui.lineEdit_instDevNo->setText(m_stdParam->value("InstDevNo").toString());
	ui.lineEdit_accumDevNo->setText(m_stdParam->value("AccumDevNo").toString());
	m_stdParam->endGroup();
}

void StdMtrParaSet::installFlowKCoe()
{
	m_stdParam->beginReadArray("FlowRate");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecUpperFlow.at(i)->setText(m_stdParam->value("UpperFlow").toString());
		m_vecAdjust.at(i)->setText(m_stdParam->value("Adjust").toString());
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::installPulse()
{
	m_stdParam->beginReadArray("Pulse");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecPulse.at(i)->setText(m_stdParam->value("Pulse").toString());
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::installFlowRate()
{
	m_stdParam->beginReadArray("DN3FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecflowPointDN3.at(i)->setText(m_stdParam->value("flowPoint").toString());
		m_vecKCoeDN3.at(i)->setText(m_stdParam->value("KCoe").toString());
	}
	m_stdParam->endArray();

	m_stdParam->beginReadArray("DN10FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecflowPointDN10.at(i)->setText(m_stdParam->value("flowPoint").toString());
		m_vecKCoeDN10.at(i)->setText(m_stdParam->value("KCoe").toString());
	}
	m_stdParam->endArray();

	m_stdParam->beginReadArray("DN25FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecflowPointDN25.at(i)->setText(m_stdParam->value("flowPoint").toString());
		m_vecKCoeDN25.at(i)->setText(m_stdParam->value("KCoe").toString());
	}
	m_stdParam->endArray();

	m_stdParam->beginReadArray("DN50FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_vecflowPointDN5.at(i)->setText(m_stdParam->value("flowPoint").toString());
		m_vecKCoeDN5.at(i)->setText(m_stdParam->value("KCoe").toString());
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::writeStdMeter()
{
	writeFlowKCoe();
	writePulse();
	writeFlowRate();
}

void StdMtrParaSet::writeFlowKCoe()
{
	m_stdParam->beginWriteArray("DN3FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue("flowPoint",m_vecflowPointDN3.at(i)->text());
		m_stdParam->setValue("KCoe",m_vecKCoeDN3.at(i)->text());
	}
	m_stdParam->endArray();

	m_stdParam->beginWriteArray("DN10FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue("flowPoint",m_vecflowPointDN10.at(i)->text());
		m_stdParam->setValue("KCoe",m_vecKCoeDN10.at(i)->text());
	}
	m_stdParam->endArray();

	m_stdParam->beginWriteArray("DN25FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue("flowPoint",m_vecflowPointDN25.at(i)->text());
		m_stdParam->setValue("KCoe",m_vecKCoeDN25.at(i)->text());
	}
	m_stdParam->endArray();

	m_stdParam->beginWriteArray("DN50FlowK");
	for (int i=0;i<FLOW_K_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue("flowPoint",m_vecflowPointDN5.at(i)->text());
		m_stdParam->setValue("KCoe",m_vecKCoeDN5.at(i)->text());
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::writePulse()
{
	m_stdParam->beginWriteArray("Pulse");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue("Pulse", m_vecPulse.at(i)->text());		
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::writeFlowRate()
{
	m_stdParam->beginWriteArray("FlowRate");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue(QString("UpperFlow"), m_vecUpperFlow.at(i)->text());
		m_stdParam->setValue(QString("Adjust"), m_vecAdjust.at(i)->text());
	}
	m_stdParam->endArray();
}

void StdMtrParaSet::writeRoute()
{
	m_stdParam->beginWriteArray("Route");
	for (int i=0;i<DIAMETER_NUM;i++)
	{
		m_stdParam->setArrayIndex(i);
		m_stdParam->setValue(QString("Diameter"), m_vecRouteDN.at(i)->currentIndex()-1);
		m_stdParam->setValue(QString("InstRoute"), m_vecInstRoute.at(i)->currentIndex()-1);
		m_stdParam->setValue(QString("AccumRoute"), m_vecAccumRoute.at(i)->currentIndex()-1);
	}
	m_stdParam->endArray();

	m_stdParam->beginGroup("DevNo.");
	m_stdParam->setValue("InstDevNo", ui.lineEdit_instDevNo->text());
	m_stdParam->setValue("AccumDevNo", ui.lineEdit_accumDevNo->text());
	m_stdParam->endGroup();
}

void StdMtrParaSet::on_btnSave_clicked()
{
	writeStdMeter();
	writeRoute();
}

void StdMtrParaSet::on_btnExit_clicked()
{
	this->close();
}