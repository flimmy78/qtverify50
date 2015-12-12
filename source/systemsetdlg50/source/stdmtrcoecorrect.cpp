/***********************************************
**  文件名:     stdcoecorrect.cpp
**  功能:       标准表系数标定
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

#include "stdmtrcoecorrect.h"

StdMtrCoeCorrect::StdMtrCoeCorrect(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"StdCoeCorrect thread:"<<QThread::currentThreadId();
	ui.setupUi(this);

	ui.tableWidget->setRowCount(18);
	ui.tableWidget->verticalHeader()->setVisible(false);
	for (int i=0; i<ui.tableWidget->rowCount(); i+=3)
	{
		ui.tableWidget->setSpan(i,0,3,1);
		ui.tableWidget->setSpan(i,8,3,1);
		ui.tableWidget->setSpan(i,9,3,1);
	}
}

StdMtrCoeCorrect::~StdMtrCoeCorrect()
{
	qDebug()<<"!!! StdCoeCorrect destructor";
}

void StdMtrCoeCorrect::on_btnExit_clicked()
{
	this->close();
}

void StdMtrCoeCorrect::resizeEvent(QResizeEvent * event)
{
	qDebug()<<"StdMtrCoeCorrect::resizeEvent...";

	int th = ui.tableWidget->size().height();
	int tw = ui.tableWidget->size().width();
	int hh = ui.tableWidget->horizontalHeader()->size().height();
	int vw = ui.tableWidget->verticalHeader()->size().width();
	int vSize = (int)((th-hh-10)/ui.tableWidget->rowCount());
	int hSize = (int)((tw-vw-10)/ui.tableWidget->columnCount());
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(vSize);
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(hSize);
}
