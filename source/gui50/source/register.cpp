#include <QtGui/QMessageBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QTextEdit>
#include <QDebug>
#include <QProcess>
#include <QFile>

#include "register.h"
#include "md5encode.h"
#include "algorithm.h"


RegisterDlg::RegisterDlg(QString code, QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	ui.lineEditCode->setText(code);
	ui.lineEditCode->setReadOnly(true);

	//判断是否授权用户
	QString adehome = QProcessEnvironment::systemEnvironment().value("ADEHOME");
	QFile license(adehome + "\\ini\\license");
	license.open(QIODevice::ReadOnly | QIODevice::Text);
	QTextStream in(&license);
	QString regCode = in.readLine();
	license.close();
	bool licensOK = isLicenseOK(regCode);
	int validDays = 0;
	if (licensOK) //授权用户
	{
		setWindowTitle(tr("Official Version"));
		ui.lineEditLicense->setText(regCode);
		ui.lineEditLicense->setReadOnly(true);
		ui.btnProbation->setEnabled(false);
		ui.btnRegister->setEnabled(false);
		ui.label_hintinfo->setText("<font color=DarkGreen size=4><b>" + tr("Register successfully!") + "</b></font>");
	}
	else //非授权用户
	{
		QDateTime regDate = getProbationStartDate();
		if (regDate.isValid()) //已经试用过
		{
			ui.btnProbation->setEnabled(false);
			validDays = PROBATION_DAYS - regDate.daysTo(QDateTime::currentDateTime());
			if (validDays > 0)
			{
				setWindowTitle(tr("Trial Version! Probation period is %1 days").arg(validDays));
			}
			else
			{
				setWindowTitle(tr("Trial Version is over date!"));
			}
		}
	}
}

RegisterDlg::~RegisterDlg()
{
}

void RegisterDlg::closeEvent( QCloseEvent * event)
{
}

void RegisterDlg::on_btnProbation_clicked()
{
	setProbationStartDate();
	this->close();
}

void RegisterDlg::on_btnRegister_clicked()
{
	QString license = ui.lineEditLicense->text();
	QString diskCode = qGetVolumeInfo();
	QString encode = QString::fromStdString(md5(diskCode.toStdString()));
	if (license == encode)
	{
		QString filename = QProcessEnvironment::systemEnvironment().value("ADEHOME") + "\\ini\\license";
		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			return;
		}
		QTextStream out(&file);
		out << license;
		file.close();
		emit signalRegisterSuccess();
		QMessageBox::information(this, tr("OK"), tr("Register successfully!"));
		this->close();
	}
	else
	{
		emit signalRegisterFailed();
		QMessageBox::warning(this, tr("Warn"), tr("Register failed!"));
	}
}

void RegisterDlg::on_btnExit_clicked()
{
	this->close();
}
