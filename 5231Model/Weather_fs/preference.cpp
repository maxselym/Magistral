#include <QtGui>
#include "preference.h"
#include "SharedFiles/xml_namelist.h"

Preferences::Preferences(QWidget *parent)
    : QDialog(parent)
{
    createDataBasePage();
    createWRFPage();
    createConfigFilesPage();
    loadPreferences();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);
    listWidget = new QListWidget;
    listWidget->addItem(tr("Data Base"));
    listWidget->addItem(tr("WRF"));
    listWidget->addItem(tr("Config files"));

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(dataBasePage);
    stackedLayout->addWidget(WPSPage);
    stackedLayout->addWidget(ConfigFilesPage);
    connect(listWidget, SIGNAL(currentRowChanged(int)),
            stackedLayout, SLOT(setCurrentIndex(int)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 3);
    mainLayout->addWidget(listWidget, 0, 0);
    mainLayout->addLayout(stackedLayout, 0, 1);
    mainLayout->addWidget(buttonBox, 1, 0, 1, 2);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(savePreferences()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle(tr("Preferences"));
    listWidget->setCurrentRow(0);
}

void Preferences::createDataBasePage()
{
    dataBasePage = new QWidget;
    dataBaseGroupBox = new QGroupBox(tr("Data Base connection"));
    hostNameLabel = new QLabel(tr("Host:"));
    hostNameLineEdit = new QLineEdit;
    dataBaseNameLabel = new QLabel(tr("Data Base name:"));
    dataBaseNameLineEdit = new QLineEdit;
    userNameLabel = new QLabel(tr("User name:"));
    userNameLineEdit = new QLineEdit;
    passwordLabel = new QLabel(tr("Password:"));
    passwordLineEdit = new QLineEdit;
    portLabel = new QLabel(tr("Port:"));
    portLineEdit = new QLineEdit;
    QGridLayout *dataBaseLayout = new QGridLayout;
    dataBaseLayout->addWidget(hostNameLabel, 0, 0);
    dataBaseLayout->addWidget(hostNameLineEdit, 0, 1);
    dataBaseLayout->addWidget(dataBaseNameLabel, 1, 0);
    dataBaseLayout->addWidget(dataBaseNameLineEdit, 1, 1);
    dataBaseLayout->addWidget(userNameLabel, 2, 0);
    dataBaseLayout->addWidget(userNameLineEdit, 2, 1);
    dataBaseLayout->addWidget(passwordLabel, 3, 0);
    dataBaseLayout->addWidget(passwordLineEdit, 3, 1);
    dataBaseLayout->addWidget(portLabel, 4, 0);
    dataBaseLayout->addWidget(portLineEdit, 4, 1);
    dataBaseGroupBox->setLayout(dataBaseLayout);
    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(dataBaseGroupBox);
    pageLayout->addStretch();
    dataBasePage->setLayout(pageLayout);
}
void Preferences::createWRFPage()
{
    WPSPage = new QWidget;
    WPSWRFGroupBox = new QGroupBox(tr("WPS/WPS settings"));
    WPSCoreNum = new QLabel(tr("Core NUM"));
    WPSSpinCore = new QSpinBox;
    WPSRootLabel = new QLabel(tr("WPS root folder:"));
    WPSRootLineEdit = new QLineEdit;
    WRFDARootLabel = new QLabel(tr("WRFDA root folder:"));
    WRFDARootLineEdit = new QLineEdit;
    WRFRootLabel = new QLabel(tr("WRF root folder:"));
    WRFRootLineEdit = new QLineEdit;
    WRFARWPostRootLabel = new QLabel("ARWPost folder");
    WRFARWPostRootLineEdit= new QLineEdit;
    WPSGEOGLabel = new QLabel(tr("Geography folder:"));
    WPSGEOGLineEdit = new QLineEdit;
    WPSWORKLabel = new QLabel(tr("WPS/WRF work folder:"));
    WPSWORKLineEdit = new QLineEdit;
    WPSGRIBLabel = new QLabel(tr("WPS GRIB input folder:"));
    WPSGRIBLineEdit = new QLineEdit;
    WRFOutLabel = new QLabel(tr("WRF output folder:"));
    WrfOutLineEdit = new QLineEdit;
    WRFDomainsLabel = new QLabel(tr("Domains to export"));
    WRFDomainsLineEdit = new QLineEdit;

    QGridLayout *dataBaseLayout = new QGridLayout;
    dataBaseLayout->addWidget(WPSCoreNum, 0, 0);
    dataBaseLayout->addWidget(WPSSpinCore, 0, 1);
    dataBaseLayout->addWidget(WPSRootLabel, 1, 0);
    dataBaseLayout->addWidget(WPSRootLineEdit, 1, 1);
    dataBaseLayout->addWidget(WRFDARootLabel, 2, 0);
    dataBaseLayout->addWidget(WRFDARootLineEdit, 2, 1);
    dataBaseLayout->addWidget(WRFRootLabel, 3, 0);
    dataBaseLayout->addWidget(WRFRootLineEdit, 3, 1);
    dataBaseLayout->addWidget(WRFARWPostRootLabel, 4, 0);
    dataBaseLayout->addWidget(WRFARWPostRootLineEdit, 4, 1);
    dataBaseLayout->addWidget(WPSGEOGLabel, 5, 0);
    dataBaseLayout->addWidget(WPSGEOGLineEdit, 5, 1);
    dataBaseLayout->addWidget(WPSWORKLabel, 6, 0);
    dataBaseLayout->addWidget(WPSWORKLineEdit, 6, 1);
    dataBaseLayout->addWidget(WPSGRIBLabel, 7, 0);
    dataBaseLayout->addWidget(WPSGRIBLineEdit, 7, 1);
    dataBaseLayout->addWidget(WRFOutLabel, 8, 0);
    dataBaseLayout->addWidget(WrfOutLineEdit, 8, 1);
    dataBaseLayout->addWidget(WRFDomainsLabel, 9, 0);
    dataBaseLayout->addWidget(WRFDomainsLineEdit, 9, 1);
    WPSWRFGroupBox->setLayout(dataBaseLayout);
    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(WPSWRFGroupBox);
    pageLayout->addStretch();
    WPSPage->setLayout(pageLayout);
}
void Preferences::createConfigFilesPage()
{
    ConfigFilesPage = new QWidget;
    CFGGroupBox = new QGroupBox(tr("Configuration files"));
    CFG_Conf_wrf_file_Label = new QLabel("Namelist XML file:");
    CFG_Conf_wrf_file_LineEdit = new QLineEdit;
    CFG_station_Label = new QLabel("Meteo station file:");
    CFG_station_LineEdit = new QLineEdit;
    QGridLayout *Layout = new QGridLayout;
    Layout->addWidget(CFG_Conf_wrf_file_Label, 0, 0);
    Layout->addWidget(CFG_Conf_wrf_file_LineEdit, 0, 1);
    Layout->addWidget(CFG_station_Label, 1, 0);
    Layout->addWidget(CFG_station_LineEdit, 1, 1);
    CFGGroupBox->setLayout(Layout);
    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(CFGGroupBox);
    pageLayout->addStretch();
    ConfigFilesPage->setLayout(pageLayout);

}
void Preferences::writeXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Root");
    xmlWriter.writeTextElement("host", hostNameLineEdit->text());
    xmlWriter.writeTextElement("databasename", dataBaseNameLineEdit->text());
    xmlWriter.writeTextElement("username", userNameLineEdit->text());
    xmlWriter.writeTextElement("password", passwordLineEdit->text());
    xmlWriter.writeTextElement("port", portLineEdit->text());
    xmlWriter.writeTextElement("Core_num",WPSSpinCore->text());
    xmlWriter.writeTextElement("Wps_root_folder", WPSRootLineEdit->text());
    xmlWriter.writeTextElement("WRFDA_root_folder", WRFDARootLineEdit->text());
    xmlWriter.writeTextElement("WRF_root_folder", WRFRootLineEdit->text());    
    xmlWriter.writeTextElement("ARWPost_root_folder", WRFARWPostRootLineEdit->text());
    xmlWriter.writeTextElement("Wps_GEOG_folder", WPSGEOGLineEdit->text());
    xmlWriter.writeTextElement("Wps_WORK_folder", WPSWORKLineEdit->text());
    xmlWriter.writeTextElement("Wps_GRIB_folder", WPSGRIBLineEdit->text());
    xmlWriter.writeTextElement("WRF_output_folder", WrfOutLineEdit->text());
    xmlWriter.writeTextElement("WRF_Domain_to_export", WRFDomainsLineEdit->text());
    xmlWriter.writeTextElement("namelist_xml", CFG_Conf_wrf_file_LineEdit->text());
    xmlWriter.writeTextElement("meteo_station_xml", CFG_station_LineEdit->text());
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

void Preferences::readXml(const QString &fileName)
{
    QFile *file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    QXmlStreamReader xmlReader(file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    hostNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    dataBaseNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    userNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    passwordLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    portLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WPSSpinCore->setValue(xmlReader.readElementText().toInt());
    xmlReader.readNext();
    xmlReader.readNext();
    WPSRootLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WRFDARootLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WRFRootLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WRFARWPostRootLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WPSGEOGLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WPSWORKLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WPSGRIBLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WrfOutLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    WRFDomainsLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    CFG_Conf_wrf_file_LineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    CFG_station_LineEdit->setText(xmlReader.readElementText());
    file->close();
}

void Preferences::savePreferences()
{
    writeXml("preferences.xml");
    xml_namelist* p_namelist_tool = new xml_namelist;
    p_namelist_tool->ReadXml(CFG_Conf_wrf_file_LineEdit->text());//Читаем wfs
    p_namelist_tool->SetWorkFolder(WPSWORKLineEdit->text()); //Поправляем пути в конфигах
    p_namelist_tool->SetGeogPath(WPSGEOGLineEdit->text());   //Поправляем пути в конфигах
    p_namelist_tool->SetOutputFolder(WrfOutLineEdit->text());//Поправляем пути в конфигах
    p_namelist_tool->writeXML(CFG_Conf_wrf_file_LineEdit->text()); //Пишем wfs
    accept();
}

void Preferences::loadPreferences()
{
    readXml("preferences.xml");
}





