/********************************************************************************
** Form generated from reading UI file 'nfc.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NFC_H
#define UI_NFC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NFC
{
public:
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidget;
    QWidget *NFC1;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QLineEdit *bottleSnLineEdit;
    QLabel *label_9;
    QLabel *label_8;
    QComboBox *bottleTypeComboBox;
    QLabel *label_10;
    QLineEdit *bottleWeightLineEdit;
    QLabel *label_11;
    QLineEdit *bottleProduceDateLineEdit;
    QLabel *label_12;
    QLineEdit *bottleOpenLineEdit;
    QLabel *label_21;
    QLineEdit *bottleUnOpenLineEdit;
    QLabel *label_13;
    QLineEdit *bottleOpenedLineEdit;
    QComboBox *bottleMaterialEnComboBox;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_2;
    QLabel *label_16;
    QLabel *label_15;
    QLineEdit *moSnLineEdit;
    QLineEdit *moExpireDateLineEdit;
    QLabel *label_17;
    QLineEdit *moPeelLeftCountLineEdit;
    QLabel *label_18;
    QLineEdit *moProduceDateLineEdit;
    QLabel *label_19;
    QComboBox *moTypeComboBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *serialPortCombo;
    QPushButton *scanSerialPort;
    QPushButton *connect;
    QPushButton *cardStatus;
    QPushButton *read;
    QPushButton *write;
    QPushButton *clear;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *NFC)
    {
        if (NFC->objectName().isEmpty())
            NFC->setObjectName(QString::fromUtf8("NFC"));
        NFC->resize(789, 657);
        QFont font;
        font.setPointSize(12);
        font.setKerning(false);
        font.setStyleStrategy(QFont::PreferDefault);
        NFC->setFont(font);
        gridLayout_3 = new QGridLayout(NFC);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidget = new QTabWidget(NFC);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        NFC1 = new QWidget();
        NFC1->setObjectName(QString::fromUtf8("NFC1"));
        horizontalLayout = new QHBoxLayout(NFC1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_7 = new QLabel(NFC1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(100, 0));
        label_7->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_7, 0, 0, 1, 1);

        bottleSnLineEdit = new QLineEdit(NFC1);
        bottleSnLineEdit->setObjectName(QString::fromUtf8("bottleSnLineEdit"));
        bottleSnLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleSnLineEdit, 0, 1, 1, 1);

        label_9 = new QLabel(NFC1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(100, 0));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 1, 0, 1, 1);

        label_8 = new QLabel(NFC1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(100, 0));
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_8, 2, 0, 1, 1);

        bottleTypeComboBox = new QComboBox(NFC1);
        bottleTypeComboBox->setObjectName(QString::fromUtf8("bottleTypeComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bottleTypeComboBox->sizePolicy().hasHeightForWidth());
        bottleTypeComboBox->setSizePolicy(sizePolicy);
        bottleTypeComboBox->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleTypeComboBox, 2, 1, 1, 1);

        label_10 = new QLabel(NFC1);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(100, 0));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 3, 0, 1, 1);

        bottleWeightLineEdit = new QLineEdit(NFC1);
        bottleWeightLineEdit->setObjectName(QString::fromUtf8("bottleWeightLineEdit"));
        bottleWeightLineEdit->setEnabled(true);
        bottleWeightLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleWeightLineEdit, 3, 1, 1, 1);

        label_11 = new QLabel(NFC1);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(100, 0));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_11, 4, 0, 1, 1);

        bottleProduceDateLineEdit = new QLineEdit(NFC1);
        bottleProduceDateLineEdit->setObjectName(QString::fromUtf8("bottleProduceDateLineEdit"));
        bottleProduceDateLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleProduceDateLineEdit, 4, 1, 1, 1);

        label_12 = new QLabel(NFC1);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(100, 0));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_12, 5, 0, 1, 1);

        bottleOpenLineEdit = new QLineEdit(NFC1);
        bottleOpenLineEdit->setObjectName(QString::fromUtf8("bottleOpenLineEdit"));
        bottleOpenLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleOpenLineEdit, 5, 1, 1, 1);

        label_21 = new QLabel(NFC1);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setMinimumSize(QSize(100, 0));
        label_21->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_21, 6, 0, 1, 1);

        bottleUnOpenLineEdit = new QLineEdit(NFC1);
        bottleUnOpenLineEdit->setObjectName(QString::fromUtf8("bottleUnOpenLineEdit"));
        bottleUnOpenLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleUnOpenLineEdit, 6, 1, 1, 1);

        label_13 = new QLabel(NFC1);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(100, 0));
        label_13->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_13, 7, 0, 1, 1);

        bottleOpenedLineEdit = new QLineEdit(NFC1);
        bottleOpenedLineEdit->setObjectName(QString::fromUtf8("bottleOpenedLineEdit"));
        bottleOpenedLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleOpenedLineEdit, 7, 1, 1, 1);

        bottleMaterialEnComboBox = new QComboBox(NFC1);
        bottleMaterialEnComboBox->setObjectName(QString::fromUtf8("bottleMaterialEnComboBox"));
        bottleMaterialEnComboBox->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(bottleMaterialEnComboBox, 1, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        tabWidget->addTab(NFC1, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_16 = new QLabel(tab);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(100, 0));
        label_16->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_16, 1, 0, 1, 1);

        label_15 = new QLabel(tab);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(100, 0));
        label_15->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_15, 0, 0, 1, 1);

        moSnLineEdit = new QLineEdit(tab);
        moSnLineEdit->setObjectName(QString::fromUtf8("moSnLineEdit"));
        moSnLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(moSnLineEdit, 0, 2, 1, 1);

        moExpireDateLineEdit = new QLineEdit(tab);
        moExpireDateLineEdit->setObjectName(QString::fromUtf8("moExpireDateLineEdit"));
        moExpireDateLineEdit->setEnabled(true);
        moExpireDateLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(moExpireDateLineEdit, 2, 2, 1, 1);

        label_17 = new QLabel(tab);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(100, 0));
        label_17->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_17, 2, 0, 1, 1);

        moPeelLeftCountLineEdit = new QLineEdit(tab);
        moPeelLeftCountLineEdit->setObjectName(QString::fromUtf8("moPeelLeftCountLineEdit"));
        moPeelLeftCountLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(moPeelLeftCountLineEdit, 4, 2, 1, 1);

        label_18 = new QLabel(tab);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMinimumSize(QSize(100, 0));
        label_18->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_18, 3, 0, 1, 1);

        moProduceDateLineEdit = new QLineEdit(tab);
        moProduceDateLineEdit->setObjectName(QString::fromUtf8("moProduceDateLineEdit"));
        moProduceDateLineEdit->setEnabled(true);
        moProduceDateLineEdit->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(moProduceDateLineEdit, 3, 2, 1, 1);

        label_19 = new QLabel(tab);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMinimumSize(QSize(100, 0));
        label_19->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_19, 4, 0, 1, 1);

        moTypeComboBox = new QComboBox(tab);
        moTypeComboBox->setObjectName(QString::fromUtf8("moTypeComboBox"));
        moTypeComboBox->setMinimumSize(QSize(0, 40));

        gridLayout_2->addWidget(moTypeComboBox, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);

        tabWidget->addTab(tab, QString());

        gridLayout_3->addWidget(tabWidget, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        serialPortCombo = new QComboBox(NFC);
        serialPortCombo->setObjectName(QString::fromUtf8("serialPortCombo"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(serialPortCombo->sizePolicy().hasHeightForWidth());
        serialPortCombo->setSizePolicy(sizePolicy1);
        serialPortCombo->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(serialPortCombo);

        scanSerialPort = new QPushButton(NFC);
        scanSerialPort->setObjectName(QString::fromUtf8("scanSerialPort"));
        scanSerialPort->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(scanSerialPort);

        connect = new QPushButton(NFC);
        connect->setObjectName(QString::fromUtf8("connect"));
        connect->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(connect);

        cardStatus = new QPushButton(NFC);
        cardStatus->setObjectName(QString::fromUtf8("cardStatus"));
        cardStatus->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(cardStatus);

        read = new QPushButton(NFC);
        read->setObjectName(QString::fromUtf8("read"));
        read->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(read);

        write = new QPushButton(NFC);
        write->setObjectName(QString::fromUtf8("write"));
        write->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(write);

        clear = new QPushButton(NFC);
        clear->setObjectName(QString::fromUtf8("clear"));
        clear->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(clear);


        gridLayout_3->addLayout(horizontalLayout_3, 1, 0, 1, 1);

        textBrowser = new QTextBrowser(NFC);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout_3->addWidget(textBrowser, 2, 0, 1, 1);


        retranslateUi(NFC);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(NFC);
    } // setupUi

    void retranslateUi(QWidget *NFC)
    {
        NFC->setWindowTitle(QApplication::translate("NFC", "NFC", nullptr));
#ifndef QT_NO_WHATSTHIS
        tabWidget->setWhatsThis(QApplication::translate("NFC", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        label_7->setText(QApplication::translate("NFC", "SN\347\240\201", nullptr));
        label_9->setText(QApplication::translate("NFC", "\346\235\220\346\226\231\350\213\261\346\226\207\345\220\215", nullptr));
        label_8->setText(QApplication::translate("NFC", "\350\247\204\346\240\274\345\236\213\345\217\267(g)", nullptr));
        label_10->setText(QApplication::translate("NFC", "\346\226\231\347\223\266\351\207\215\351\207\217(g)", nullptr));
        bottleWeightLineEdit->setText(QString());
        label_11->setText(QApplication::translate("NFC", "\347\224\237\344\272\247\346\227\245\346\234\237", nullptr));
        label_12->setText(QApplication::translate("NFC", "\345\274\200\345\260\201\346\227\245\346\234\237", nullptr));
        label_21->setText(QApplication::translate("NFC", "\345\274\200\345\260\201\345\211\215\344\277\235\350\264\250\346\234\237", nullptr));
        label_13->setText(QApplication::translate("NFC", "\345\274\200\345\260\201\345\220\216\344\277\235\350\264\250\346\234\237", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(NFC1), QApplication::translate("NFC", "\346\240\221\350\204\202\347\223\266", nullptr));
        label_16->setText(QApplication::translate("NFC", "\350\247\204\346\240\274\345\236\213\345\217\267", nullptr));
        label_15->setText(QApplication::translate("NFC", "SN\347\240\201", nullptr));
        label_17->setText(QApplication::translate("NFC", "\344\277\235\350\264\250\346\234\237", nullptr));
        label_18->setText(QApplication::translate("NFC", "\347\224\237\344\272\247\346\227\245\346\234\237", nullptr));
        label_19->setText(QApplication::translate("NFC", "\345\211\251\344\275\231\345\211\245\347\246\273\346\254\241\346\225\260", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("NFC", "\347\246\273\345\236\213\346\250\241", nullptr));
        scanSerialPort->setText(QApplication::translate("NFC", "\346\211\253\346\217\217\344\270\262\345\217\243", nullptr));
        connect->setText(QApplication::translate("NFC", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        cardStatus->setText(QApplication::translate("NFC", "\346\227\240\345\215\241", nullptr));
        read->setText(QApplication::translate("NFC", "\350\257\273\345\217\226", nullptr));
        write->setText(QApplication::translate("NFC", "\345\206\231\345\205\245", nullptr));
        clear->setText(QApplication::translate("NFC", "\346\270\205\347\251\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NFC: public Ui_NFC {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NFC_H
