#include "dlg_namelist_edit.h"

dlg_namelist_edit::dlg_namelist_edit(QWidget *parent) :
    QDialog(parent)
{
    setMinimumSize(500,500);
    curFile ="namelist.wfs"; //Default file name;
    QHBoxLayout *h_box = new QHBoxLayout;
    button_save = new QPushButton("Save");
    button_save_as = new QPushButton("Save As...");
    button_open = new QPushButton("Open");
    statusBar = new QStatusBar();
    statusBar->showMessage(tr("Ready"));
    h_box->addWidget(button_open);
    h_box->addWidget(button_save);
    h_box->addWidget(button_save_as);
    connect(button_open,SIGNAL(clicked()),this,SLOT(OpenFile()));
    connect(button_save,SIGNAL(clicked()),this,SLOT(SaveFile()));
    connect(button_save_as,SIGNAL(clicked()),this,SLOT(SaveFileAs()));

    p_tree = new XMLTree;
    p_tree->read(curFile);
    QVBoxLayout *v_box = new QVBoxLayout;
    v_box->addItem(h_box);
    v_box->addWidget(p_tree);
    v_box->addWidget(statusBar);
    setLayout(v_box);
}


bool dlg_namelist_edit::save(const QString &fileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor); //Курсор часики
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Namelist WFS"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    if (p_tree->write(curFile))
    {
        SetTitle();
        statusBar->showMessage(tr("File saved"), 2000);
    }
    QApplication::restoreOverrideCursor(); //Курсор обычный
    return true;
}

void dlg_namelist_edit::SetTitle()
{
    QFileInfo pathInfo(curFile);
    QString fileName(pathInfo.fileName());
    setWindowTitle(fileName);
}

void dlg_namelist_edit::OpenFile()
{
    curFile = QFileDialog::getOpenFileName(this, tr("Open Namelist WPS"),
                                         QDir::currentPath(),
                                         tr("WFS Files (*.wfs)"));
    if (curFile.isEmpty())
        return;
    if(p_tree->read(curFile))
    {
        SetTitle();
        statusBar->showMessage(tr("File loaded"), 2000);
    }
    else statusBar->showMessage(tr("XML read ERROR!!!"), 2000);
}

void dlg_namelist_edit::SaveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return;
    curFile = fileName;
    save(fileName);
}

void dlg_namelist_edit::SaveFile()
{
    save(curFile);
}

void dlg_namelist_edit::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Save changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No |QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::No);
    int result = msgBox.exec();
    switch (result)
    {
    case QMessageBox::Save:
        save(curFile);
        event->accept();
        break;
    case QMessageBox::No:
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
