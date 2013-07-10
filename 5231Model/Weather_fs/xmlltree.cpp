#include "xmlltree.h"

XMLTree::XMLTree(QWidget *parent)
    : QTreeWidget(parent)
{
    QStringList labels;
    labels << tr("Key") << tr("Value")<<tr("Description");
    header()->setResizeMode(QHeaderView::ResizeToContents);
    setHeaderLabels(labels);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                         QIcon::Normal, QIcon::Off);
    folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                         QIcon::Normal, QIcon::On);
    bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));    
}

bool XMLTree::read(QString file_name)
{
    cur_name=file_name;
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
    if (root.tagName() != "xbel") {
        QMessageBox::information(window(), tr("WFS reader"),
                                 tr("The file is not an WFS file."));
        return false;
    } else if (root.hasAttribute("version")
               && root.attribute("version") != "1.0") {
        QMessageBox::information(window(), tr("WFS reader"),
                                 tr("The file is not an WFS version 1.0 "
                                    "file."));
        return false;
    }
    clear();
    disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this, SLOT(updateDomElement(QTreeWidgetItem*,int)));

    QDomElement child = root.firstChildElement("folder");
    while (!child.isNull()) {
        parseFolderElement(child);
        child = child.nextSiblingElement("folder");
    }

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this, SLOT(updateDomElement(QTreeWidgetItem*,int)));
    connect(this,SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this,SLOT(SwitchCollapsed(QTreeWidgetItem*)));
    connect(this,SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this,SLOT(SwitchCollapsed(QTreeWidgetItem*)));

    file.close();
    return true;
}


void XMLTree::SwitchCollapsed(QTreeWidgetItem *item)
{
     QDomElement element = domElementForItem.value(item);
      if (!element.isNull())
      {
          QString s = element.attribute("folded");
          if(s=="yes")
              element.setAttribute("folded","no");
          else
              element.setAttribute("folded","yes");
      }
}
void XMLTree::updateDomElement(QTreeWidgetItem *item, int column)
{
    QDomElement element = domElementForItem.value(item);
    if (!element.isNull()) {
        switch (column) {
        case 0:
            element.setAttribute("title", item->text(0));
            break;
        case 1:
            if (element.tagName() == "Key"){
                element.setAttribute("value", item->text(1));}
            break;
        case 2:
            if (element.tagName() == "Key"){
                element.setAttribute("description", item->text(2));}
            break;
        }
    }
}

void XMLTree::parseFolderElement(const QDomElement &element,
                                  QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = createItem(element, parentItem); //Создать элемент дерева
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setIcon(0, folderIcon);
    item->setText(0, element.attribute("title"));//читаем название из атрибута

    bool folded = (element.attribute("folded") != "no"); //Атрибут того что папка развернута
    setItemExpanded(item, !folded);

    QDomElement child = element.firstChildElement(); //Берем первый элемент XML
    while (!child.isNull()) {
        if (child.tagName() == "folder") {
            parseFolderElement(child, item); //Если элемент папка, то рекурсивно читаем ее
        } else if (child.tagName() == "Key") //Если элемент значение, то разбираем
        {
            QTreeWidgetItem *childItem = createItem(child, item); //Подвешиваем элемент на дерево
            childItem->setFlags(item->flags() | Qt::ItemIsEditable);
            childItem->setIcon(0, bookmarkIcon);
            childItem->setText(0, child.attribute("title"));//читаем название из атрибута
            childItem->setText(1, child.attribute("value"));//читаем значение из атрибута
            childItem->setText(2, child.attribute("description"));
        }
        child = child.nextSiblingElement(); //Переходим к следующему элементу
    }
}

QTreeWidgetItem *XMLTree::createItem(const QDomElement &element,
                                      QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item;
    if (parentItem) {
        item = new QTreeWidgetItem(parentItem);
    } else {
        item = new QTreeWidgetItem(this);
    }
    domElementForItem.insert(item, element);
    return item;
}

bool XMLTree::write(QString file_name)
{
    cur_name=file_name;
    QFile file(cur_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Write file"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(cur_name)
                             .arg(file.errorString()));
        return false;
    }
    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
    return true;
}
