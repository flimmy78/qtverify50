/***********************************************
**  �ļ���:     ReadComConfig.cpp
**  ����:       ��ȡ $ADEHOME/ini/comconfig.xml�ļ��Ĵ�������
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2014/6/16
**  רҵ��:     ��³���������
**  ���������: �α���
**  ����Ա:     �α���
**  �汾��ʷ:   2014/06 ��һ��
**  ���ݰ���:
**  ˵��:
**  ���¼�¼:   2015/11���˫��ƽ��˫���ư�����
***********************************************/
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QFile>
#include <QtXml/QtXml>
#include <iostream>

#include "readcomconfig.h"
#include "algorithm.h"
#include "commondefine.h"

ReadComConfig::ReadComConfig()
{
	m_com_settings = new QSettings(getFullIniFileName("comconfig.ini"), QSettings::IniFormat);
}

ReadComConfig::~ReadComConfig()
{
	if (NULL != m_com_settings)
	{
		delete m_com_settings;
		m_com_settings = NULL;
	}
}

//��ȡ��������
ComInfoStruct ReadComConfig::ReadValveConfig()
{
	return ReadConfigByName("Valve");
}

//��ȡ��������2
ComInfoStruct ReadComConfig::ReadValveConfig2()
{
	return ReadConfigByName("Valve_2");
}

//��ȡ��ƽ����
ComInfoStruct ReadComConfig::ReadBalanceConfig()
{
	return ReadConfigByName("Balance");
}

//��ȡ��ƽ����2
ComInfoStruct ReadComConfig::ReadBalanceConfig2()
{
	return ReadConfigByName("Balance_2");
}

//��ȡ�¶Ȳɼ�����
ComInfoStruct ReadComConfig::ReadTempConfig()
{
	return ReadConfigByName("TempSenor");
}

//��ȡ��׼�¶ȼ�����
ComInfoStruct ReadComConfig::ReadStdTempConfig()
{
	return ReadConfigByName("StdTmpSensor");
}

//��ȡ˲ʱ������׼������
ComInfoStruct ReadComConfig::ReadInstStdConfig()
{
	return ReadConfigByName("InstStd");
}

//��ȡ�ۻ�������׼������
ComInfoStruct ReadComConfig::ReadAccumStdConfig()
{
	return ReadConfigByName("AccumStd");
}

ComInfoStruct ReadComConfig::ReadMeterConfigByNum(int MeterNum)
{
	return ReadConfigByName(meter(MeterNum));
}

//��xml�е�id��ȡ����
ComInfoStruct ReadComConfig::ReadConfigByName(QString ConfigId)
{
	ComInfoStruct com_info;

	//�ж��Ƿ��ȡ���������
	if (ConfigId.contains(METER_STR))
	{
		m_com_settings->beginGroup("MetersProperty");
		com_info.baudRate = m_com_settings->value("baud").toString().split(SEP)[0].toInt();
		com_info.dataBit  = m_com_settings->value("bits").toString().split(SEP)[0].toInt();
		com_info.parity   = m_com_settings->value("chkbit").toString().split(SEP)[0].toInt();
		com_info.stopBit  = m_com_settings->value("endbit").toString().split(SEP)[0].toInt();
		m_com_settings->endGroup();

		m_com_settings->beginGroup("MetersCom");
		com_info.portName = "COM" + m_com_settings->value(ConfigId).toString();
		m_com_settings->endGroup();
	}
	else
	{
		m_com_settings->beginGroup(ConfigId);
		com_info.portName = m_com_settings->value("com_name").toString().split(SEP)[0];
		com_info.baudRate = m_com_settings->value("baud").toString().split(SEP)[0].toInt();
		com_info.dataBit  = m_com_settings->value("bits").toString().split(SEP)[0].toInt();
		com_info.parity   = m_com_settings->value("chkbit").toString().split(SEP)[0].toInt();
		com_info.stopBit  = m_com_settings->value("endbit").toString().split(SEP)[0].toInt();
		m_com_settings->endGroup();
	}

	return com_info;
}

QStringList ReadComConfig::ReadIndexByName(QString ConfigId)
{
	QStringList com_info;
	if (ConfigId.contains(METER_STR))
	{
		m_com_settings->beginGroup("MetersCom");
		com_info.append( QString::number(m_com_settings->value(ConfigId).toInt()-1));
		m_com_settings->endGroup();

		m_com_settings->beginGroup("MetersProperty");
		com_info.append(m_com_settings->value("baud").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("bits").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("chkbit").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("endbit").toString().split(SEP)[1]);
		m_com_settings->endGroup();
	}
	else
	{
		m_com_settings->beginGroup(ConfigId);
		com_info.append(m_com_settings->value("com_name").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("baud").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("bits").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("chkbit").toString().split(SEP)[1]);
		com_info.append(m_com_settings->value("endbit").toString().split(SEP)[1]);
		m_com_settings->endGroup();
	}

	return com_info;
}

/*
**  ���ݶ˿ںŷ��ض�Ӧ�ı�λ��
**  ����: QString comName, �˿�����,����"COM1"
**  ����ֵ:����������ļ��гɹ����ҵ���λ��, �򷵻ظñ�λ��;������Ҵ˱�λ��ʧ��,�򷵻�-1
*/
int ReadComConfig::getMeterPosByComName(QString comName)
{
		ComInfoStruct meterConfigSTR;
		for (int i=1; i <= METER_QUANTITY; i++)
		{
			meterConfigSTR = ReadMeterConfigByNum(i);
			if (meterConfigSTR.portName == comName)			
				return i;			
		}
		return -1;
}

/*
** ��ȡ��ƽ����
** ���ڲ�����
    maxWht: ��ƽ������ ��kg��
	bottomWht����ƽ��ˮ���� (kg��
*/
void ReadComConfig::getBalancePara(float &maxWht, float &bottomWht)
{
	maxWht = m_com_settings->value("BalanceType/maxweight").toFloat();
	bottomWht = m_com_settings->value("BalanceType/bottomwht").toFloat();
}

/*
** ��ȡ��ƽ����
*/
int ReadComConfig::getBalanceType()
{
	int type = m_com_settings->value("BalanceType/type").toInt();
	return type;
}

/*
** ��ȡ��ƽ����2
** ���ڲ�����
    maxWht: ��ƽ������ ��kg��
	bottomWht����ƽ��ˮ���� (kg��
*/
void ReadComConfig::getBalancePara2(float &maxWht, float &bottomWht)
{
	maxWht = m_com_settings->value("BalanceType_2/maxweight").toFloat();
	bottomWht = m_com_settings->value("BalanceType_2/bottomwht").toFloat();
}

/*
** ��ȡ��ƽ����
*/
int ReadComConfig::getBalanceType2()
{
	int type = m_com_settings->value("BalanceType_2/type").toInt();
	return type;
}

/*
** ��ȡװ��ID
*/
int ReadComConfig::getDeviceInfoID()
{
	int deviceID = m_com_settings->value("deviceInfo/deviceInfoID").toInt();
	return deviceID;
}
