/********************************************************************************
** Form generated from reading UI file 'wpsform.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WPSFORM_H
#define UI_WPSFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_WPSForm
{
public:

    void setupUi(QDialog *WPSForm)
    {
        if (WPSForm->objectName().isEmpty())
            WPSForm->setObjectName(QString::fromUtf8("WPSForm"));
        WPSForm->resize(400, 450);
        WPSForm->setMinimumSize(QSize(285, 65));

        retranslateUi(WPSForm);

        QMetaObject::connectSlotsByName(WPSForm);
    } // setupUi

    void retranslateUi(QDialog *WPSForm)
    {
        WPSForm->setWindowTitle(QApplication::translate("WPSForm", "WPS", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WPSForm: public Ui_WPSForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WPSFORM_H
