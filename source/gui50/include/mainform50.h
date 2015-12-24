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
	UserManageDlg *m_userManageDlg;//�û�Ȩ�޹���
	RegisterDlg *m_registerDlg; //ע�����
	ScanCodeDlg *m_scanCodeDlg; //ɨ��д���
	SetComDlg *m_setcom; //���ڲ�������
	DataTestDlg50 *m_datatestdlg; //�ɼ�����Ʋ��Գ���
	AdjustRateDlg *m_adjustRateDlg;//�Զ���������
	SetPortFrm *m_portSet; //�˿�ģ������
	CMasterSlave *m_masterslave; //����-�ӻ�����
	QProcess *m_comProcess; //�����ⲿ���ڵ��Թ���
	FlowWeightDlg50 *m_flowWeightDlg;      //�����춨(������)
 	FlowStandardDlg50 *m_flowStandardDlg;  //�����춨(��׼��)
	TotalWeightDlg50 *m_totalWeightDlg;    //�����춨(��׼��)
 	TotalStandardDlg50 *m_totalStandardDlg;//�����춨(��׼��)
	WaterWeightDlg50 *m_waterWeight50;     //ˮ��춨����������
	WaterStandardDlg50 *m_waterStandard50; //ˮ��춨����׼����


	CalcDlg *m_calcDlg; //�������춨
	CmbVerifyDlg *m_cmbVerifyDlg; //��ϼ춨
	tvercompDlg *m_tvercompDlg; //�¶ȼ춨(�ȽϷ�)
	tverparamDlg *m_tverparaDlg; //�¶ȼ춨(������)

	StdMtrParaSet *m_stdParaSet; //��׼������趨
	StdMtrCoeCorrect *m_stdCoeCorrect; //��׼��ϵ���궨
	stdplasensorDlg *m_stdPtParaDlg; //��׼����������趨
	chkplasensorDlg *m_chkPtParaDlg; //���첬�����������

	///////////////////////////////////////////////
	////              �춨���              ////////
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

	//ˮ��춨
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

private:
	Ui::qMainFormClass50 ui;
};

extern MainForm50 *g_mainform;

#endif // MAINFORM50_H
