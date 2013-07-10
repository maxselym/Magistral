#ifndef DLG_NAMELIST_EDIT_H
#define DLG_NAMELIST_EDIT_H

#include "QtGui"
#include <QDialog>
#include "xmlltree.h"


/** Класс формма для редактирования праметро Namelist */
class dlg_namelist_edit : public QDialog
{
    Q_OBJECT

public:
    /** Конструктор */
    explicit dlg_namelist_edit(QWidget *parent = 0);

private slots:
    /** Открыть файл WFS*/
    void OpenFile();
        /** Сохранить файл WFS*/
    void SaveFile();
    /** Сохранить файл WFS под другим именем*/
    void SaveFileAs();
protected:
    /** События при закрытии окна*/
    void closeEvent(QCloseEvent * event);

private:
    /** Функция записи файла*/
    bool save(const QString &fileName);
    /** Поменять заголовок в соответствии с открытым файлом*/
    void SetTitle();
    QPushButton* button_save; /**< указатель на кнопку сохранить*/
    QPushButton* button_save_as; /**< указатель на кнопку сохранить как*/
    QPushButton* button_open; /**< указатель на кнопку открыть*/
    QStatusBar* statusBar; /**< указатель на статусбар*/
    XMLTree* p_tree; /**< указатель на xml дерево*/
    QString curFile; /**< текущий открытый файл*/
};

#endif // DLG_NAMELIST_EDIT_H
