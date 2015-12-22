#ifndef FLOWSTANDARD50_H
#define FLOWSTANDARD50_H

#ifdef FLOWSTANDARD50_DLL
#  ifdef WIN32
#  define FLOWSTANDARD50_EXPORT __declspec(dllexport)
#  else
#  define FLOWSTANDARD50_EXPORT
#  endif
#else
#  ifdef WIN32
#  define FLOWSTANDARD50_EXPORT __declspec(dllimport)
#  else
#  define FLOWSTANDARD50_EXPORT
#  endif
#endif

#include <QtGui/QWidget>
#include <QByteArray>
#include <QSettings>
#include <QtGui/QDataWidgetMapper>

#include "ui_flowstandard50.h"
#include "comobject.h"

class CAlgorithm;
class ParaSetDlg;
class ParaSetReader;
class ReadComConfig;
class CStdMeterReader;

class FLOWSTANDARD50_EXPORT FlowStandardDlg50 : public QWidget
{
	Q_OBJECT

public:
	FlowStandardDlg50(QWidget *parent = 0, Qt::WFlags flags = 0);
	~FlowStandardDlg50();

	QDataWidgetMapper *m_meterStdMapper;

	QTimer *m_exaustTimer; //������ʱ��

	ComThread m_tempThread;  //�¶Ȳɼ��߳�
	TempComObject *m_tempObj;
	QTimer *m_tempTimer;

	ComThread m_valveThread;   //���ſ����߳�
	ControlComObject *m_controlObj;

	ComThread m_valveThread2;   //���ſ����߳�
	ControlComObject *m_controlObj2;

	QMap<int, bool> m_valveStatus; //���ź��뷧��״̬��ӳ���ϵ
	QMap<int, QToolButton*> m_valveBtn;	//���ź��뷧�Ű�ť��ӳ���ϵ
	int m_nowPortNo;	//��ǰ���Ʒ��Ŷ˿ں�

	ComThread *m_meterThread;	//������ͨѶ�߳�
	MeterComObject *m_meterObj;	

	CAlgorithm *m_chkAlg;//�춨�����õ��ļ��㷽��

	bool m_stopFlag;     //�رս�����˳�

	//�춨������صĿ��Ʋ��� begin
	ParaSetReader *m_paraSetReader;
	ParaSetDlg *m_paraSetDlg;
	Verify_Params_PTR m_nowParams; //��ǰ�춨����
	bool m_continueVerify;    //�Ƿ������춨
	bool m_resetZero;         //�Ƿ��ֵ����
	bool m_autopick;          //�Ƿ��Զ��ɼ�
	int m_flowPointNum;       //������ĸ���
	int m_exaustSecond;		  //����ʱ��
	int m_standard;           //����
	int m_model;              //���ͺ�
	int m_pickcode;			  //�ɼ�����(�ȱ�ͨѶЭ��汾��)
	QString m_numPrefix;      //���ǰ׺(14λ��ŵ�ǰ6λ��
	float m_flowSC;           //�����춨��ȫϵ��
	bool m_adjErr;            //�Ƿ�������
	bool m_writeNO;           //�Ƿ��޸ı��
	bool m_repeatverify;      //�Ƿ��ظ����
	//�춨������صĿ��Ʋ��� end

	int m_avgTFCount;		  //����ƽ���¶Ⱥ�ƽ�������õ��ۼӼ�����
	int m_maxMeterNum;        //������������
	int m_oldMaxMeterNum;     //�ϴα�����������
	int m_validMeterNum;          //ʵ�ʱ����ĸ���
	QMap<int, int> m_meterPosMap; //������±����λ�ŵ�ӳ���ϵ
	QMap<int, float> m_gradeErrA;  //��ͬ�ȼ��ȱ��Ӧ�ı�׼������A
	QMap<int, float> m_gradeErrB;  //��ͬ�ȼ��ȱ��Ӧ�ı�׼������B
	QMap<int, float> m_mapNormalFlow;  //��ͬ����ȱ��Ӧ�ĳ�������

	float *m_meterStartValue; //�����ĳ�ֵ
	float *m_meterEndValue;   //��������ֵ
	float *m_meterTemper;	  //�������¶�
	float *m_meterDensity;    //�������ܶ�
	float *m_meterStdValue;   //�����ı�׼ֵ
	float *m_meterError;	  //���������(��ǰ�����㲻ͬ��λ�����)
	float m_stdStartVol;	  //��׼�������ֵ
	float m_stdEndVol;		  //��׼�������ֵ
	float m_StdStartMass;     //���¶������ı�׼��������ֵ
	float m_StdEndMass;       //���¶������ı�׼��������ֵ
	float m_pipeInTemper;     //����¶�
	float m_pipeOutTemper;    //�����¶�
	float m_realFlow;		  //����(m3/h��
	float **m_meterErr;       //���������(��ͬ��λ����ͬ����������)
	int *m_meterResult;       //����� 1:�ϸ�0:���ϸ�
	MeterCoe_PTR *m_oldMeterCoe; //������ԭ���ĸ�������ϵ��

	Flow_Verify_Record_PTR m_recPtr; //��Ч�ļ춨��¼
	QString m_timeStamp; //ʱ��� ����
	QString m_nowDate;  
	QString m_validDate;
	UINT32 m_newMeterNO;   //�±��
	QMap<int, UINT32> m_mapMeterPosAndNewNO; //������ϸ�ı�λ���±��ӳ���ϵ

	int m_nowOrder;				//��ǰ�춨����

	now_state m_state; //��ǰ����״̬����ʼ̬������ֵ̬������ֵ̬��

	ReadComConfig *m_readComConfig; //����������
	PortSet_Ini_STR m_portsetinfo;  //�˿�����

	QTimer *m_regSmallTimer; //��ʱ�������ڶ�̬��ʾС���ڷ��Ŀ���
	int m_smallOpening; 
	QTimer *m_regMid1Timer; //��ʱ�������ڶ�̬��ʾ��һ���ڷ��Ŀ���
	int m_mid1Opening; 
	QTimer *m_regMid2Timer; //��ʱ�������ڶ�̬��ʾ�ж����ڷ��Ŀ���
	int m_mid2Opening; 
	QTimer *m_regBigTimer;  //��ʱ�������ڶ�̬��ʾ����ڷ��Ŀ���
	int m_bigOpening; 

	QMap<int, QLineEdit*> m_RegLineEdit; //���ڷ��˿ں�����ڷ�������ʾ�ؼ���ӳ���ϵ
	QMap<int, QSpinBox*> m_RegSpinBox; //���ڷ��˿ں�����ڷ�Ŀ�꿪�ȿؼ���ӳ���ϵ
	int m_lastPortNO; //��¼��һ��������ķ��Ŷ˿ںţ�һ����·�ܶ��������ʱʹ�ã�

	void initTemperatureCom(); //�¶Ȳɼ�����
	void initControlCom();     //���ſ��ƴ���
	void initControlCom2();    //���ſ��ƴ���2
	void initMeterCom();       //��������
	void initValveStatus();	   //��ʼ������״̬
	void initRegulateStatus(); //��ʼ���綯���ڷ�״̬

	int isComAndPortNormal();   //���ڡ��˿������Ƿ�����
	int isDataCollectNormal();	//������ݲɼ��Ƿ���������ƽ���¶ȡ���������Ƶȣ�
	int isMeterPosValid(int meterPos); //�жϱ�λ���Ƿ���Ч(�ñ�λ�Ƿ���Ҫ���)
	int getValidMeterNum();       //��ȡ��Ч�ļ�����()

	void showEvent(QShowEvent * event);
	void closeEvent(QCloseEvent * event);
	void resizeEvent(QResizeEvent * event);

public slots:

	int readNowParaConfig();	 //��ȡ��ǰ�춨����
	void showNowKeyParaConfig(); //��ʾ��ǰ�ؼ�����������Ϣ
	void initTableWidget();     //���ñ������

	void on_btnStart_clicked();   //���"��ʼ"��ť
	void on_btnExhaust_clicked(); //���"����"��ť
	void on_btnGoOn_clicked();    //���"����"��ť
	void on_btnStop_clicked();    //���"��ֹ���"��ť
	void on_btnExit_clicked();    //���"�˳�"��ť
	void on_btnReCalc_clicked();  //���"���¼���"��ť
	int startExhaustCountDown();  //��ʼ��������ʱ
	void slotExaustFinished();    //����ʱ�����
	int readAllMeterFlowCoe();    //��ȡ���б���������ϵ��
	int setAllMeterVerifyStatus();//�������������춨״̬
	int openAllValveAndPump();    //�����з��ź�ˮ��
	int closeAllValveAndPumpOpenOutValve(); //�ر����з��ź�ˮ�á��򿪷�ˮ��
	int closeAllFlowPointValves();//�ر����������㷧��
	int judgeTartgetVolAndCalcAvgTemperAndFlow(double initV, double verifyV); //�ж��Ƿ���ɼ춨���춨�������ۼӽ������¶ȣ�ÿ���ۼ�һ�Σ����ڼ��������ƽ���¶�
	void stopVerify();            //ֹͣ�춨
	void startVerify();           //��ʼ�춨
	int prepareVerifyFlowPoint(int order);     //׼������������ļ춨
	int startVerifyFlowPoint(int order);       //��ʼ����������ļ춨
	int openValve(UINT8 portno);    //�򿪿��Ʒ�
	int closeValve(UINT8 portno);   //�رտ��Ʒ�
	int operateValve(UINT8 portno); //�������Ʒ����򿪻��߹ر�
	int openWaterPump();			//��ˮ��
	int closeWaterPump();			//�ر�ˮ��
	int operateWaterPump();			//����ˮ�ã��򿪻��߹ر�
	int getMeterStartValue();       //��ȡ���ֵ
	int getMeterEndValue();         //��ȡ����ֵ
	void makeStartValueByLastEndValue(); //����һ�μ춨�����ֵ��Ϊ���εĳ�ֵ
	int calcAllMeterError();      //�������б��������
	int calcMeterError(int idx);  //����ĳ��������
	int calcVerifyResult();       //����춨���

	void slotAskPipeTemperature();//�����·�¶�
	void slotFreshComTempValue(const QString& tempStr); //ˢ���¶�ֵ

	void slotSetValveBtnStatus(const UINT8 &portno, const bool &status); //�̵������سɹ���Ӧ�Ĳۺ���
	void slotSetRegulateOk();     //���ڷ����سɹ���Ӧ�Ĳۺ���

	void slotSetMeterNumber(const QString& comName, const QString& meterNumber);
	void slotSetMeterFlow(const QString& comName, const QString& flow);
	void slotFreshBigCoe(const QString& comName, const QString& bigCoe); //ˢ�´�����ϵ��
	void slotFreshMid2Coe(const QString& comName, const QString& mid2Coe); //ˢ������2����ϵ��
	void slotFreshMid1Coe(const QString& comName, const QString& mid1Coe); //ˢ������1����ϵ��
	void slotFreshSmallCoe(const QString& comName, const QString& smallCoe); //ˢ��С����ϵ��
	void setValveBtnBackColor(QToolButton *btn, bool status); //���÷��Ű�ť����ɫ
	void setRegBtnBackColor(QPushButton *btn, bool status);	  //���õ��ڷ���ť����ɫ

	void on_btnParaSet_clicked();	   //��������
	void on_btnWaterIn_clicked();      //��ˮ��
	void on_btnBigWaterIn_clicked();   //����ƽ��ˮ��
	void on_btnBigWaterOut_clicked();  //����ƽ��ˮ��
	void on_btnSmallWaterIn_clicked(); //С��ƽ��ˮ��
	void on_btnSmallWaterOut_clicked();//С��ƽ��ˮ��
	void on_btnValveBig_clicked();     //��������
	void on_btnValveMiddle1_clicked(); //����һ
	void on_btnValveMiddle2_clicked(); //������
	void on_btnValveSmall_clicked();   //С������

	void on_btnWaterPump_clicked(); //ˮ��
	void on_btnSetFreq_clicked();   //���ñ�Ƶ��Ƶ��

	void on_tableWidget_cellChanged(int row, int column);
	int saveAllVerifyRecords(); //�������б����ļ춨��¼
	void clearTableContents();

	void on_btnAllReadData_clicked();    //��������(���б�
	void on_btnAllReadNO_clicked();      //�����(���б�
	void on_btnAllVerifyStatus_clicked();//���ü춨״̬(���б�
	void on_btnAllAdjError_clicked(); //�������(���б�)
	void on_btnAllModifyNO_clicked(); //�޸ı��(���б�

	void slotModifyMeterNO(const int &row); //�޸ı��
	void slotAdjustError(const int &row);   //�������
	void slotReadData(const int &row);      //��������(������)
	void slotVerifyStatus(const int &row);  //�춨״̬
	void slotReadNO(const int &row);        //�����

	void saveStartMeterNO(); //������ʼ���


	/*******************�綯���ڷ�******************************/
	void openAllRegulator();  //�����е��ڷ����趨�Ŀ���
	void closeAllRegulator(); //�ر����е��ڷ�
	void setRegulatorOpening(int regNO, int opening); //���õ������ڷ����ȣ����ڽ�����ʾ

	void on_btnRegulateSmall_clicked();
	void on_btnRegulateMid1_clicked();
	void on_btnRegulateMid2_clicked();
	void on_btnRegulateBig_clicked();
	void askControlRegulate(int retNO, int opening); //���Ϳ��Ƶ��ڷ���������

	void on_lineEditOpeningSmall_textChanged(const QString & text);
	void on_lineEditOpeningMid1_textChanged(const QString & text);
	void on_lineEditOpeningMid2_textChanged(const QString & text);
	void on_lineEditOpeningBig_textChanged(const QString & text);

	void slotFreshSmallRegOpening();
	void slotFreshMid1RegOpening();
	void slotFreshMid2RegOpening();
	void slotFreshBigRegOpening();
	/******************�綯���ڷ�end***************************/

private slots:

signals:
	void signalClosed();

private:
	Ui::FlowStandardClass50 ui;
	/*******************��׼������******************************/
	CStdMeterReader* m_stdMeterReader;
	QMap<flow_rate_wdg, QLCDNumber *> m_mapInstWdg;
	QMap<flow_rate_wdg, QLCDNumber *> m_mapAccumWdg;
	/******************��׼������end***************************/
	void exportReport();
};

#endif //FLOWSTANDARD50_H