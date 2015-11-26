/********************************************************************************
** Form generated from reading UI file 'datatestdlg50.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATATESTDLG50_H
#define UI_DATATESTDLG50_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataTestDlgClass50
{
public:
    QGridLayout *gridLayout_16;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_4;
    QPushButton *btnStdTempCollect;
    QPushButton *btnStdTempStop;
    QLabel *label_2;
    QLineEdit *lnEditInStdResist;
    QLabel *label_3;
    QLineEdit *lnEditInStdTemp;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lnEditOutStdResist;
    QLabel *label_6;
    QLineEdit *lnEditOutStdTemp;
    QLabel *label_7;
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_11;
    QPushButton *btnValveMiddle2;
    QPushButton *btnWaterIn;
    QPushButton *btnValveSmall;
    QPushButton *btnValveMiddle1;
    QPushButton *btnValveBig;
    QPushButton *btnBigWaterIn;
    QPushButton *btnSmallWaterIn;
    QPushButton *btnSmallWaterOut;
    QPushButton *btnWaterPump;
    QPushButton *btnBigWaterOut;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QLabel *label_8;
    QLineEdit *lnEditBigBalance;
    QLabel *label_13;
    QLabel *label_9;
    QLineEdit *lnEditSmallBalance;
    QLabel *label_14;
    QLabel *label_10;
    QLineEdit *lnEditFlowRate;
    QLabel *label_15;
    QLabel *label_11;
    QLineEdit *lnEditTempIn;
    QLabel *label_16;
    QLabel *label_12;
    QLineEdit *lnEditTempOut;
    QLabel *label_17;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label_18;
    QSpinBox *spinBoxFreq;
    QLabel *label_52;
    QLineEdit *lnEditMaxRate;
    QLabel *label_39;
    QSpinBox *spinBoxValveOpening;
    QPushButton *btnRegulate1;
    QPushButton *btnRegulate3;
    QPushButton *btnSetFreq;
    QLabel *label_51;
    QLineEdit *lnEditTargetRate;
    QLabel *label_50;
    QSpinBox *spinBoxValveOpening2;
    QPushButton *btnRegulate2;
    QPushButton *btnRegulate4;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QLabel *label_53;
    QLineEdit *lnEditKp;
    QLabel *label_55;
    QLineEdit *lnEditKd;
    QLabel *label_54;
    QLineEdit *lnEditKi;
    QLabel *label_56;
    QLineEdit *lnEditCycleTime;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *gBox_StdDN25;
    QGridLayout *gridLayout_15;
    QLabel *label_42;
    QLCDNumber *lcdInstStdMeter_25;
    QLabel *label_43;
    QLCDNumber *lcdAccumStdMeter_25;
    QGroupBox *gBox_StdDN10;
    QGridLayout *gridLayout_17;
    QLabel *label_44;
    QLabel *label_45;
    QLCDNumber *lcdAccumStdMeter_10;
    QLCDNumber *lcdInstStdMeter_10;
    QGroupBox *gBox_StdDN3;
    QGridLayout *gridLayout_14;
    QLabel *label_40;
    QLCDNumber *lcdInstStdMeter_3;
    QLabel *label_41;
    QLCDNumber *lcdAccumStdMeter_3;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_13;
    QLabel *label_48;
    QLabel *label_46;
    QLCDNumber *lcdAccumStdMeter;
    QLCDNumber *lcdStdMeterFlowRate;
    QLabel *label_47;
    QLabel *label_49;
    QGroupBox *groupBox_heatmeter;
    QGridLayout *gridLayout_22;
    QGridLayout *gridLayout_10;
    QLabel *label_19;
    QComboBox *portNameComboBox;
    QLabel *label_20;
    QComboBox *baudRateComboBox;
    QLabel *label_21;
    QComboBox *dataBitsComboBox;
    QLabel *label_22;
    QComboBox *parityComboBox;
    QLabel *label_23;
    QComboBox *stopBitsComboBox;
    QPushButton *btnOpenCom;
    QPushButton *btnCloseCom;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_7;
    QLabel *label_25;
    QLineEdit *lnEditMeterNo;
    QLabel *label_26;
    QLineEdit *lnEditMeterTempIn;
    QLabel *label_27;
    QLineEdit *lnEditMeterTempOut;
    QLabel *label_28;
    QLineEdit *lnEditMeterTotalFlow;
    QLabel *label_29;
    QLineEdit *lnEditMeterHeat;
    QLabel *label_31;
    QDateEdit *dateEditMeter;
    QLabel *label_32;
    QLineEdit *lnEditNewMeterNo;
    QGridLayout *gridLayout_21;
    QGridLayout *gridLayout_8;
    QLabel *label_24;
    QLabel *label_30;
    QLabel *label_37;
    QLabel *label_38;
    QLabel *label_33;
    QLineEdit *lnEditBigNewError;
    QLineEdit *lnEditBigOldError;
    QLineEdit *lnEditBigOldCoe;
    QLineEdit *lnEditBigOrgCoe;
    QLabel *label_34;
    QLineEdit *lnEditMid2NewError;
    QLineEdit *lnEditMid2OldError;
    QLineEdit *lnEditMid2OldCoe;
    QLineEdit *lnEditMid2OrgCoe;
    QLabel *label_35;
    QLineEdit *lnEditMid1NewError;
    QLineEdit *lnEditMid1OldError;
    QLineEdit *lnEditMid1OldCoe;
    QLineEdit *lnEditMid1OrgCoe;
    QLabel *label_36;
    QLineEdit *lnEditSmallNewError;
    QLineEdit *lnEditSmallOldError;
    QLineEdit *lnEditSmallOldCoe;
    QLineEdit *lnEditSmallOrgCoe;
    QPushButton *btnReadMeterData;
    QPushButton *btnSetVerifyStatus;
    QPushButton *btnModifyMeterNo;
    QPushButton *btnModifyFlowCoe;
    QPushButton *btn2ModifyFlowCoe;
    QPushButton *btnExitVerifyStatus;
    QPushButton *btnSetTime;
    QGridLayout *gridLayout_2;
    QComboBox *cmbStandard;
    QPushButton *btnSetStandard;
    QGridLayout *gridLayout_20;
    QLabel *label;
    QLineEdit *lnEditCurAddr1;
    QGridLayout *gridLayout_19;
    QLabel *label_58;
    QLineEdit *lnEditAddress1;
    QPushButton *btnSetAddr1;
    QGridLayout *gridLayout_18;
    QLabel *label_59;
    QLineEdit *lnEditAddress2;
    QPushButton *btnSetAddr2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_9;
    QPushButton *btnParaSet;
    QPushButton *btnExit;

    void setupUi(QWidget *DataTestDlgClass50)
    {
        if (DataTestDlgClass50->objectName().isEmpty())
            DataTestDlgClass50->setObjectName(QString::fromUtf8("DataTestDlgClass50"));
        DataTestDlgClass50->setWindowModality(Qt::NonModal);
        DataTestDlgClass50->resize(1027, 894);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/datatestdlg/images/datatest.png"), QSize(), QIcon::Normal, QIcon::Off);
        DataTestDlgClass50->setWindowIcon(icon);
        gridLayout_16 = new QGridLayout(DataTestDlgClass50);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        groupBox_5 = new QGroupBox(DataTestDlgClass50);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_4 = new QGridLayout(groupBox_5);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        btnStdTempCollect = new QPushButton(groupBox_5);
        btnStdTempCollect->setObjectName(QString::fromUtf8("btnStdTempCollect"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnStdTempCollect->sizePolicy().hasHeightForWidth());
        btnStdTempCollect->setSizePolicy(sizePolicy);
        btnStdTempCollect->setMinimumSize(QSize(120, 35));
        btnStdTempCollect->setMaximumSize(QSize(120, 35));

        gridLayout_4->addWidget(btnStdTempCollect, 0, 0, 1, 1);

        btnStdTempStop = new QPushButton(groupBox_5);
        btnStdTempStop->setObjectName(QString::fromUtf8("btnStdTempStop"));
        btnStdTempStop->setMinimumSize(QSize(120, 24));
        btnStdTempStop->setMaximumSize(QSize(120, 35));

        gridLayout_4->addWidget(btnStdTempStop, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 0, 2, 1, 1);

        lnEditInStdResist = new QLineEdit(groupBox_5);
        lnEditInStdResist->setObjectName(QString::fromUtf8("lnEditInStdResist"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lnEditInStdResist->sizePolicy().hasHeightForWidth());
        lnEditInStdResist->setSizePolicy(sizePolicy1);
        lnEditInStdResist->setMinimumSize(QSize(0, 30));
        lnEditInStdResist->setMaximumSize(QSize(16777215, 30));
        lnEditInStdResist->setReadOnly(false);

        gridLayout_4->addWidget(lnEditInStdResist, 0, 3, 1, 1);

        label_3 = new QLabel(groupBox_5);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 0, 4, 1, 1);

        lnEditInStdTemp = new QLineEdit(groupBox_5);
        lnEditInStdTemp->setObjectName(QString::fromUtf8("lnEditInStdTemp"));
        sizePolicy1.setHeightForWidth(lnEditInStdTemp->sizePolicy().hasHeightForWidth());
        lnEditInStdTemp->setSizePolicy(sizePolicy1);
        lnEditInStdTemp->setMinimumSize(QSize(0, 30));
        lnEditInStdTemp->setMaximumSize(QSize(16777215, 30));
        lnEditInStdTemp->setReadOnly(false);

        gridLayout_4->addWidget(lnEditInStdTemp, 0, 5, 1, 1);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 0, 6, 1, 1);

        label_5 = new QLabel(groupBox_5);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 0, 7, 1, 1);

        lnEditOutStdResist = new QLineEdit(groupBox_5);
        lnEditOutStdResist->setObjectName(QString::fromUtf8("lnEditOutStdResist"));
        sizePolicy1.setHeightForWidth(lnEditOutStdResist->sizePolicy().hasHeightForWidth());
        lnEditOutStdResist->setSizePolicy(sizePolicy1);
        lnEditOutStdResist->setMinimumSize(QSize(0, 30));
        lnEditOutStdResist->setMaximumSize(QSize(16777215, 30));
        lnEditOutStdResist->setReadOnly(false);

        gridLayout_4->addWidget(lnEditOutStdResist, 0, 8, 1, 1);

        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 0, 9, 1, 1);

        lnEditOutStdTemp = new QLineEdit(groupBox_5);
        lnEditOutStdTemp->setObjectName(QString::fromUtf8("lnEditOutStdTemp"));
        sizePolicy1.setHeightForWidth(lnEditOutStdTemp->sizePolicy().hasHeightForWidth());
        lnEditOutStdTemp->setSizePolicy(sizePolicy1);
        lnEditOutStdTemp->setMinimumSize(QSize(0, 30));
        lnEditOutStdTemp->setMaximumSize(QSize(16777215, 30));
        lnEditOutStdTemp->setReadOnly(false);

        gridLayout_4->addWidget(lnEditOutStdTemp, 0, 10, 1, 1);

        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 0, 11, 1, 1);


        gridLayout_16->addWidget(groupBox_5, 0, 0, 1, 2);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        groupBox_2 = new QGroupBox(DataTestDlgClass50);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        gridLayout_11 = new QGridLayout(groupBox_2);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        btnValveMiddle2 = new QPushButton(groupBox_2);
        btnValveMiddle2->setObjectName(QString::fromUtf8("btnValveMiddle2"));
        sizePolicy.setHeightForWidth(btnValveMiddle2->sizePolicy().hasHeightForWidth());
        btnValveMiddle2->setSizePolicy(sizePolicy);
        btnValveMiddle2->setMinimumSize(QSize(55, 110));
        btnValveMiddle2->setMaximumSize(QSize(55, 110));
        btnValveMiddle2->setLayoutDirection(Qt::LeftToRight);
        btnValveMiddle2->setAutoFillBackground(false);
        btnValveMiddle2->setAutoDefault(false);
        btnValveMiddle2->setDefault(false);
        btnValveMiddle2->setFlat(false);

        gridLayout_11->addWidget(btnValveMiddle2, 0, 3, 1, 1);

        btnWaterIn = new QPushButton(groupBox_2);
        btnWaterIn->setObjectName(QString::fromUtf8("btnWaterIn"));
        sizePolicy.setHeightForWidth(btnWaterIn->sizePolicy().hasHeightForWidth());
        btnWaterIn->setSizePolicy(sizePolicy);
        btnWaterIn->setMinimumSize(QSize(55, 110));
        btnWaterIn->setMaximumSize(QSize(55, 110));
        btnWaterIn->setLayoutDirection(Qt::LeftToRight);
        btnWaterIn->setAutoFillBackground(false);
        btnWaterIn->setAutoDefault(false);
        btnWaterIn->setDefault(false);
        btnWaterIn->setFlat(false);

        gridLayout_11->addWidget(btnWaterIn, 0, 0, 1, 1);

        btnValveSmall = new QPushButton(groupBox_2);
        btnValveSmall->setObjectName(QString::fromUtf8("btnValveSmall"));
        sizePolicy.setHeightForWidth(btnValveSmall->sizePolicy().hasHeightForWidth());
        btnValveSmall->setSizePolicy(sizePolicy);
        btnValveSmall->setMinimumSize(QSize(55, 110));
        btnValveSmall->setMaximumSize(QSize(55, 110));
        btnValveSmall->setLayoutDirection(Qt::LeftToRight);
        btnValveSmall->setAutoFillBackground(false);
        btnValveSmall->setAutoDefault(false);
        btnValveSmall->setDefault(false);
        btnValveSmall->setFlat(false);

        gridLayout_11->addWidget(btnValveSmall, 0, 1, 1, 1);

        btnValveMiddle1 = new QPushButton(groupBox_2);
        btnValveMiddle1->setObjectName(QString::fromUtf8("btnValveMiddle1"));
        sizePolicy.setHeightForWidth(btnValveMiddle1->sizePolicy().hasHeightForWidth());
        btnValveMiddle1->setSizePolicy(sizePolicy);
        btnValveMiddle1->setMinimumSize(QSize(55, 110));
        btnValveMiddle1->setMaximumSize(QSize(55, 110));
        btnValveMiddle1->setLayoutDirection(Qt::LeftToRight);
        btnValveMiddle1->setAutoFillBackground(false);
        btnValveMiddle1->setAutoDefault(false);
        btnValveMiddle1->setDefault(false);
        btnValveMiddle1->setFlat(false);

        gridLayout_11->addWidget(btnValveMiddle1, 0, 2, 1, 1);

        btnValveBig = new QPushButton(groupBox_2);
        btnValveBig->setObjectName(QString::fromUtf8("btnValveBig"));
        sizePolicy.setHeightForWidth(btnValveBig->sizePolicy().hasHeightForWidth());
        btnValveBig->setSizePolicy(sizePolicy);
        btnValveBig->setMinimumSize(QSize(55, 110));
        btnValveBig->setMaximumSize(QSize(55, 110));
        btnValveBig->setLayoutDirection(Qt::LeftToRight);
        btnValveBig->setAutoFillBackground(false);
        btnValveBig->setAutoDefault(false);
        btnValveBig->setDefault(false);
        btnValveBig->setFlat(false);

        gridLayout_11->addWidget(btnValveBig, 0, 4, 1, 1);

        btnBigWaterIn = new QPushButton(groupBox_2);
        btnBigWaterIn->setObjectName(QString::fromUtf8("btnBigWaterIn"));
        sizePolicy.setHeightForWidth(btnBigWaterIn->sizePolicy().hasHeightForWidth());
        btnBigWaterIn->setSizePolicy(sizePolicy);
        btnBigWaterIn->setMinimumSize(QSize(55, 110));
        btnBigWaterIn->setMaximumSize(QSize(55, 110));
        btnBigWaterIn->setLayoutDirection(Qt::LeftToRight);
        btnBigWaterIn->setAutoFillBackground(false);
        btnBigWaterIn->setAutoDefault(false);
        btnBigWaterIn->setDefault(false);
        btnBigWaterIn->setFlat(false);

        gridLayout_11->addWidget(btnBigWaterIn, 0, 5, 1, 1);

        btnSmallWaterIn = new QPushButton(groupBox_2);
        btnSmallWaterIn->setObjectName(QString::fromUtf8("btnSmallWaterIn"));
        sizePolicy.setHeightForWidth(btnSmallWaterIn->sizePolicy().hasHeightForWidth());
        btnSmallWaterIn->setSizePolicy(sizePolicy);
        btnSmallWaterIn->setMinimumSize(QSize(55, 110));
        btnSmallWaterIn->setMaximumSize(QSize(55, 110));
        btnSmallWaterIn->setLayoutDirection(Qt::LeftToRight);
        btnSmallWaterIn->setAutoFillBackground(false);
        btnSmallWaterIn->setAutoDefault(false);
        btnSmallWaterIn->setDefault(false);
        btnSmallWaterIn->setFlat(false);

        gridLayout_11->addWidget(btnSmallWaterIn, 0, 7, 1, 1);

        btnSmallWaterOut = new QPushButton(groupBox_2);
        btnSmallWaterOut->setObjectName(QString::fromUtf8("btnSmallWaterOut"));
        sizePolicy.setHeightForWidth(btnSmallWaterOut->sizePolicy().hasHeightForWidth());
        btnSmallWaterOut->setSizePolicy(sizePolicy);
        btnSmallWaterOut->setMinimumSize(QSize(55, 110));
        btnSmallWaterOut->setMaximumSize(QSize(55, 110));
        btnSmallWaterOut->setLayoutDirection(Qt::LeftToRight);
        btnSmallWaterOut->setAutoFillBackground(false);
        btnSmallWaterOut->setAutoDefault(false);
        btnSmallWaterOut->setDefault(false);
        btnSmallWaterOut->setFlat(false);

        gridLayout_11->addWidget(btnSmallWaterOut, 0, 9, 1, 1);

        btnWaterPump = new QPushButton(groupBox_2);
        btnWaterPump->setObjectName(QString::fromUtf8("btnWaterPump"));
        sizePolicy.setHeightForWidth(btnWaterPump->sizePolicy().hasHeightForWidth());
        btnWaterPump->setSizePolicy(sizePolicy);
        btnWaterPump->setMinimumSize(QSize(55, 110));
        btnWaterPump->setMaximumSize(QSize(55, 110));
        btnWaterPump->setLayoutDirection(Qt::LeftToRight);
        btnWaterPump->setAutoFillBackground(false);
        btnWaterPump->setAutoDefault(false);
        btnWaterPump->setDefault(false);
        btnWaterPump->setFlat(false);

        gridLayout_11->addWidget(btnWaterPump, 0, 10, 1, 1);

        btnBigWaterOut = new QPushButton(groupBox_2);
        btnBigWaterOut->setObjectName(QString::fromUtf8("btnBigWaterOut"));
        sizePolicy.setHeightForWidth(btnBigWaterOut->sizePolicy().hasHeightForWidth());
        btnBigWaterOut->setSizePolicy(sizePolicy);
        btnBigWaterOut->setMinimumSize(QSize(55, 110));
        btnBigWaterOut->setMaximumSize(QSize(55, 110));
        btnBigWaterOut->setLayoutDirection(Qt::LeftToRight);
        btnBigWaterOut->setAutoFillBackground(false);
        btnBigWaterOut->setAutoDefault(false);
        btnBigWaterOut->setDefault(false);
        btnBigWaterOut->setFlat(false);

        gridLayout_11->addWidget(btnBigWaterOut, 0, 6, 1, 1);


        gridLayout_12->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(DataTestDlgClass50);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_6->addWidget(label_8, 0, 0, 1, 1);

        lnEditBigBalance = new QLineEdit(groupBox_3);
        lnEditBigBalance->setObjectName(QString::fromUtf8("lnEditBigBalance"));
        sizePolicy1.setHeightForWidth(lnEditBigBalance->sizePolicy().hasHeightForWidth());
        lnEditBigBalance->setSizePolicy(sizePolicy1);
        lnEditBigBalance->setReadOnly(true);

        gridLayout_6->addWidget(lnEditBigBalance, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_6->addWidget(label_13, 0, 2, 1, 1);

        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_6->addWidget(label_9, 1, 0, 1, 1);

        lnEditSmallBalance = new QLineEdit(groupBox_3);
        lnEditSmallBalance->setObjectName(QString::fromUtf8("lnEditSmallBalance"));
        sizePolicy1.setHeightForWidth(lnEditSmallBalance->sizePolicy().hasHeightForWidth());
        lnEditSmallBalance->setSizePolicy(sizePolicy1);
        lnEditSmallBalance->setReadOnly(true);

        gridLayout_6->addWidget(lnEditSmallBalance, 1, 1, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout_6->addWidget(label_14, 1, 2, 1, 1);

        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_6->addWidget(label_10, 2, 0, 1, 1);

        lnEditFlowRate = new QLineEdit(groupBox_3);
        lnEditFlowRate->setObjectName(QString::fromUtf8("lnEditFlowRate"));
        sizePolicy1.setHeightForWidth(lnEditFlowRate->sizePolicy().hasHeightForWidth());
        lnEditFlowRate->setSizePolicy(sizePolicy1);
        lnEditFlowRate->setReadOnly(false);

        gridLayout_6->addWidget(lnEditFlowRate, 2, 1, 1, 1);

        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_6->addWidget(label_15, 2, 2, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_6->addWidget(label_11, 3, 0, 1, 1);

        lnEditTempIn = new QLineEdit(groupBox_3);
        lnEditTempIn->setObjectName(QString::fromUtf8("lnEditTempIn"));
        sizePolicy1.setHeightForWidth(lnEditTempIn->sizePolicy().hasHeightForWidth());
        lnEditTempIn->setSizePolicy(sizePolicy1);
        lnEditTempIn->setReadOnly(true);

        gridLayout_6->addWidget(lnEditTempIn, 3, 1, 1, 1);

        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_6->addWidget(label_16, 3, 2, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_6->addWidget(label_12, 4, 0, 1, 1);

        lnEditTempOut = new QLineEdit(groupBox_3);
        lnEditTempOut->setObjectName(QString::fromUtf8("lnEditTempOut"));
        sizePolicy1.setHeightForWidth(lnEditTempOut->sizePolicy().hasHeightForWidth());
        lnEditTempOut->setSizePolicy(sizePolicy1);
        lnEditTempOut->setReadOnly(true);

        gridLayout_6->addWidget(lnEditTempOut, 4, 1, 1, 1);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_6->addWidget(label_17, 4, 2, 1, 1);


        gridLayout_12->addWidget(groupBox_3, 0, 1, 1, 1);


        gridLayout_16->addLayout(gridLayout_12, 1, 0, 1, 2);

        groupBox = new QGroupBox(DataTestDlgClass50);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_18 = new QLabel(groupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy);
        label_18->setMinimumSize(QSize(45, 30));
        label_18->setMaximumSize(QSize(45, 30));

        gridLayout->addWidget(label_18, 0, 0, 1, 1);

        spinBoxFreq = new QSpinBox(groupBox);
        spinBoxFreq->setObjectName(QString::fromUtf8("spinBoxFreq"));
        sizePolicy1.setHeightForWidth(spinBoxFreq->sizePolicy().hasHeightForWidth());
        spinBoxFreq->setSizePolicy(sizePolicy1);
        spinBoxFreq->setMinimumSize(QSize(0, 30));
        spinBoxFreq->setMaximumSize(QSize(16777215, 30));
        spinBoxFreq->setMaximum(9999);

        gridLayout->addWidget(spinBoxFreq, 0, 1, 1, 1);

        label_52 = new QLabel(groupBox);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        sizePolicy.setHeightForWidth(label_52->sizePolicy().hasHeightForWidth());
        label_52->setSizePolicy(sizePolicy);
        label_52->setMinimumSize(QSize(60, 30));
        label_52->setMaximumSize(QSize(1000, 30));

        gridLayout->addWidget(label_52, 0, 2, 1, 1);

        lnEditMaxRate = new QLineEdit(groupBox);
        lnEditMaxRate->setObjectName(QString::fromUtf8("lnEditMaxRate"));
        sizePolicy1.setHeightForWidth(lnEditMaxRate->sizePolicy().hasHeightForWidth());
        lnEditMaxRate->setSizePolicy(sizePolicy1);
        lnEditMaxRate->setMinimumSize(QSize(50, 30));
        lnEditMaxRate->setMaximumSize(QSize(16777215, 30));
        lnEditMaxRate->setReadOnly(false);

        gridLayout->addWidget(lnEditMaxRate, 0, 3, 1, 1);

        label_39 = new QLabel(groupBox);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        sizePolicy.setHeightForWidth(label_39->sizePolicy().hasHeightForWidth());
        label_39->setSizePolicy(sizePolicy);
        label_39->setMinimumSize(QSize(60, 30));
        label_39->setMaximumSize(QSize(60, 30));

        gridLayout->addWidget(label_39, 0, 4, 1, 1);

        spinBoxValveOpening = new QSpinBox(groupBox);
        spinBoxValveOpening->setObjectName(QString::fromUtf8("spinBoxValveOpening"));
        sizePolicy1.setHeightForWidth(spinBoxValveOpening->sizePolicy().hasHeightForWidth());
        spinBoxValveOpening->setSizePolicy(sizePolicy1);
        spinBoxValveOpening->setMinimumSize(QSize(0, 30));
        spinBoxValveOpening->setMaximumSize(QSize(16777215, 30));
        spinBoxValveOpening->setMaximum(100);

        gridLayout->addWidget(spinBoxValveOpening, 0, 5, 1, 1);

        btnRegulate1 = new QPushButton(groupBox);
        btnRegulate1->setObjectName(QString::fromUtf8("btnRegulate1"));
        sizePolicy.setHeightForWidth(btnRegulate1->sizePolicy().hasHeightForWidth());
        btnRegulate1->setSizePolicy(sizePolicy);
        btnRegulate1->setMinimumSize(QSize(80, 35));
        btnRegulate1->setMaximumSize(QSize(100, 35));

        gridLayout->addWidget(btnRegulate1, 0, 6, 1, 1);

        btnRegulate3 = new QPushButton(groupBox);
        btnRegulate3->setObjectName(QString::fromUtf8("btnRegulate3"));
        sizePolicy.setHeightForWidth(btnRegulate3->sizePolicy().hasHeightForWidth());
        btnRegulate3->setSizePolicy(sizePolicy);
        btnRegulate3->setMinimumSize(QSize(80, 35));
        btnRegulate3->setMaximumSize(QSize(100, 35));

        gridLayout->addWidget(btnRegulate3, 0, 7, 1, 1);

        btnSetFreq = new QPushButton(groupBox);
        btnSetFreq->setObjectName(QString::fromUtf8("btnSetFreq"));
        sizePolicy.setHeightForWidth(btnSetFreq->sizePolicy().hasHeightForWidth());
        btnSetFreq->setSizePolicy(sizePolicy);
        btnSetFreq->setMinimumSize(QSize(100, 35));
        btnSetFreq->setMaximumSize(QSize(100, 35));

        gridLayout->addWidget(btnSetFreq, 1, 0, 1, 2);

        label_51 = new QLabel(groupBox);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        sizePolicy.setHeightForWidth(label_51->sizePolicy().hasHeightForWidth());
        label_51->setSizePolicy(sizePolicy);
        label_51->setMinimumSize(QSize(60, 30));
        label_51->setMaximumSize(QSize(1000, 30));

        gridLayout->addWidget(label_51, 1, 2, 1, 1);

        lnEditTargetRate = new QLineEdit(groupBox);
        lnEditTargetRate->setObjectName(QString::fromUtf8("lnEditTargetRate"));
        sizePolicy1.setHeightForWidth(lnEditTargetRate->sizePolicy().hasHeightForWidth());
        lnEditTargetRate->setSizePolicy(sizePolicy1);
        lnEditTargetRate->setMinimumSize(QSize(50, 30));
        lnEditTargetRate->setMaximumSize(QSize(16777215, 30));
        lnEditTargetRate->setReadOnly(false);

        gridLayout->addWidget(lnEditTargetRate, 1, 3, 1, 1);

        label_50 = new QLabel(groupBox);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        sizePolicy.setHeightForWidth(label_50->sizePolicy().hasHeightForWidth());
        label_50->setSizePolicy(sizePolicy);
        label_50->setMinimumSize(QSize(60, 30));
        label_50->setMaximumSize(QSize(60, 30));

        gridLayout->addWidget(label_50, 1, 4, 1, 1);

        spinBoxValveOpening2 = new QSpinBox(groupBox);
        spinBoxValveOpening2->setObjectName(QString::fromUtf8("spinBoxValveOpening2"));
        sizePolicy1.setHeightForWidth(spinBoxValveOpening2->sizePolicy().hasHeightForWidth());
        spinBoxValveOpening2->setSizePolicy(sizePolicy1);
        spinBoxValveOpening2->setMinimumSize(QSize(0, 30));
        spinBoxValveOpening2->setMaximumSize(QSize(16777215, 30));
        spinBoxValveOpening2->setMaximum(100);

        gridLayout->addWidget(spinBoxValveOpening2, 1, 5, 1, 1);

        btnRegulate2 = new QPushButton(groupBox);
        btnRegulate2->setObjectName(QString::fromUtf8("btnRegulate2"));
        sizePolicy.setHeightForWidth(btnRegulate2->sizePolicy().hasHeightForWidth());
        btnRegulate2->setSizePolicy(sizePolicy);
        btnRegulate2->setMinimumSize(QSize(80, 35));
        btnRegulate2->setMaximumSize(QSize(100, 35));

        gridLayout->addWidget(btnRegulate2, 1, 6, 1, 1);

        btnRegulate4 = new QPushButton(groupBox);
        btnRegulate4->setObjectName(QString::fromUtf8("btnRegulate4"));
        sizePolicy.setHeightForWidth(btnRegulate4->sizePolicy().hasHeightForWidth());
        btnRegulate4->setSizePolicy(sizePolicy);
        btnRegulate4->setMinimumSize(QSize(80, 35));
        btnRegulate4->setMaximumSize(QSize(100, 35));

        gridLayout->addWidget(btnRegulate4, 1, 7, 1, 1);

        spinBoxValveOpening->raise();
        spinBoxValveOpening2->raise();
        label_50->raise();
        btnRegulate1->raise();
        label_39->raise();
        btnSetFreq->raise();
        lnEditMaxRate->raise();
        label_18->raise();
        label_51->raise();
        label_52->raise();
        btnRegulate2->raise();
        spinBoxFreq->raise();
        lnEditTargetRate->raise();
        btnRegulate3->raise();
        btnRegulate4->raise();

        gridLayout_16->addWidget(groupBox, 2, 0, 1, 1);

        groupBox_4 = new QGroupBox(DataTestDlgClass50);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_53 = new QLabel(groupBox_4);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        sizePolicy.setHeightForWidth(label_53->sizePolicy().hasHeightForWidth());
        label_53->setSizePolicy(sizePolicy);
        label_53->setMinimumSize(QSize(30, 30));
        label_53->setMaximumSize(QSize(1000, 30));

        gridLayout_3->addWidget(label_53, 0, 0, 1, 1);

        lnEditKp = new QLineEdit(groupBox_4);
        lnEditKp->setObjectName(QString::fromUtf8("lnEditKp"));
        sizePolicy1.setHeightForWidth(lnEditKp->sizePolicy().hasHeightForWidth());
        lnEditKp->setSizePolicy(sizePolicy1);
        lnEditKp->setMinimumSize(QSize(50, 30));
        lnEditKp->setMaximumSize(QSize(16777215, 30));
        lnEditKp->setReadOnly(false);

        gridLayout_3->addWidget(lnEditKp, 0, 1, 1, 1);

        label_55 = new QLabel(groupBox_4);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        sizePolicy.setHeightForWidth(label_55->sizePolicy().hasHeightForWidth());
        label_55->setSizePolicy(sizePolicy);
        label_55->setMinimumSize(QSize(30, 30));
        label_55->setMaximumSize(QSize(1000, 30));

        gridLayout_3->addWidget(label_55, 0, 2, 1, 1);

        lnEditKd = new QLineEdit(groupBox_4);
        lnEditKd->setObjectName(QString::fromUtf8("lnEditKd"));
        sizePolicy1.setHeightForWidth(lnEditKd->sizePolicy().hasHeightForWidth());
        lnEditKd->setSizePolicy(sizePolicy1);
        lnEditKd->setMinimumSize(QSize(50, 30));
        lnEditKd->setMaximumSize(QSize(16777215, 30));
        lnEditKd->setReadOnly(false);

        gridLayout_3->addWidget(lnEditKd, 0, 3, 1, 1);

        label_54 = new QLabel(groupBox_4);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        sizePolicy.setHeightForWidth(label_54->sizePolicy().hasHeightForWidth());
        label_54->setSizePolicy(sizePolicy);
        label_54->setMinimumSize(QSize(30, 30));
        label_54->setMaximumSize(QSize(1000, 30));

        gridLayout_3->addWidget(label_54, 1, 0, 1, 1);

        lnEditKi = new QLineEdit(groupBox_4);
        lnEditKi->setObjectName(QString::fromUtf8("lnEditKi"));
        sizePolicy1.setHeightForWidth(lnEditKi->sizePolicy().hasHeightForWidth());
        lnEditKi->setSizePolicy(sizePolicy1);
        lnEditKi->setMinimumSize(QSize(50, 30));
        lnEditKi->setMaximumSize(QSize(16777215, 30));
        lnEditKi->setReadOnly(false);

        gridLayout_3->addWidget(lnEditKi, 1, 1, 1, 1);

        label_56 = new QLabel(groupBox_4);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        sizePolicy.setHeightForWidth(label_56->sizePolicy().hasHeightForWidth());
        label_56->setSizePolicy(sizePolicy);
        label_56->setMinimumSize(QSize(30, 30));
        label_56->setMaximumSize(QSize(1000, 30));

        gridLayout_3->addWidget(label_56, 1, 2, 1, 1);

        lnEditCycleTime = new QLineEdit(groupBox_4);
        lnEditCycleTime->setObjectName(QString::fromUtf8("lnEditCycleTime"));
        sizePolicy1.setHeightForWidth(lnEditCycleTime->sizePolicy().hasHeightForWidth());
        lnEditCycleTime->setSizePolicy(sizePolicy1);
        lnEditCycleTime->setMinimumSize(QSize(50, 30));
        lnEditCycleTime->setMaximumSize(QSize(16777215, 30));
        lnEditCycleTime->setReadOnly(false);

        gridLayout_3->addWidget(lnEditCycleTime, 1, 3, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 1);


        gridLayout_16->addWidget(groupBox_4, 2, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        gBox_StdDN25 = new QGroupBox(DataTestDlgClass50);
        gBox_StdDN25->setObjectName(QString::fromUtf8("gBox_StdDN25"));
        gridLayout_15 = new QGridLayout(gBox_StdDN25);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        label_42 = new QLabel(gBox_StdDN25);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_42->sizePolicy().hasHeightForWidth());
        label_42->setSizePolicy(sizePolicy3);
        label_42->setMinimumSize(QSize(30, 20));

        gridLayout_15->addWidget(label_42, 0, 0, 1, 1);

        lcdInstStdMeter_25 = new QLCDNumber(gBox_StdDN25);
        lcdInstStdMeter_25->setObjectName(QString::fromUtf8("lcdInstStdMeter_25"));
        sizePolicy1.setHeightForWidth(lcdInstStdMeter_25->sizePolicy().hasHeightForWidth());
        lcdInstStdMeter_25->setSizePolicy(sizePolicy1);
        lcdInstStdMeter_25->setMinimumSize(QSize(40, 0));
        lcdInstStdMeter_25->setNumDigits(10);
        lcdInstStdMeter_25->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_15->addWidget(lcdInstStdMeter_25, 0, 1, 1, 1);

        label_43 = new QLabel(gBox_StdDN25);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        sizePolicy3.setHeightForWidth(label_43->sizePolicy().hasHeightForWidth());
        label_43->setSizePolicy(sizePolicy3);
        label_43->setMinimumSize(QSize(30, 20));

        gridLayout_15->addWidget(label_43, 1, 0, 1, 1);

        lcdAccumStdMeter_25 = new QLCDNumber(gBox_StdDN25);
        lcdAccumStdMeter_25->setObjectName(QString::fromUtf8("lcdAccumStdMeter_25"));
        sizePolicy1.setHeightForWidth(lcdAccumStdMeter_25->sizePolicy().hasHeightForWidth());
        lcdAccumStdMeter_25->setSizePolicy(sizePolicy1);
        lcdAccumStdMeter_25->setMinimumSize(QSize(40, 0));
        lcdAccumStdMeter_25->setNumDigits(10);
        lcdAccumStdMeter_25->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_15->addWidget(lcdAccumStdMeter_25, 1, 1, 1, 1);

        label_42->raise();
        label_43->raise();
        lcdAccumStdMeter_25->raise();
        lcdInstStdMeter_25->raise();

        horizontalLayout_3->addWidget(gBox_StdDN25);

        gBox_StdDN10 = new QGroupBox(DataTestDlgClass50);
        gBox_StdDN10->setObjectName(QString::fromUtf8("gBox_StdDN10"));
        gridLayout_17 = new QGridLayout(gBox_StdDN10);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        label_44 = new QLabel(gBox_StdDN10);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        sizePolicy3.setHeightForWidth(label_44->sizePolicy().hasHeightForWidth());
        label_44->setSizePolicy(sizePolicy3);
        label_44->setMinimumSize(QSize(30, 20));

        gridLayout_17->addWidget(label_44, 0, 0, 1, 1);

        label_45 = new QLabel(gBox_StdDN10);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        sizePolicy3.setHeightForWidth(label_45->sizePolicy().hasHeightForWidth());
        label_45->setSizePolicy(sizePolicy3);
        label_45->setMinimumSize(QSize(30, 20));

        gridLayout_17->addWidget(label_45, 1, 0, 1, 1);

        lcdAccumStdMeter_10 = new QLCDNumber(gBox_StdDN10);
        lcdAccumStdMeter_10->setObjectName(QString::fromUtf8("lcdAccumStdMeter_10"));
        sizePolicy1.setHeightForWidth(lcdAccumStdMeter_10->sizePolicy().hasHeightForWidth());
        lcdAccumStdMeter_10->setSizePolicy(sizePolicy1);
        lcdAccumStdMeter_10->setMinimumSize(QSize(40, 0));
        lcdAccumStdMeter_10->setNumDigits(10);
        lcdAccumStdMeter_10->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_17->addWidget(lcdAccumStdMeter_10, 1, 1, 1, 1);

        lcdInstStdMeter_10 = new QLCDNumber(gBox_StdDN10);
        lcdInstStdMeter_10->setObjectName(QString::fromUtf8("lcdInstStdMeter_10"));
        sizePolicy1.setHeightForWidth(lcdInstStdMeter_10->sizePolicy().hasHeightForWidth());
        lcdInstStdMeter_10->setSizePolicy(sizePolicy1);
        lcdInstStdMeter_10->setMinimumSize(QSize(40, 0));
        lcdInstStdMeter_10->setNumDigits(10);
        lcdInstStdMeter_10->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_17->addWidget(lcdInstStdMeter_10, 0, 1, 1, 1);


        horizontalLayout_3->addWidget(gBox_StdDN10);

        gBox_StdDN3 = new QGroupBox(DataTestDlgClass50);
        gBox_StdDN3->setObjectName(QString::fromUtf8("gBox_StdDN3"));
        gridLayout_14 = new QGridLayout(gBox_StdDN3);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_40 = new QLabel(gBox_StdDN3);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        sizePolicy3.setHeightForWidth(label_40->sizePolicy().hasHeightForWidth());
        label_40->setSizePolicy(sizePolicy3);
        label_40->setMinimumSize(QSize(30, 20));

        gridLayout_14->addWidget(label_40, 0, 0, 1, 1);

        lcdInstStdMeter_3 = new QLCDNumber(gBox_StdDN3);
        lcdInstStdMeter_3->setObjectName(QString::fromUtf8("lcdInstStdMeter_3"));
        sizePolicy1.setHeightForWidth(lcdInstStdMeter_3->sizePolicy().hasHeightForWidth());
        lcdInstStdMeter_3->setSizePolicy(sizePolicy1);
        lcdInstStdMeter_3->setMinimumSize(QSize(40, 0));
        lcdInstStdMeter_3->setNumDigits(10);
        lcdInstStdMeter_3->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_14->addWidget(lcdInstStdMeter_3, 0, 1, 1, 1);

        label_41 = new QLabel(gBox_StdDN3);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        sizePolicy3.setHeightForWidth(label_41->sizePolicy().hasHeightForWidth());
        label_41->setSizePolicy(sizePolicy3);
        label_41->setMinimumSize(QSize(30, 20));

        gridLayout_14->addWidget(label_41, 1, 0, 1, 1);

        lcdAccumStdMeter_3 = new QLCDNumber(gBox_StdDN3);
        lcdAccumStdMeter_3->setObjectName(QString::fromUtf8("lcdAccumStdMeter_3"));
        sizePolicy1.setHeightForWidth(lcdAccumStdMeter_3->sizePolicy().hasHeightForWidth());
        lcdAccumStdMeter_3->setSizePolicy(sizePolicy1);
        lcdAccumStdMeter_3->setMinimumSize(QSize(40, 0));
        lcdAccumStdMeter_3->setNumDigits(10);
        lcdAccumStdMeter_3->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_14->addWidget(lcdAccumStdMeter_3, 1, 1, 1, 1);


        horizontalLayout_3->addWidget(gBox_StdDN3);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        groupBox_8 = new QGroupBox(DataTestDlgClass50);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_13 = new QGridLayout(groupBox_8);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        label_48 = new QLabel(groupBox_8);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        sizePolicy3.setHeightForWidth(label_48->sizePolicy().hasHeightForWidth());
        label_48->setSizePolicy(sizePolicy3);
        label_48->setAlignment(Qt::AlignCenter);

        gridLayout_13->addWidget(label_48, 1, 0, 1, 1);

        label_46 = new QLabel(groupBox_8);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        sizePolicy3.setHeightForWidth(label_46->sizePolicy().hasHeightForWidth());
        label_46->setSizePolicy(sizePolicy3);
        label_46->setMinimumSize(QSize(70, 20));

        gridLayout_13->addWidget(label_46, 2, 0, 1, 1);

        lcdAccumStdMeter = new QLCDNumber(groupBox_8);
        lcdAccumStdMeter->setObjectName(QString::fromUtf8("lcdAccumStdMeter"));
        sizePolicy1.setHeightForWidth(lcdAccumStdMeter->sizePolicy().hasHeightForWidth());
        lcdAccumStdMeter->setSizePolicy(sizePolicy1);
        lcdAccumStdMeter->setMinimumSize(QSize(80, 0));
        lcdAccumStdMeter->setNumDigits(10);
        lcdAccumStdMeter->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_13->addWidget(lcdAccumStdMeter, 2, 1, 1, 1);

        lcdStdMeterFlowRate = new QLCDNumber(groupBox_8);
        lcdStdMeterFlowRate->setObjectName(QString::fromUtf8("lcdStdMeterFlowRate"));
        sizePolicy1.setHeightForWidth(lcdStdMeterFlowRate->sizePolicy().hasHeightForWidth());
        lcdStdMeterFlowRate->setSizePolicy(sizePolicy1);
        lcdStdMeterFlowRate->setNumDigits(5);
        lcdStdMeterFlowRate->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_13->addWidget(lcdStdMeterFlowRate, 1, 1, 1, 1);

        label_47 = new QLabel(groupBox_8);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        sizePolicy3.setHeightForWidth(label_47->sizePolicy().hasHeightForWidth());
        label_47->setSizePolicy(sizePolicy3);

        gridLayout_13->addWidget(label_47, 2, 2, 1, 1);

        label_49 = new QLabel(groupBox_8);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        sizePolicy3.setHeightForWidth(label_49->sizePolicy().hasHeightForWidth());
        label_49->setSizePolicy(sizePolicy3);

        gridLayout_13->addWidget(label_49, 1, 2, 1, 1);


        horizontalLayout_4->addWidget(groupBox_8);


        gridLayout_16->addLayout(horizontalLayout_4, 3, 0, 1, 2);

        groupBox_heatmeter = new QGroupBox(DataTestDlgClass50);
        groupBox_heatmeter->setObjectName(QString::fromUtf8("groupBox_heatmeter"));
        sizePolicy2.setHeightForWidth(groupBox_heatmeter->sizePolicy().hasHeightForWidth());
        groupBox_heatmeter->setSizePolicy(sizePolicy2);
        groupBox_heatmeter->setFlat(false);
        gridLayout_22 = new QGridLayout(groupBox_heatmeter);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        label_19 = new QLabel(groupBox_heatmeter);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);

        gridLayout_10->addWidget(label_19, 0, 0, 1, 1);

        portNameComboBox = new QComboBox(groupBox_heatmeter);
        portNameComboBox->setObjectName(QString::fromUtf8("portNameComboBox"));
        sizePolicy1.setHeightForWidth(portNameComboBox->sizePolicy().hasHeightForWidth());
        portNameComboBox->setSizePolicy(sizePolicy1);
        portNameComboBox->setMinimumSize(QSize(100, 0));
        portNameComboBox->setMaximumSize(QSize(160, 16777215));

        gridLayout_10->addWidget(portNameComboBox, 0, 1, 1, 2);

        label_20 = new QLabel(groupBox_heatmeter);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy);

        gridLayout_10->addWidget(label_20, 1, 0, 1, 1);

        baudRateComboBox = new QComboBox(groupBox_heatmeter);
        baudRateComboBox->setObjectName(QString::fromUtf8("baudRateComboBox"));
        sizePolicy1.setHeightForWidth(baudRateComboBox->sizePolicy().hasHeightForWidth());
        baudRateComboBox->setSizePolicy(sizePolicy1);
        baudRateComboBox->setMinimumSize(QSize(100, 0));
        baudRateComboBox->setMaximumSize(QSize(160, 16777215));

        gridLayout_10->addWidget(baudRateComboBox, 1, 1, 1, 2);

        label_21 = new QLabel(groupBox_heatmeter);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        sizePolicy.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy);

        gridLayout_10->addWidget(label_21, 2, 0, 1, 1);

        dataBitsComboBox = new QComboBox(groupBox_heatmeter);
        dataBitsComboBox->setObjectName(QString::fromUtf8("dataBitsComboBox"));
        sizePolicy1.setHeightForWidth(dataBitsComboBox->sizePolicy().hasHeightForWidth());
        dataBitsComboBox->setSizePolicy(sizePolicy1);
        dataBitsComboBox->setMinimumSize(QSize(100, 0));
        dataBitsComboBox->setMaximumSize(QSize(160, 16777215));

        gridLayout_10->addWidget(dataBitsComboBox, 2, 1, 1, 2);

        label_22 = new QLabel(groupBox_heatmeter);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        sizePolicy.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy);

        gridLayout_10->addWidget(label_22, 3, 0, 1, 1);

        parityComboBox = new QComboBox(groupBox_heatmeter);
        parityComboBox->setObjectName(QString::fromUtf8("parityComboBox"));
        sizePolicy1.setHeightForWidth(parityComboBox->sizePolicy().hasHeightForWidth());
        parityComboBox->setSizePolicy(sizePolicy1);
        parityComboBox->setMinimumSize(QSize(100, 0));
        parityComboBox->setMaximumSize(QSize(160, 16777215));

        gridLayout_10->addWidget(parityComboBox, 3, 1, 1, 2);

        label_23 = new QLabel(groupBox_heatmeter);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        sizePolicy.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy);

        gridLayout_10->addWidget(label_23, 4, 0, 1, 1);

        stopBitsComboBox = new QComboBox(groupBox_heatmeter);
        stopBitsComboBox->setObjectName(QString::fromUtf8("stopBitsComboBox"));
        sizePolicy1.setHeightForWidth(stopBitsComboBox->sizePolicy().hasHeightForWidth());
        stopBitsComboBox->setSizePolicy(sizePolicy1);
        stopBitsComboBox->setMinimumSize(QSize(100, 0));
        stopBitsComboBox->setMaximumSize(QSize(160, 16777215));

        gridLayout_10->addWidget(stopBitsComboBox, 4, 1, 1, 2);

        btnOpenCom = new QPushButton(groupBox_heatmeter);
        btnOpenCom->setObjectName(QString::fromUtf8("btnOpenCom"));
        sizePolicy1.setHeightForWidth(btnOpenCom->sizePolicy().hasHeightForWidth());
        btnOpenCom->setSizePolicy(sizePolicy1);
        btnOpenCom->setMinimumSize(QSize(90, 0));
        btnOpenCom->setMaximumSize(QSize(90, 16777215));

        gridLayout_10->addWidget(btnOpenCom, 5, 0, 1, 2);

        btnCloseCom = new QPushButton(groupBox_heatmeter);
        btnCloseCom->setObjectName(QString::fromUtf8("btnCloseCom"));
        sizePolicy1.setHeightForWidth(btnCloseCom->sizePolicy().hasHeightForWidth());
        btnCloseCom->setSizePolicy(sizePolicy1);
        btnCloseCom->setMinimumSize(QSize(90, 0));
        btnCloseCom->setMaximumSize(QSize(90, 16777215));

        gridLayout_10->addWidget(btnCloseCom, 5, 2, 1, 1);


        gridLayout_22->addLayout(gridLayout_10, 0, 0, 1, 1);

        groupBox_9 = new QGroupBox(groupBox_heatmeter);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_7 = new QGridLayout(groupBox_9);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label_25 = new QLabel(groupBox_9);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        sizePolicy3.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(label_25, 0, 0, 1, 1);

        lnEditMeterNo = new QLineEdit(groupBox_9);
        lnEditMeterNo->setObjectName(QString::fromUtf8("lnEditMeterNo"));
        lnEditMeterNo->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMeterNo->sizePolicy().hasHeightForWidth());
        lnEditMeterNo->setSizePolicy(sizePolicy1);
        lnEditMeterNo->setReadOnly(true);

        gridLayout_7->addWidget(lnEditMeterNo, 0, 1, 1, 1);

        label_26 = new QLabel(groupBox_9);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_7->addWidget(label_26, 1, 0, 1, 1);

        lnEditMeterTempIn = new QLineEdit(groupBox_9);
        lnEditMeterTempIn->setObjectName(QString::fromUtf8("lnEditMeterTempIn"));
        lnEditMeterTempIn->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMeterTempIn->sizePolicy().hasHeightForWidth());
        lnEditMeterTempIn->setSizePolicy(sizePolicy1);
        lnEditMeterTempIn->setReadOnly(true);

        gridLayout_7->addWidget(lnEditMeterTempIn, 1, 1, 1, 1);

        label_27 = new QLabel(groupBox_9);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_7->addWidget(label_27, 2, 0, 1, 1);

        lnEditMeterTempOut = new QLineEdit(groupBox_9);
        lnEditMeterTempOut->setObjectName(QString::fromUtf8("lnEditMeterTempOut"));
        lnEditMeterTempOut->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMeterTempOut->sizePolicy().hasHeightForWidth());
        lnEditMeterTempOut->setSizePolicy(sizePolicy1);
        lnEditMeterTempOut->setReadOnly(true);

        gridLayout_7->addWidget(lnEditMeterTempOut, 2, 1, 1, 1);

        label_28 = new QLabel(groupBox_9);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_7->addWidget(label_28, 3, 0, 1, 1);

        lnEditMeterTotalFlow = new QLineEdit(groupBox_9);
        lnEditMeterTotalFlow->setObjectName(QString::fromUtf8("lnEditMeterTotalFlow"));
        lnEditMeterTotalFlow->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMeterTotalFlow->sizePolicy().hasHeightForWidth());
        lnEditMeterTotalFlow->setSizePolicy(sizePolicy1);
        lnEditMeterTotalFlow->setReadOnly(true);

        gridLayout_7->addWidget(lnEditMeterTotalFlow, 3, 1, 1, 1);

        label_29 = new QLabel(groupBox_9);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_7->addWidget(label_29, 4, 0, 1, 1);

        lnEditMeterHeat = new QLineEdit(groupBox_9);
        lnEditMeterHeat->setObjectName(QString::fromUtf8("lnEditMeterHeat"));
        lnEditMeterHeat->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMeterHeat->sizePolicy().hasHeightForWidth());
        lnEditMeterHeat->setSizePolicy(sizePolicy1);
        lnEditMeterHeat->setReadOnly(true);

        gridLayout_7->addWidget(lnEditMeterHeat, 4, 1, 1, 1);

        label_31 = new QLabel(groupBox_9);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_7->addWidget(label_31, 5, 0, 1, 1);

        dateEditMeter = new QDateEdit(groupBox_9);
        dateEditMeter->setObjectName(QString::fromUtf8("dateEditMeter"));
        dateEditMeter->setEnabled(false);
        dateEditMeter->setReadOnly(true);

        gridLayout_7->addWidget(dateEditMeter, 5, 1, 1, 1);

        label_32 = new QLabel(groupBox_9);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_7->addWidget(label_32, 6, 0, 1, 1);

        lnEditNewMeterNo = new QLineEdit(groupBox_9);
        lnEditNewMeterNo->setObjectName(QString::fromUtf8("lnEditNewMeterNo"));
        sizePolicy1.setHeightForWidth(lnEditNewMeterNo->sizePolicy().hasHeightForWidth());
        lnEditNewMeterNo->setSizePolicy(sizePolicy1);
        lnEditNewMeterNo->setAutoFillBackground(true);

        gridLayout_7->addWidget(lnEditNewMeterNo, 6, 1, 1, 1);


        gridLayout_22->addWidget(groupBox_9, 0, 1, 2, 1);

        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_24 = new QLabel(groupBox_heatmeter);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        sizePolicy1.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(label_24, 0, 1, 1, 1);

        label_30 = new QLabel(groupBox_heatmeter);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        sizePolicy1.setHeightForWidth(label_30->sizePolicy().hasHeightForWidth());
        label_30->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(label_30, 0, 2, 1, 1);

        label_37 = new QLabel(groupBox_heatmeter);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        sizePolicy1.setHeightForWidth(label_37->sizePolicy().hasHeightForWidth());
        label_37->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(label_37, 0, 3, 1, 1);

        label_38 = new QLabel(groupBox_heatmeter);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        sizePolicy1.setHeightForWidth(label_38->sizePolicy().hasHeightForWidth());
        label_38->setSizePolicy(sizePolicy1);

        gridLayout_8->addWidget(label_38, 0, 4, 1, 1);

        label_33 = new QLabel(groupBox_heatmeter);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_8->addWidget(label_33, 1, 0, 1, 1);

        lnEditBigNewError = new QLineEdit(groupBox_heatmeter);
        lnEditBigNewError->setObjectName(QString::fromUtf8("lnEditBigNewError"));
        sizePolicy1.setHeightForWidth(lnEditBigNewError->sizePolicy().hasHeightForWidth());
        lnEditBigNewError->setSizePolicy(sizePolicy1);
        lnEditBigNewError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditBigNewError, 1, 1, 1, 1);

        lnEditBigOldError = new QLineEdit(groupBox_heatmeter);
        lnEditBigOldError->setObjectName(QString::fromUtf8("lnEditBigOldError"));
        lnEditBigOldError->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditBigOldError->sizePolicy().hasHeightForWidth());
        lnEditBigOldError->setSizePolicy(sizePolicy1);
        lnEditBigOldError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditBigOldError, 1, 2, 1, 1);

        lnEditBigOldCoe = new QLineEdit(groupBox_heatmeter);
        lnEditBigOldCoe->setObjectName(QString::fromUtf8("lnEditBigOldCoe"));
        lnEditBigOldCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditBigOldCoe->sizePolicy().hasHeightForWidth());
        lnEditBigOldCoe->setSizePolicy(sizePolicy1);
        lnEditBigOldCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditBigOldCoe, 1, 3, 1, 1);

        lnEditBigOrgCoe = new QLineEdit(groupBox_heatmeter);
        lnEditBigOrgCoe->setObjectName(QString::fromUtf8("lnEditBigOrgCoe"));
        lnEditBigOrgCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditBigOrgCoe->sizePolicy().hasHeightForWidth());
        lnEditBigOrgCoe->setSizePolicy(sizePolicy1);
        lnEditBigOrgCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditBigOrgCoe, 1, 4, 1, 1);

        label_34 = new QLabel(groupBox_heatmeter);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_8->addWidget(label_34, 2, 0, 1, 1);

        lnEditMid2NewError = new QLineEdit(groupBox_heatmeter);
        lnEditMid2NewError->setObjectName(QString::fromUtf8("lnEditMid2NewError"));
        sizePolicy1.setHeightForWidth(lnEditMid2NewError->sizePolicy().hasHeightForWidth());
        lnEditMid2NewError->setSizePolicy(sizePolicy1);
        lnEditMid2NewError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid2NewError, 2, 1, 1, 1);

        lnEditMid2OldError = new QLineEdit(groupBox_heatmeter);
        lnEditMid2OldError->setObjectName(QString::fromUtf8("lnEditMid2OldError"));
        lnEditMid2OldError->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid2OldError->sizePolicy().hasHeightForWidth());
        lnEditMid2OldError->setSizePolicy(sizePolicy1);
        lnEditMid2OldError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid2OldError, 2, 2, 1, 1);

        lnEditMid2OldCoe = new QLineEdit(groupBox_heatmeter);
        lnEditMid2OldCoe->setObjectName(QString::fromUtf8("lnEditMid2OldCoe"));
        lnEditMid2OldCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid2OldCoe->sizePolicy().hasHeightForWidth());
        lnEditMid2OldCoe->setSizePolicy(sizePolicy1);
        lnEditMid2OldCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid2OldCoe, 2, 3, 1, 1);

        lnEditMid2OrgCoe = new QLineEdit(groupBox_heatmeter);
        lnEditMid2OrgCoe->setObjectName(QString::fromUtf8("lnEditMid2OrgCoe"));
        lnEditMid2OrgCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid2OrgCoe->sizePolicy().hasHeightForWidth());
        lnEditMid2OrgCoe->setSizePolicy(sizePolicy1);
        lnEditMid2OrgCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid2OrgCoe, 2, 4, 1, 1);

        label_35 = new QLabel(groupBox_heatmeter);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_8->addWidget(label_35, 3, 0, 1, 1);

        lnEditMid1NewError = new QLineEdit(groupBox_heatmeter);
        lnEditMid1NewError->setObjectName(QString::fromUtf8("lnEditMid1NewError"));
        sizePolicy1.setHeightForWidth(lnEditMid1NewError->sizePolicy().hasHeightForWidth());
        lnEditMid1NewError->setSizePolicy(sizePolicy1);
        lnEditMid1NewError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid1NewError, 3, 1, 1, 1);

        lnEditMid1OldError = new QLineEdit(groupBox_heatmeter);
        lnEditMid1OldError->setObjectName(QString::fromUtf8("lnEditMid1OldError"));
        lnEditMid1OldError->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid1OldError->sizePolicy().hasHeightForWidth());
        lnEditMid1OldError->setSizePolicy(sizePolicy1);
        lnEditMid1OldError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid1OldError, 3, 2, 1, 1);

        lnEditMid1OldCoe = new QLineEdit(groupBox_heatmeter);
        lnEditMid1OldCoe->setObjectName(QString::fromUtf8("lnEditMid1OldCoe"));
        lnEditMid1OldCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid1OldCoe->sizePolicy().hasHeightForWidth());
        lnEditMid1OldCoe->setSizePolicy(sizePolicy1);
        lnEditMid1OldCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid1OldCoe, 3, 3, 1, 1);

        lnEditMid1OrgCoe = new QLineEdit(groupBox_heatmeter);
        lnEditMid1OrgCoe->setObjectName(QString::fromUtf8("lnEditMid1OrgCoe"));
        lnEditMid1OrgCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditMid1OrgCoe->sizePolicy().hasHeightForWidth());
        lnEditMid1OrgCoe->setSizePolicy(sizePolicy1);
        lnEditMid1OrgCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditMid1OrgCoe, 3, 4, 1, 1);

        label_36 = new QLabel(groupBox_heatmeter);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_8->addWidget(label_36, 4, 0, 1, 1);

        lnEditSmallNewError = new QLineEdit(groupBox_heatmeter);
        lnEditSmallNewError->setObjectName(QString::fromUtf8("lnEditSmallNewError"));
        sizePolicy1.setHeightForWidth(lnEditSmallNewError->sizePolicy().hasHeightForWidth());
        lnEditSmallNewError->setSizePolicy(sizePolicy1);
        lnEditSmallNewError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditSmallNewError, 4, 1, 1, 1);

        lnEditSmallOldError = new QLineEdit(groupBox_heatmeter);
        lnEditSmallOldError->setObjectName(QString::fromUtf8("lnEditSmallOldError"));
        lnEditSmallOldError->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditSmallOldError->sizePolicy().hasHeightForWidth());
        lnEditSmallOldError->setSizePolicy(sizePolicy1);
        lnEditSmallOldError->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditSmallOldError, 4, 2, 1, 1);

        lnEditSmallOldCoe = new QLineEdit(groupBox_heatmeter);
        lnEditSmallOldCoe->setObjectName(QString::fromUtf8("lnEditSmallOldCoe"));
        lnEditSmallOldCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditSmallOldCoe->sizePolicy().hasHeightForWidth());
        lnEditSmallOldCoe->setSizePolicy(sizePolicy1);
        lnEditSmallOldCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditSmallOldCoe, 4, 3, 1, 1);

        lnEditSmallOrgCoe = new QLineEdit(groupBox_heatmeter);
        lnEditSmallOrgCoe->setObjectName(QString::fromUtf8("lnEditSmallOrgCoe"));
        lnEditSmallOrgCoe->setEnabled(false);
        sizePolicy1.setHeightForWidth(lnEditSmallOrgCoe->sizePolicy().hasHeightForWidth());
        lnEditSmallOrgCoe->setSizePolicy(sizePolicy1);
        lnEditSmallOrgCoe->setAutoFillBackground(true);

        gridLayout_8->addWidget(lnEditSmallOrgCoe, 4, 4, 1, 1);


        gridLayout_21->addLayout(gridLayout_8, 0, 0, 1, 5);

        btnReadMeterData = new QPushButton(groupBox_heatmeter);
        btnReadMeterData->setObjectName(QString::fromUtf8("btnReadMeterData"));
        sizePolicy.setHeightForWidth(btnReadMeterData->sizePolicy().hasHeightForWidth());
        btnReadMeterData->setSizePolicy(sizePolicy);
        btnReadMeterData->setMinimumSize(QSize(60, 60));
        btnReadMeterData->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnReadMeterData, 1, 0, 1, 1);

        btnSetVerifyStatus = new QPushButton(groupBox_heatmeter);
        btnSetVerifyStatus->setObjectName(QString::fromUtf8("btnSetVerifyStatus"));
        sizePolicy.setHeightForWidth(btnSetVerifyStatus->sizePolicy().hasHeightForWidth());
        btnSetVerifyStatus->setSizePolicy(sizePolicy);
        btnSetVerifyStatus->setMinimumSize(QSize(60, 60));
        btnSetVerifyStatus->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnSetVerifyStatus, 1, 1, 1, 1);

        btnModifyMeterNo = new QPushButton(groupBox_heatmeter);
        btnModifyMeterNo->setObjectName(QString::fromUtf8("btnModifyMeterNo"));
        sizePolicy.setHeightForWidth(btnModifyMeterNo->sizePolicy().hasHeightForWidth());
        btnModifyMeterNo->setSizePolicy(sizePolicy);
        btnModifyMeterNo->setMinimumSize(QSize(60, 60));
        btnModifyMeterNo->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnModifyMeterNo, 1, 2, 1, 1);

        btnModifyFlowCoe = new QPushButton(groupBox_heatmeter);
        btnModifyFlowCoe->setObjectName(QString::fromUtf8("btnModifyFlowCoe"));
        sizePolicy.setHeightForWidth(btnModifyFlowCoe->sizePolicy().hasHeightForWidth());
        btnModifyFlowCoe->setSizePolicy(sizePolicy);
        btnModifyFlowCoe->setMinimumSize(QSize(60, 60));
        btnModifyFlowCoe->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnModifyFlowCoe, 1, 3, 1, 1);

        btn2ModifyFlowCoe = new QPushButton(groupBox_heatmeter);
        btn2ModifyFlowCoe->setObjectName(QString::fromUtf8("btn2ModifyFlowCoe"));
        sizePolicy.setHeightForWidth(btn2ModifyFlowCoe->sizePolicy().hasHeightForWidth());
        btn2ModifyFlowCoe->setSizePolicy(sizePolicy);
        btn2ModifyFlowCoe->setMinimumSize(QSize(60, 60));
        btn2ModifyFlowCoe->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btn2ModifyFlowCoe, 1, 4, 1, 1);

        btnExitVerifyStatus = new QPushButton(groupBox_heatmeter);
        btnExitVerifyStatus->setObjectName(QString::fromUtf8("btnExitVerifyStatus"));
        sizePolicy.setHeightForWidth(btnExitVerifyStatus->sizePolicy().hasHeightForWidth());
        btnExitVerifyStatus->setSizePolicy(sizePolicy);
        btnExitVerifyStatus->setMinimumSize(QSize(60, 60));
        btnExitVerifyStatus->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnExitVerifyStatus, 2, 0, 1, 1);

        btnSetTime = new QPushButton(groupBox_heatmeter);
        btnSetTime->setObjectName(QString::fromUtf8("btnSetTime"));
        sizePolicy.setHeightForWidth(btnSetTime->sizePolicy().hasHeightForWidth());
        btnSetTime->setSizePolicy(sizePolicy);
        btnSetTime->setMinimumSize(QSize(60, 60));
        btnSetTime->setMaximumSize(QSize(60, 60));

        gridLayout_21->addWidget(btnSetTime, 2, 1, 1, 1);


        gridLayout_22->addLayout(gridLayout_21, 0, 2, 2, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cmbStandard = new QComboBox(groupBox_heatmeter);
        cmbStandard->setObjectName(QString::fromUtf8("cmbStandard"));
        cmbStandard->setMinimumSize(QSize(0, 35));

        gridLayout_2->addWidget(cmbStandard, 0, 0, 1, 1);

        btnSetStandard = new QPushButton(groupBox_heatmeter);
        btnSetStandard->setObjectName(QString::fromUtf8("btnSetStandard"));
        sizePolicy.setHeightForWidth(btnSetStandard->sizePolicy().hasHeightForWidth());
        btnSetStandard->setSizePolicy(sizePolicy);
        btnSetStandard->setMinimumSize(QSize(105, 35));
        btnSetStandard->setMaximumSize(QSize(105, 35));

        gridLayout_2->addWidget(btnSetStandard, 0, 1, 1, 1);


        gridLayout_22->addLayout(gridLayout_2, 1, 0, 1, 1);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        label = new QLabel(groupBox_heatmeter);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_20->addWidget(label, 0, 0, 1, 1);

        lnEditCurAddr1 = new QLineEdit(groupBox_heatmeter);
        lnEditCurAddr1->setObjectName(QString::fromUtf8("lnEditCurAddr1"));
        lnEditCurAddr1->setEnabled(true);
        sizePolicy1.setHeightForWidth(lnEditCurAddr1->sizePolicy().hasHeightForWidth());
        lnEditCurAddr1->setSizePolicy(sizePolicy1);
        lnEditCurAddr1->setMaximumSize(QSize(120, 16777215));
        lnEditCurAddr1->setReadOnly(false);

        gridLayout_20->addWidget(lnEditCurAddr1, 0, 1, 1, 1);


        gridLayout_22->addLayout(gridLayout_20, 2, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        label_58 = new QLabel(groupBox_heatmeter);
        label_58->setObjectName(QString::fromUtf8("label_58"));

        gridLayout_19->addWidget(label_58, 0, 0, 1, 1);

        lnEditAddress1 = new QLineEdit(groupBox_heatmeter);
        lnEditAddress1->setObjectName(QString::fromUtf8("lnEditAddress1"));
        lnEditAddress1->setEnabled(true);
        sizePolicy1.setHeightForWidth(lnEditAddress1->sizePolicy().hasHeightForWidth());
        lnEditAddress1->setSizePolicy(sizePolicy1);
        lnEditAddress1->setMaximumSize(QSize(120, 16777215));
        lnEditAddress1->setReadOnly(false);

        gridLayout_19->addWidget(lnEditAddress1, 0, 1, 1, 1);

        btnSetAddr1 = new QPushButton(groupBox_heatmeter);
        btnSetAddr1->setObjectName(QString::fromUtf8("btnSetAddr1"));
        sizePolicy.setHeightForWidth(btnSetAddr1->sizePolicy().hasHeightForWidth());
        btnSetAddr1->setSizePolicy(sizePolicy);
        btnSetAddr1->setMinimumSize(QSize(100, 25));
        btnSetAddr1->setMaximumSize(QSize(120, 25));

        gridLayout_19->addWidget(btnSetAddr1, 0, 2, 1, 1);


        gridLayout_22->addLayout(gridLayout_19, 2, 1, 1, 1);

        gridLayout_18 = new QGridLayout();
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        label_59 = new QLabel(groupBox_heatmeter);
        label_59->setObjectName(QString::fromUtf8("label_59"));

        gridLayout_18->addWidget(label_59, 0, 0, 1, 1);

        lnEditAddress2 = new QLineEdit(groupBox_heatmeter);
        lnEditAddress2->setObjectName(QString::fromUtf8("lnEditAddress2"));
        lnEditAddress2->setEnabled(true);
        sizePolicy1.setHeightForWidth(lnEditAddress2->sizePolicy().hasHeightForWidth());
        lnEditAddress2->setSizePolicy(sizePolicy1);
        lnEditAddress2->setMinimumSize(QSize(100, 0));
        lnEditAddress2->setReadOnly(false);

        gridLayout_18->addWidget(lnEditAddress2, 0, 1, 1, 1);

        btnSetAddr2 = new QPushButton(groupBox_heatmeter);
        btnSetAddr2->setObjectName(QString::fromUtf8("btnSetAddr2"));
        sizePolicy.setHeightForWidth(btnSetAddr2->sizePolicy().hasHeightForWidth());
        btnSetAddr2->setSizePolicy(sizePolicy);
        btnSetAddr2->setMinimumSize(QSize(100, 25));
        btnSetAddr2->setMaximumSize(QSize(120, 25));

        gridLayout_18->addWidget(btnSetAddr2, 0, 2, 1, 1);


        gridLayout_22->addLayout(gridLayout_18, 2, 2, 1, 1);


        gridLayout_16->addWidget(groupBox_heatmeter, 4, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(731, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        btnParaSet = new QPushButton(DataTestDlgClass50);
        btnParaSet->setObjectName(QString::fromUtf8("btnParaSet"));
        sizePolicy.setHeightForWidth(btnParaSet->sizePolicy().hasHeightForWidth());
        btnParaSet->setSizePolicy(sizePolicy);
        btnParaSet->setMinimumSize(QSize(180, 35));
        btnParaSet->setMaximumSize(QSize(180, 35));
        btnParaSet->setAutoFillBackground(true);

        gridLayout_9->addWidget(btnParaSet, 0, 0, 1, 1);

        btnExit = new QPushButton(DataTestDlgClass50);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        sizePolicy.setHeightForWidth(btnExit->sizePolicy().hasHeightForWidth());
        btnExit->setSizePolicy(sizePolicy);
        btnExit->setMinimumSize(QSize(180, 35));
        btnExit->setMaximumSize(QSize(180, 35));

        gridLayout_9->addWidget(btnExit, 0, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout_9);


        gridLayout_16->addLayout(horizontalLayout, 5, 0, 1, 2);


        retranslateUi(DataTestDlgClass50);

        baudRateComboBox->setCurrentIndex(0);
        dataBitsComboBox->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(DataTestDlgClass50);
    } // setupUi

    void retranslateUi(QWidget *DataTestDlgClass50)
    {
        DataTestDlgClass50->setWindowTitle(QApplication::translate("DataTestDlgClass50", "DA and Test", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("DataTestDlgClass50", "Std  Thermometer", 0, QApplication::UnicodeUTF8));
        btnStdTempCollect->setText(QApplication::translate("DataTestDlgClass50", "Collect", 0, QApplication::UnicodeUTF8));
        btnStdTempStop->setText(QApplication::translate("DataTestDlgClass50", "Stop", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DataTestDlgClass50", "In", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DataTestDlgClass50", "\316\251", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DataTestDlgClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DataTestDlgClass50", "Out", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DataTestDlgClass50", "\316\251", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("DataTestDlgClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("DataTestDlgClass50", "Valve Control", 0, QApplication::UnicodeUTF8));
        btnValveMiddle2->setText(QApplication::translate("DataTestDlgClass50", "m\n"
"i\n"
"d\n"
"2", 0, QApplication::UnicodeUTF8));
        btnWaterIn->setText(QApplication::translate("DataTestDlgClass50", "i\n"
"n", 0, QApplication::UnicodeUTF8));
        btnValveSmall->setText(QApplication::translate("DataTestDlgClass50", "s\n"
"m\n"
"a\n"
"l\n"
"l", 0, QApplication::UnicodeUTF8));
        btnValveMiddle1->setText(QApplication::translate("DataTestDlgClass50", "m\n"
"i\n"
"d\n"
"1", 0, QApplication::UnicodeUTF8));
        btnValveBig->setText(QApplication::translate("DataTestDlgClass50", "b\n"
"i\n"
"g", 0, QApplication::UnicodeUTF8));
        btnBigWaterIn->setText(QApplication::translate("DataTestDlgClass50", "b\n"
"i\n"
"g\n"
"i\n"
"n", 0, QApplication::UnicodeUTF8));
        btnSmallWaterIn->setText(QApplication::translate("DataTestDlgClass50", "s\n"
"i\n"
"n", 0, QApplication::UnicodeUTF8));
        btnSmallWaterOut->setText(QApplication::translate("DataTestDlgClass50", "s\n"
"o\n"
"u\n"
"t", 0, QApplication::UnicodeUTF8));
        btnWaterPump->setText(QApplication::translate("DataTestDlgClass50", "p\n"
"u\n"
"m\n"
"p", 0, QApplication::UnicodeUTF8));
        btnBigWaterOut->setText(QApplication::translate("DataTestDlgClass50", "b\n"
"i\n"
"g\n"
"o\n"
"u\n"
"t", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("DataTestDlgClass50", "Data Acquisition", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("DataTestDlgClass50", "Big Balance:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("DataTestDlgClass50", "kg", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("DataTestDlgClass50", "Small Balance:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("DataTestDlgClass50", "kg", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("DataTestDlgClass50", "Flow Rate:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("DataTestDlgClass50", "m\302\263/h", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("DataTestDlgClass50", "In Temp:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("DataTestDlgClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("DataTestDlgClass50", "Out Temp:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("DataTestDlgClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DataTestDlgClass50", "Freqence and Regulate", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("DataTestDlgClass50", "Freq:", 0, QApplication::UnicodeUTF8));
        label_52->setText(QApplication::translate("DataTestDlgClass50", "Max Rate:", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("DataTestDlgClass50", "Open(%):", 0, QApplication::UnicodeUTF8));
        btnRegulate1->setText(QApplication::translate("DataTestDlgClass50", "Regulate1", 0, QApplication::UnicodeUTF8));
        btnRegulate3->setText(QApplication::translate("DataTestDlgClass50", "Regulate3", 0, QApplication::UnicodeUTF8));
        btnSetFreq->setText(QApplication::translate("DataTestDlgClass50", "SetFreq", 0, QApplication::UnicodeUTF8));
        label_51->setText(QApplication::translate("DataTestDlgClass50", "Set Rate:", 0, QApplication::UnicodeUTF8));
        label_50->setText(QApplication::translate("DataTestDlgClass50", "Open(%):", 0, QApplication::UnicodeUTF8));
        btnRegulate2->setText(QApplication::translate("DataTestDlgClass50", "Regulate2", 0, QApplication::UnicodeUTF8));
        btnRegulate4->setText(QApplication::translate("DataTestDlgClass50", "Regulate4", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("DataTestDlgClass50", "Pid Parameter", 0, QApplication::UnicodeUTF8));
        label_53->setText(QApplication::translate("DataTestDlgClass50", "Kp:", 0, QApplication::UnicodeUTF8));
        label_55->setText(QApplication::translate("DataTestDlgClass50", "Kd:", 0, QApplication::UnicodeUTF8));
        label_54->setText(QApplication::translate("DataTestDlgClass50", "Ki:", 0, QApplication::UnicodeUTF8));
        label_56->setText(QApplication::translate("DataTestDlgClass50", "T:", 0, QApplication::UnicodeUTF8));
        gBox_StdDN25->setTitle(QApplication::translate("DataTestDlgClass50", "DN25", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("DataTestDlgClass50", "Inst", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("DataTestDlgClass50", "Accum", 0, QApplication::UnicodeUTF8));
        gBox_StdDN10->setTitle(QApplication::translate("DataTestDlgClass50", "DN10", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("DataTestDlgClass50", "Inst", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("DataTestDlgClass50", "Accum", 0, QApplication::UnicodeUTF8));
        gBox_StdDN3->setTitle(QApplication::translate("DataTestDlgClass50", "DN3", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("DataTestDlgClass50", "Inst", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("DataTestDlgClass50", "Accum", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("DataTestDlgClass50", "StdMeterValue", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("DataTestDlgClass50", "FlowRate:", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("DataTestDlgClass50", "Std Meter:", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("DataTestDlgClass50", "L", 0, QApplication::UnicodeUTF8));
        label_49->setText(QApplication::translate("DataTestDlgClass50", "m\302\263/h", 0, QApplication::UnicodeUTF8));
        groupBox_heatmeter->setTitle(QApplication::translate("DataTestDlgClass50", "Heat Meter", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("DataTestDlgClass50", "Com:", 0, QApplication::UnicodeUTF8));
        portNameComboBox->clear();
        portNameComboBox->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM11", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM13", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM14", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM17", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM18", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM19", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM21", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM22", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM23", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM24", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM26", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM27", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM28", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM29", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "COM30", 0, QApplication::UnicodeUTF8)
        );
        label_20->setText(QApplication::translate("DataTestDlgClass50", "BaudRate:", 0, QApplication::UnicodeUTF8));
        baudRateComboBox->clear();
        baudRateComboBox->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "2400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "4800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "9600", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "14400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "19200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "38400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "56000", 0, QApplication::UnicodeUTF8)
        );
        label_21->setText(QApplication::translate("DataTestDlgClass50", "DataBits:", 0, QApplication::UnicodeUTF8));
        dataBitsComboBox->clear();
        dataBitsComboBox->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "8", 0, QApplication::UnicodeUTF8)
        );
        label_22->setText(QApplication::translate("DataTestDlgClass50", "Parity:", 0, QApplication::UnicodeUTF8));
        parityComboBox->clear();
        parityComboBox->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "None", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "Odd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "Even", 0, QApplication::UnicodeUTF8)
        );
        label_23->setText(QApplication::translate("DataTestDlgClass50", "StopBits:", 0, QApplication::UnicodeUTF8));
        stopBitsComboBox->clear();
        stopBitsComboBox->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "1.5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "2", 0, QApplication::UnicodeUTF8)
        );
        btnOpenCom->setText(QApplication::translate("DataTestDlgClass50", "OpenCom", 0, QApplication::UnicodeUTF8));
        btnCloseCom->setText(QApplication::translate("DataTestDlgClass50", "CloseCom", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("DataTestDlgClass50", "MeterInfo:", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("DataTestDlgClass50", "MeterNO.", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("DataTestDlgClass50", "T In(\342\204\203)", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("DataTestDlgClass50", "T Out(\342\204\203)", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("DataTestDlgClass50", "Flow(L)", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("DataTestDlgClass50", "Heat(kwh)", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("DataTestDlgClass50", "Date", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("DataTestDlgClass50", "New NO.", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("DataTestDlgClass50", "err(%)", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("DataTestDlgClass50", "olderr(%)", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("DataTestDlgClass50", "oldCoe", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("DataTestDlgClass50", "orgCoe", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("DataTestDlgClass50", "big", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("DataTestDlgClass50", "mid2", 0, QApplication::UnicodeUTF8));
        lnEditMid2NewError->setText(QString());
        lnEditMid2OldError->setText(QString());
        lnEditMid2OldCoe->setText(QString());
        lnEditMid2OrgCoe->setText(QString());
        label_35->setText(QApplication::translate("DataTestDlgClass50", "mid1", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("DataTestDlgClass50", "small", 0, QApplication::UnicodeUTF8));
        lnEditSmallNewError->setText(QString());
        lnEditSmallOldError->setText(QString());
        lnEditSmallOldCoe->setText(QString());
        lnEditSmallOrgCoe->setText(QString());
        btnReadMeterData->setText(QApplication::translate("DataTestDlgClass50", "Read\n"
"Data", 0, QApplication::UnicodeUTF8));
        btnSetVerifyStatus->setText(QApplication::translate("DataTestDlgClass50", "Verify\n"
"Status", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnModifyMeterNo->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        btnModifyMeterNo->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        btnModifyMeterNo->setText(QApplication::translate("DataTestDlgClass50", "Modify\n"
"MeterNo", 0, QApplication::UnicodeUTF8));
        btnModifyFlowCoe->setText(QApplication::translate("DataTestDlgClass50", "Modify\n"
"Coe", 0, QApplication::UnicodeUTF8));
        btn2ModifyFlowCoe->setText(QApplication::translate("DataTestDlgClass50", "2Modify\n"
"Coe", 0, QApplication::UnicodeUTF8));
        btnExitVerifyStatus->setText(QApplication::translate("DataTestDlgClass50", "Exit\n"
"Verify", 0, QApplication::UnicodeUTF8));
        btnSetTime->setText(QApplication::translate("DataTestDlgClass50", "Set\n"
"Time", 0, QApplication::UnicodeUTF8));
        cmbStandard->clear();
        cmbStandard->insertItems(0, QStringList()
         << QApplication::translate("DataTestDlgClass50", "DN15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN40", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN65", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN80", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN125", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN150", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN250", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("DataTestDlgClass50", "DN300", 0, QApplication::UnicodeUTF8)
        );
        btnSetStandard->setText(QApplication::translate("DataTestDlgClass50", "SetStandard", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DataTestDlgClass50", "CurAddr1(1bit):", 0, QApplication::UnicodeUTF8));
        label_58->setText(QApplication::translate("DataTestDlgClass50", "NewAddr1(1bit):", 0, QApplication::UnicodeUTF8));
        btnSetAddr1->setText(QApplication::translate("DataTestDlgClass50", "SetAddr1", 0, QApplication::UnicodeUTF8));
        label_59->setText(QApplication::translate("DataTestDlgClass50", "NewAddr2(8bit):", 0, QApplication::UnicodeUTF8));
        btnSetAddr2->setText(QApplication::translate("DataTestDlgClass50", "SetAddr2", 0, QApplication::UnicodeUTF8));
        btnParaSet->setText(QApplication::translate("DataTestDlgClass50", "ParaSet", 0, QApplication::UnicodeUTF8));
        btnExit->setText(QApplication::translate("DataTestDlgClass50", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataTestDlgClass50: public Ui_DataTestDlgClass50 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATATESTDLG50_H
