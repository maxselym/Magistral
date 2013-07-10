/********************************************************************************
** Form generated from reading UI file 'wpswizard.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WPSWIZARD_H
#define UI_WPSWIZARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_WPSWizard
{
public:
    QWizardPage *wizardPage1;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_new;
    QRadioButton *radioButton_contin;
    QWizardPage *wizardPage2;
    QVBoxLayout *verticalLayout_3;
    QCalendarWidget *calendarWidget;
    QHBoxLayout *horizontalLayout_4;
    QLineEdit *lineEdit_st_date;
    QPushButton *pushButton_set_st_date;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_end_date;
    QPushButton *pushButton_set_end_date;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label;
    QLineEdit *lineEdit_path;
    QPushButton *pushButton_browse_path;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_2;
    QLineEdit *lineEdit_path_grib;
    QPushButton *pushButton_browse_path_2;
    QPushButton *pushButton_wps_load;
    QPushButton *pushButton;
    QWizardPage *wizardPage;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEdit;
    QPushButton *pushButton_RunWPS;
    QWizardPage *wizardPage_finish;

    void setupUi(QWizard *WPSWizard)
    {
        if (WPSWizard->objectName().isEmpty())
            WPSWizard->setObjectName(QString::fromUtf8("WPSWizard"));
        WPSWizard->resize(533, 500);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(WPSWizard->sizePolicy().hasHeightForWidth());
        WPSWizard->setSizePolicy(sizePolicy);
        WPSWizard->setMinimumSize(QSize(285, 203));
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QString::fromUtf8("wizardPage1"));
        horizontalLayout = new QHBoxLayout(wizardPage1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(wizardPage1);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        radioButton_new = new QRadioButton(groupBox);
        radioButton_new->setObjectName(QString::fromUtf8("radioButton_new"));
        radioButton_new->setChecked(true);

        verticalLayout->addWidget(radioButton_new);

        radioButton_contin = new QRadioButton(groupBox);
        radioButton_contin->setObjectName(QString::fromUtf8("radioButton_contin"));

        verticalLayout->addWidget(radioButton_contin);


        horizontalLayout->addWidget(groupBox);

        WPSWizard->setPage(1, wizardPage1);
        wizardPage2 = new QWizardPage();
        wizardPage2->setObjectName(QString::fromUtf8("wizardPage2"));
        verticalLayout_3 = new QVBoxLayout(wizardPage2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        calendarWidget = new QCalendarWidget(wizardPage2);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        calendarWidget->setFirstDayOfWeek(Qt::Monday);
        calendarWidget->setGridVisible(true);
        calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
        calendarWidget->setVerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers);

        verticalLayout_3->addWidget(calendarWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        lineEdit_st_date = new QLineEdit(wizardPage2);
        lineEdit_st_date->setObjectName(QString::fromUtf8("lineEdit_st_date"));

        horizontalLayout_4->addWidget(lineEdit_st_date);

        pushButton_set_st_date = new QPushButton(wizardPage2);
        pushButton_set_st_date->setObjectName(QString::fromUtf8("pushButton_set_st_date"));

        horizontalLayout_4->addWidget(pushButton_set_st_date);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEdit_end_date = new QLineEdit(wizardPage2);
        lineEdit_end_date->setObjectName(QString::fromUtf8("lineEdit_end_date"));

        horizontalLayout_3->addWidget(lineEdit_end_date);

        pushButton_set_end_date = new QPushButton(wizardPage2);
        pushButton_set_end_date->setObjectName(QString::fromUtf8("pushButton_set_end_date"));

        horizontalLayout_3->addWidget(pushButton_set_end_date);


        verticalLayout_3->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label = new QLabel(wizardPage2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        lineEdit_path = new QLineEdit(wizardPage2);
        lineEdit_path->setObjectName(QString::fromUtf8("lineEdit_path"));

        horizontalLayout_5->addWidget(lineEdit_path);

        pushButton_browse_path = new QPushButton(wizardPage2);
        pushButton_browse_path->setObjectName(QString::fromUtf8("pushButton_browse_path"));

        horizontalLayout_5->addWidget(pushButton_browse_path);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_2 = new QLabel(wizardPage2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_6->addWidget(label_2);

        lineEdit_path_grib = new QLineEdit(wizardPage2);
        lineEdit_path_grib->setObjectName(QString::fromUtf8("lineEdit_path_grib"));

        horizontalLayout_6->addWidget(lineEdit_path_grib);

        pushButton_browse_path_2 = new QPushButton(wizardPage2);
        pushButton_browse_path_2->setObjectName(QString::fromUtf8("pushButton_browse_path_2"));

        horizontalLayout_6->addWidget(pushButton_browse_path_2);


        verticalLayout_3->addLayout(horizontalLayout_6);

        pushButton_wps_load = new QPushButton(wizardPage2);
        pushButton_wps_load->setObjectName(QString::fromUtf8("pushButton_wps_load"));

        verticalLayout_3->addWidget(pushButton_wps_load);

        pushButton = new QPushButton(wizardPage2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_3->addWidget(pushButton);

        WPSWizard->setPage(2, wizardPage2);
        wizardPage = new QWizardPage();
        wizardPage->setObjectName(QString::fromUtf8("wizardPage"));
        verticalLayout_2 = new QVBoxLayout(wizardPage);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textEdit = new QTextEdit(wizardPage);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout_2->addWidget(textEdit);

        pushButton_RunWPS = new QPushButton(wizardPage);
        pushButton_RunWPS->setObjectName(QString::fromUtf8("pushButton_RunWPS"));

        verticalLayout_2->addWidget(pushButton_RunWPS);

        WPSWizard->setPage(3, wizardPage);
        wizardPage_finish = new QWizardPage();
        wizardPage_finish->setObjectName(QString::fromUtf8("wizardPage_finish"));
        WPSWizard->setPage(10, wizardPage_finish);

        retranslateUi(WPSWizard);

        QMetaObject::connectSlotsByName(WPSWizard);
    } // setupUi

    void retranslateUi(QWizard *WPSWizard)
    {
        WPSWizard->setWindowTitle(QApplication::translate("WPSWizard", "Wizard", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("WPSWizard", "GroupBox", 0, QApplication::UnicodeUTF8));
        radioButton_new->setText(QApplication::translate("WPSWizard", "New Domain", 0, QApplication::UnicodeUTF8));
        radioButton_contin->setText(QApplication::translate("WPSWizard", "Open or Delete Domain", 0, QApplication::UnicodeUTF8));
        pushButton_set_st_date->setText(QApplication::translate("WPSWizard", "Set Start DATE", 0, QApplication::UnicodeUTF8));
        pushButton_set_end_date->setText(QApplication::translate("WPSWizard", "Set End  DATE", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WPSWizard", "Domain path", 0, QApplication::UnicodeUTF8));
        pushButton_browse_path->setText(QApplication::translate("WPSWizard", "Browse", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("WPSWizard", "Grib       path", 0, QApplication::UnicodeUTF8));
        pushButton_browse_path_2->setText(QApplication::translate("WPSWizard", "Browse", 0, QApplication::UnicodeUTF8));
        pushButton_wps_load->setText(QApplication::translate("WPSWizard", "EditNamelist WPS", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("WPSWizard", "Init", 0, QApplication::UnicodeUTF8));
        pushButton_RunWPS->setText(QApplication::translate("WPSWizard", "Fire!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WPSWizard: public Ui_WPSWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WPSWIZARD_H
