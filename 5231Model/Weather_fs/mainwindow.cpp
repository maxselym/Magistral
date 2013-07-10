#include <QtGui>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251")); //Выбираем кодировку для всего проекта


    ui->setupUi(this);
    workspace = new QWorkspace;
    setCentralWidget(workspace);

  //  openWps(); //Запускаем окошко с WPS для начала

    customize(); //Устанавливаем связи между слотами и Action-ми

    /** Resize window to maximum size**/
    QDesktopWidget *desktop = QApplication::desktop(); /** Get device size**/
    QRect screenSize = desktop->screenGeometry();
    this->resize(screenSize.width(),screenSize.height()); /** Set window size**/

    p_special_param_dlg = NULL;

}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::changeEvent(QEvent *e)
//{
//    //Возможно понадобится...
////    QMainWindow::changeEvent(e);
////    switch (e->type()) {
////    case QEvent::LanguageChange:
////        ui->retranslateUi(this);
////        break;
////    default:
////        break;
//    //    }
//}
void MainWindow::WPS_setup()
{
    if(workspace->windowList().indexOf(p_WPS_set_dlg)==-1)
    {
        p_WPS_set_dlg = new WPS_set_dlg;
        workspace->addWindow(p_WPS_set_dlg);
        p_WPS_set_dlg->show();
    }else workspace->setActiveWindow(p_WPS_set_dlg);

}
void MainWindow::WPS_namelist()
{
    if(workspace->windowList().indexOf(p_namelist_wps)==-1)
    {
        p_namelist_wps = new dlg_namelist_edit;
        workspace->addWindow(p_namelist_wps);
        p_namelist_wps->show();
    }else workspace->setActiveWindow(p_namelist_wps);
}
void MainWindow::WPS_run()
{
    if(workspace->windowList().indexOf(p_wpsrun_dlg)==-1)
    {
        p_wpsrun_dlg = new wpsrun_dlg;
        workspace->addWindow(p_wpsrun_dlg);
        p_wpsrun_dlg->show();
    }else workspace->setActiveWindow(p_wpsrun_dlg);
}

void MainWindow::openWrf()
{
    WPS_run();
}

void MainWindow::openWrfDA()
{
    QMessageBox msgBox;
    msgBox.setText("WRFDA");
    msgBox.exec();
}

void MainWindow::preferences()
{
    Preferences* p_dialog;
    p_dialog = new Preferences(this);
    p_dialog->exec();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Weather_fs"),
            tr("<h2>Weather_fs</h2>"
               "<p>Copyright &copy; 2012 by Maxselym"
               "<p>Weather_fs is a small application that help "
               " to run WRF .</p>"));
}

void MainWindow::customize()
{
    //WRF menu
    connect(ui->actionSetup_WPS,SIGNAL(triggered()),this,SLOT(WPS_setup()));
    connect(ui->actionRun_WPS, SIGNAL(triggered()), this, SLOT(WPS_run()));
    connect(ui->actionEdit_namelist_wps,SIGNAL(triggered()),this,SLOT(WPS_namelist()));
    connect(ui->actionCalc_Special_Parameters,SIGNAL(triggered()),this,SLOT(OpenSpecParam()));
    connect(ui->actionWeather_stations,SIGNAL(triggered()),this,SLOT(OpenRawPGP()));



    connect(ui->actionWRF, SIGNAL(triggered()), this,
             SLOT(openWrf()));
    connect(ui->actionWRFDA, SIGNAL(triggered()), this,
            SLOT(openWrfDA()));
    connect(ui->action_Preferences, SIGNAL(triggered()), this,
            SLOT(preferences()));


    //Window managment
    connect(ui->action_Cascade, SIGNAL(triggered()),
            workspace, SLOT(cascade()));

    connect(ui->actionTile, SIGNAL(triggered()),
            workspace, SLOT(tile()));

    connect(ui->action_Next, SIGNAL(triggered()),
            workspace, SLOT(activateNextWindow()));

    connect(ui->actionPre_vious, SIGNAL(triggered()),
            workspace, SLOT(activatePreviousWindow()));

    connect(ui->action_Close, SIGNAL(triggered()),
            workspace, SLOT(closeActiveWindow()));

    connect(ui->actionClose_All, SIGNAL(triggered()),
            workspace, SLOT(closeAllWindows()));

    ui->action_Close->setShortcut(QKeySequence::Close);


    //About
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_About, SIGNAL(triggered()), this, SLOT(about()));
    //About

}

void MainWindow::OpenSpecParam()
{
    if(workspace->windowList().indexOf(p_special_param_dlg)==-1)
    {
        p_special_param_dlg = new Special_param_dlg;
        workspace->addWindow(p_special_param_dlg);
        p_special_param_dlg->show();
    }else workspace->setActiveWindow(p_special_param_dlg);
}

void MainWindow::OpenRawPGP()
{
    if(workspace->windowList().indexOf(p_raw_pgp_dlg)==-1)
    {
        p_raw_pgp_dlg = new raw_pgp_dlg;
        workspace->addWindow(p_raw_pgp_dlg);
        p_raw_pgp_dlg->show();
    }else workspace->setActiveWindow(p_raw_pgp_dlg);
}
