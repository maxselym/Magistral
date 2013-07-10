#include <QDomDocument>
#include <QtGui>

/** Дерево связанное с XML-файлом */
class XMLTree : public QTreeWidget
{
    Q_OBJECT

public:
    /** Конструктор */
    XMLTree(QWidget *parent = 0);
    /** Прочесть XML файл*/
    bool read(QString file_name);
    /** Записать XML файл*/
    bool write(QString file_name);

private slots:
    /** Обновить xml элемент*/
    void updateDomElement(QTreeWidgetItem *item, int column);
    /** Обновить аттрибут свернуто в xml элементе*/
    void SwitchCollapsed(QTreeWidgetItem *item);
private:
    /** Текущий xml файл*/
    QString cur_name;
    /** Рекурсивное сканирование элементов с тегом Folder*/
    void parseFolderElement(const QDomElement &element,
                            QTreeWidgetItem *parentItem = 0);
    /** Добавить связанный с xml элемент к дереву*/
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);

    /** Домовская модель*/
    QDomDocument domDocument;
    /** Массив связывающий дерево и xml*/
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    /** Иконки*/
    QIcon folderIcon;
    QIcon bookmarkIcon;
};
