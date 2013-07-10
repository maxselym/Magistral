/********************************************************************************
** Form generated from reading UI file 'dialog_nlwps.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_NLWPS_H
#define UI_DIALOG_NLWPS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Dialog_NLWPS
{
public:

    void setupUi(QDialog *Dialog_NLWPS)
    {
        if (Dialog_NLWPS->objectName().isEmpty())
            Dialog_NLWPS->setObjectName(QString::fromUtf8("Dialog_NLWPS"));
        Dialog_NLWPS->resize(400, 300);

        retranslateUi(Dialog_NLWPS);

        QMetaObject::connectSlotsByName(Dialog_NLWPS);
    } // setupUi

    void retranslateUi(QDialog *Dialog_NLWPS)
    {
        Dialog_NLWPS->setWindowTitle(QApplication::translate("Dialog_NLWPS", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog_NLWPS: public Ui_Dialog_NLWPS {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_NLWPS_H
