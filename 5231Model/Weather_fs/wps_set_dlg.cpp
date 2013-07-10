#include "wps_set_dlg.h"
#include "ui_wps_set_dlg.h"
#include "QFileDialog"
#include "QDebug"
#include "QMessageBox"


WPS_set_dlg::WPS_set_dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WPS_set_dlg)
{
    ui->setupUi(this);
    p_namelist = 0;
    init();
}

WPS_set_dlg::~WPS_set_dlg()
{
    delete ui;
}

void WPS_set_dlg::init()
{
    ReadPref("preferences.xml");
    p_namelist = new xml_namelist;
    if(!p_namelist->ReadXml(ui->lineEdit_wfs->text()))
    {
        qDebug()<<"Fail to load "+ui->lineEdit_wfs->text();
    }
    else
    {
        SetTimeDateForms();
        /** Прочли информацию о центре домена*/
        float f_lat;
        float f_lon;
        p_namelist->GetCentreDomain(f_lat,f_lon);
        ui->doubleSpinBox_reflat->setValue(f_lat);
        ui->doubleSpinBox_reflon->setValue(f_lon);
        /** Прочли информацию о Шаге и их количестве*/
        QString dx;
        QString dy;
        QString num_x;
        QString num_y;
        p_namelist->GetNumAndStep(num_x,dx,num_y,dy);

        ui->lineEditX->setText(num_x);
        ui->lineEditY->setText(num_y);

        ui->lineEditDx->setText(dx);
        ui->lineEditDy->setText(dy);
        /** Прочли информацию о количестве доменов*/
        ui->doubleSpinBox_domCount->setValue(p_namelist->GetDomainCount());

        QString s_parent_start_i;
        QString s_parent_start_j;
        p_namelist->GetParentStart(s_parent_start_i,s_parent_start_j);
        ui->lineEdit_i_parent_start->setText(s_parent_start_i);
        ui->lineEdit_j_parent_start->setText(s_parent_start_j);

        QString s_id;
        QString s_ratio;
        p_namelist->GetParentIDAndRatio(s_id,s_ratio);
        ui->lineEdit_parent_id->setText(s_id);
        ui->lineEdit_parent_grid_ratio->setText(s_ratio);

    }

}

void WPS_set_dlg::on_toolButton_browse_GRIB_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_GRIB->setText(dir+"/");
}

void WPS_set_dlg::on_toolButton_browse_out_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "/home",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_out->setText(dir+"/");

}

void WPS_set_dlg::accept()
{
    qDebug()<<"Accept";
    WritePref("preferences.xml");
    WriteNamelistWFS();
    QDialog::accept();
}


bool WPS_set_dlg::ReadPref(QString cur_name)
{
    QFile file(cur_name);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Read file"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(cur_name)
                             .arg(file.errorString()));
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("DOM Bookmarks"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return false;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "Root") {
        QMessageBox::information(window(), tr("Preferences reader"),
                                 tr("The file is not an WFS file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("WFS reader"),
                                 tr("The file is not an WFS version 1.0 "
                                    "file."));
        return false;
    }
    QDomElement child = root.firstChildElement(); //Берем первый элемент XML
    while (!child.isNull()) {
        if (child.tagName() == "Wps_WORK_folder")
        {ui->lineEdit_out->setText(child.text());}
        else if (child.tagName() == "Wps_GRIB_folder") //Папка с файлами GRIB
        {ui->lineEdit_GRIB->setText(child.text());}
        else if (child.tagName() == "namelist_xml") //XML файл для генерации namelist
        {ui->lineEdit_wfs->setText(child.text());}
        child = child.nextSiblingElement(); //Переходим к следующему элементу
    }
    file.close();
    return true;
}

bool WPS_set_dlg::WritePref(QString cur_name)
{
    QFile file(cur_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Write file"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(cur_name)
                             .arg(file.errorString()));
        return false;
    }
    const int IndentSize = 4;
    QDomElement child = domDocument.documentElement().firstChildElement(); //Берем первый элемент XML
    while (!child.isNull()) {
        if (child.tagName() == "Wps_WORK_folder") //Рабочая папка
        {child.firstChild().setNodeValue(ui->lineEdit_out->text());} //Текст между тегами это ребенок элемента
        else if (child.tagName() == "Wps_GRIB_folder") //Папка с файлами GRIB
        {child.firstChild().setNodeValue(ui->lineEdit_GRIB->text());}
        else if (child.tagName() == "namelist_xml") //XML файл для генерации namelist
        {child.firstChild().setNodeValue(ui->lineEdit_wfs->text());}
        child = child.nextSiblingElement(); //Переходим к следующему элементу
    }
    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
    return true;
}

bool WPS_set_dlg::WriteNamelistWFS()
{
    /** Записываем количество доменов*/
    p_namelist->SetDomainCount(ui->doubleSpinBox_domCount->value());
    /** Записываем время по количеству доменов*/
    QDateTime start;
    QDateTime end;
    start.setDate(ui->dateTimeEdit_start->date());
    start.setTime(ui->dateTimeEdit_start->time());
    end.setDate(ui->dateTimeEdit_end->date());
    end.setTime(ui->dateTimeEdit_end->time());
    p_namelist->SetDates(&start,&end);
    /** Записываем центр домена*/
    p_namelist->SetCentreDomain(ui->doubleSpinBox_reflat->value(),ui->doubleSpinBox_reflon->value());
    /** Записываем количество шагов и шаг XY*/
    QString st_dx = ui->lineEditDx->text();
    QString st_dy = ui->lineEditDy->text();
    QString st_x = ui->lineEditX->text();
    QString st_y = ui->lineEditY->text();
    p_namelist->SetNumAndStep(st_x,st_dx,st_y,st_dy);
    p_namelist->SetParentStart(ui->lineEdit_i_parent_start->text(),ui->lineEdit_j_parent_start->text());
    p_namelist->SetParentIDAndRatio(ui->lineEdit_parent_id->text(),ui->lineEdit_parent_grid_ratio->text());
    return true;
}

void WPS_set_dlg::on_toolButton_browse_wfs_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open WFS"), 0, tr("WFS Files (*.wfs)"));
    ui->lineEdit_wfs->setText(fileName);
}

void WPS_set_dlg::on_pushButton_timeFromGRIB_clicked()
{
    if(ui->lineEdit_GRIB->text()=="")
        return;
    if(p_namelist->SetTimeDateFromGRIB(ui->lineEdit_GRIB->text()))
    {
        //statusBar->showMessage("GRIB date and time loaded!");
        SetTimeDateForms();
    }
    else
    {
        //statusBar->showMessage("Error reading GRIB!!!");
    }
}

void WPS_set_dlg::SetTimeDateForms()
{
    if(p_namelist!=0)
    {
        ui->dateTimeEdit_start->setDateTime(p_namelist->GetDateTimeStart());
        ui->dateTimeEdit_end->setDateTime(p_namelist->GetDateTimeEnd());
    }
}
