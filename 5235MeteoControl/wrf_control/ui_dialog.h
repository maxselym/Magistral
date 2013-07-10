/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *LoadNetCDF;
    QPushButton *LoadCSV;
    QPushButton *Check_button;
    QLabel *StatusLabel;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(660, 342);
        verticalLayout_2 = new QVBoxLayout(Dialog);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        LoadNetCDF = new QPushButton(Dialog);
        LoadNetCDF->setObjectName(QString::fromUtf8("LoadNetCDF"));

        horizontalLayout->addWidget(LoadNetCDF);

        LoadCSV = new QPushButton(Dialog);
        LoadCSV->setObjectName(QString::fromUtf8("LoadCSV"));

        horizontalLayout->addWidget(LoadCSV);

        Check_button = new QPushButton(Dialog);
        Check_button->setObjectName(QString::fromUtf8("Check_button"));

        horizontalLayout->addWidget(Check_button);


        verticalLayout_2->addLayout(horizontalLayout);

        StatusLabel = new QLabel(Dialog);
        StatusLabel->setObjectName(QString::fromUtf8("StatusLabel"));
        StatusLabel->setFrameShape(QFrame::Panel);
        StatusLabel->setFrameShadow(QFrame::Plain);
        StatusLabel->setLineWidth(2);
        StatusLabel->setTextFormat(Qt::AutoText);
        StatusLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(StatusLabel);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        LoadNetCDF->setText(QApplication::translate("Dialog", "Load NetCDF...", 0, QApplication::UnicodeUTF8));
        LoadCSV->setText(QApplication::translate("Dialog", "Load CSV...", 0, QApplication::UnicodeUTF8));
        Check_button->setText(QApplication::translate("Dialog", "Check!", 0, QApplication::UnicodeUTF8));
        StatusLabel->setText(QApplication::translate("Dialog", "Ready!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
