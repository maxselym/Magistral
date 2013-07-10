#include "special_param_dlg.h"
#include "ui_special_param_dlg.h"
#include "QFileInfo"
#include "QFile"
#include "QFileDialog"
#include "QCloseEvent"
#include "QMessageBox"

Special_param_dlg::Special_param_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Special_param_dlg)
{
    ui->setupUi(this);
    statusBar = new QStatusBar();
    statusBar->showMessage(tr("Ready"));
    ui->verticalLayout->addWidget(statusBar);
    s_param = new special_param(this);
    SetTitle("Special Parameters");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Index")));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("x")));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("y")));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("z")));
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Режим выбора в таблице - синглселект
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // Режим выбора в таблице - выбираем строки
}

void Special_param_dlg::closeEvent(QCloseEvent *event)
{
    if (true)   //The dialog should close
    {
        event->accept();
        s_param->db->closeConnection();
    }
    else
        event->ignore(); // Don't close.
 }

Special_param_dlg::~Special_param_dlg()
{
    delete ui;
}

void Special_param_dlg::on_pushButton_loadNetCDF_clicked()
{
    /** Чистим таблицу со станциями**/
    while(ui->tableWidget->rowCount()!=0)
    {
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
    }
    /** Открываем диалог для выбора файла**/
    QString curFile = QFileDialog::getOpenFileName(this, tr("Open NetCDF"),
                                         QDir::currentPath(),
                                         tr("Other files (*);; NetCDF Files (*.nc)"));
    QFileInfo f_info(curFile);
    if (curFile.isEmpty())
        return;

    /** Открываем NetCDF**/
    if(s_param->OpenNetCDFFile(curFile))
    {
        statusBar->showMessage(f_info.baseName() +" Loaded!", 5000);
        SetTitle(f_info.baseName());
        ui->horizontalSlider_Time->setEnabled(true);
        ui->horizontalSlider_Time->setMinimum(0);
        ui->horizontalSlider_Time->setMaximum(s_param->GetDimSize(0)-1); //Устанавливаем длину слайдера равную длине оси времени Dim 0
    }
    else statusBar->showMessage("Load NetCDF ERROR!!!",5000);
    /** Зачитываем список станций РЖД**/
    if(!FetchStationList())
        statusBar->showMessage("Database ERROR!!!",5000);

}

void Special_param_dlg::SetTitle(QString s_title)
{
    QFileInfo pathInfo(s_title);
    setWindowTitle(pathInfo.fileName());
}
bool Special_param_dlg::FetchStationList()
{
    QVector<int> vector = s_param->GetStnListInArea();
    float x=0;
    float y=0;
    float z=0;
    QString str;
    for(int i=0; i<vector.count();i++)
    {
        if(s_param->GetStationCoordinates(vector[i], x,y,z))
        {
            ui->tableWidget->insertRow(0);
            ui->tableWidget->setItem(0, 0, new QTableWidgetItem(str.setNum(vector[i]))); //Индекс станции
            ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEnabled); //Выключаем редактрование
            ui->tableWidget->setItem(0, 1, new QTableWidgetItem(s_param->db->GetStnNameByIndex(vector[i]))); //Имя станции
            ui->tableWidget->setItem(0, 2, new QTableWidgetItem(str.setNum(x))); //x
            ui->tableWidget->setItem(0, 3, new QTableWidgetItem(str.setNum(y))); //y
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    return true;
}

void Special_param_dlg::on_horizontalSlider_Time_valueChanged(int value)
{
    ui->label_timeslider->setText(s_param->GetTime(value));
    if(ui->tableWidget->currentRow() >= 0) //Выбрана хотябы одна строчка
        on_tableWidget_itemSelectionChanged();
}


void Special_param_dlg::on_tableWidget_itemSelectionChanged()
{
    QString s_st_num ="";
    if(ui->tableWidget->currentIndex().row() >= 0)
    {
       s_st_num = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0)->text();
    }
    int n_time = ui->horizontalSlider_Time->value(); //Берем время из слайдера (номер позиции)


    float f_lat=0;
    float f_long=0;
    float z=0;

    if(s_param->GetStationCoordinates(s_st_num.toInt(), f_lat,f_long,z))
    {
        int out_lat = 0;
        int out_lon = 0;
        s_param->GetClosestXLAT_XLONG(n_time,f_lat,f_long,out_lat,out_lon);
        float Vis = s_param->GetVis(n_time,f_lat,f_long);
        float f_t = s_param->GetT2M_C(n_time,out_lat,out_lon);
        float f_p = s_param->GetPSFC(n_time,out_lat,out_lon);
        float WSpeed = s_param->GetWindSpeed(n_time,out_lat,out_lon);
        float WAngle = s_param->GetWindAngle(n_time,out_lat,out_lon);
        float T_rail = s_param->GetRailwayT(s_st_num, n_time,out_lat,out_lon)-273.16;
        int GlaccyIce = s_param->GetGlaccyIce(n_time,out_lat,out_lon);
        int Hurricane = s_param->GetHurricane(n_time,out_lat,out_lon);
        ui->textEdit_Pressure->setText(tr("%1").arg(f_p));
        ui->textEdit_Temperature->setText(tr("%1").arg(f_t));
        ui->textEdit_wind_speed->setText(tr("%1").arg(WSpeed));
        ui->textEdit_windAngle->setText(tr("%1").arg(WAngle));
        if(GlaccyIce == 0)
            ui->textEdit_GlaccyIce->setText("No");
        else
            ui->textEdit_GlaccyIce->setText("Yes");
        if(Hurricane == 0)
            ui->textEdit_hurricane->setText("No");
        else
            ui->textEdit_hurricane->setText("Yes");
        ui->textEdit_railway->setText(tr("%1").arg(T_rail));
        ui->textEdit_visibility->setText(tr("%1").arg(Vis));
    }

}

void Special_param_dlg::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(ui->tableWidget->currentIndex().row() >= 0) //Определяем, что курсор стоит в строчке
    {
        QString s_index = ui->tableWidget->item(item->row(), 0)->text();
        QString s_name = ui->tableWidget->item(item->row(), 1)->text();
        QString s_lat= ui->tableWidget->item(item->row(), 2)->text();
        QString s_long = ui->tableWidget->item(item->row(), 3)->text();

        if(s_param->db->AlterStnRowByIndex(s_name, s_lat, s_long, "2", s_index))//Z  в таблице не играется пока стоит заглушка
        {
            statusBar->showMessage("Cell changed!", 1000);
        }
        else statusBar->showMessage("Error edit DB!", 1000);
    }
}

void Special_param_dlg::on_pushButtonSendSelectedToDB_clicked()
{
    this->setCursor(Qt::WaitCursor); // курсор - часики
    statusBar->showMessage("Work!", 1000);
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows(); //Берем выбранные строки

    if(selected.count() == 0) //Если ничего не выбрано
    {
        this->setCursor(Qt::ArrowCursor); // Обычный курсор
        QMessageBox msg;
        msg.setText("Nothing selected!!!");
        msg.exec();
        return;
    }

    int indexRow;

    for(int i= 0; i< selected.count();i++)
    {
        indexRow = selected.at(i).row(); //Номер выбранной строки
    }
    int index = ui->tableWidget->item(indexRow, 0)->text().toInt(); // Индекс станции

    for(int j=0;j<s_param->GetDimSize(0)-1;j++)
    {
        statusBar->showMessage(QString("%1 Send").arg(j), 10);
        s_param->FillSpecarr(index,j);
    }
    this->setCursor(Qt::ArrowCursor); // Обычный курсор
    statusBar->showMessage("Done!", 1000);
}
void Special_param_dlg::on_pushButtonSendAlltoDB_clicked()
{
    this->setCursor(Qt::WaitCursor);
    statusBar->showMessage("Working!", 10);
    int index;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        statusBar->showMessage(QString("%1 Send").arg(i), 10);
        index = ui->tableWidget->item(i, 0)->text().toInt();
        for(int j=0;j<s_param->GetDimSize(0)-1;j++)
        {
            s_param->FillSpecarr(index,j);
        }
    }
    this->setCursor(Qt::ArrowCursor);
    statusBar->showMessage("Done!", 10);
}
void Special_param_dlg::on_pushButtonSendSelectedLittle_r_ASCII_clicked()
{
    QModelIndexList selected = ui->tableWidget->selectionModel()->selectedRows(); //Берем выбранные строки

    if(selected.count() == 0) //Если ничего не выбрано
    {
        QMessageBox msg;
        msg.setText("Nothing selected!!!");
        msg.exec();
        return;
    }

    int indexRow;
    for(int i= 0; i< selected.count();i++)
    {
        indexRow = selected.at(i).row(); //Номер выбранной строки
    }
    int index = ui->tableWidget->item(indexRow, 0)->text().toInt(); // Индекс станции

    QString s_time = s_param->GetTime(ui->horizontalSlider_Time->value()); //Берем время из слайдера;

    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Document"),
        QDir::currentPath(),
        tr("ASCII (*.ASCII)") );

    Little_r_fm12* little_r_fm12 = new Little_r_fm12(this); /**< указатель на Класс для записи в Little_r приземных наблюдений*/
    little_r_fm12->OpenFile(filename); //Открываем файл для записи
    QDateTime date = QDateTime::fromString(s_time,"yyyy-MM-dd_hh:mm:ss");

    little_r_fm12->writeRZD_DB_data(index,date); //Пишем станцию

    little_r_fm12->CloseFile(); // Закрываем файл
}

void Special_param_dlg::on_pushButtonSendAllLittle_R_clicked()
{
    this->setCursor(Qt::WaitCursor);
    QString s_time = s_param->GetTime(ui->horizontalSlider_Time->value()); //Берем время из слайдера;

    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save Document"),
        QDir::currentPath(),
        tr("ASCII (*.ASCII)") );

    Little_r_fm12* little_r_fm12 = new Little_r_fm12(this); /**< указатель на Класс для записи в Little_r приземных наблюдений*/
    little_r_fm12->OpenFile(filename); //Открываем файл для записи
    QDateTime date = QDateTime::fromString(s_time,"yyyy-MM-dd_hh:mm:ss");

    this->setCursor(Qt::WaitCursor);
    int index;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        index = ui->tableWidget->item(i, 0)->text().toInt(); //Индекс станции
        little_r_fm12->writeRZD_DB_data(index,date); //Пишем станцию
    }
    little_r_fm12->CloseFile(); // Закрываем файл
    this->setCursor(Qt::ArrowCursor);
}
