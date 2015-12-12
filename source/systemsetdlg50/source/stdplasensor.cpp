#include "algorithm.h"
#include "stdplasensor.h"

stdplasensorDlg::stdplasensorDlg(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
}

stdplasensorDlg::~stdplasensorDlg()
{

}

void stdplasensorDlg::showEvent(QShowEvent *)
{
	m_config = new QSettings(getFullIniFileName("stdplasensor.ini"), QSettings::IniFormat);

	readInUse();
	readmodelconfig();
	m_tbls_inited = false;
	initTbls();
	ui.tbl_pt100_in->setEnabled(true);
	ui.tbl_pt100_out->setEnabled(true);

	int tw = ui.tbl_pt100_in->size().width();
	int vw = ui.tbl_pt100_in->verticalHeader()->size().width();
	int hSize = (int)((tw-vw-5)/ui.tbl_pt100_in->columnCount());
	ui.tbl_pt100_in->horizontalHeader()->setDefaultSectionSize(hSize);

	int tw2 = ui.tbl_pt100_out->size().width();
	int vw2 = ui.tbl_pt100_out->verticalHeader()->size().width();
	int hSize2 = (int)((tw2-vw2-5)/ui.tbl_pt100_out->columnCount());
	ui.tbl_pt100_out->horizontalHeader()->setDefaultSectionSize(hSize2);
}

void stdplasensorDlg::initTbls()
{
	initPt100tbl(ui.tbl_pt100_in);
	initPt100tbl(ui.tbl_pt100_out);
	m_tbls_inited = true;
}

void stdplasensorDlg::initPt100tbl(QTableWidget *tbl)
{
	for (int i=0; i<tbl->rowCount();i++)
	{
		for (int j=0;j<tbl->columnCount();j++)
		{
			QTableWidgetItem* new_tbl_item = new QTableWidgetItem();
			//new_tbl_item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
			tbl->setItem(i, j, new_tbl_item);
		}
	}
}

void stdplasensorDlg::closeEvent(QCloseEvent * event)
{
	if (m_config)
	{
		delete m_config;
		m_config = NULL;
	}
}

void stdplasensorDlg::on_btn_pt25_save_clicked()
{
	m_config->beginGroup("pt25");
	m_config->setValue("in_rtp", ui.lineEdit_pt25_in_rtp->text());
	m_config->setValue("in_a", ui.lineEdit_pt25_in_a->text());
	m_config->setValue("in_b", ui.lineEdit_pt25_in_b->text());
	m_config->setValue("out_rtp", ui.lineEdit_pt25_out_rtp->text());
	m_config->setValue("out_a", ui.lineEdit_pt25_out_a->text());
	m_config->setValue("out_b", ui.lineEdit_pt25_out_b->text());
	m_config->endGroup();
	m_config->beginGroup("in_use");
	m_config->setValue("pt", "pt25");
	m_config->endGroup();
}

void stdplasensorDlg::on_btn_pt100_calc_clicked()
{
	if (m_tbls_inited)
	{
		if (tblFilled(ui.tbl_pt100_in))
			calcPt100In();

		if (tblFilled(ui.tbl_pt100_out))
			calcPt100Out();
	}
}

bool stdplasensorDlg::tblFilled(QTableWidget* tbl)
{
	int row = tbl->rowCount();
	int col = tbl->columnCount();
	for (int i=0; i<row;i++)
	{
		for (int j=0;j<col;j++)
		{
			if (tbl->item(i, j)->text().isEmpty()||tbl->item(i, j)->text().isNull())
				return false;
		}
	}
	
	return true;
}

void stdplasensorDlg::on_btn_pt100_save_clicked()
{
	m_config->beginGroup("pt100");
	m_config->setValue("in_rtp", ui.lineEdit_pt100_in_rtp->text());
	m_config->setValue("in_a", ui.lineEdit_pt100_in_a->text());
	m_config->setValue("in_b", ui.lineEdit_pt100_in_b->text());
	m_config->setValue("out_rtp", ui.lineEdit_pt100_out_rtp->text());
	m_config->setValue("out_a", ui.lineEdit_pt100_out_a->text());
	m_config->setValue("out_b", ui.lineEdit_pt100_out_b->text());
	m_config->endGroup();
	m_config->beginGroup("in_use");
	m_config->setValue("pt", "pt100");
	m_config->endGroup();
}

void stdplasensorDlg::on_btn_pt100_exit_clicked()
{
	this->close();
}

void stdplasensorDlg::on_btn_model_save_clicked()
{
	const QObjectList list=ui.gBox_model->children();
	foreach(QObject *obj, list)
	{
		QString class_name = QString::fromAscii( obj->metaObject()->className() );
		if (class_name == "QRadioButton")
		{
			QRadioButton *rBtn = (QRadioButton*)obj;
			if (rBtn->isChecked())
			{
				m_config->beginGroup("in_use");
				QString str = rBtn->objectName().toLower();

				if (str == "rbtn_inst")
				{
					m_config->setValue("model", TEMPERATURE_TYPE_METROLOGY);
				}
				else if (str == "rbtn_weili")
				{
					m_config->setValue("model", TEMPERATURE_TYPE_WEILI);
				}
				else if (str == "rbtn_huayi")
				{
					m_config->setValue("model", TEMPERATURE_TYPE_HUAYI);
				}
				else
				{
					m_config->setValue("model", -1);
				}
				m_config->endGroup();
				return;
			}
		}
	}
}

void stdplasensorDlg::on_gBox_pt25_clicked()
{
	if (ui.gBox_pt25->isChecked())
	{
		ui.gBox_pt100->setChecked(false);
		readpt25config();
	}
}

void stdplasensorDlg::on_gBox_pt100_clicked()
{
	if (ui.gBox_pt100->isChecked())
	{
		ui.gBox_pt25->setChecked(false);
		ui.tbl_pt100_in->setEnabled(true);
		ui.tbl_pt100_out->setEnabled(true);
		readpt100config();
	}
}

void stdplasensorDlg::readpt25config()
{
	ui.lineEdit_pt25_in_rtp->setText(m_config->value("pt25/in_rtp").toString());
	ui.lineEdit_pt25_in_a->setText(m_config->value("pt25/in_a").toString());
	ui.lineEdit_pt25_in_b->setText(m_config->value("pt25/in_b").toString());
	ui.lineEdit_pt25_out_rtp->setText(m_config->value("pt25/out_rtp").toString());
	ui.lineEdit_pt25_out_a->setText(m_config->value("pt25/out_a").toString());
	ui.lineEdit_pt25_out_b->setText(m_config->value("pt25/out_b").toString());
}

void stdplasensorDlg::readpt100config()
{
	ui.lineEdit_pt100_in_rtp->setText(m_config->value("pt100/in_rtp").toString());
	ui.lineEdit_pt100_in_a->setText(m_config->value("pt100/in_a").toString());
	ui.lineEdit_pt100_in_b->setText(m_config->value("pt100/in_b").toString());
	ui.lineEdit_pt100_out_rtp->setText(m_config->value("pt100/out_rtp").toString());
	ui.lineEdit_pt100_out_a->setText(m_config->value("pt100/out_a").toString());
	ui.lineEdit_pt100_out_b->setText(m_config->value("pt100/out_b").toString());
}

void stdplasensorDlg::readmodelconfig()
{
	int model = m_config->value("in_use/model").toInt();
	switch(model)
	{
	case TEMPERATURE_TYPE_METROLOGY:
		ui.rbtn_inst->setChecked(true);
		break;
	case TEMPERATURE_TYPE_WEILI:
		ui.rbtn_weili->setChecked(true);
		break;
	case TEMPERATURE_TYPE_HUAYI:
		ui.rbtn_huayi->setChecked(true);
		break;
	default:
		break;
	}	
}

void stdplasensorDlg::readInUse()
{
	QString in_user = m_config->value("in_use/pt").toString();
	if (in_user == "pt25")
	{
		readpt25config();
		ui.gBox_pt25->setChecked(true);
		ui.gBox_pt100->setChecked(false);
	}
	else
	{
		readpt100config();
		ui.gBox_pt25->setChecked(false);
		ui.gBox_pt100->setChecked(true);
	}	
}

void stdplasensorDlg::calcPt100In()
{
	pla_T_R_PTR prt = new pla_T_R_STR[3];
	prt[0].tmp = ui.tbl_pt100_in->item(0,0)->text().toFloat();
	prt[1].tmp = ui.tbl_pt100_in->item(0,1)->text().toFloat();
	prt[2].tmp = ui.tbl_pt100_in->item(0,2)->text().toFloat();

	prt[0].resis = ui.tbl_pt100_in->item(1,0)->text().toFloat();
	prt[1].resis = ui.tbl_pt100_in->item(1,1)->text().toFloat();
	prt[2].resis = ui.tbl_pt100_in->item(1,2)->text().toFloat();

	plaParam_PTR m_pla_param = getPlaParam(prt);

	ui.lineEdit_pt100_in_rtp->setText(QString::number(m_pla_param->r0));
	ui.lineEdit_pt100_in_a->setText(QString::number(m_pla_param->a));
	ui.lineEdit_pt100_in_b->setText(QString::number(m_pla_param->b));
	if (prt)
	{
		delete prt;
		prt = NULL;
	}

	if (m_pla_param)
	{
		delete m_pla_param;
		m_pla_param = NULL;
	}
}

void stdplasensorDlg::calcPt100Out()
{
	pla_T_R_PTR prt = new pla_T_R_STR[3];
	prt[0].tmp = ui.tbl_pt100_out->item(0,0)->text().toFloat();
	prt[1].tmp = ui.tbl_pt100_out->item(0,1)->text().toFloat();
	prt[2].tmp = ui.tbl_pt100_out->item(0,2)->text().toFloat();

	prt[0].resis = ui.tbl_pt100_out->item(1,0)->text().toFloat();
	prt[1].resis = ui.tbl_pt100_out->item(1,1)->text().toFloat();
	prt[2].resis = ui.tbl_pt100_out->item(1,2)->text().toFloat();

	plaParam_PTR m_pla_param = getPlaParam(prt);

	ui.lineEdit_pt100_out_rtp->setText(QString::number(m_pla_param->r0));
	ui.lineEdit_pt100_out_a->setText(QString::number(m_pla_param->a));
	ui.lineEdit_pt100_out_b->setText(QString::number(m_pla_param->b));
	if (prt)
	{
		delete prt;
		prt = NULL;
	}

	if (m_pla_param)
	{
		delete m_pla_param;
		m_pla_param = NULL;
	}
}
