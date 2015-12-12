/***********************************************
**  �ļ���:     stdcoecorrect.cpp
**  ����:       ��׼��ϵ���궨
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2015/3/5
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2015/03 ��һ��
**  ���ݰ���:
**  ˵��:
**  ���¼�¼:
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
