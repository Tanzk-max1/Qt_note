#include "childdialog.h"
#include "ui_childdialog.h"
#include <QDebug>
// ChildDialog::ChildDialog(QWidget *parent)
//     : QDialog(parent),_parent(parent)
//     , ui(new Ui::ChildDialog)
// {
//     ui->setupUi(this);
//     connnect(ui->showMainWindow,&QPushButton::clicked,this,&ChildDialog::showMainWindow);
// }
ChildDialog::ChildDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChildDialog),_parent(parent)
{
    ui->setupUi(this);
    connect(ui->showMainWindow, &QPushButton::clicked, this, &ChildDialog::showMainWindow);
}

ChildDialog::~ChildDialog()
{
    delete ui;
}

void ChildDialog::showMainWindow()
{
    qDebug() << "show main window" << endl;
    _parent->show();
    this->hide();
}
