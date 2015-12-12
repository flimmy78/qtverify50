/***********************************************
**  文件名:     SetComFrm.cpp
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
**  更新记录:   2015-5增加设置天平类型、天平容量、天平底量(杨深)
***********************************************/

#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QFile>
#include <QtXml/QtXml>
#include <iostream>

#include "basedef.h"
#include "algorithm.h"
#include "setcomdlg.h"
#include "commondefine.h"
#include "readcomconfig.h"
#include "qtexdb.h"

SetComDlg::SetComDlg(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	gui.setupUi(this);

	btnGroupBalanceType = new QButtonGroup(gui.grpBoxBalanceType); //天平类型
	btnGroupBalanceType->addButton(gui.radioBtnSatorius, 0);
	btnGroupBalanceType->addButton(gui.radioBtnSatoriusH, 1);
	btnGroupBalanceType->addButton(gui.radioBtnBizerba, 2);
	btnGroupBalanceType->addButton(gui.radioBtnMettler, 3);

	btnGroupBalanceType2 = new QButtonGroup(gui.grpBoxBalanceType_2); //天平类型2
	btnGroupBalanceType2->addButton(gui.radioBtnSatorius_2, 0);
	btnGroupBalanceType2->addButton(gui.radioBtnSatoriusH_2, 1);
	btnGroupBalanceType2->addButton(gui.radioBtnBizerba_2, 2);
	btnGroupBalanceType2->addButton(gui.radioBtnMettler_2, 3);
}

SetComDlg::~SetComDlg()
{
	qDebug()<<"SetComDlg ~ destructor ";
	if (btnGroupBalanceType)
	{
		delete btnGroupBalanceType;
		btnGroupBalanceType = NULL;
	}
}

void SetComDlg::showEvent(QShowEvent *)
{
	m_config = new ReadComConfig();
	m_com_settings = new QSettings(getFullIniFileName("comconfig.ini"), QSettings::IniFormat);
	InstallConfigs();

	m_model = new QSqlTableModel(this, g_defaultdb);
	m_model->setTable("T_Meter_Standard");
	m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Standard"));
	m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Quantity"));
	m_model->select();
	gui.tableView->setModel(m_model);
	gui.tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
	gui.tableView->hideColumn(0);
}

void SetComDlg::closeEvent(QCloseEvent *)
{
	if (m_config)
	{
		delete m_config;
		m_config=NULL;
	}

	if (m_com_settings)
	{
		delete m_com_settings;
		m_com_settings = NULL;
	}

	if (m_model)
	{
		delete m_model;
		m_model = NULL;
	}
}

void SetComDlg::on_btnExit_clicked()
{
	this->close();
}

void SetComDlg::on_btnSave_clicked()
{
	WriteValveConfig();
	WriteValveConfig2();
	WriteBalanceConfig();
	WriteBalanceConfig2();
	WriteBalanceTypeConfig();
	WriteBalanceTypeConfig2();
	WriteTempConfig();
	WriteStdTempConfig();
	WriteInstStdConfig();
	WriteAccumStdConfig();
	WriteMetersConfig();
	QMessageBox::information(this, tr("OK"), tr("Save Settings Successfully !"));
}

void SetComDlg::InstallConfigs()
{
	InstallValeConfig();
	InstallValeConfig2();
	InstallBalanceConfig();
	InstallBalanceConfig2();
	InstallBalanceTypeConfig();
	InstallBalanceTypeConfig2();
	InstallTempConfig();
	InstallStdtmpConfig();
	InstallInstStdConfig();
	InstallAccumStdConfig();
	InstallMetersConfig();
}

void SetComDlg::InstallValeConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("Valve");
	gui.comboValveSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboValveBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboValveBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboValveChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboValveEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallValeConfig2()
{
	QStringList valve_index_2 = m_config->ReadIndexByName("Valve_2");
	gui.comboValveSerialNum_2->setCurrentIndex(valve_index_2[0].toInt());
	gui.comboValveBaudRate_2->setCurrentIndex(valve_index_2[1].toInt());
	gui.comboValveBits_2->setCurrentIndex(valve_index_2[2].toInt());
	gui.comboValveChkBit_2->setCurrentIndex(valve_index_2[3].toInt());
	gui.comboValveEndBit_2->setCurrentIndex(valve_index_2[4].toInt());
}

void SetComDlg::InstallBalanceConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("Balance");
	gui.comboBalSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboBalBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboBalBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboBalChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboBalEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallBalanceConfig2()
{
	QStringList valve_index_2 = m_config->ReadIndexByName("Balance_2");
	gui.comboBalSerialNum_2->setCurrentIndex(valve_index_2[0].toInt());
	gui.comboBalBaudRate_2->setCurrentIndex(valve_index_2[1].toInt());
	gui.comboBalBits_2->setCurrentIndex(valve_index_2[2].toInt());
	gui.comboBalChkBit_2->setCurrentIndex(valve_index_2[3].toInt());
	gui.comboBalEndBit_2->setCurrentIndex(valve_index_2[4].toInt());
}

void SetComDlg::InstallBalanceTypeConfig()
{
	int type = m_com_settings->value("BalanceType/type").toInt();
	QString maxWht = m_com_settings->value("BalanceType/maxweight").toString();
	QString bottomWht = m_com_settings->value("BalanceType/bottomwht").toString();

	switch (type)
	{
	case 0:
		gui.radioBtnSatorius->setChecked(true);
		break;
	case 1:
		gui.radioBtnSatoriusH->setChecked(true);
		break;
	case 2:
		gui.radioBtnBizerba->setChecked(true);
		break;
	case 3:
		gui.radioBtnMettler->setChecked(true);
		break;
	default:
		gui.radioBtnSatorius->setChecked(true);
		break;
	}
	gui.lnEditMaxWht->setText(maxWht);
	gui.lnEditBottomWht->setText(bottomWht);
}

void SetComDlg::InstallBalanceTypeConfig2()
{
	int type = m_com_settings->value("BalanceType_2/type").toInt();
	QString maxWht = m_com_settings->value("BalanceType_2/maxweight").toString();
	QString bottomWht = m_com_settings->value("BalanceType_2/bottomwht").toString();

	switch (type)
	{
	case 0:
		gui.radioBtnSatorius_2->setChecked(true);
		break;
	case 1:
		gui.radioBtnSatoriusH_2->setChecked(true);
		break;
	case 2:
		gui.radioBtnBizerba_2->setChecked(true);
		break;
	case 3:
		gui.radioBtnMettler_2->setChecked(true);
		break;
	default:
		gui.radioBtnSatorius_2->setChecked(true);
		break;
	}
	gui.lnEditMaxWht_2->setText(maxWht);
	gui.lnEditBottomWht_2->setText(bottomWht);
}

void SetComDlg::InstallTempConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("TempSenor");
	gui.comboTempSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboTempBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboTempBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboTempChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboTempEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallStdtmpConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("StdTmpSensor");
	gui.comboStdTmpSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboStdTmpBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboStdTmpBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboStdTmpChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboStdTmpEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallInstStdConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("InstStd");
	gui.comboInstStdSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboInstStdBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboInstStdBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboInstStdChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboInstStdEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallAccumStdConfig()
{
	QStringList valve_index = m_config->ReadIndexByName("AccumStd");
	gui.comboAccumStdSerialNum->setCurrentIndex(valve_index[0].toInt());
	gui.comboAccumStdBaudRate->setCurrentIndex(valve_index[1].toInt());
	gui.comboAccumStdBits->setCurrentIndex(valve_index[2].toInt());
	gui.comboAccumStdChkBit->setCurrentIndex(valve_index[3].toInt());
	gui.comboAccumStdEndBit->setCurrentIndex(valve_index[4].toInt());
}

void SetComDlg::InstallMetersConfig()
{
	m_com_settings->beginGroup("MetersProperty");
	gui.comboMeterBaudRate->setCurrentIndex(m_com_settings->value("baud").toString().split(SEP)[1].toInt());	
	gui.comboMeterBits->setCurrentIndex(m_com_settings->value("bits").toString().split(SEP)[1].toInt());	
	gui.comboMeterChkBit->setCurrentIndex(m_com_settings->value("chkbit").toString().split(SEP)[1].toInt());	
	gui.comboMeterEndBit->setCurrentIndex(m_com_settings->value("endbit").toString().split(SEP)[1].toInt());
	m_com_settings->endGroup();

	m_com_settings->beginGroup("MetersCom");
	gui.spinBox_Meter_1->setValue(m_com_settings->value(meter(1)).toInt());
	gui.spinBox_Meter_2->setValue(m_com_settings->value(meter(2)).toInt());
	gui.spinBox_Meter_3->setValue(m_com_settings->value(meter(3)).toInt());
	gui.spinBox_Meter_4->setValue(m_com_settings->value(meter(4)).toInt());
	gui.spinBox_Meter_5->setValue(m_com_settings->value(meter(5)).toInt());
	gui.spinBox_Meter_6->setValue(m_com_settings->value(meter(6)).toInt());
	gui.spinBox_Meter_7->setValue(m_com_settings->value(meter(7)).toInt());
	gui.spinBox_Meter_8->setValue(m_com_settings->value(meter(8)).toInt());
	gui.spinBox_Meter_9->setValue(m_com_settings->value(meter(9)).toInt());
	gui.spinBox_Meter_10->setValue(m_com_settings->value(meter(10)).toInt());
	gui.spinBox_Meter_11->setValue(m_com_settings->value(meter(11)).toInt());
	gui.spinBox_Meter_12->setValue(m_com_settings->value(meter(12)).toInt());
	gui.spinBox_Meter_13->setValue(m_com_settings->value(meter(13)).toInt());
	gui.spinBox_Meter_14->setValue(m_com_settings->value(meter(14)).toInt());
	gui.spinBox_Meter_15->setValue(m_com_settings->value(meter(15)).toInt());
	gui.spinBox_Meter_16->setValue(m_com_settings->value(meter(16)).toInt());
	gui.spinBox_Meter_17->setValue(m_com_settings->value(meter(17)).toInt());
	gui.spinBox_Meter_18->setValue(m_com_settings->value(meter(18)).toInt());
	gui.spinBox_Meter_19->setValue(m_com_settings->value(meter(19)).toInt());
	gui.spinBox_Meter_20->setValue(m_com_settings->value(meter(20)).toInt());
	gui.spinBox_Meter_21->setValue(m_com_settings->value(meter(21)).toInt());
	gui.spinBox_Meter_22->setValue(m_com_settings->value(meter(22)).toInt());
	gui.spinBox_Meter_23->setValue(m_com_settings->value(meter(23)).toInt());
	gui.spinBox_Meter_24->setValue(m_com_settings->value(meter(24)).toInt());
	m_com_settings->endGroup();
}

//写入阀门设置
void SetComDlg::WriteValveConfig()
{
	m_com_settings->beginGroup(gui.gBoxValve->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxValve);
	m_com_settings->endGroup();
}

//写入阀门设置-第二块
void SetComDlg::WriteValveConfig2()
{
	m_com_settings->beginGroup(gui.gBoxValve_2->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxValve_2);
	m_com_settings->endGroup();
}

//写入天平设置
void SetComDlg::WriteBalanceConfig()
{
	m_com_settings->beginGroup(gui.gBoxBalance->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxBalance);
	m_com_settings->endGroup();
}

//写入天平设置-第二个天平（小天平）
void SetComDlg::WriteBalanceConfig2()
{
	m_com_settings->beginGroup(gui.gBoxBalance_2->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxBalance_2);
	m_com_settings->endGroup();
}

//写入天平类型设置
void SetComDlg::WriteBalanceTypeConfig()
{
	int type = btnGroupBalanceType->checkedId();
	QString maxWht = gui.lnEditMaxWht->text();
	QString bottomWht = gui.lnEditBottomWht->text();

	m_com_settings->setValue("BalanceType/type", type);
	m_com_settings->setValue("BalanceType/maxweight", maxWht);
	m_com_settings->setValue("BalanceType/bottomwht", bottomWht);
}

//写入天平类型设置2
void SetComDlg::WriteBalanceTypeConfig2()
{
	int type = btnGroupBalanceType2->checkedId();
	QString maxWht = gui.lnEditMaxWht_2->text();
	QString bottomWht = gui.lnEditBottomWht_2->text();

	m_com_settings->setValue("BalanceType_2/type", type);
	m_com_settings->setValue("BalanceType_2/maxweight", maxWht);
	m_com_settings->setValue("BalanceType_2/bottomwht", bottomWht);
}

//写入温度采集设置
void SetComDlg::WriteTempConfig()
{
	m_com_settings->beginGroup(gui.gBoxTempSenor->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxTempSenor);
	m_com_settings->endGroup();
}

//写入标准温度计设置
void SetComDlg::WriteStdTempConfig()
{
	m_com_settings->beginGroup(gui.gBoxStdTmpSensor->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxStdTmpSensor);
	m_com_settings->endGroup();
}

//写入采集瞬时流量模块配置
void SetComDlg::WriteInstStdConfig()
{
	m_com_settings->beginGroup(gui.gBoxInstStd->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxInstStd);
	m_com_settings->endGroup();
}

//写入采集累积流量模块配置
void SetComDlg::WriteAccumStdConfig()
{
	m_com_settings->beginGroup(gui.gBoxAccumStd->objectName().split("gBox")[1]);
	WriteConfigById(gui.gBoxAccumStd);
	m_com_settings->endGroup();
}

//写入被检表设置
void SetComDlg::WriteMetersConfig()
{
	QString baud_rate = gui.comboMeterBaudRate->currentText()+ SEP + QString::number(gui.comboMeterBaudRate->currentIndex(), 10);	
	QString data_bits = gui.comboMeterBits->currentText()+ SEP + QString::number(gui.comboMeterBits->currentIndex(), 10);	
	QString chk_bit = QString::number(gui.comboMeterChkBit->currentIndex(), 10)+ SEP + QString::number(gui.comboMeterChkBit->currentIndex(), 10);	
	QString stop_bit = QString::number(gui.comboMeterEndBit->currentIndex(), 10)+ SEP + QString::number(gui.comboMeterEndBit->currentIndex(), 10);

	m_com_settings->beginGroup("MetersProperty");
	m_com_settings->setValue("baud", baud_rate);
	m_com_settings->setValue("bits", data_bits);
	m_com_settings->setValue("chkbit", chk_bit);
	m_com_settings->setValue("endbit", stop_bit);
	m_com_settings->endGroup();

	m_com_settings->beginGroup("MetersCom");
	m_com_settings->setValue(meter(1),gui.spinBox_Meter_1->value());
	m_com_settings->setValue(meter(2),gui.spinBox_Meter_2->value());
	m_com_settings->setValue(meter(3),gui.spinBox_Meter_3->value());
	m_com_settings->setValue(meter(4),gui.spinBox_Meter_4->value());
	m_com_settings->setValue(meter(5),gui.spinBox_Meter_5->value());
	m_com_settings->setValue(meter(6),gui.spinBox_Meter_6->value());
	m_com_settings->setValue(meter(7),gui.spinBox_Meter_7->value());
	m_com_settings->setValue(meter(8),gui.spinBox_Meter_8->value());
	m_com_settings->setValue(meter(9),gui.spinBox_Meter_9->value());
	m_com_settings->setValue(meter(10),gui.spinBox_Meter_10->value());
	m_com_settings->setValue(meter(11),gui.spinBox_Meter_11->value());
	m_com_settings->setValue(meter(12),gui.spinBox_Meter_12->value());
	m_com_settings->setValue(meter(13),gui.spinBox_Meter_13->value());
	m_com_settings->setValue(meter(14),gui.spinBox_Meter_14->value());
	m_com_settings->setValue(meter(15),gui.spinBox_Meter_15->value());
	m_com_settings->setValue(meter(16),gui.spinBox_Meter_16->value());
	m_com_settings->setValue(meter(17),gui.spinBox_Meter_17->value());
	m_com_settings->setValue(meter(18),gui.spinBox_Meter_18->value());
	m_com_settings->setValue(meter(19),gui.spinBox_Meter_19->value());
	m_com_settings->setValue(meter(20),gui.spinBox_Meter_20->value());
	m_com_settings->setValue(meter(21),gui.spinBox_Meter_21->value());
	m_com_settings->setValue(meter(22),gui.spinBox_Meter_22->value());
	m_com_settings->setValue(meter(23),gui.spinBox_Meter_23->value());
	m_com_settings->setValue(meter(24),gui.spinBox_Meter_24->value());
	m_com_settings->endGroup();
}

void SetComDlg::WriteConfigById(QGroupBox *gBox)
{
	QString obj_name = gBox->objectName();
	QString com_name ;
	QString baud_rate ;
	QString data_bits ;
	QString chk_bit ;
	QString stop_bit ;
	const QObjectList list=gBox->children();
	foreach(QObject* obj, list)
	{
		QString class_name = QString::fromAscii( obj->metaObject()->className() );
		if(class_name=="QComboBox")
		{
			QComboBox *CBox=(QComboBox*)obj;
			QString object_name = CBox->objectName();

			if (bool occur = object_name.contains("SerialNum",Qt::CaseSensitive))
			{
				com_name = CBox->currentText() + SEP + QString::number(CBox->currentIndex(), 10);
			}
			else if (occur = object_name.contains("BaudRate",Qt::CaseSensitive))
			{
				baud_rate = CBox->currentText()+ SEP + QString::number(CBox->currentIndex(), 10);
			}
			else if (occur = object_name.contains("Bits",Qt::CaseSensitive))
			{
				data_bits = CBox->currentText()+ SEP + QString::number(CBox->currentIndex(), 10);
			}
			else if (occur = object_name.contains("ChkBit",Qt::CaseSensitive))
			{
				chk_bit = QString::number(CBox->currentIndex(), 10)+ SEP + QString::number(CBox->currentIndex(), 10);
			}
			else if (occur = object_name.contains("EndBit",Qt::CaseSensitive))
			{
				stop_bit = QString::number(CBox->currentIndex(), 10)+ SEP + QString::number(CBox->currentIndex(), 10);
			}
		}
	}
	
	m_com_settings->setValue("com_name",com_name);
	m_com_settings->setValue("baud", baud_rate);
	m_com_settings->setValue("bits", data_bits);
	m_com_settings->setValue("chkbit", chk_bit);
	m_com_settings->setValue("endbit", stop_bit);
}