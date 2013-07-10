/********************************************************************************
** Form generated from reading UI file 'namelist_wps.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAMELIST_WPS_H
#define UI_NAMELIST_WPS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_namelist_wps
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QAction *actionAdd_Item;
    QAction *Save_As;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *namelist_wps)
    {
        if (namelist_wps->objectName().isEmpty())
            namelist_wps->setObjectName(QString::fromUtf8("namelist_wps"));
        namelist_wps->resize(689, 371);
        actionOpen = new QAction(namelist_wps);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(namelist_wps);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionExit = new QAction(namelist_wps);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAdd_Item = new QAction(namelist_wps);
        actionAdd_Item->setObjectName(QString::fromUtf8("actionAdd_Item"));
        Save_As = new QAction(namelist_wps);
        Save_As->setObjectName(QString::fromUtf8("Save_As"));
        centralwidget = new QWidget(namelist_wps);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        namelist_wps->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(namelist_wps);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        namelist_wps->setStatusBar(statusbar);
        toolBar = new QToolBar(namelist_wps);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        namelist_wps->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addAction(Save_As);
        toolBar->addAction(actionExit);
        toolBar->addSeparator();

        retranslateUi(namelist_wps);
        QObject::connect(actionExit, SIGNAL(activated()), namelist_wps, SLOT(close()));

        QMetaObject::connectSlotsByName(namelist_wps);
    } // setupUi

    void retranslateUi(QMainWindow *namelist_wps)
    {
        namelist_wps->setWindowTitle(QApplication::translate("namelist_wps", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("namelist_wps", "Open", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("namelist_wps", "Save", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("namelist_wps", "Exit", 0, QApplication::UnicodeUTF8));
        actionAdd_Item->setText(QApplication::translate("namelist_wps", "Add Item", 0, QApplication::UnicodeUTF8));
        Save_As->setText(QApplication::translate("namelist_wps", "Save As...", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("namelist_wps", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class namelist_wps: public Ui_namelist_wps {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAMELIST_WPS_H
