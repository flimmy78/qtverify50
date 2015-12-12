/***********************************************
**  �ļ���:     parasetdlg.cpp
**  ����:       ������-�����춨-�������ý���
**  ����ϵͳ:   ����Trolltech Qt4.8.5�Ŀ�ƽ̨ϵͳ
**  ����ʱ��:   2014/6/12
**  רҵ��:     ��³���������
**  ���������: YS
**  ����Ա:     YS
**  �汾��ʷ:   2014/07 ��һ��
**  ���ݰ���:
**  ˵��:
**  ���¼�¼:
***********************************************/

#include <QtGui/QMessageBox>
#include <QtCore/QDebug>
#include <QThread>
#include <QtCore/QSettings>
#include <QTextCodec>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>
#include <QtCore/QProcessEnvironment>
#include <QSqlDatabase>

#include "parasetdlg.h"
#include "commondefine.h"
#include "qtexdb.h"
#include "algorithm.h"

ParaSetDlg::ParaSetDlg(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	qDebug()<<"ParaSetDlg thread:"<<QThread::currentThreadId();
	ui.setupUi(this);
	ui.gBoxWaterPara->hide(); //Ĭ�ϲ���ʾˮ�������
	cBoxData_inited = false;
	
	m_meterStdNum = 0;
	m_meterStdPtr = NULL;
	m_manuFacNum = 0;
	m_manuFacPtr = NULL;
	initUiData();//�����ݿ��е�����, ��ʼ��comboBox��ֵ

	QString filename = getFullIniFileName("verifyparaset.ini");//�����ļ����ļ���
	settings = new QSettings(filename, QSettings::IniFormat);
	settings->setIniCodec("GB2312");//�����ini�ļ���д��������

	lastParams = new ParaSetReader();
	flowPointVector();
	installLastParams();//�����ϴε�������Ϣ
	//m_standVerify = false;//Ĭ�ϲ�ʹ�ñ�׼���ⷨ
}

ParaSetDlg::~ParaSetDlg()
{
	qDebug()<<"!!! ParaSetDlg destructor";
}

void ParaSetDlg::on_btnExit_clicked()
{
	this->close();
}

void ParaSetDlg::showEvent(QShowEvent * event)
{
}

void ParaSetDlg::closeEvent(QCloseEvent * event)
{
	if (m_meterStdPtr)
	{
		delete []m_meterStdPtr;
		m_meterStdPtr = NULL;
	}

	if (m_manuFacPtr)
	{
		delete []m_manuFacPtr;
		m_manuFacPtr = NULL;
	}
	if (settings)
	{
		delete settings;
		settings = NULL;
	}
	if (lastParams)
	{
		delete lastParams;
		lastParams=NULL;
	}

}

void ParaSetDlg::showWaterPara()
{
	ui.gBoxWaterPara->show();
}

void ParaSetDlg::on_cmbStandard_currentIndexChanged()
{
// 	qDebug()<<"on_cmbStandard_currentIndexChanged is called; currentIndex is :"<< ui.cmbStandard->currentIndex();
	if (cBoxData_inited)
	{
		installDftDBinfo();
	}
}

void ParaSetDlg::installDftDBinfo()
{
	int idx = ui.cmbStandard->currentIndex();//����ĵ�ǰ����ֵ
	int count;//��ѯ���ļ�¼����
	DftDbInfo_PTR dbinfo_ptr;
	if (getDftDBinfo(count, dbinfo_ptr, idx))
	{
		ui.cmbFlow->setCurrentIndex(dbinfo_ptr[0].stand_id);
		for(int i=0; i < VERIFY_POINTS; i++)
		{
			cBox_valves[i]->setCurrentIndex(dbinfo_ptr[i].vale_num);
			lineEdit_uppers[i]->setText(QString::number(dbinfo_ptr[i].upper_flow));
			lineEdit_flows[i]->setText(QString::number(dbinfo_ptr[i].v_flow));
			lineEdit_quantites[i]->setText(QString::number(dbinfo_ptr[i].v_quan));
			lineEdit_freqs[i]->setText(QString::number(dbinfo_ptr[i].pump_freq));
			cBox_seqs[i]->setCurrentIndex(dbinfo_ptr[i].seq);
			lineEdit_Openings[i]->setText(QString::number(dbinfo_ptr[i].opening));
		}
	}
}

void ParaSetDlg::initUiData()
{
	//����
	getMeterStandard(m_meterStdNum, m_meterStdPtr);
	for (int i=0; i<m_meterStdNum; i++)
	{
// 		qDebug()<<"id:"<<m_meterStdPtr[i].id<<",name:"<<m_meterStdPtr[i].name;
		ui.cmbStandard->insertItem(i, m_meterStdPtr[i].name);
	}

	//�ɼ����� �������ļ�pickcode.ini��ȡ
// 	QStringList strlist = getPickCodeStringList();
// 	for (int m=0; m<strlist.count(); m++)
// 	{
// 		ui.cmbCollectCode->insertItem(m, strlist.at(m));
// 	}
	//�ɼ�����
	mapPickCodeModel();

	//���쵥λ
	mapManuDeptModel();

	//�ͼ쵥λ
	mapVfDeptModel();

	//���Ա
	mapUserModel();

	//�ͺ�
	mapMeterModelModel();

	cBoxData_inited = true;//�������ѳ�ʼ�����

	initBtnGroup();
}

//ӳ��ɼ�����
void ParaSetDlg::mapPickCodeModel()
{
	m_pickCodeModel = new QSqlTableModel(this, g_defaultdb);  
	m_pickCodeModel->setTable("T_Meter_PickCode");  
	m_pickCodeModel->select();
	ui.cmbPickCode->setModel(m_pickCodeModel);  
	ui.cmbPickCode->setModelColumn(m_pickCodeModel->fieldIndex("F_Desc")); // ʹ���ֶ����õ���ȷ�ı�������,��ʹ��Ͽ���ʾ��������  
	m_pickCodeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//ӳ�����쵥λ
void ParaSetDlg::mapManuDeptModel()
{
	m_manuDeptModel = new QSqlTableModel(this, g_defaultdb);  
	m_manuDeptModel->setTable("T_Manufacture_Dept");  
	m_manuDeptModel->select();
	ui.cmbManufacture->setModel(m_manuDeptModel);  
	ui.cmbManufacture->setModelColumn(m_manuDeptModel->fieldIndex("F_Desc")); // ʹ���ֶ����õ���ȷ�ı�������,��ʹ��Ͽ���ʾ������  
	m_manuDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//ӳ���ͼ쵥λ
void ParaSetDlg::mapVfDeptModel()
{
	m_vfDeptModel = new QSqlTableModel(this, g_defaultdb);  
	m_vfDeptModel->setTable("T_Verify_Dept");  
	m_vfDeptModel->select();
	ui.cmbVerifyCompany->setModel(m_vfDeptModel);  
	ui.cmbVerifyCompany->setModelColumn(m_vfDeptModel->fieldIndex("F_Desc")); // ʹ���ֶ����õ���ȷ�ı�������,��ʹ��Ͽ���ʾ������  
	m_vfDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

/*	QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, g_defaultdb);  
	model->setTable("T_Verify_Dept");  
	int col_id = model->fieldIndex("F_ID");
	model->setRelation(col_id, QSqlRelation("T_Verify_Dept","F_ID","F_Desc"));  
	//QComboBox��QListWidget������,��Ϊ����һ���ڲ�ģ��ȥ��������������Ŀ,�����������Լ�����ģ�ʹ����Ǹ��Դ���ģ�͡�
	//����QSqlRelationalTableModelʹ�õĹ�ϵģ�ͣ����ģ��������,����ָ����Ͽ�Ӧ����ʾ��һ��   
	m_vfDeptModel = model->relationModel(col_id); // ����ID   
	ui.cmbVerifyCompany->setModel(m_vfDeptModel);  
	ui.cmbVerifyCompany->setModelColumn(m_vfDeptModel->fieldIndex("F_Desc")); // ʹ���ֶ����õ���ȷ�ı�������,��ʹ��Ͽ���ʾ������  
	m_vfDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
*/
}

//ӳ���ͼ�Ա�ͺ���Ա
void ParaSetDlg::mapUserModel()
{
	m_userModel = new QSqlTableModel(this, g_defaultdb);  
	m_userModel->setTable("T_User_Def_Tab");  
	m_userModel->select();  
	ui.cmbVerifyPerson->setModel(m_userModel);  
	ui.cmbVerifyPerson->setModelColumn(m_userModel->fieldIndex("F_Desc"));

	ui.cmbChkPerson->setModel(m_userModel);
	ui.cmbChkPerson->setModelColumn(m_userModel->fieldIndex("F_Desc"));
	m_userModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//ӳ����ͺ�
void ParaSetDlg::mapMeterModelModel()
{
	m_modelModel = new QSqlTableModel(this, g_defaultdb);  
	m_modelModel->setTable("T_Meter_Model");  
	m_modelModel->select();
	ui.cmbModel->setModel(m_modelModel);  
	ui.cmbModel->setModelColumn(m_modelModel->fieldIndex("F_Desc")); 
	m_modelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

void ParaSetDlg::initBtnGroup()
{
	m_btnGroup_autopick = new QButtonGroup(ui.gBox_autopick); //������λ
	m_btnGroup_autopick->addButton(ui.tBtn_autoPick_true, true);
	m_btnGroup_autopick->addButton(ui.tBtn_autoPick_false, false);
	connect(m_btnGroup_autopick, SIGNAL(buttonClicked(int)), this, SLOT(slot_autopick_clicked(int)));
}

void ParaSetDlg::slot_autopick_clicked(int id)
{
	ui.tBtn_adjustError_true->setEnabled(id);
	ui.tBtn_adjustError_false->setEnabled(id);

	ui.tBtn_writeNum_true->setEnabled(id);
	ui.tBtn_writeNum_false->setEnabled(id);
	
	if (!id)
	{
		//�������
		ui.tBtn_adjustError_true->setChecked(false);
		ui.tBtn_adjustError_false->setChecked(true);

		//д���
		ui.tBtn_writeNum_true->setChecked(false);
		ui.tBtn_writeNum_false->setChecked(true);
	}	
}

/*
** �춨������仯�󣬸������������㡢����·�����������Զ�ѡ���Ӧ��·
** num: 0-���·��1-�ж���·�� 2-��һ��·�� 3-С��· 
*/
void ParaSetDlg::onVerifyFlowChanged(QString vFlow, int num)
{
	bool ok;
	float flow = vFlow.toFloat(&ok);
	if (!ok)
	{
		return;
	}

	float bigUpperFlow = ui.lineEdit_Upper_1->text().toFloat();
	float mid2UpperFlow = ui.lineEdit_Upper_2->text().toFloat();
	float mid1UpperFlow = ui.lineEdit_Upper_3->text().toFloat();
	float smallUpperFlow = ui.lineEdit_Upper_4->text().toFloat();

	if (flow <= bigUpperFlow && flow > mid2UpperFlow)
	{
		cBox_valves[num]->setCurrentIndex(0);
	}
	else if (flow <= mid2UpperFlow && flow > mid1UpperFlow)
	{
		cBox_valves[num]->setCurrentIndex(1);
	}
	else if (flow <= mid1UpperFlow && flow > smallUpperFlow)
	{
		cBox_valves[num]->setCurrentIndex(2);
	}
	else if (flow <= smallUpperFlow)
	{
		cBox_valves[num]->setCurrentIndex(3);
	}
}

void ParaSetDlg::on_lnEdit_Flow1_textChanged(const QString & text)
{
	onVerifyFlowChanged(text, 0);
}

void ParaSetDlg::on_lnEdit_Flow2_textChanged(const QString & text)
{
	onVerifyFlowChanged(text, 1);
}

void ParaSetDlg::on_lnEdit_Flow3_textChanged(const QString & text)
{
	onVerifyFlowChanged(text, 2);
}

void ParaSetDlg::on_lnEdit_Flow4_textChanged(const QString & text)
{
	onVerifyFlowChanged(text, 3);
}

/*
** �춨������仯�󣬸������������㡢����·�����������Զ�ѡ���Ӧ��·
** num: 0-���·��1-�ж���·�� 2-��һ��·�� 3-С��· 
*/
void ParaSetDlg::onUpperFlowChanged()
{
	on_lnEdit_Flow1_textChanged(ui.lnEdit_Flow1->text());
	on_lnEdit_Flow2_textChanged(ui.lnEdit_Flow2->text());
	on_lnEdit_Flow3_textChanged(ui.lnEdit_Flow3->text());
	on_lnEdit_Flow4_textChanged(ui.lnEdit_Flow4->text());
}

void ParaSetDlg::on_lineEdit_Upper_1_textChanged(const QString & text)
{
	onUpperFlowChanged();
}

void ParaSetDlg::on_lineEdit_Upper_2_textChanged(const QString & text)
{
	onUpperFlowChanged();
}

void ParaSetDlg::on_lineEdit_Upper_3_textChanged(const QString & text)
{
	onUpperFlowChanged();
}

void ParaSetDlg::on_lineEdit_Upper_4_textChanged(const QString & text)
{
	onUpperFlowChanged();
}

void ParaSetDlg::installLastParams()
{
	installHead();
	//installStdDevNo();
	installFlowPoint();
	installBool();
	installOther();
	installWaterPara();
}

void ParaSetDlg::installHead()
{
	ui.cmbStandard->setCurrentIndex(lastParams->m_params->m_stand);
	ui.cmbPickCode->setCurrentIndex(lastParams->m_params->m_pickcode);
	ui.cmbManufacture->setCurrentIndex(lastParams->m_params->m_manufac);
	ui.cmbGrade->setCurrentIndex(lastParams->m_params->m_grade-1);
	ui.cmbModel->setCurrentIndex(lastParams->m_params->m_model);
	ui.cmbVerifyCompany->setCurrentIndex(lastParams->m_params->m_vcomp);
	ui.cmbVerifyPerson->setCurrentIndex(lastParams->m_params->m_vperson);
	ui.cmbFlow->setCurrentIndex(lastParams->m_params->m_nflowpnt);
	/********����������***************************************************/
	ui.cmbChkPerson->setCurrentIndex(lastParams->m_params->m_cperson);
	ui.lnEditVersion_2->setText(lastParams->m_params->m_temper);
	ui.lnEditVersion_3->setText(lastParams->m_params->m_humidity);
	ui.lnEditVersion_4->setText(lastParams->m_params->m_airpress);
	/********************************************************************/
}

void ParaSetDlg::installFlowPoint()
{
	// ��i������
	for (int i=0; i<VERIFY_POINTS; i++)
	{
		lineEdit_uppers[i]->setText(QString::number(lastParams->m_params->fp_info[i].fp_upperlmt));
		lineEdit_flows[i]->setText(QString::number(lastParams->m_params->fp_info[i].fp_verify));
		lineEdit_quantites[i]->setText(QString::number(lastParams->m_params->fp_info[i].fp_quantity));
		cBox_valves[i]->setCurrentIndex(lastParams->m_params->fp_info[i].fp_valve_idx);
		lineEdit_freqs[i]->setText(QString::number(lastParams->m_params->fp_info[i].fp_freq));
		lineEdit_Openings[i]->setText(QString::number(lastParams->m_params->fp_info[i].fp_opening));
		cBox_seqs[i]->setCurrentIndex(lastParams->m_params->fp_info[i].fp_seq);
	}
}

void ParaSetDlg::installBool()
{
	//�Զ��ɼ�
	ui.tBtn_autoPick_true->setChecked(lastParams->m_params->bo_autopick) ;
	ui.tBtn_autoPick_false->setChecked(!(lastParams->m_params->bo_autopick)) ;
	 
	 
	//�������
	ui.tBtn_adjustError_true->setChecked(lastParams->m_params->bo_adjerror) ;
	ui.tBtn_adjustError_false->setChecked(!(lastParams->m_params->bo_adjerror)) ;
	//д���
	ui.tBtn_writeNum_true->setChecked(lastParams->m_params->bo_writenum) ;
	ui.tBtn_writeNum_false->setChecked(!(lastParams->m_params->bo_writenum)) ;
	 
	if (!lastParams->m_params->bo_autopick)
	{
		//�������
		ui.tBtn_adjustError_true->setEnabled(false);
		ui.tBtn_adjustError_false->setEnabled(false);
		 
		//д���
		ui.tBtn_writeNum_true->setEnabled(false);
		ui.tBtn_writeNum_false->setEnabled(false);
	}

	//�����춨
	ui.tBtn_continuous_true->setChecked(lastParams->m_params->bo_converify) ;
	ui.tBtn_continuous_false->setChecked(!(lastParams->m_params->bo_converify)) ;
	
	//��ֵ����
	ui.tBtn_resetzero_true->setChecked(lastParams->m_params->bo_resetzero);
	ui.tBtn_resetzero_false->setChecked(!(lastParams->m_params->bo_resetzero));

	//�ظ����
	ui.chkBoxRepeatVerify->setChecked(lastParams->m_params->bo_repeatverify);
}

void ParaSetDlg::installOther()
{
	ui.lineEdit_sc_flow->setText(QString::number(lastParams->m_params->sc_flow));
	ui.lineEdit_sc_thermal->setText(QString::number(lastParams->m_params->sc_thermal));
	ui.lineEdit_exTime->setText(QString::number(lastParams->m_params->ex_time));
	ui.lineEditMeterNO->setText(QString::number(lastParams->m_params->meterNo));
}

void ParaSetDlg::installWaterPara()
{
	for (int i=0; i<ui.cmbQ3BiQ1->count(); i++)
	{
		if (ui.cmbQ3BiQ1->itemText(i).toFloat()==lastParams->m_params->Q3BiQ1)
		{
			ui.cmbQ3BiQ1->setCurrentIndex(i);
			break;;
		}
	}

	for (int j=0; j<ui.cmbQ3->count(); j++)
	{
		if (ui.cmbQ3->itemText(j).toFloat()==lastParams->m_params->Q3)
		{
			ui.cmbQ3->setCurrentIndex(j);
			break;;
		}
	}

	for (int m=0; m<ui.cmbQ2BiQ1->count(); m++)
	{
		if (ui.cmbQ2BiQ1->itemText(m).toFloat()==lastParams->m_params->Q2BiQ1)
		{
			ui.cmbQ2BiQ1->setCurrentIndex(m);
			break;;
		}
	}
}

/*
** ������������, ���ƹ��ܵĿؼ���������, ����ʹ��; 
   ��ͺñȽ����ϵ�һ��ؼ�����һ����ϵ��(��ϵ�����ݿ�)�е�һ����¼, 
   ��ÿ���ؼ������¼�еĲ�ͬ����
*/
void ParaSetDlg::flowPointVector()
{
	//����������
	lineEdit_uppers.append(ui.lineEdit_Upper_1);
	lineEdit_uppers.append(ui.lineEdit_Upper_2);
	lineEdit_uppers.append(ui.lineEdit_Upper_3);
	lineEdit_uppers.append(ui.lineEdit_Upper_4);
	//���������
	lineEdit_flows.append(ui.lnEdit_Flow1);
	lineEdit_flows.append(ui.lnEdit_Flow2);
	lineEdit_flows.append(ui.lnEdit_Flow3);
	lineEdit_flows.append(ui.lnEdit_Flow4);
	//�춨��
	lineEdit_quantites.append(ui.lineEdit_Quantity_1);
	lineEdit_quantites.append(ui.lineEdit_Quantity_2);
	lineEdit_quantites.append(ui.lineEdit_Quantity_3);
	lineEdit_quantites.append(ui.lineEdit_Quantity_4);
	//���Ʒ�
	cBox_valves.append(ui.cBox_Valve_1);
	cBox_valves.append(ui.cBox_Valve_2);
	cBox_valves.append(ui.cBox_Valve_3);
	cBox_valves.append(ui.cBox_Valve_4);
	//ˮ��Ƶ��
	lineEdit_freqs.append(ui.lineEdit_freq_1);
	lineEdit_freqs.append(ui.lineEdit_freq_2);
	lineEdit_freqs.append(ui.lineEdit_freq_3);
	lineEdit_freqs.append(ui.lineEdit_freq_4);
	//�춨����
	cBox_seqs.append((ui.cBox_seq_1));
	cBox_seqs.append((ui.cBox_seq_2));
	cBox_seqs.append((ui.cBox_seq_3));
	cBox_seqs.append((ui.cBox_seq_4));
	//���ڷ�����
	lineEdit_Openings.append(ui.lineEdit_Opening_1);
	lineEdit_Openings.append(ui.lineEdit_Opening_2);
	lineEdit_Openings.append(ui.lineEdit_Opening_3);
	lineEdit_Openings.append(ui.lineEdit_Opening_4);
}

void ParaSetDlg::on_btnSave_clicked()
{
	//�ȼ��ѡ������ĺϷ���
	if (!chkSeq())
	{
		QMessageBox::warning(this, tr("Warning"), tr("verify sequence is invalid!"));
		return;
	}

	//�������������ȷ, �ټ����������õļ춨�����Ƿ����
	if (!chkTotalQuantity())
	{
		QMessageBox::warning(this, tr("Warning"), tr("verify QUANTITY is invalid!\nPlease check each flow point's quantity"));
		return;
	}

	SaveHead();
	SaveFlowPoint();
	SaveBool();
	SaveOther();
	SaveWaterPara();

 	m_vfDeptModel->submitAll();
	ui.cmbVerifyCompany->setCurrentIndex(m_curVfDeptIdx);

	m_manuDeptModel->submitAll();
	ui.cmbManufacture->setCurrentIndex(m_curManuDeptIdx);

	m_userModel->submitAll();
	ui.cmbVerifyPerson->setCurrentIndex(m_curVerifyPersonIdx);
	ui.cmbChkPerson->setCurrentIndex(m_curCheckPersonIdx);

	m_modelModel->submitAll();
	ui.cmbModel->setCurrentIndex(m_curModelIdx);

	m_pickCodeModel->submitAll();
	ui.cmbPickCode->setCurrentIndex(m_curPickCodeIdx);

	//�������ݿ��T_Meter_Default_Params
	Update_T_Meter_Default_Params();

//	QMessageBox::information(this, tr("OK"), tr("Save successfully!"));
	emit saveSuccessSignal();
	ui.labelHintInfo->setText(tr("Save successfully!"));
	wait(CYCLE_TIME);
	ui.labelHintInfo->clear();
}

/*
** ��鱻�û�ѡ���ļ춨���к��Ƿ�Ϸ�
   �춨�������������1��ʼ, �м䲻��ϵ���Ȼ������
   ����: [1]; [1, 2]; [1, 2, 3]; ��������.
   ������һ����������
   ��nΪ�����Ԫ��ֵ
   �����������ظ���,
   ��n�����е�Ԫ�ظ������
   ��ô�������һ������1��ʼ,��n��ֹ����Ȼ����
*/
bool ParaSetDlg::chkSeq()
{
	int max_seq = 0;//���Ĵ����
	int total_seqs = 0;//����ļ춨����ŵĸ���
	QVector<int> repeat_seq;//�ѷ��ֵ�Ԫ������

	for (int i=0; i < VERIFY_POINTS; i++)
	{
		int current_seq = cBox_seqs[i]->currentIndex();

		if (current_seq > 0)//�������е���Ȼ�����в�������Ԫ��
		{
			//��������!*����Ԫ��*!�����
			if (max_seq < current_seq)
			{
				max_seq = current_seq;
			}

			total_seqs ++;//����!*����Ԫ��*!�ĸ���
			//�������*�ظ�*��!*����Ԫ��*!, �򷵻�false;���򽫵�ǰԪ�ؼ����ѷ��ֵ�Ԫ������
			if (repeat_seq.contains(current_seq))
			{
				return false;
			}
			else
			{
				repeat_seq.append(current_seq);
			}
		}
	}
	//������Ĵ����Ϊ0, ����Ϊ�û�©ѡ�˼춨����
	if (max_seq == 0)
	{
		return false;
	}

	return (max_seq == total_seqs);
}

bool ParaSetDlg::chkTotalQuantity()
{
	QSettings com_config(getFullIniFileName("comconfig.ini"), QSettings::IniFormat);
	float maxWht = com_config.value("BalanceType/maxweight").toFloat();
	float bottomWht = com_config.value("BalanceType/bottomwht").toFloat();

	float totalQuantity = 0.0;
	for (int i=0; i<lineEdit_quantites.count(); i++)
	{
		if (cBox_seqs[i]->currentIndex()>0)
		{
			totalQuantity += lineEdit_quantites[i]->text().toFloat();
		}
	}

	return (totalQuantity<(maxWht-bottomWht));
}

void ParaSetDlg::SaveHead()
{
	settings->beginGroup("Head");
	settings->setValue("standard", ui.cmbStandard->currentIndex());
	settings->setValue("manufacture", ui.cmbManufacture->currentIndex());
	m_curManuDeptIdx = ui.cmbManufacture->currentIndex();
	settings->setValue("grade", ui.cmbGrade->currentIndex()+1);
	settings->setValue("model", ui.cmbModel->currentIndex());
	m_curModelIdx = ui.cmbModel->currentIndex();
	settings->setValue("verifycompany", ui.cmbVerifyCompany->currentIndex());
	m_curVfDeptIdx = ui.cmbVerifyCompany->currentIndex();
	settings->setValue("verifyperson", ui.cmbVerifyPerson->currentIndex());
	m_curVerifyPersonIdx = ui.cmbVerifyPerson->currentIndex();
	settings->setValue("pickcode", ui.cmbPickCode->currentIndex());
	m_curPickCodeIdx = ui.cmbPickCode->currentIndex(); 
	settings->setValue("nflowpoint", ui.cmbFlow->currentIndex());
	
	/********����������***************************************************/
	settings->setValue("chkperson",ui.cmbChkPerson->currentIndex());
	m_curCheckPersonIdx = ui.cmbChkPerson->currentIndex();
	settings->setValue("temper",ui.lnEditVersion_2->text());
	settings->setValue("humidity",ui.lnEditVersion_3->text());
	settings->setValue("airpress",ui.lnEditVersion_4->text());
	/********************************************************************/
	settings->endGroup();
}

/*
** �����i���������
** i: �����ϵĵ�i��������, �����Ǽ춨˳�� 
*/
void ParaSetDlg::SaveFlowPoint()
{
	settings->beginWriteArray("FlowPoints");
	for (int i = 0; i < VERIFY_POINTS; i++) 
	{
		settings->setArrayIndex(i);
		settings->setValue("upperflow", lineEdit_uppers[i]->text());//��������ֵ
		settings->setValue("verifyflow", lineEdit_flows[i]->text());//������
		settings->setValue("flowquantity", lineEdit_quantites[i]->text());//�춨ˮ��
		settings->setValue("pumpfrequencey", lineEdit_freqs[i]->text());//��Ƶ��Ƶ��
		settings->setValue("opening", lineEdit_Openings[i]->text());//���ڷ�����
		settings->setValue("valve", cBox_valves[i]->currentIndex());//��Ӧ�ķ���
		settings->setValue("seq", cBox_seqs[i]->currentIndex());//�춨˳��
	}
	settings->endArray();
}

void ParaSetDlg::SaveBool()
{
	settings->beginGroup("Bool");
	settings->setValue("autopick", ui.tBtn_autoPick_true->isChecked() );//�Ƿ��Զ��ɼ�
	settings->setValue("adjusterror", ui.tBtn_adjustError_true->isChecked() );//�Ƿ��������
	settings->setValue("writemeternumber", ui.tBtn_writeNum_true->isChecked() );//�Ƿ�д���
	settings->setValue("continuouscheck", ui.tBtn_continuous_true->isChecked() );//�Ƿ������춨
	settings->setValue("resetzero", ui.tBtn_resetzero_true->isChecked() );//�Ƿ��ֵ����
	settings->setValue("repeatverify", ui.chkBoxRepeatVerify->isChecked() );//�Ƿ��ظ����
	settings->endGroup();
}

void ParaSetDlg::SaveOther()
{
	settings->beginGroup("Other");
	settings->setValue("flowsafecoefficient", ui.lineEdit_sc_flow->text());//������ȫϵ��
	settings->setValue("thermalsafecoefficient", ui.lineEdit_sc_thermal->text());//������ȫϵ��
	settings->setValue("exhausttime", ui.lineEdit_exTime->text());//����ʱ��
	settings->setValue("meternumber", ui.lineEditMeterNO->text());//��ʼ���
	settings->endGroup();
}

void ParaSetDlg::SaveWaterPara()
{
	settings->beginGroup("WaterPara");
	settings->setValue("Q3BiQ1", ui.cmbQ3BiQ1->currentText());
	settings->setValue("Q3", ui.cmbQ3->currentText());
	settings->setValue("Q2BiQ1", ui.cmbQ2BiQ1->currentText());
	settings->endGroup();
}

void ParaSetDlg::Update_T_Meter_Default_Params()
{
	DftDbInfo_STR dbinfo_ptr[VERIFY_POINTS];
	memset(dbinfo_ptr, 0, sizeof(DftDbInfo_STR)*VERIFY_POINTS);
	
	for(int i=0; i<VERIFY_POINTS; i++)
	{
		dbinfo_ptr[i].upper_flow = lineEdit_uppers[i]->text().toFloat();
		dbinfo_ptr[i].v_flow = lineEdit_flows[i]->text().toFloat();
		dbinfo_ptr[i].v_quan = lineEdit_quantites[i]->text().toFloat();
		dbinfo_ptr[i].pump_freq = lineEdit_freqs[i]->text().toFloat();
		dbinfo_ptr[i].opening = lineEdit_Openings[i]->text().toFloat();
	}

	updateDftDBinfo(dbinfo_ptr, ui.cmbStandard->currentIndex());
}

/*************************************************************************
************************ParaSetReader Start*************************
**************************************************************************/
ParaSetReader::ParaSetReader()
{
	initValveMap();
	readIniFile();
	m_params = NULL;
	m_params = new Verify_Params_STR;
	readParamValues();
}

ParaSetReader::~ParaSetReader()
{
	if (m_settings)
	{
		delete m_settings;
		m_settings = NULL;
	}
	if (m_params)
	{
		delete m_params;
		m_params=NULL;
	}
}

int ParaSetReader::readIniFile()
{
	//�춨ǰ�Ĳ����ļ�
	m_settings = new QSettings(getFullIniFileName("verifyparaset.ini"), QSettings::IniFormat);
	m_settings->setIniCodec("GB2312");//�����ini�ļ���д��������

	//�˿ںŵ������ļ�
	m_port_config = new QSettings(getFullIniFileName("portset.ini"), QSettings::IniFormat);
	m_port_config->setIniCodec("GB2312");

	return true;
}

Verify_Params_PTR ParaSetReader::getParams()
{
	readIniFile();
	readParamValues();
	return m_params;
}

/*
** �������ļ������comboBox��������
** �˿��ֶζ�Ӧ
*/
void ParaSetReader::initValveMap()
{
	m_valvePortMap.insert(0,"bigNo");
	m_valvePortMap.insert(1,"middle2No");
	m_valvePortMap.insert(2,"middle1No");	
	m_valvePortMap.insert(3,"smallNo");

	m_regPortMap.insert(0,"bigNo");
	m_regPortMap.insert(1,"middle2No");
	m_regPortMap.insert(2,"middle1No");	
	m_regPortMap.insert(3,"smallNo");
}

/*
** ��ȡ�����ļ�����Ϣ
*/
void ParaSetReader::readParamValues()
{
	readHead();
	readBool();
	readOther();
	readFlowPoints();
	readWaterPara();
}

void ParaSetReader::readHead()
{
	m_params->m_stand = m_settings->value("head/standard").toInt();
	/////////////////////��ȡ�������/////////////////////////
	m_params->m_maxMeters = getMaxMeterByIdx(m_params->m_stand);

	///////////////////////////////////////////////////////////////////
	m_params->m_manufac = m_settings->value("head/manufacture").toInt();	
	m_params->m_model = m_settings->value("head/model").toInt();
	m_params->m_vcomp = m_settings->value("head/verifycompany").toInt();
	m_params->m_vperson = m_settings->value("head/verifyperson").toInt();
	m_params->m_pickcode = m_settings->value("head/pickcode").toInt();
	m_params->m_grade = m_settings->value("head/grade").toInt();
	m_params->m_nflowpnt = m_settings->value("head/nflowpoint").toInt();
	/******************����������****************************************/
	m_params->m_cperson = m_settings->value("head/chkperson").toInt();
	m_params->m_temper = m_settings->value("head/temper").toString();
	m_params->m_humidity = m_settings->value("head/humidity").toString();
	m_params->m_airpress = m_settings->value("head/airpress").toString();
	/************************************************************************/
}

void ParaSetReader::readFlowPoints()
{
	m_params->total_fp = 0;//����������Ŀ��Ϊ��
	QStringList heads = m_settings->childGroups();//�����ļ��е�����
	// ��i������
	m_settings->beginReadArray("FlowPoints");
	for (int i = 0; i < VERIFY_POINTS; i++) {
		m_settings->setArrayIndex(i);
		m_params->fp_info[i].fp_freq =  m_settings->value("pumpfrequencey").toFloat();
		m_params->fp_info[i].fp_upperlmt =  m_settings->value("upperflow").toFloat();
		m_params->fp_info[i].fp_verify =  m_settings->value("verifyflow").toFloat();
		m_params->fp_info[i].fp_quantity =  m_settings->value("flowquantity").toFloat();
		m_params->fp_info[i].fp_valve_idx =  m_settings->value("valve").toInt();
		m_params->fp_info[i].fp_valve =  m_port_config->value("Relay/" + m_valvePortMap[m_params->fp_info[i].fp_valve_idx]).toInt();
		m_params->fp_info[i].fp_regno =  m_port_config->value("Regulate/" + m_regPortMap[m_params->fp_info[i].fp_valve_idx]).toInt();
		m_params->fp_info[i].fp_seq =  m_settings->value("seq").toInt();
		m_params->fp_info[i].fp_opening =  m_settings->value("opening").toInt();
		if (m_params->fp_info[i].fp_seq)
		{
			m_params->total_fp++;
		}
	}
	m_settings->endArray();
}

void ParaSetReader::readBool()
{
	m_params->bo_adjerror	= m_settings->value("Bool/adjusterror").toBool();
	m_params->bo_autopick = m_settings->value("Bool/autopick").toBool();
	m_params->bo_converify = m_settings->value("Bool/continuouscheck").toBool();
	m_params->bo_writenum = m_settings->value("Bool/writemeternumber").toBool();
	m_params->bo_resetzero = m_settings->value("Bool/resetzero").toBool();
	m_params->bo_repeatverify = m_settings->value("Bool/repeatverify").toBool();	
}

void ParaSetReader::readOther()
{
	m_params->sc_flow =  m_settings->value("Other/flowsafecoefficient").toFloat();
	m_params->sc_thermal =  m_settings->value("Other/thermalsafecoefficient").toFloat();
	m_params->ex_time =  m_settings->value("Other/exhausttime").toFloat();
	m_params->meterNo =  m_settings->value("Other/meternumber").toUInt();
}

void ParaSetReader::readWaterPara()
{
	m_params->Q3BiQ1 =  m_settings->value("WaterPara/Q3BiQ1").toFloat();
	m_params->Q3 =  m_settings->value("WaterPara/Q3").toFloat();
	m_params->Q2BiQ1 =  m_settings->value("WaterPara/Q2BiQ1").toFloat();
}

/*
** �õ�������к�Ϊi����������Ϣ,i��1��ʼ,
** ��m_params->total_fp����
** �쳣: i ����[1...m_params->total_fp]֮��
*/
Flow_Point_Info ParaSetReader::getFpBySeq(int i)
{
	//��������Ч������; �����ǰ������ļ춨����Ϊi, �򷵻ش���������Ϣ
	for (int j=0; j < VERIFY_POINTS; j++)
	{
		if (m_params->fp_info[j].fp_seq == i)
		{
			return m_params->fp_info[j];
		}
	}
	
	//return m_params->fp_info[0];
	throw i;//�����������Ч�������û��ƥ��ļ춨����,��ô�˼춨���򲻴���
}

int ParaSetReader::getBigSmallBySeq(int i)
{
	//��������Ч������; �����ǰ������ļ춨����Ϊi, �򷵻ش��������������
	for (int j=0; j < VERIFY_POINTS; j++)
	{
		if (m_params->fp_info[j].fp_seq == i)
		{
			return j;
		}
	}

	return 0;//Ĭ�Ϸ��ش�����
}