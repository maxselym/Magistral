#include "raw_pgp_dlg.h"
#include "ui_raw_pgp_dlg.h"
#include "QMessageBox"
#include "QFileDialog"

raw_pgp_dlg::raw_pgp_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::raw_pgp_dlg)
{
    ui->setupUi(this);
    dbengine = NULL;
    statusBar = new QStatusBar();
    statusBar->showMessage(tr("Ready"));
    ui->verticalLayout->addWidget(statusBar);
    setWindowTitle("Weather Station Data");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Index")));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("x")));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("y")));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("z")));
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Режим выбора в таблице - синглселект
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // Режим выбора в таблице - выбираем строки
    vecDay = 0;
    vecTime = 0;
    init();
}

bool raw_pgp_dlg::FetchStationList()
{
    QVector<int> vector = dbengine->GetStnIndexLst();
    float x=0;
    float y=0;
    float z=0;
    QString str;
    for(int i=0; i<vector.count();i++)
    {
        if(dbengine->GetStationCoordinates(vector[i], x,y,z))
        {
            ui->tableWidget->insertRow(0);
            ui->tableWidget->setItem(0, 0, new QTableWidgetItem(str.setNum(vector[i]))); //Индекс станции
            ui->tableWidget->item(0,0)->setFlags(Qt::ItemIsEnabled); //Выключаем редактрование
            ui->tableWidget->setItem(0, 1, new QTableWidgetItem(dbengine->GetStnNameByIndex(vector[i]))); //Имя станции
            ui->tableWidget->setItem(0, 2, new QTableWidgetItem(str.setNum(x))); //x
            ui->tableWidget->setItem(0, 3, new QTableWidgetItem(str.setNum(y))); //y
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    return true;
}

raw_pgp_dlg::~raw_pgp_dlg()
{
    delete ui;
}

void raw_pgp_dlg::init()
{
    if(dbengine==NULL)
    {
        dbengine = new DBEngine(this);
    }
    if(!dbengine->IsConnected()) //Если нет соединения, то соединяем
    {
        if(!dbengine->createConnection()) //Если нет соединение не открылось, то жалуемся
        {
            statusBar->showMessage("Cannot open database:" + dbengine->LastError().text());
            return;
        }
    }
    /** Чистим таблицу со станциями**/
    while(ui->tableWidget->rowCount()!=0)
    {
        ui->tableWidget->removeRow(ui->tableWidget->rowCount()-1);
    }


    //    ui->horizontalSlider_Time->setEnabled(true);
     //   ui->horizontalSlider_Time->setMinimum(0);
     //   ui->horizontalSlider_Time->setMaximum(s_param->GetDimSize(0)-1); //Устанавливаем длину слайдера равную длине оси времени Dim 0
    /** Зачитываем список станций РЖД**/
    if(!FetchStationList())
        statusBar->showMessage("Database ERROR!!!",5000);
}

void raw_pgp_dlg::on_tableWidget_itemSelectionChanged()
{

    if(ui->tableWidget->currentIndex().row() >= 0)
    {
       n_st_num = ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0)->text().toInt();
    }
    vecDay = dbengine->GetStnDateByIndex(n_st_num);
    ui->comboBoxDate->clear();
    for(int i=0; i<vecDay->count();i++)
    {
        ui->comboBoxDate->addItem(vecDay->at(i).toString("dd.MM.yyyy"));
    }
}

void raw_pgp_dlg::on_comboBoxDate_currentIndexChanged(int index)
{
    if(index<0)
    {
         ui->comboBoxTime->clear();
         return;
    }
    QDate dt = vecDay->at(index);
    vecTime = dbengine->GetStnTimeByIndex(n_st_num,dt);
    ui->comboBoxTime->clear();
    for(int i=0; i<vecTime->count();i++)
    {
        ui->comboBoxTime->addItem(vecTime->at(i).toString("hh:mm:ss"));
    }
}

void raw_pgp_dlg::on_pushButton_selectedToLittle_R_clicked()
{


    //    little_r_fm12->writeHeader(60.00, -52.75,
    //                               "4220", "SFC OBS from NCAR ADP DS464.0", "FM-12 TEMP", "",
    //                               48,
    //                               6,     //Количество полей
    //                               0,
    //                               0,
    //                               1,     //Количество измерений
    //                               0,
    //                               true,false,false,
    //                               little_r_fm12->NoData,
    //                               little_r_fm12->NoData,
    //                               date,
    //                               99960,0);
    //    little_r_fm12->writeData(102930.,                   //Давление
    //                             7.,                        //Высота
    //                             262.05,                    //Температура
    //                             258.45,                    //Точка росы
    //                             1.02889,                   //Сила ветра
    //                             120.,                      //Направление ветра
    //                             little_r_fm12->NoData,     // U
    //                             little_r_fm12->NoData,     // V
    //                             little_r_fm12->NoData,     // RH
    //                             little_r_fm12->NoData);    //Thickness
    //    little_r_fm12->writeEnding();
    //    little_r_fm12->writeTail(1,0,0);
    //    little_r_fm12->CloseFile();

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

        QString s_time = ui->comboBoxTime->currentText();
        QString s_date = ui->comboBoxDate->currentText();
        QString s_date_time = s_date+"_"+s_time;
        QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save Document"),
            QDir::currentPath(),
            tr("ASCII (*.ASCII)") );

        Little_r_fm12* little_r_fm12 = new Little_r_fm12(this); /**< указатель на Класс для записи в Little_r приземных наблюдений*/
        little_r_fm12->OpenFile(filename); //Открываем файл для записи
        QDateTime date = QDateTime::fromString(s_date_time,"dd.MM.yyyy_hh:mm:ss");

        little_r_fm12->writeRZD_DB_data(index,date); //Пишем станцию

        little_r_fm12->CloseFile(); // Закрываем файл
}
