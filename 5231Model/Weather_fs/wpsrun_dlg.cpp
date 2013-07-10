#include "wpsrun_dlg.h"
#include "ui_wpsrun_dlg.h"

wpsrun_dlg::wpsrun_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wpsrun_dlg)
{
    b_manual = false;
    ui->setupUi(this);
    p_wpsrun = new WPSRun;
    proc = NULL;
    DisableControls(true);
}

wpsrun_dlg::~wpsrun_dlg()
{

    delete ui;
}

void wpsrun_dlg::ProcSetup()
{
    DisableControls(true);
    ui->pushButton_init->setDisabled(true);
    if(proc == NULL)
    {
        proc = new QProcess();
        proc->setWorkingDirectory(p_wpsrun->GetWorkFolder()); //рабочая директория
        //  proc->setProcessChannelMode(QProcess::MergedChannels); //Читаем весь вывод!
        connect(proc,SIGNAL(readyReadStandardOutput()),this, SLOT(ReadStdout())); //Если есть информация в стандарном выводе то вызваем readstdout
        connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(Finished(int,QProcess::ExitStatus)));
        n_proc = 0;
    }
}

void wpsrun_dlg::on_pushButton_run_clicked()
{
    n_proc = 0;
    b_manual = false;
    ProcSetup();
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
}

void wpsrun_dlg::ReadStdout()
{
    ui->textEdit->append(proc->readAllStandardOutput()); //читаем стандартный вывод
}
void wpsrun_dlg::Run_proc(QString output_root,int n)
{
    QString report;
    if(b_manual) //Если в ручном режиме, то выходим
    {
        DisableControls(false);
        b_manual = false;
        return;
    }
    switch(n) {
    case GEOGRID:   //0
        n_proc++;
        proc->start(output_root + "geogrid.exe");
        break;
    case UNGRIB:    //1
        n_proc++;
        proc->start(output_root + "ungrib.exe");
        break;
    case METGRID:   //2
        n_proc++;
        proc->start(output_root + "metgrid.exe");
        break;
    case REAL:      //3
        n_proc++;
        if(ui->checkBoxMPI->checkState())
        {
            //Intel
            proc->start(QString("mpirun -np %1 ").arg(p_wpsrun->GetCoreNum())+output_root + "real.exe");
        }
        else
        {
            //OpenMPI
            proc->start(QString("mpirun -np %1 --hostfile mpd.hosts ").arg(p_wpsrun->GetCoreNum())+output_root + "real.exe");
        }
        break;
    case OBSPROC:   //4
        n_proc++;
        proc->start(output_root + "obsproc.exe");
        break;
    case DA:        //5
        n_proc++;
        if(!QFile::link(output_root+"wrfinput_d01", output_root+"fg"))// link first guess file as fg
        {
            ui->listWidget->addItem("Error link wrfinput_d01 to fg");
            Finished(1,QProcess::CrashExit);
        }
        if(!QFile::link(output_root+p_wpsrun->GetOBSPROC3DVARName(), output_root+"ob.ascii"))// link OBSPROC processed observation file as ob.ascii
        {
            ui->listWidget->addItem("Error link");
            ui->listWidget->addItem(p_wpsrun->GetOBSPROC3DVARName());
            ui->listWidget->addItem("to ob.ascii");
            Finished(1,QProcess::CrashExit);
        }
        proc->start(output_root + "da_wrfvar.exe");
        break;
    case UPDATEBC:  //6
        n_proc++;
        proc->start(output_root + "da_update_bc.exe");
        break;
    case WRF:       //7
        n_proc++;
        if(ui->checkBoxMPI->checkState())
        {
            //Intel
            proc->start(QString("mpirun -np %1 ").arg(p_wpsrun->GetCoreNum())+output_root + "wrf.exe");
        }
        else
        {
            //OpenMPI
            proc->start(QString("mpirun -np %1 --hostfile mpd.hosts ").arg(p_wpsrun->GetCoreNum())+output_root + "wrf.exe");
        }
        break;
    case ARWpost:   //8
        n_proc++;
        proc->start(output_root + "ARWpost.exe");
        break;
    case 9:         //9
        if(!p_wpsrun->MoveWrfOut(p_wpsrun->GetOutFolder(), report))
        {
            ui->listWidget->addItem(report);
            Finished(1,QProcess::CrashExit);
        } else
        {
            ui->listWidget->addItem(report);
            n_proc++;
            Finished(0,QProcess::NormalExit);
        }
        break;
    default:
        ui->listWidget->addItem("All DONE!!!");
        DisableControls(false);
        ui->pushButton_init->setDisabled(false);
    };
}
void wpsrun_dlg::Finished(int n, QProcess::ExitStatus)
{
    if(n == 0)
    {
        ui->listWidget->addItem(QString("Module %1 finished!").arg(n_proc));
        Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    }
    else
    {
        ui->listWidget->addItem("Error!!!");
        DisableControls(false);
        b_manual = false;
    }
}

void wpsrun_dlg::on_pushButton_time_from_grib_clicked()
{
    p_wpsrun->SetTimeDateFromGRIB();
    ui->textEdit->append("Date and time from GRIB files:");
    ui->textEdit->append(p_wpsrun->GetTimeDateRUN());
    if(!p_wpsrun->initialize())
    {
        ui->textEdit->append("Error Link files!");
        DisableControls(true);
    }
    else
    {
        ui->textEdit->append("File linked!");
    }
}
void wpsrun_dlg::DisableControls(bool b)
{
    ui->pushButton_time_from_grib->setDisabled(b);
    ui->pushButton_run->setDisabled(b);
    ui->pushButton_geogrid->setDisabled(b);
    ui->pushButton_Ungrib->setDisabled(b);
    ui->pushButton_Metgrid->setDisabled(b);
    ui->pushButton_Real->setDisabled(b);
    ui->pushButton_OBSPROC->setDisabled(b);
    ui->pushButton_da->setDisabled(b);
    ui->pushButton_UpdateBC->setDisabled(b);
    ui->pushButton_wrf->setDisabled(b);
}

void wpsrun_dlg::on_pushButton_init_clicked()
{
    if(!p_wpsrun->ReadPreferenceXML("preferences.xml"))
    {
        ui->textEdit->append("Error reading preference.xml");
        DisableControls(true);
        return;
    }
    else
    {
        ui->textEdit->append("Preferences loaded!");
    }
    if(!p_wpsrun->ReadWFS())
    {
        ui->textEdit->append("Error read WFS!");
        DisableControls(true);
    }
    else
    {
        ui->textEdit->append("WFS readed!");
    }
    ui->textEdit->append(p_wpsrun->GetTimeDateRUN());
    if(!p_wpsrun->initialize())
    {
        ui->textEdit->append("Error Link files!");
        DisableControls(true);
    }
    else
    {
        ui->textEdit->append("File linked!");
        ui->lcdNumber->display(p_wpsrun->GetCoreNum());
        DisableControls(false);
    }
}

void wpsrun_dlg::on_pushButton_geogrid_clicked()
{
    ProcSetup();
    n_proc = GEOGRID;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_Ungrib_clicked()
{
    ProcSetup();
    n_proc = UNGRIB;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_Metgrid_clicked()
{
    ProcSetup();
    n_proc = METGRID;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_Real_clicked()
{
    ProcSetup();
    n_proc = REAL;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_OBSPROC_clicked()
{
    ProcSetup();
    n_proc = OBSPROC;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_da_clicked()
{
    ProcSetup();
    n_proc = DA;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}

void wpsrun_dlg::on_pushButton_UpdateBC_clicked()
{
    ProcSetup();
    n_proc = UPDATEBC;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}



void wpsrun_dlg::on_pushButton_wrf_clicked()
{
    ProcSetup();
    n_proc = WRF;
    Run_proc(p_wpsrun->GetWorkFolder(), n_proc);
    b_manual = true;
}
