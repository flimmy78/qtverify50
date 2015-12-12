/***********************************************
**  文件名:     ReadComConfig.cpp
**  功能:       读取 $ADEHOME/ini/comconfig.xml文件的串口配置
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2014/6/16
**  专业组:     德鲁计量软件组
**  程序设计者: 宋宝善
**  程序员:     宋宝善
**  版本历史:   2014/06 第一版
**  内容包含:
**  说明:
**  更新记录:   2015/11针对双天平、双控制板配置
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

//读取阀门设置
ComInfoStruct ReadComConfig::ReadValveConfig()
{
	return ReadConfigByName("Valve");
}

//读取阀门设置2
ComInfoStruct ReadComConfig::ReadValveConfig2()
{
	return ReadConfigByName("Valve_2");
}

//读取天平设置
ComInfoStruct ReadComConfig::ReadBalanceConfig()
{
	return ReadConfigByName("Balance");
}

//读取天平设置2
ComInfoStruct ReadComConfig::ReadBalanceConfig2()
{
	return ReadConfigByName("Balance_2");
}

//读取温度采集设置
ComInfoStruct ReadComConfig::ReadTempConfig()
{
	return ReadConfigByName("TempSenor");
}

//读取标准温度计设置
ComInfoStruct ReadComConfig::ReadStdTempConfig()
{
	return ReadConfigByName("StdTmpSensor");
}

//读取瞬时流量标准表设置
ComInfoStruct ReadComConfig::ReadInstStdConfig()
{
	return ReadConfigByName("InstStd");
}

//读取累积流量标准表设置
ComInfoStruct ReadComConfig::ReadAccumStdConfig()
{
	return ReadConfigByName("AccumStd");
}

ComInfoStruct ReadComConfig::ReadMeterConfigByNum(int MeterNum)
{
	return ReadConfigByName(meter(MeterNum));
}

//按xml中的id读取设置
ComInfoStruct ReadComConfig::ReadConfigByName(QString ConfigId)
{
	ComInfoStruct com_info;

	//判断是否读取被检表配置
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
**  根据端口号返回对应的表位号
**  参数: QString comName, 端口名称,例如"COM1"
**  返回值:如果在配置文件中成功查找到表位号, 则返回该表位号;如果查找此表位号失败,则返回-1
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
** 获取天平参数
** 出口参数：
    maxWht: 天平最大称量 （kg）
	bottomWht：天平回水底量 (kg）
*/
void ReadComConfig::getBalancePara(float &maxWht, float &bottomWht)
{
	maxWht = m_com_settings->value("BalanceType/maxweight").toFloat();
	bottomWht = m_com_settings->value("BalanceType/bottomwht").toFloat();
}

/*
** 获取天平类型
*/
int ReadComConfig::getBalanceType()
{
	int type = m_com_settings->value("BalanceType/type").toInt();
	return type;
}

/*
** 获取天平参数2
** 出口参数：
    maxWht: 天平最大称量 （kg）
	bottomWht：天平回水底量 (kg）
*/
void ReadComConfig::getBalancePara2(float &maxWht, float &bottomWht)
{
	maxWht = m_com_settings->value("BalanceType_2/maxweight").toFloat();
	bottomWht = m_com_settings->value("BalanceType_2/bottomwht").toFloat();
}

/*
** 获取天平类型
*/
int ReadComConfig::getBalanceType2()
{
	int type = m_com_settings->value("BalanceType_2/type").toInt();
	return type;
}

/*
** 获取装置ID
*/
int ReadComConfig::getDeviceInfoID()
{
	int deviceID = m_com_settings->value("deviceInfo/deviceInfoID").toInt();
	return deviceID;
}
