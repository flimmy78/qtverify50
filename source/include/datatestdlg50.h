#ifndef DATATESTDLG_H
#define DATATESTDLG_H

#ifdef DATATESTDLG_DLL
#  ifdef WIN32
#  define DATATESTDLG_EXPORT __declspec(dllexport)
#  else
#  define DATATESTDLG_EXPORT
#  endif
#else
#  ifdef WIN32
#  define DATATESTDLG_EXPORT __declspec(dllimport)
#  else
#  define DATATESTDLG_EXPORT
#  endif
#endif

#include <QtGui/QWidget>
#include <QSettings>
#include "ui_datatestdlg50.h"
#include "comobject.h"
#include "qtexdb.h"
#include "basedef.h"

class ParaSetReader;
class ParaSetDlg;
class ReadComConfig;

/*
** ������DataTestDlg50
** ���ܣ����ݲɼ������
*/

class DATATESTDLG_EXPORT DataTestDlg50 : public QWidget
{
	Q_OBJECT

public:
	DataTestDlg50(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DataTestDlg50();

	ParaSetReader *m_paraSetReader;
	ParaSetDlg *m_paraSetDlg;

	ReadComConfig *m_readComConfig; //����������

	ComThread m_tempThread;  //�¶Ȳɼ��߳�
	TempComObject *m_tempObj;
	QTimer *m_tempTimer;  //��ʱ��:���������·�¶�

	ComThread m_stdTempThread;  //��׼�¶Ȳɼ��߳�
	StdTempComObject *m_stdTempObj;
	QTimer *m_stdTempTimer;  //��ʱ��:���������׼�¶�
	stdTempCommand m_stdTempCommand;

	ComThread m_valveThread;   //���ſ����߳�
	ControlComObject *m_controlObj;

	ComThread m_valveThread2;   //���ſ����߳�2
	ControlComObject *m_controlObj2;

	QMap<int, bool> m_valveStatus;//<���Ŷ˿ںţ�����״̬>
	QMap<int, QPushButton*> m_valveBtn;	//<���Ŷ˿ںţ����Ű�ť>
	int m_nowPortNo;	//��ǰ���Ʒ��Ŷ˿ں�

	QMap<int, bool> m_regStatus; //<���ڷ��˿ںţ����ڷ�״̬>
	QMap<int, QPushButton*> m_regBtn;//<���ڷ��˿ںţ����ڷ���ť>	
	QMap<int, QLineEdit*> m_regLineEdit;//<���ڷ��˿ںţ����ڷ�������ʾ��>	
	int m_nowRegNo;  //��ǰ���ڷ��˿ں�

	ComThread m_balanceThread; //��ƽ�ɼ��߳�
	BalanceComObject *m_balanceObj;

	ComThread m_balanceThread2; //��ƽ�ɼ��߳�2
	BalanceComObject *m_balanceObj2;

	ComThread m_meterThread;  //�������߳�
	MeterComObject *m_meterObj;

	//����ƽ��������ͻ�ˮ����
	float m_balMaxWht;
	float m_balBottomWht;

	//С��ƽ��������ͻ�ˮ����
	float m_balMaxWht2;
	float m_balBottomWht2;

	//����������
	uint m_totalcount;  //������
	float m_startWeight;//��ƽ��ֵ
	float m_endWeight;  //��ƽ��ֵ
	float m_deltaWeight[FLOW_SAMPLE_NUM];
	QTimer *m_flowRateTimer;  //��ʱ��:���ڼ�������

	PortSet_Ini_STR m_portsetinfo; //50̨�ӵĶ˿�����

	MeterCoe_PTR m_oldCoe; //�ȱ�ǰ������ϵ��

	QTimer *m_regSmallTimer; //��ʱ�������ڶ�̬��ʾС���ڷ��Ŀ���
	int m_smallOpening; 
	QTimer *m_regMid1Timer; //��ʱ�������ڶ�̬��ʾ��һ���ڷ��Ŀ���
	int m_mid1Opening; 
	QTimer *m_regMid2Timer; //��ʱ�������ڶ�̬��ʾ�ж����ڷ��Ŀ���
	int m_mid2Opening; 
	QTimer *m_regBigTimer;  //��ʱ�������ڶ�̬��ʾ����ڷ��Ŀ���
	int m_bigOpening; 

	void closeEvent(QCloseEvent * event);
	void showEvent(QShowEvent *event);
	void initTemperatureCom();    //�¶Ȳɼ�����
	void initStdTemperatureCom(); //��׼�¶Ȳɼ�����
	void initBalanceCom();        //��ƽ����
	void initBalanceCom2();       //��ƽ����2
	void initControlCom();        //���ſ��ƴ���
	void initControlCom2();       //���ſ��ƴ���2
	void initComOfHeatMeter();    //������1����

	void initValveStatus();	   //��ʼ������״̬
	void initRegulateStatus(); //��ʼ�����ڷ�״̬

public slots:
	void operateRelay();
	void on_btnWaterIn_clicked(); //��ˮ��
	void on_btnBigWaterIn_clicked(); //����ƽ��ˮ��
	void on_btnBigWaterOut_clicked();//����ƽ��ˮ��
	void on_btnSmallWaterIn_clicked(); //С��ƽ��ˮ��
	void on_btnSmallWaterOut_clicked();//С��ƽ��ˮ��
	void on_btnValveBig_clicked(); //��������
	void on_btnValveMiddle1_clicked(); //����һ
	void on_btnValveMiddle2_clicked(); //������
	void on_btnValveSmall_clicked(); //С������
	void on_btnWaterPump_clicked(); //ˮ��
	void on_btnSetFreq_clicked();   //����Ƶ��

	void on_btnParaSet_clicked();
	void on_btnExit_clicked();

	void slotAskPipeTemperature();
	void slotFreshComTempValue(const QString& tempStr); //ˢ���¶�ֵ
	void slotFreshStdTempValue(const QString& stdTempStr); //ˢ�±�׼�¶�ֵ
	void slotFreshBigBalanceValue(const float& balValue);   //ˢ�´���ƽ��ֵ
	void slotFreshSmallBalanceValue(const float& balValue); //ˢ��С��ƽ��ֵ
	
	void slotFreshMeterNo(const QString& comName, const QString& meterNo);//ˢ�±��
	void slotFreshMeterFlow(const QString& comName, const QString& flow); //ˢ�±�����
	void slotFreshMeterHeat(const QString& comName, const QString& heat); //ˢ�±�����
	void slotFreshMeterDate(const QString& comName, const QString& date); //ˢ�±�����
	void slotFreshMeterTemper(const QString& comName, const QString& tempIn, const QString& tempOut); //ˢ�±����ˮ�¶�
	void slotFreshBigCoe(const QString& comName, const QString& bigCoe); //ˢ�´�����ϵ��
	void slotFreshMid2Coe(const QString& comName, const QString& mid2Coe); //ˢ������2����ϵ��
	void slotFreshMid1Coe(const QString& comName, const QString& mid1Coe); //ˢ������1����ϵ��
	void slotFreshSmallCoe(const QString& comName, const QString& smallCoe); //ˢ��С����ϵ��


	void slotSetValveBtnStatus(const UINT8 &portno, const bool &status); //�̵������سɹ���Ӧ�Ĳۺ���
	void slotSetRegulateOk();

	void setValveBtnBackColor(QPushButton *btn, bool status); //���÷��Ű�ť����ɫ
	void setRegBtnBackColor(QPushButton *btn, bool status);	//���õ��ڷ���ť����ɫ

	void slotFreshFlowRate();//��������

	void on_btnSetVerifyStatus_clicked(); //���ü춨״̬
	void on_btnExitVerifyStatus_clicked();//�˳��춨״̬
	void on_btnReadMeterData_clicked(); //��������
	void on_btnModifyMeterNo_clicked(); //�޸ı��
	void on_btnModifyFlowCoe_clicked(); //�޸�����ϵ��
	void on_btn2ModifyFlowCoe_clicked();//�����޸�����ϵ��
	void on_btnSetStandard_clicked();//���ÿھ�
	void on_btnSetTime_clicked();//����ϵͳʱ��
	void on_btnSetAddr1_clicked();//����һ����ַ
	void on_btnSetAddr2_clicked();//���ö�����ַ
	

	void on_btnOpenCom_clicked(); //�򿪴���
	void on_btnCloseCom_clicked();//�رմ���

	void on_btnStdTempCollect_clicked(); //�ɼ���׼�¶�
	void on_btnStdTempStop_clicked();    //ֹͣ�ɼ���׼�¶�
	void on_lnEditInStdResist_textChanged(const QString & text);
	void on_lnEditOutStdResist_textChanged(const QString & text);

	void slotAskStdTemperature();
	void clearMeterDispInfo();
	void setMeterComboxEnabled(bool flag);
	void setMeterOperBtnEnabled(bool flag);

private:
	Ui::DataTestDlgClass50 ui;
	/*******************��׼������******************************/
	void initInstStdCom();//˲ʱ�������ڳ�ʼ��
	void initAccumStdCom();//�ۻ��������ڳ�ʼ��

	uchar m_accumDevAddress;//��ǰ�ۻ������ɼ���ʹ�õ�����ģ���豸��ַ, Ĭ��Ϊ0x01
	uchar m_instDevAddress;//��ǰ˲ʱ�����ɼ���ʹ�õ�����ģ���豸��ַ, Ĭ��Ϊ0x01

	lcModRtuComObject *m_instantFlowCom;//˲ʱ�������ڶ���
	ComThread m_instantFlowThread;//˲ʱ�����ɼ��߳�
	QTimer* m_instSTDMeterTimer;//˲ʱ������ʱ��
	QByteArray m_instStdPulse;//˲ʱ��������ֵ, ����μӹ�

	lcModRtuComObject *m_accumulateFlowCom;//�ۻ��������ڶ���
	ComThread m_accumFlowThread;//�ۻ������ɼ��߳�
	QTimer* m_accumSTDMeterTimer;//�ۻ�������ʱ��
	QByteArray m_accumStdPulse;//16·�ۻ���������ֵ, ����μӹ�

	QList<int> m_instRouteIsRead;//˲ʱ������ͨ�����Ƿ񱻲ɼ���
	QList<int> m_accumRouteIsRead;//�ۻ�������ͨ�����Ƿ񱻲ɼ���
	QSettings *m_stdParam;//��ȡ��׼������

	int getRouteByWdg(flow_rate_wdg, flow_type);//���ݲ����Ŷ�ȡ��׼���ͨ����
	float getStdUpperFlow(flow_rate_wdg wdgIdx);//���ݲ����Ŷ�ȡ��Ӧ��׼�����������ֵ
	double getStdPulse(flow_rate_wdg wdgIdx);//���ݲ����Ŷ�ȡ��Ӧ��׼�������ֵ

	void freshInstStdMeter();//ˢ��˲ʱ����
	void freshAccumStdMeter();//ˢ���ۻ�����

	float getInstFlowRate(flow_rate_wdg idx);
	float getAccumFLowVolume(flow_rate_wdg idx);

	/******************��׼������end***************************/
	
private slots:
	/*******************��׼������******************************/
	void slotAskInstPulse();//����˲ʱ����
	void slotAskAccumPulse();//�����ۻ�����
	void slotGetInstStdMeterPulse(const QByteArray &);//˲ʱ�����ۺ���
	void slotGetAccumStdMeterPulse(const QByteArray &);//�ۻ������ۺ���
	/******************��׼������end***************************/

	void openPump();
	void closePump();

	/*******************�綯���ڷ�******************************/
	void on_btnRegulateSmall_clicked();
	void on_btnRegulateMid1_clicked();
	void on_btnRegulateMid2_clicked();
	void on_btnRegulateBig_clicked();
	void setRegulate(int retNO, int opening);

	void on_lineEditOpeningSmall_textChanged(const QString & text);
	void on_lineEditOpeningMid1_textChanged(const QString & text);
	void on_lineEditOpeningMid2_textChanged(const QString & text);
	void on_lineEditOpeningBig_textChanged(const QString & text);

	void slotFreshSmallRegOpening();
	void slotFreshMid1RegOpening();
	void slotFreshMid2RegOpening();
	void slotFreshBigRegOpening();
	/******************�綯���ڷ�end***************************/
};

#endif // DATATESTDLG_H
