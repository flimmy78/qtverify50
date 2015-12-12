/***********************************************
**  文件名:     parasetdlg.cpp
**  功能:       质量法-分量检定-参数设置界面
**  操作系统:   基于Trolltech Qt4.8.5的跨平台系统
**  生成时间:   2014/6/12
**  专业组:     德鲁计量软件组
**  程序设计者: YS
**  程序员:     YS
**  版本历史:   2014/07 第一版
**  内容包含:
**  说明:
**  更新记录:
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
	ui.gBoxWaterPara->hide(); //默认不显示水表检测参数
	cBoxData_inited = false;
	
	m_meterStdNum = 0;
	m_meterStdPtr = NULL;
	m_manuFacNum = 0;
	m_manuFacPtr = NULL;
	initUiData();//以数据库中的数据, 初始化comboBox的值

	QString filename = getFullIniFileName("verifyparaset.ini");//配置文件的文件名
	settings = new QSettings(filename, QSettings::IniFormat);
	settings->setIniCodec("GB2312");//解决向ini文件中写汉字乱码

	lastParams = new ParaSetReader();
	flowPointVector();
	installLastParams();//加载上次的配置信息
	//m_standVerify = false;//默认不使用标准表检测法
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
	int idx = ui.cmbStandard->currentIndex();//表规格的当前索引值
	int count;//查询到的记录个数
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
	//表规格
	getMeterStandard(m_meterStdNum, m_meterStdPtr);
	for (int i=0; i<m_meterStdNum; i++)
	{
// 		qDebug()<<"id:"<<m_meterStdPtr[i].id<<",name:"<<m_meterStdPtr[i].name;
		ui.cmbStandard->insertItem(i, m_meterStdPtr[i].name);
	}

	//采集代码 从配置文件pickcode.ini获取
// 	QStringList strlist = getPickCodeStringList();
// 	for (int m=0; m<strlist.count(); m++)
// 	{
// 		ui.cmbCollectCode->insertItem(m, strlist.at(m));
// 	}
	//采集代码
	mapPickCodeModel();

	//制造单位
	mapManuDeptModel();

	//送检单位
	mapVfDeptModel();

	//检测员
	mapUserModel();

	//型号
	mapMeterModelModel();

	cBoxData_inited = true;//下拉条已初始化完毕

	initBtnGroup();
}

//映射采集代码
void ParaSetDlg::mapPickCodeModel()
{
	m_pickCodeModel = new QSqlTableModel(this, g_defaultdb);  
	m_pickCodeModel->setTable("T_Meter_PickCode");  
	m_pickCodeModel->select();
	ui.cmbPickCode->setModel(m_pickCodeModel);  
	ui.cmbPickCode->setModelColumn(m_pickCodeModel->fieldIndex("F_Desc")); // 使用字段名得到正确的标题索引,以使组合框显示中文描述  
	m_pickCodeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//映射制造单位
void ParaSetDlg::mapManuDeptModel()
{
	m_manuDeptModel = new QSqlTableModel(this, g_defaultdb);  
	m_manuDeptModel->setTable("T_Manufacture_Dept");  
	m_manuDeptModel->select();
	ui.cmbManufacture->setModel(m_manuDeptModel);  
	ui.cmbManufacture->setModelColumn(m_manuDeptModel->fieldIndex("F_Desc")); // 使用字段名得到正确的标题索引,以使组合框显示部门名  
	m_manuDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
}

//映射送检单位
void ParaSetDlg::mapVfDeptModel()
{
	m_vfDeptModel = new QSqlTableModel(this, g_defaultdb);  
	m_vfDeptModel->setTable("T_Verify_Dept");  
	m_vfDeptModel->select();
	ui.cmbVerifyCompany->setModel(m_vfDeptModel);  
	ui.cmbVerifyCompany->setModelColumn(m_vfDeptModel->fieldIndex("F_Desc")); // 使用字段名得到正确的标题索引,以使组合框显示部门名  
	m_vfDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

/*	QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, g_defaultdb);  
	model->setTable("T_Verify_Dept");  
	int col_id = model->fieldIndex("F_ID");
	model->setRelation(col_id, QSqlRelation("T_Verify_Dept","F_ID","F_Desc"));  
	//QComboBox与QListWidget很相拟,因为它有一个内部模型去保存它的数据条目,所以我们用自己建的模型代替那个自带的模型。
	//给出QSqlRelationalTableModel使用的关系模型，这个模型有两列,必须指出组合框应该显示哪一列   
	m_vfDeptModel = model->relationModel(col_id); // 部门ID   
	ui.cmbVerifyCompany->setModel(m_vfDeptModel);  
	ui.cmbVerifyCompany->setModelColumn(m_vfDeptModel->fieldIndex("F_Desc")); // 使用字段名得到正确的标题索引,以使组合框显示部门名  
	m_vfDeptModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
*/
}

//映射送检员和核验员
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

//映射表型号
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
	m_btnGroup_autopick = new QButtonGroup(ui.gBox_autopick); //计量单位
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
		//调整误差
		ui.tBtn_adjustError_true->setChecked(false);
		ui.tBtn_adjustError_false->setChecked(true);

		//写表号
		ui.tBtn_writeNum_true->setChecked(false);
		ui.tBtn_writeNum_false->setChecked(true);
	}	
}

/*
** 检定流量点变化后，根据上限流量点、各管路的上限流量自动选择对应管路
** num: 0-大管路；1-中二管路； 2-中一管路； 3-小管路 
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
** 检定流量点变化后，根据上限流量点、各管路的上限流量自动选择对应管路
** num: 0-大管路；1-中二管路； 2-中一管路； 3-小管路 
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
	/********新增参数项***************************************************/
	ui.cmbChkPerson->setCurrentIndex(lastParams->m_params->m_cperson);
	ui.lnEditVersion_2->setText(lastParams->m_params->m_temper);
	ui.lnEditVersion_3->setText(lastParams->m_params->m_humidity);
	ui.lnEditVersion_4->setText(lastParams->m_params->m_airpress);
	/********************************************************************/
}

void ParaSetDlg::installFlowPoint()
{
	// 第i流量点
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
	//自动采集
	ui.tBtn_autoPick_true->setChecked(lastParams->m_params->bo_autopick) ;
	ui.tBtn_autoPick_false->setChecked(!(lastParams->m_params->bo_autopick)) ;
	 
	 
	//调整误差
	ui.tBtn_adjustError_true->setChecked(lastParams->m_params->bo_adjerror) ;
	ui.tBtn_adjustError_false->setChecked(!(lastParams->m_params->bo_adjerror)) ;
	//写表号
	ui.tBtn_writeNum_true->setChecked(lastParams->m_params->bo_writenum) ;
	ui.tBtn_writeNum_false->setChecked(!(lastParams->m_params->bo_writenum)) ;
	 
	if (!lastParams->m_params->bo_autopick)
	{
		//调整误差
		ui.tBtn_adjustError_true->setEnabled(false);
		ui.tBtn_adjustError_false->setEnabled(false);
		 
		//写表号
		ui.tBtn_writeNum_true->setEnabled(false);
		ui.tBtn_writeNum_false->setEnabled(false);
	}

	//连续检定
	ui.tBtn_continuous_true->setChecked(lastParams->m_params->bo_converify) ;
	ui.tBtn_continuous_false->setChecked(!(lastParams->m_params->bo_converify)) ;
	
	//初值回零
	ui.tBtn_resetzero_true->setChecked(lastParams->m_params->bo_resetzero);
	ui.tBtn_resetzero_false->setChecked(!(lastParams->m_params->bo_resetzero));

	//重复检测
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
** 将各流量点中, 相似功能的控件加入数组, 便于使用; 
   这就好比界面上的一组控件代表一个关系表(关系型数据库)中的一条记录, 
   而每个控件代表记录中的不同的域
*/
void ParaSetDlg::flowPointVector()
{
	//上限流量点
	lineEdit_uppers.append(ui.lineEdit_Upper_1);
	lineEdit_uppers.append(ui.lineEdit_Upper_2);
	lineEdit_uppers.append(ui.lineEdit_Upper_3);
	lineEdit_uppers.append(ui.lineEdit_Upper_4);
	//检测流量点
	lineEdit_flows.append(ui.lnEdit_Flow1);
	lineEdit_flows.append(ui.lnEdit_Flow2);
	lineEdit_flows.append(ui.lnEdit_Flow3);
	lineEdit_flows.append(ui.lnEdit_Flow4);
	//检定量
	lineEdit_quantites.append(ui.lineEdit_Quantity_1);
	lineEdit_quantites.append(ui.lineEdit_Quantity_2);
	lineEdit_quantites.append(ui.lineEdit_Quantity_3);
	lineEdit_quantites.append(ui.lineEdit_Quantity_4);
	//控制阀
	cBox_valves.append(ui.cBox_Valve_1);
	cBox_valves.append(ui.cBox_Valve_2);
	cBox_valves.append(ui.cBox_Valve_3);
	cBox_valves.append(ui.cBox_Valve_4);
	//水泵频率
	lineEdit_freqs.append(ui.lineEdit_freq_1);
	lineEdit_freqs.append(ui.lineEdit_freq_2);
	lineEdit_freqs.append(ui.lineEdit_freq_3);
	lineEdit_freqs.append(ui.lineEdit_freq_4);
	//检定次序
	cBox_seqs.append((ui.cBox_seq_1));
	cBox_seqs.append((ui.cBox_seq_2));
	cBox_seqs.append((ui.cBox_seq_3));
	cBox_seqs.append((ui.cBox_seq_4));
	//调节阀开度
	lineEdit_Openings.append(ui.lineEdit_Opening_1);
	lineEdit_Openings.append(ui.lineEdit_Opening_2);
	lineEdit_Openings.append(ui.lineEdit_Opening_3);
	lineEdit_Openings.append(ui.lineEdit_Opening_4);
}

void ParaSetDlg::on_btnSave_clicked()
{
	//先检查选定次序的合法性
	if (!chkSeq())
	{
		QMessageBox::warning(this, tr("Warning"), tr("verify sequence is invalid!"));
		return;
	}

	//如果序列设置正确, 再检查各序列设置的检定量和是否合理
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

	//保存数据库表T_Meter_Default_Params
	Update_T_Meter_Default_Params();

//	QMessageBox::information(this, tr("OK"), tr("Save successfully!"));
	emit saveSuccessSignal();
	ui.labelHintInfo->setText(tr("Save successfully!"));
	wait(CYCLE_TIME);
	ui.labelHintInfo->clear();
}

/*
** 检查被用户选定的检定序列号是否合法
   检定序列里必须是以1起始, 中间不间断的自然数序列
   比如: [1]; [1, 2]; [1, 2, 3]; 依此类推.
   假设有一正整数序列
   设n为其最大元素值
   若此序列无重复项,
   且n与序列的元素个数相等
   那么这个序列一定是以1起始,以n终止的自然数列
*/
bool ParaSetDlg::chkSeq()
{
	int max_seq = 0;//最大的次序号
	int total_seqs = 0;//非零的检定次序号的个数
	QVector<int> repeat_seq;//已发现的元素序列

	for (int i=0; i < VERIFY_POINTS; i++)
	{
		int current_seq = cBox_seqs[i]->currentIndex();

		if (current_seq > 0)//本函数中的自然数序列不包含零元素
		{
			//挑出最大的!*非零元素*!次序号
			if (max_seq < current_seq)
			{
				max_seq = current_seq;
			}

			total_seqs ++;//计算!*非零元素*!的个数
			//如果存在*重复*的!*非零元素*!, 则返回false;否则将当前元素加入已发现的元素序列
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
	//如果最大的次序号为0, 则认为用户漏选了检定次序
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
	
	/********新增参数项***************************************************/
	settings->setValue("chkperson",ui.cmbChkPerson->currentIndex());
	m_curCheckPersonIdx = ui.cmbChkPerson->currentIndex();
	settings->setValue("temper",ui.lnEditVersion_2->text());
	settings->setValue("humidity",ui.lnEditVersion_3->text());
	settings->setValue("airpress",ui.lnEditVersion_4->text());
	/********************************************************************/
	settings->endGroup();
}

/*
** 保存第i流量点参数
** i: 界面上的第i个流量点, 而不是检定顺序 
*/
void ParaSetDlg::SaveFlowPoint()
{
	settings->beginWriteArray("FlowPoints");
	for (int i = 0; i < VERIFY_POINTS; i++) 
	{
		settings->setArrayIndex(i);
		settings->setValue("upperflow", lineEdit_uppers[i]->text());//上限流量值
		settings->setValue("verifyflow", lineEdit_flows[i]->text());//流量点
		settings->setValue("flowquantity", lineEdit_quantites[i]->text());//检定水量
		settings->setValue("pumpfrequencey", lineEdit_freqs[i]->text());//变频器频率
		settings->setValue("opening", lineEdit_Openings[i]->text());//调节阀开度
		settings->setValue("valve", cBox_valves[i]->currentIndex());//对应的阀门
		settings->setValue("seq", cBox_seqs[i]->currentIndex());//检定顺序
	}
	settings->endArray();
}

void ParaSetDlg::SaveBool()
{
	settings->beginGroup("Bool");
	settings->setValue("autopick", ui.tBtn_autoPick_true->isChecked() );//是否自动采集
	settings->setValue("adjusterror", ui.tBtn_adjustError_true->isChecked() );//是否修正误差
	settings->setValue("writemeternumber", ui.tBtn_writeNum_true->isChecked() );//是否写表号
	settings->setValue("continuouscheck", ui.tBtn_continuous_true->isChecked() );//是否连续检定
	settings->setValue("resetzero", ui.tBtn_resetzero_true->isChecked() );//是否初值回零
	settings->setValue("repeatverify", ui.chkBoxRepeatVerify->isChecked() );//是否重复检测
	settings->endGroup();
}

void ParaSetDlg::SaveOther()
{
	settings->beginGroup("Other");
	settings->setValue("flowsafecoefficient", ui.lineEdit_sc_flow->text());//流量安全系数
	settings->setValue("thermalsafecoefficient", ui.lineEdit_sc_thermal->text());//热量安全系数
	settings->setValue("exhausttime", ui.lineEdit_exTime->text());//排气时间
	settings->setValue("meternumber", ui.lineEditMeterNO->text());//起始表号
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
	//检定前的参数文件
	m_settings = new QSettings(getFullIniFileName("verifyparaset.ini"), QSettings::IniFormat);
	m_settings->setIniCodec("GB2312");//解决向ini文件中写汉字乱码

	//端口号的配置文件
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
** 将配置文件保存的comboBox索引号与
** 端口字段对应
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
** 读取配置文件的信息
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
	/////////////////////读取最大检表数/////////////////////////
	m_params->m_maxMeters = getMaxMeterByIdx(m_params->m_stand);

	///////////////////////////////////////////////////////////////////
	m_params->m_manufac = m_settings->value("head/manufacture").toInt();	
	m_params->m_model = m_settings->value("head/model").toInt();
	m_params->m_vcomp = m_settings->value("head/verifycompany").toInt();
	m_params->m_vperson = m_settings->value("head/verifyperson").toInt();
	m_params->m_pickcode = m_settings->value("head/pickcode").toInt();
	m_params->m_grade = m_settings->value("head/grade").toInt();
	m_params->m_nflowpnt = m_settings->value("head/nflowpoint").toInt();
	/******************新增参数项****************************************/
	m_params->m_cperson = m_settings->value("head/chkperson").toInt();
	m_params->m_temper = m_settings->value("head/temper").toString();
	m_params->m_humidity = m_settings->value("head/humidity").toString();
	m_params->m_airpress = m_settings->value("head/airpress").toString();
	/************************************************************************/
}

void ParaSetReader::readFlowPoints()
{
	m_params->total_fp = 0;//将流量点数目置为零
	QStringList heads = m_settings->childGroups();//配置文件中的组名
	// 第i流量点
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
** 得到检测序列号为i的流量点信息,i从1开始,
** 以m_params->total_fp结束
** 异常: i 不在[1...m_params->total_fp]之间
*/
Flow_Point_Info ParaSetReader::getFpBySeq(int i)
{
	//遍历各有效流量点; 如果当前流量点的检定次序为i, 则返回此流量点信息
	for (int j=0; j < VERIFY_POINTS; j++)
	{
		if (m_params->fp_info[j].fp_seq == i)
		{
			return m_params->fp_info[j];
		}
	}
	
	//return m_params->fp_info[0];
	throw i;//如果遍历各有效流量点后没有匹配的检定次序,那么此检定次序不存在
}

int ParaSetReader::getBigSmallBySeq(int i)
{
	//遍历各有效流量点; 如果当前流量点的检定次序为i, 则返回此流量点界面索引
	for (int j=0; j < VERIFY_POINTS; j++)
	{
		if (m_params->fp_info[j].fp_seq == i)
		{
			return j;
		}
	}

	return 0;//默认返回大流量
}