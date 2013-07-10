#include "dialog.h"
#include "ui_dialog.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QStringListModel"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    Table = new QTableWidget(0, 6);
    ui->verticalLayout_2->addWidget(Table);
    QStringList labels;
    labels << tr("Station name") << tr("x")<<tr("y")<<tr("z")<<tr("Pressure")<<tr("Temperature");
    Table->setHorizontalHeaderLabels(labels);
    Table->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    Table->verticalHeader()->hide();
    Table->setShowGrid(false);
    if(!LoadPreferences("preferences.wfs"))
    {
        QMessageBox::warning(this,"WARNING","File preferences.wfs missing!");
    }
    wrffile = new WrfFileRead;

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_LoadNetCDF_clicked()
{
    QString curFile = QFileDialog::getOpenFileName(this, tr("Open NetCDF"),
                                         QDir::currentPath(),
                                         tr("NetCDF Files (*.nc)"));
    QFileInfo f_info(curFile);

    if (curFile.isEmpty())
        return;
    if(wrffile->OpenFile(curFile))
    {
        this->ui->StatusLabel->setText(f_info.baseName() +" Loaded!");
    }
    else this->ui->StatusLabel->setText("Load NetCDF ERROR!!!");
}

bool Dialog::LoadPreferences(QString fileName)
{
    station_info = new xml_namelist;
    if(!station_info->ReadXml(fileName))
        return false;
    else
    {
        int x=0;
        int y=0;
        int z=0;
        for(int i=1;i<station_info->GetStationCount()+1;i++)
        {
            QString str = station_info->GetStation(i,x,y,z);
            int row = Table->rowCount();
            Table->insertRow(row);
            Table->setItem(row, 0, new QTableWidgetItem(str));
            Table->setItem(row, 1, new QTableWidgetItem(tr("%1").arg(x)));
            Table->setItem(row, 2, new QTableWidgetItem(tr("%1").arg(y)));
            Table->setItem(row, 3, new QTableWidgetItem(tr("%1").arg(z)));
            Table->setItem(row, 4, new QTableWidgetItem(tr("%1").arg(0)));
            Table->setItem(row, 5, new QTableWidgetItem(tr("%1").arg(0)));
        }
    }return true;

}

void Dialog::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to close?");
    msgBox.setStandardButtons(QMessageBox::Close | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Close);
    int result = msgBox.exec();
    switch (result) {
      case QMessageBox::Close:
          event->accept();
          break;
      case QMessageBox::Cancel:
          event->ignore();
          break;
      default:
          QDialog::closeEvent(event);
          break;
    }
}

void Dialog::on_LoadCSV_clicked()
{
    QString curFile = QFileDialog::getOpenFileName(this, tr("Open CSV"),
                                         QDir::currentPath(),
                                         tr("CSV Files (*.csv)"));
    QFileInfo f_info(curFile);

    if (curFile.isEmpty())
        return;
    if(CSVReader(curFile))
    {
        this->ui->StatusLabel->setText(f_info.baseName() +" Loaded!");
    }
    else this->ui->StatusLabel->setText("Load CSV ERROR!!!");
}

int Dialog::FindItem(QString station_name)
{
    QList<QTableWidgetItem *> list = Table->findItems(station_name,Qt::MatchExactly);
    if(list.count()!=1)
        qDebug()<<"Dublicate found!";
    else return list[0]->row();
    return -1;
}

bool Dialog::CSVReader(QString fileName)
{
    QFile inputFile(fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL,"Alert","File not found");
        return NULL;
    }
    QTextStream* in = new QTextStream(&inputFile);
    QString line;
    line = in->readLine();
    while (!line.isNull())
    {
        QStringList lst = line.split(",");
        int n = FindItem(lst[1]);
        if(n>=0)
        {
            Table->setItem(n,4,new QTableWidgetItem(lst[4]));
            Table->setItem(n,5,new QTableWidgetItem(lst[3]));
        }
        else qDebug()<<"Station not found";
        line = in->readLine();
    }

    return true;
}

void Dialog::on_Check_button_clicked()
{
    Analize();
}

void Dialog::Analize()
{
    for(int i=0;i<Table->rowCount();i++)
    {
        int x = Table->item(i,1)->text().toInt();
        int y = Table->item(i,2)->text().toInt();
        int z = Table->item(i,3)->text().toInt();
        float press = wrffile->GetVar4D("P",1,z,x,y)/100;
        float temp = wrffile->GetVar3D("T2",1,x,y)-273.16;
        if(Table->item(i,4)->text() != "0")
        {
            float press1 = Table->item(i,4)->text().toFloat();
            press1 = press1-press;
            QString s_press = tr("%1").arg(press1);
            Table->item(i,4)->setText(s_press);
        }
        if(Table->item(i,5)->text() != "0")
        {
            float temp1 = Table->item(i,5)->text().toFloat();
            temp1 = temp1-temp;
            QString s_temp = tr("%1").arg(temp1);
            Table->item(i,5)->setText(s_temp);
        }
    }
}
