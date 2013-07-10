#ifndef RAW_PGP_DLG_H
#define RAW_PGP_DLG_H

#include <QDialog>
#include "QStatusBar"
#include "SharedFiles/dbengine.h"
#include "SharedFiles/little_r_fm12.h"

namespace Ui {
class raw_pgp_dlg;
}

class raw_pgp_dlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit raw_pgp_dlg(QWidget *parent = 0);
    /** Заполнить список станций*/
    bool FetchStationList();
    ~raw_pgp_dlg();
    
private slots:
    void on_tableWidget_itemSelectionChanged();

    void on_comboBoxDate_currentIndexChanged(int index);

    void on_pushButton_selectedToLittle_R_clicked();

private:
    QVector<QDate>* vecDay;
    QVector<QTime>* vecTime;
    int n_st_num;
    DBEngine* dbengine;
    void init();
    Ui::raw_pgp_dlg *ui;
    QStatusBar* statusBar; /**< указатель на статусбар*/
};

#endif // RAW_PGP_DLG_H
