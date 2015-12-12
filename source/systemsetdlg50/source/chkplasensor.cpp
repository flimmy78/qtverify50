#include "chkplasensor.h"
#include "algorithm.h"

chkplasensorDlg::chkplasensorDlg(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
}

chkplasensorDlg::~chkplasensorDlg()
{

}

void chkplasensorDlg::showEvent(QShowEvent *)
{
	m_config = new QSettings(getFullIniFileName("chkplasensor.ini"), QSettings::IniFormat);
	readConfig();
	initTbl(ui.tbl_t_r);

	int tw = ui.tbl_t_r->size().width();
	int vw = ui.tbl_t_r->verticalHeader()->size().width();
	int hSize = (int)((tw-vw-5)/ui.tbl_t_r->columnCount());
	ui.tbl_t_r->horizontalHeader()->setDefaultSectionSize(hSize);
}

void chkplasensorDlg::initTbl(QTableWidget *tbl)
{
	for (int i=0; i<tbl->rowCount();i++)
	{
		for (int j=0;j<tbl->columnCount();j++)
		{
			QTableWidgetItem* new_tbl_item = new QTableWidgetItem();
			tbl->setItem(i, j, new_tbl_item);
		}
	}
}

void chkplasensorDlg::closeEvent(QCloseEvent *event)
{
	if (m_config)
	{
		delete m_config;
		m_config = NULL;
	}
}

void chkplasensorDlg::on_btn_calc_clicked()
{
	if (!tblFilled(ui.tbl_t_r))
	{
		return;
	}

	pla_T_R_PTR prt = new pla_T_R_STR[3];
	prt[0].tmp = ui.tbl_t_r->item(0,0)->text().toFloat();
	prt[1].tmp = ui.tbl_t_r->item(0,1)->text().toFloat();
	prt[2].tmp = ui.tbl_t_r->item(0,2)->text().toFloat();

	prt[0].resis = ui.tbl_t_r->item(1,0)->text().toFloat();
	prt[1].resis = ui.tbl_t_r->item(1,1)->text().toFloat();
	prt[2].resis = ui.tbl_t_r->item(1,2)->text().toFloat();

	plaParam_PTR m_pla_param = getPlaParam(prt);

	ui.lineEdit_r0->setText(QString::number(m_pla_param->r0));
	ui.lineEdit_a->setText(QString::number(m_pla_param->a));
	ui.lineEdit_b->setText(QString::number(m_pla_param->b));
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

bool chkplasensorDlg::tblFilled(QTableWidget* tbl)
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

void chkplasensorDlg::on_btn_default_clicked()
{
	ui.lineEdit_r0->setText(m_config->value("default/r0").toString());
	ui.lineEdit_a->setText(m_config->value("default/a").toString());
	ui.lineEdit_b->setText(m_config->value("default/b").toString());
}

void chkplasensorDlg::on_btn_save_clicked()
{
	m_config->beginGroup("setting");
	m_config->setValue("r0", ui.lineEdit_r0->text());
	m_config->setValue("a", ui.lineEdit_a->text());
	m_config->setValue("b", ui.lineEdit_b->text());
	m_config->endGroup();
}

void chkplasensorDlg::on_btn_exit_clicked()
{
	this->close();
}

void chkplasensorDlg::readConfig()
{
	ui.lineEdit_r0->setText(m_config->value("setting/r0").toString());
	ui.lineEdit_a->setText(m_config->value("setting/a").toString());
	ui.lineEdit_b->setText(m_config->value("setting/b").toString());
}