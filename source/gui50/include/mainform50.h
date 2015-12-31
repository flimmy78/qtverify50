#ifndef MAINFORM50_H
#define MAINFORM50_H

#include <QtGui/QMainWindow>
#include <QtGui/QCloseEvent>
#include <QtCore/QProcess>

#include "ui_mainform50.h"

class FlowResultDlg;
class SetComDlg;
class DataTestDlg50;
class SetPortFrm;
class CMasterSlave;
class CAlgorithm;
class FlowWeightDlg50;
class FlowStandardDlg50;
class TotalWeightDlg50;
class TotalStandardDlg50;
class CalcDlg;
class CmbVerifyDlg;
class StdMtrParaSet;
class StdMtrCoeCorrect;
class tvercompDlg;
class tverparamDlg;
class stdplasensorDlg;
class chkplasensorDlg;
class PlaResultDlg;
class CalcResultDlg;
class CmbResultDlg;
class TotalResultDlg;
class WaterResultDlg;
class ScanCodeDlg;
class RegisterDlg;
class AdjustRateDlg;
class UserManageDlg;
class WaterWeightDlg50;
class WaterStandardDlg50;

class MainForm50 : public QMainWindow
{
	Q_OBJECT

public:
	MainForm50( bool licenseOK = true, int validDays = 0, QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainForm50();

	QLabel *m_probationinfo;
	CAlgorithm *m_alg;
	UserManageDlg *m_userManageDlg;//用户权限管理
	RegisterDlg *m_registerDlg; //注册管理
	ScanCodeDlg *m_scanCodeDlg; //扫码写表号
	SetComDlg *m_setcom; //串口参数设置
	DataTestDlg50 *m_datatestdlg; //采集与控制测试程序
	AdjustRateDlg *m_adjustRateDlg;//自动调整流速
	SetPortFrm *m_portSet; //端口模块设置
	CMasterSlave *m_masterslave; //主机-从机设置
	QProcess *m_comProcess; //调用外部串口调试工具
	FlowWeightDlg50 *m_flowWeightDlg;      //流量检定(质量法)
 	FlowStandardDlg50 *m_flowStandardDlg;  //流量检定(标准表法)
	TotalWeightDlg50 *m_totalWeightDlg;    //总量检定(标准表法)
 	TotalStandardDlg50 *m_totalStandardDlg;//总量检定(标准表法)
	WaterWeightDlg50 *m_waterWeight50;     //水表检定（质量法）
	WaterStandardDlg50 *m_waterStandard50; //水表检定（标准表法）


	CalcDlg *m_calcDlg; //计算器检定
	CmbVerifyDlg *m_cmbVerifyDlg; //组合检定
	tvercompDlg *m_tvercompDlg; //温度检定(比较法)
	tverparamDlg *m_tverparaDlg; //温度检定(参数法)

	StdMtrParaSet *m_stdParaSet; //标准表参数设定
	StdMtrCoeCorrect *m_stdCoeCorrect; //标准表系数标定
	stdplasensorDlg *m_stdPtParaDlg; //标准铂电阻参数设定
	chkplasensorDlg *m_chkPtParaDlg; //被检铂电阻参数设置

	///////////////////////////////////////////////
	////              检定结果              ////////
	///////////////////////////////////////////////
	FlowResultDlg *m_flowResultDlg;
	PlaResultDlg* m_PlaResultDlg;
	CalcResultDlg* m_CalcResultDlg;
	CmbResultDlg* m_CmbResultDlg;
	TotalResultDlg* m_TotalResultDlg;
	WaterResultDlg* m_WaterResultDlg;

	void closeEvent(QCloseEvent * event);

public slots:
	//SystemSet
	void on_actionScanCode_triggered();
	void on_actionComSet_triggered();
	void on_actionPortSet_triggered();
	void on_actionComDebuger_triggered();
	void on_actionDataTest_triggered();
	void on_actionAdjustFlowRate_triggered();
	void on_actionStdMtrParaSet_triggered();
	void on_actionStdMtrCoeCorrect_triggered();
	void on_actionStdPtParaSet_triggered();
	void on_actionPtParaSet_triggered();
	
	void on_actionMasterSlaveSet_triggered();

	//Component Verify
	void on_actionFlowWeight_triggered();
 	void on_actionFlowStandard_triggered();
	void on_actionPtCompare_triggered();
	void on_actionPtPara_triggered();
	void on_actionCalculator_triggered();
	void on_actionCombine_triggered();

	//水表检定
	void on_actionWaterWeight_triggered();
	void on_actionWaterStandard_triggered();

	//Total Verify
	void on_actionTotalWeight_triggered();
	void on_actionTotalStandard_triggered();

	//Data Query
	void on_actionFlowResult_triggered();
	void on_actionTotalResult_triggered();
	void on_actionWaterResult_triggered();
	void on_actionPtResult_triggered();
	void on_actionCalculatorResult_triggered();
	void on_actionCombineResult_triggered();
	void on_actionQueryExcel_triggered();

	//About
	void on_actionExit_triggered();
	void on_actionUserManage_triggered();
	void on_actionAbout_triggered();
	void on_actionRegister_triggered();

	//Style
	void on_actionDefault_triggered();
	void on_actionClassic_triggered();
	void on_actionFashion_triggered();

	//Language
	void chaneLanguage(QString lang);
	void on_actionEnglish_triggered();
	void on_actionChinese_triggered();

	//
	void processError(QProcess::ProcessError error);
	void slotRegisterSuccess();

	void slotOnStdMtrCoeCorrectClosed();
	void slotOnDataTestDlg50Closed();
private:
	Ui::qMainFormClass50 ui;
};

extern MainForm50 *g_mainform;

#endif // MAINFORM50_H
