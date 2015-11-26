/********************************************************************************
** Form generated from reading UI file 'flowweight50.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLOWWEIGHT50_H
#define UI_FLOWWEIGHT50_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlowWeightClass50
{
public:
    QGridLayout *gridLayout_6;
    QTableWidget *tableWidget;
    QPushButton *btnParaSet;
    QGridLayout *gridLayout_4;
    QPushButton *btnAllReadNO;
    QPushButton *btnAllReadData;
    QPushButton *btnAllVerifyStatus;
    QPushButton *btnAllAdjError;
    QPushButton *btnAllModifyNO;
    QPushButton *btnReCalc;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_9;
    QGridLayout *gridLayout_7;
    QLabel *label;
    QLabel *labelHintPoint;
    QLabel *label_2;
    QLabel *labelHintProcess;
    QPushButton *btnStart;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QLabel *labelStandard;
    QLineEdit *lnEditStandard;
    QLabel *label_14;
    QComboBox *cmbAutoPick;
    QLabel *label_16;
    QComboBox *cmbResetZero;
    QLabel *label_15;
    QComboBox *cmbContinue;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBoxFreq;
    QLabel *label_13;
    QToolButton *btnWaterIn;
    QToolButton *btnValveSmall;
    QToolButton *btnValveMiddle1;
    QToolButton *btnValveMiddle2;
    QToolButton *btnValveBig;
    QToolButton *btnBigWaterIn;
    QToolButton *btnBigWaterOut;
    QToolButton *btnSmallWaterIn;
    QToolButton *btnSmallWaterOut;
    QToolButton *btnSetFreq;
    QToolButton *btnWaterPump;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QLineEdit *lcdBigBalance;
    QLabel *label_11;
    QLabel *label_10;
    QLineEdit *lcdSmallBalance;
    QLabel *label_12;
    QLabel *label_3;
    QLCDNumber *lcdFlowRate;
    QLabel *label_6;
    QLabel *label_4;
    QLCDNumber *lcdInTemper;
    QLabel *label_7;
    QLabel *label_5;
    QLCDNumber *lcdOutTemper;
    QLabel *label_8;
    QPushButton *btnExhaust;
    QPushButton *btnGoOn;
    QPushButton *btnStop;
    QPushButton *btnExit;

    void setupUi(QWidget *FlowWeightClass50)
    {
        if (FlowWeightClass50->objectName().isEmpty())
            FlowWeightClass50->setObjectName(QString::fromUtf8("FlowWeightClass50"));
        FlowWeightClass50->resize(1641, 856);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/flowweight/images/flowweight.png"), QSize(), QIcon::Normal, QIcon::Off);
        FlowWeightClass50->setWindowIcon(icon);
        gridLayout_6 = new QGridLayout(FlowWeightClass50);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        tableWidget = new QTableWidget(FlowWeightClass50);
        if (tableWidget->columnCount() < 16)
            tableWidget->setColumnCount(16);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setBackground(QColor(128, 0, 0));
        tableWidget->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(10, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(11, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(12, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(13, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(14, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(15, __qtablewidgetitem15);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setInputMethodHints(Qt::ImhDigitsOnly|Qt::ImhFormattedNumbersOnly|Qt::ImhPreferNumbers);
        tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);

        gridLayout_6->addWidget(tableWidget, 0, 0, 1, 5);

        btnParaSet = new QPushButton(FlowWeightClass50);
        btnParaSet->setObjectName(QString::fromUtf8("btnParaSet"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btnParaSet->sizePolicy().hasHeightForWidth());
        btnParaSet->setSizePolicy(sizePolicy);
        btnParaSet->setMinimumSize(QSize(220, 35));
        btnParaSet->setMaximumSize(QSize(220, 35));

        gridLayout_6->addWidget(btnParaSet, 1, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        btnAllReadNO = new QPushButton(FlowWeightClass50);
        btnAllReadNO->setObjectName(QString::fromUtf8("btnAllReadNO"));
        sizePolicy.setHeightForWidth(btnAllReadNO->sizePolicy().hasHeightForWidth());
        btnAllReadNO->setSizePolicy(sizePolicy);
        btnAllReadNO->setMinimumSize(QSize(80, 35));
        btnAllReadNO->setMaximumSize(QSize(110, 35));

        gridLayout_4->addWidget(btnAllReadNO, 0, 0, 1, 1);

        btnAllReadData = new QPushButton(FlowWeightClass50);
        btnAllReadData->setObjectName(QString::fromUtf8("btnAllReadData"));
        sizePolicy.setHeightForWidth(btnAllReadData->sizePolicy().hasHeightForWidth());
        btnAllReadData->setSizePolicy(sizePolicy);
        btnAllReadData->setMinimumSize(QSize(80, 35));
        btnAllReadData->setMaximumSize(QSize(110, 35));

        gridLayout_4->addWidget(btnAllReadData, 0, 1, 1, 1);

        btnAllVerifyStatus = new QPushButton(FlowWeightClass50);
        btnAllVerifyStatus->setObjectName(QString::fromUtf8("btnAllVerifyStatus"));
        sizePolicy.setHeightForWidth(btnAllVerifyStatus->sizePolicy().hasHeightForWidth());
        btnAllVerifyStatus->setSizePolicy(sizePolicy);
        btnAllVerifyStatus->setMinimumSize(QSize(80, 35));
        btnAllVerifyStatus->setMaximumSize(QSize(110, 35));

        gridLayout_4->addWidget(btnAllVerifyStatus, 0, 2, 1, 1);

        btnAllAdjError = new QPushButton(FlowWeightClass50);
        btnAllAdjError->setObjectName(QString::fromUtf8("btnAllAdjError"));
        sizePolicy.setHeightForWidth(btnAllAdjError->sizePolicy().hasHeightForWidth());
        btnAllAdjError->setSizePolicy(sizePolicy);
        btnAllAdjError->setMinimumSize(QSize(80, 35));
        btnAllAdjError->setMaximumSize(QSize(110, 35));

        gridLayout_4->addWidget(btnAllAdjError, 0, 3, 1, 1);

        btnAllModifyNO = new QPushButton(FlowWeightClass50);
        btnAllModifyNO->setObjectName(QString::fromUtf8("btnAllModifyNO"));
        sizePolicy.setHeightForWidth(btnAllModifyNO->sizePolicy().hasHeightForWidth());
        btnAllModifyNO->setSizePolicy(sizePolicy);
        btnAllModifyNO->setMinimumSize(QSize(80, 35));
        btnAllModifyNO->setMaximumSize(QSize(110, 35));

        gridLayout_4->addWidget(btnAllModifyNO, 0, 4, 1, 1);


        gridLayout_6->addLayout(gridLayout_4, 1, 1, 1, 1);

        btnReCalc = new QPushButton(FlowWeightClass50);
        btnReCalc->setObjectName(QString::fromUtf8("btnReCalc"));
        sizePolicy.setHeightForWidth(btnReCalc->sizePolicy().hasHeightForWidth());
        btnReCalc->setSizePolicy(sizePolicy);
        btnReCalc->setMinimumSize(QSize(260, 35));
        btnReCalc->setMaximumSize(QSize(400, 35));

        gridLayout_6->addWidget(btnReCalc, 1, 2, 1, 1);

        groupBox_3 = new QGroupBox(FlowWeightClass50);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy1);
        groupBox_3->setMinimumSize(QSize(300, 0));
        gridLayout_9 = new QGridLayout(groupBox_3);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_7->addWidget(label, 0, 0, 1, 1);

        labelHintPoint = new QLabel(groupBox_3);
        labelHintPoint->setObjectName(QString::fromUtf8("labelHintPoint"));
        sizePolicy1.setHeightForWidth(labelHintPoint->sizePolicy().hasHeightForWidth());
        labelHintPoint->setSizePolicy(sizePolicy1);
        labelHintPoint->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Aharoni\";"));
        labelHintPoint->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        labelHintPoint->setWordWrap(true);

        gridLayout_7->addWidget(labelHintPoint, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_7->addWidget(label_2, 1, 0, 1, 1);

        labelHintProcess = new QLabel(groupBox_3);
        labelHintProcess->setObjectName(QString::fromUtf8("labelHintProcess"));
        sizePolicy1.setHeightForWidth(labelHintProcess->sizePolicy().hasHeightForWidth());
        labelHintProcess->setSizePolicy(sizePolicy1);
        labelHintProcess->setStyleSheet(QString::fromUtf8("font: 75 12pt \"Aharoni\";"));
        labelHintProcess->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        labelHintProcess->setWordWrap(true);

        gridLayout_7->addWidget(labelHintProcess, 1, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_7, 0, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_3, 1, 3, 5, 1);

        btnStart = new QPushButton(FlowWeightClass50);
        btnStart->setObjectName(QString::fromUtf8("btnStart"));
        sizePolicy.setHeightForWidth(btnStart->sizePolicy().hasHeightForWidth());
        btnStart->setSizePolicy(sizePolicy);
        btnStart->setMinimumSize(QSize(100, 30));
        btnStart->setMaximumSize(QSize(100, 30));

        gridLayout_6->addWidget(btnStart, 1, 4, 1, 1);

        groupBox = new QGroupBox(FlowWeightClass50);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(220, 0));
        groupBox->setMaximumSize(QSize(220, 16777215));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        labelStandard = new QLabel(groupBox);
        labelStandard->setObjectName(QString::fromUtf8("labelStandard"));
        sizePolicy2.setHeightForWidth(labelStandard->sizePolicy().hasHeightForWidth());
        labelStandard->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(labelStandard, 0, 0, 1, 1);

        lnEditStandard = new QLineEdit(groupBox);
        lnEditStandard->setObjectName(QString::fromUtf8("lnEditStandard"));
        lnEditStandard->setEnabled(false);
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lnEditStandard->sizePolicy().hasHeightForWidth());
        lnEditStandard->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(lnEditStandard, 0, 1, 1, 1);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        sizePolicy2.setHeightForWidth(label_14->sizePolicy().hasHeightForWidth());
        label_14->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(label_14, 1, 0, 1, 1);

        cmbAutoPick = new QComboBox(groupBox);
        cmbAutoPick->setObjectName(QString::fromUtf8("cmbAutoPick"));
        cmbAutoPick->setEnabled(false);
        sizePolicy3.setHeightForWidth(cmbAutoPick->sizePolicy().hasHeightForWidth());
        cmbAutoPick->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(cmbAutoPick, 1, 1, 1, 1);

        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        sizePolicy2.setHeightForWidth(label_16->sizePolicy().hasHeightForWidth());
        label_16->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(label_16, 2, 0, 1, 1);

        cmbResetZero = new QComboBox(groupBox);
        cmbResetZero->setObjectName(QString::fromUtf8("cmbResetZero"));
        cmbResetZero->setEnabled(false);
        sizePolicy3.setHeightForWidth(cmbResetZero->sizePolicy().hasHeightForWidth());
        cmbResetZero->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(cmbResetZero, 2, 1, 1, 1);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        sizePolicy2.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(label_15, 3, 0, 1, 1);

        cmbContinue = new QComboBox(groupBox);
        cmbContinue->setObjectName(QString::fromUtf8("cmbContinue"));
        cmbContinue->setEnabled(false);
        sizePolicy3.setHeightForWidth(cmbContinue->sizePolicy().hasHeightForWidth());
        cmbContinue->setSizePolicy(sizePolicy3);

        gridLayout_3->addWidget(cmbContinue, 3, 1, 1, 1);


        gridLayout_6->addWidget(groupBox, 2, 0, 4, 1);

        groupBox_2 = new QGroupBox(FlowWeightClass50);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        spinBoxFreq = new QSpinBox(groupBox_2);
        spinBoxFreq->setObjectName(QString::fromUtf8("spinBoxFreq"));
        sizePolicy.setHeightForWidth(spinBoxFreq->sizePolicy().hasHeightForWidth());
        spinBoxFreq->setSizePolicy(sizePolicy);
        spinBoxFreq->setMinimumSize(QSize(70, 30));
        spinBoxFreq->setMaximumSize(QSize(70, 30));

        horizontalLayout->addWidget(spinBoxFreq);

        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);
        label_13->setMinimumSize(QSize(20, 20));
        label_13->setMaximumSize(QSize(20, 20));

        horizontalLayout->addWidget(label_13);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        btnWaterIn = new QToolButton(groupBox_2);
        btnWaterIn->setObjectName(QString::fromUtf8("btnWaterIn"));
        sizePolicy.setHeightForWidth(btnWaterIn->sizePolicy().hasHeightForWidth());
        btnWaterIn->setSizePolicy(sizePolicy);
        btnWaterIn->setMinimumSize(QSize(55, 130));
        btnWaterIn->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnWaterIn, 0, 1, 3, 1);

        btnValveSmall = new QToolButton(groupBox_2);
        btnValveSmall->setObjectName(QString::fromUtf8("btnValveSmall"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btnValveSmall->sizePolicy().hasHeightForWidth());
        btnValveSmall->setSizePolicy(sizePolicy4);
        btnValveSmall->setMinimumSize(QSize(55, 130));
        btnValveSmall->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnValveSmall, 0, 2, 3, 1);

        btnValveMiddle1 = new QToolButton(groupBox_2);
        btnValveMiddle1->setObjectName(QString::fromUtf8("btnValveMiddle1"));
        sizePolicy4.setHeightForWidth(btnValveMiddle1->sizePolicy().hasHeightForWidth());
        btnValveMiddle1->setSizePolicy(sizePolicy4);
        btnValveMiddle1->setMinimumSize(QSize(55, 130));
        btnValveMiddle1->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnValveMiddle1, 0, 3, 3, 1);

        btnValveMiddle2 = new QToolButton(groupBox_2);
        btnValveMiddle2->setObjectName(QString::fromUtf8("btnValveMiddle2"));
        sizePolicy4.setHeightForWidth(btnValveMiddle2->sizePolicy().hasHeightForWidth());
        btnValveMiddle2->setSizePolicy(sizePolicy4);
        btnValveMiddle2->setMinimumSize(QSize(55, 130));
        btnValveMiddle2->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnValveMiddle2, 0, 4, 3, 1);

        btnValveBig = new QToolButton(groupBox_2);
        btnValveBig->setObjectName(QString::fromUtf8("btnValveBig"));
        sizePolicy4.setHeightForWidth(btnValveBig->sizePolicy().hasHeightForWidth());
        btnValveBig->setSizePolicy(sizePolicy4);
        btnValveBig->setMinimumSize(QSize(55, 130));
        btnValveBig->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnValveBig, 0, 5, 3, 1);

        btnBigWaterIn = new QToolButton(groupBox_2);
        btnBigWaterIn->setObjectName(QString::fromUtf8("btnBigWaterIn"));
        sizePolicy4.setHeightForWidth(btnBigWaterIn->sizePolicy().hasHeightForWidth());
        btnBigWaterIn->setSizePolicy(sizePolicy4);
        btnBigWaterIn->setMinimumSize(QSize(55, 130));
        btnBigWaterIn->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnBigWaterIn, 0, 6, 3, 1);

        btnBigWaterOut = new QToolButton(groupBox_2);
        btnBigWaterOut->setObjectName(QString::fromUtf8("btnBigWaterOut"));
        sizePolicy4.setHeightForWidth(btnBigWaterOut->sizePolicy().hasHeightForWidth());
        btnBigWaterOut->setSizePolicy(sizePolicy4);
        btnBigWaterOut->setMinimumSize(QSize(55, 130));
        btnBigWaterOut->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnBigWaterOut, 0, 7, 3, 1);

        btnSmallWaterIn = new QToolButton(groupBox_2);
        btnSmallWaterIn->setObjectName(QString::fromUtf8("btnSmallWaterIn"));
        sizePolicy4.setHeightForWidth(btnSmallWaterIn->sizePolicy().hasHeightForWidth());
        btnSmallWaterIn->setSizePolicy(sizePolicy4);
        btnSmallWaterIn->setMinimumSize(QSize(55, 130));
        btnSmallWaterIn->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnSmallWaterIn, 0, 8, 3, 1);

        btnSmallWaterOut = new QToolButton(groupBox_2);
        btnSmallWaterOut->setObjectName(QString::fromUtf8("btnSmallWaterOut"));
        sizePolicy4.setHeightForWidth(btnSmallWaterOut->sizePolicy().hasHeightForWidth());
        btnSmallWaterOut->setSizePolicy(sizePolicy4);
        btnSmallWaterOut->setMinimumSize(QSize(55, 130));
        btnSmallWaterOut->setMaximumSize(QSize(60, 130));

        gridLayout->addWidget(btnSmallWaterOut, 0, 9, 3, 1);

        btnSetFreq = new QToolButton(groupBox_2);
        btnSetFreq->setObjectName(QString::fromUtf8("btnSetFreq"));
        sizePolicy.setHeightForWidth(btnSetFreq->sizePolicy().hasHeightForWidth());
        btnSetFreq->setSizePolicy(sizePolicy);
        btnSetFreq->setMinimumSize(QSize(100, 40));
        btnSetFreq->setMaximumSize(QSize(100, 40));

        gridLayout->addWidget(btnSetFreq, 1, 0, 1, 1);

        btnWaterPump = new QToolButton(groupBox_2);
        btnWaterPump->setObjectName(QString::fromUtf8("btnWaterPump"));
        sizePolicy.setHeightForWidth(btnWaterPump->sizePolicy().hasHeightForWidth());
        btnWaterPump->setSizePolicy(sizePolicy);
        btnWaterPump->setMinimumSize(QSize(100, 40));
        btnWaterPump->setMaximumSize(QSize(100, 40));

        gridLayout->addWidget(btnWaterPump, 2, 0, 1, 1);


        gridLayout_6->addWidget(groupBox_2, 2, 1, 4, 1);

        groupBox_4 = new QGroupBox(FlowWeightClass50);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(260, 0));
        groupBox_4->setMaximumSize(QSize(400, 16777215));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy2.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_9, 0, 0, 1, 1);

        lcdBigBalance = new QLineEdit(groupBox_4);
        lcdBigBalance->setObjectName(QString::fromUtf8("lcdBigBalance"));

        gridLayout_2->addWidget(lcdBigBalance, 0, 1, 1, 1);

        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy2.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_11, 0, 2, 1, 1);

        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy2.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_10, 1, 0, 1, 1);

        lcdSmallBalance = new QLineEdit(groupBox_4);
        lcdSmallBalance->setObjectName(QString::fromUtf8("lcdSmallBalance"));

        gridLayout_2->addWidget(lcdSmallBalance, 1, 1, 1, 1);

        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy2.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_12, 1, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 0, 0, 1, 3);

        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_3, 1, 0, 1, 1);

        lcdFlowRate = new QLCDNumber(groupBox_4);
        lcdFlowRate->setObjectName(QString::fromUtf8("lcdFlowRate"));
        sizePolicy3.setHeightForWidth(lcdFlowRate->sizePolicy().hasHeightForWidth());
        lcdFlowRate->setSizePolicy(sizePolicy3);
        lcdFlowRate->setNumDigits(7);
        lcdFlowRate->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_5->addWidget(lcdFlowRate, 1, 1, 1, 1);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy2.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy2);

        gridLayout_5->addWidget(label_6, 1, 2, 1, 1);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_4, 2, 0, 1, 1);

        lcdInTemper = new QLCDNumber(groupBox_4);
        lcdInTemper->setObjectName(QString::fromUtf8("lcdInTemper"));
        sizePolicy3.setHeightForWidth(lcdInTemper->sizePolicy().hasHeightForWidth());
        lcdInTemper->setSizePolicy(sizePolicy3);
        QPalette palette;
        QBrush brush(QColor(255, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(100, 10, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        lcdInTemper->setPalette(palette);
        lcdInTemper->setNumDigits(7);
        lcdInTemper->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_5->addWidget(lcdInTemper, 2, 1, 1, 1);

        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy2.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy2);

        gridLayout_5->addWidget(label_7, 2, 2, 1, 1);

        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_5->addWidget(label_5, 3, 0, 1, 1);

        lcdOutTemper = new QLCDNumber(groupBox_4);
        lcdOutTemper->setObjectName(QString::fromUtf8("lcdOutTemper"));
        sizePolicy3.setHeightForWidth(lcdOutTemper->sizePolicy().hasHeightForWidth());
        lcdOutTemper->setSizePolicy(sizePolicy3);
        QPalette palette1;
        QBrush brush3(QColor(0, 100, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        lcdOutTemper->setPalette(palette1);
        lcdOutTemper->setSmallDecimalPoint(false);
        lcdOutTemper->setNumDigits(7);
        lcdOutTemper->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_5->addWidget(lcdOutTemper, 3, 1, 1, 1);

        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy2.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy2);

        gridLayout_5->addWidget(label_8, 3, 2, 1, 1);


        gridLayout_6->addWidget(groupBox_4, 2, 2, 4, 1);

        btnExhaust = new QPushButton(FlowWeightClass50);
        btnExhaust->setObjectName(QString::fromUtf8("btnExhaust"));
        sizePolicy.setHeightForWidth(btnExhaust->sizePolicy().hasHeightForWidth());
        btnExhaust->setSizePolicy(sizePolicy);
        btnExhaust->setMinimumSize(QSize(100, 30));
        btnExhaust->setMaximumSize(QSize(100, 30));

        gridLayout_6->addWidget(btnExhaust, 2, 4, 1, 1);

        btnGoOn = new QPushButton(FlowWeightClass50);
        btnGoOn->setObjectName(QString::fromUtf8("btnGoOn"));
        sizePolicy.setHeightForWidth(btnGoOn->sizePolicy().hasHeightForWidth());
        btnGoOn->setSizePolicy(sizePolicy);
        btnGoOn->setMinimumSize(QSize(100, 30));
        btnGoOn->setMaximumSize(QSize(100, 30));

        gridLayout_6->addWidget(btnGoOn, 3, 4, 1, 1);

        btnStop = new QPushButton(FlowWeightClass50);
        btnStop->setObjectName(QString::fromUtf8("btnStop"));
        sizePolicy.setHeightForWidth(btnStop->sizePolicy().hasHeightForWidth());
        btnStop->setSizePolicy(sizePolicy);
        btnStop->setMinimumSize(QSize(100, 30));
        btnStop->setMaximumSize(QSize(100, 30));

        gridLayout_6->addWidget(btnStop, 4, 4, 1, 1);

        btnExit = new QPushButton(FlowWeightClass50);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        sizePolicy.setHeightForWidth(btnExit->sizePolicy().hasHeightForWidth());
        btnExit->setSizePolicy(sizePolicy);
        btnExit->setMinimumSize(QSize(100, 30));
        btnExit->setMaximumSize(QSize(100, 30));

        gridLayout_6->addWidget(btnExit, 5, 4, 1, 1);


        retranslateUi(FlowWeightClass50);

        QMetaObject::connectSlotsByName(FlowWeightClass50);
    } // setupUi

    void retranslateUi(QWidget *FlowWeightClass50)
    {
        FlowWeightClass50->setWindowTitle(QApplication::translate("FlowWeightClass50", "Flow Verify(Weight Method)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("FlowWeightClass50", "MeterNo", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("FlowWeightClass50", "FlowPoint(m\302\263/h)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("FlowWeightClass50", "MeterValue0(L)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("FlowWeightClass50", "MeterValue1(L)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("FlowWeightClass50", "BalWeight0(kg)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("FlowWeightClass50", "BalWeight1(kg)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("FlowWeightClass50", "Temp(\342\204\203)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("FlowWeightClass50", "Density(kg/L)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("FlowWeightClass50", "StdValue(L)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QApplication::translate("FlowWeightClass50", "DsipError(%)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->horizontalHeaderItem(10);
        ___qtablewidgetitem10->setText(QApplication::translate("FlowWeightClass50", "StdError(%)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->horizontalHeaderItem(11);
        ___qtablewidgetitem11->setText(QApplication::translate("FlowWeightClass50", "ReadNO", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->horizontalHeaderItem(12);
        ___qtablewidgetitem12->setText(QApplication::translate("FlowWeightClass50", "ReadData", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->horizontalHeaderItem(13);
        ___qtablewidgetitem13->setText(QApplication::translate("FlowWeightClass50", "VerifySt", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->horizontalHeaderItem(14);
        ___qtablewidgetitem14->setText(QApplication::translate("FlowWeightClass50", "AdjustErr", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->horizontalHeaderItem(15);
        ___qtablewidgetitem15->setText(QApplication::translate("FlowWeightClass50", "ModifyNO", 0, QApplication::UnicodeUTF8));
        btnParaSet->setText(QApplication::translate("FlowWeightClass50", "ParaSet", 0, QApplication::UnicodeUTF8));
        btnAllReadNO->setText(QApplication::translate("FlowWeightClass50", "ReadNO", 0, QApplication::UnicodeUTF8));
        btnAllReadData->setText(QApplication::translate("FlowWeightClass50", "ReadData", 0, QApplication::UnicodeUTF8));
        btnAllVerifyStatus->setText(QApplication::translate("FlowWeightClass50", "VerifySt", 0, QApplication::UnicodeUTF8));
        btnAllAdjError->setText(QApplication::translate("FlowWeightClass50", "AdjustErr", 0, QApplication::UnicodeUTF8));
        btnAllModifyNO->setText(QApplication::translate("FlowWeightClass50", "ModifyNO", 0, QApplication::UnicodeUTF8));
        btnReCalc->setText(QApplication::translate("FlowWeightClass50", "ReCalc", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("FlowWeightClass50", "Process Hint", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        labelHintPoint->setText(QString());
        label_2->setText(QString());
        labelHintProcess->setText(QString());
        btnStart->setText(QApplication::translate("FlowWeightClass50", "Start", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FlowWeightClass50", "NowPara(KeyInfo)", 0, QApplication::UnicodeUTF8));
        labelStandard->setText(QApplication::translate("FlowWeightClass50", "Standard:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("FlowWeightClass50", "DA Mode:", 0, QApplication::UnicodeUTF8));
        cmbAutoPick->clear();
        cmbAutoPick->insertItems(0, QStringList()
         << QApplication::translate("FlowWeightClass50", "Manual", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FlowWeightClass50", "Auto", 0, QApplication::UnicodeUTF8)
        );
        label_16->setText(QApplication::translate("FlowWeightClass50", "ResetZero:", 0, QApplication::UnicodeUTF8));
        cmbResetZero->clear();
        cmbResetZero->insertItems(0, QStringList()
         << QApplication::translate("FlowWeightClass50", "No", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FlowWeightClass50", "Yes", 0, QApplication::UnicodeUTF8)
        );
        label_15->setText(QApplication::translate("FlowWeightClass50", "Continuous:", 0, QApplication::UnicodeUTF8));
        cmbContinue->clear();
        cmbContinue->insertItems(0, QStringList()
         << QApplication::translate("FlowWeightClass50", "No", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("FlowWeightClass50", "Yes", 0, QApplication::UnicodeUTF8)
        );
        groupBox_2->setTitle(QApplication::translate("FlowWeightClass50", "Controller", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("FlowWeightClass50", "Hz", 0, QApplication::UnicodeUTF8));
        btnWaterIn->setText(QApplication::translate("FlowWeightClass50", "i\n"
"n\n"
"", 0, QApplication::UnicodeUTF8));
        btnValveSmall->setText(QApplication::translate("FlowWeightClass50", "s\n"
"m\n"
"a\n"
"l\n"
"l", 0, QApplication::UnicodeUTF8));
        btnValveMiddle1->setText(QApplication::translate("FlowWeightClass50", "m\n"
"i\n"
"d\n"
"1", 0, QApplication::UnicodeUTF8));
        btnValveMiddle2->setText(QApplication::translate("FlowWeightClass50", "m\n"
"i\n"
"d\n"
"2", 0, QApplication::UnicodeUTF8));
        btnValveBig->setText(QApplication::translate("FlowWeightClass50", "b\n"
"i\n"
"g", 0, QApplication::UnicodeUTF8));
        btnBigWaterIn->setText(QApplication::translate("FlowWeightClass50", "b\n"
"i\n"
"g\n"
"i\n"
"n", 0, QApplication::UnicodeUTF8));
        btnBigWaterOut->setText(QApplication::translate("FlowWeightClass50", "b\n"
"i\n"
"g\n"
"o\n"
"u\n"
"t", 0, QApplication::UnicodeUTF8));
        btnSmallWaterIn->setText(QApplication::translate("FlowWeightClass50", "s\n"
"i\n"
"n", 0, QApplication::UnicodeUTF8));
        btnSmallWaterOut->setText(QApplication::translate("FlowWeightClass50", "s\n"
"o\n"
"u\n"
"t", 0, QApplication::UnicodeUTF8));
        btnSetFreq->setText(QApplication::translate("FlowWeightClass50", "Set Freq", 0, QApplication::UnicodeUTF8));
        btnWaterPump->setText(QApplication::translate("FlowWeightClass50", "Pump", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("FlowWeightClass50", "DataAcquisition", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("FlowWeightClass50", "Big Balance: ", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("FlowWeightClass50", "\343\216\217  ", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("FlowWeightClass50", "Small Balance: ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("FlowWeightClass50", "\343\216\217  ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("FlowWeightClass50", "FlowRate:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("FlowWeightClass50", "m\302\263/h", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FlowWeightClass50", "In Temp:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("FlowWeightClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("FlowWeightClass50", "Out Temp:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("FlowWeightClass50", "\342\204\203", 0, QApplication::UnicodeUTF8));
        btnExhaust->setText(QApplication::translate("FlowWeightClass50", "Exhaust", 0, QApplication::UnicodeUTF8));
        btnGoOn->setText(QApplication::translate("FlowWeightClass50", "GoOn", 0, QApplication::UnicodeUTF8));
        btnStop->setText(QApplication::translate("FlowWeightClass50", "Stop", 0, QApplication::UnicodeUTF8));
        btnExit->setText(QApplication::translate("FlowWeightClass50", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FlowWeightClass50: public Ui_FlowWeightClass50 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLOWWEIGHT50_H
