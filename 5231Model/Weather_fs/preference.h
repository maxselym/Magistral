#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include "QSpinBox"

class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QStackedLayout;

/** Класс основных настроек программы*/
class Preferences : public QDialog
{
    Q_OBJECT

public:
    /** Конструктор */
    Preferences(QWidget *parent = 0);

private slots:
    /** Сохранить настройки */
    void savePreferences();

private:
    /** Создать вкладку база данных*/
    void createDataBasePage();
    /** Создать вкладку WRF*/
    void createWRFPage();
    /** Создать вкладку Конфигурационные файлы*/
    void createConfigFilesPage();
    /** Записать xml */
    void writeXml(const QString &fileName);
    /** Прочесть xml */
    void readXml(const QString &fileName);
    /** Прочесть xml с именем preferences.xml*/
    void loadPreferences();

    QStackedLayout *stackedLayout;
    QListWidget *listWidget;
    QWidget *dataBasePage;
    QWidget *WPSPage;
    QWidget *ConfigFilesPage;
    QDialogButtonBox *buttonBox;

    //Data Base Page
    QGroupBox *dataBaseGroupBox;
    QLabel *hostNameLabel;
    QLineEdit *hostNameLineEdit;
    QLabel *dataBaseNameLabel;
    QLineEdit *dataBaseNameLineEdit;
    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    //WPS/WRF Page
    QGroupBox *WPSWRFGroupBox;
    QLabel *WPSCoreNum;
    QSpinBox *WPSSpinCore;
    QLabel *WPSRootLabel;
    QLineEdit *WPSRootLineEdit;
    QLabel *WRFDARootLabel;
    QLineEdit *WRFDARootLineEdit;
    QLabel *WRFRootLabel;
    QLineEdit *WRFRootLineEdit;
    QLabel *WRFARWPostRootLabel;
    QLineEdit *WRFARWPostRootLineEdit;
    QLabel *WPSGEOGLabel;
    QLineEdit *WPSGEOGLineEdit;
    QLabel *WPSWORKLabel;
    QLineEdit *WPSWORKLineEdit;
    QLabel *WPSGRIBLabel;
    QLineEdit *WPSGRIBLineEdit;
    QLabel *WRFOutLabel;
    QLineEdit *WrfOutLineEdit;
    QLabel *WRFDomainsLabel;
    QLineEdit *WRFDomainsLineEdit;

    //Config Files Page
    QGroupBox *CFGGroupBox;
    QLabel *CFG_Conf_wrf_file_Label;
    QLineEdit *CFG_Conf_wrf_file_LineEdit;
    QLabel *CFG_station_Label;
    QLineEdit *CFG_station_LineEdit;
};

#endif // PREFERENCEDIALOG_H
