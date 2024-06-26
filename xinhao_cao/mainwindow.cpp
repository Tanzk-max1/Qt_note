#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "childdialog.h"
#include<QDialog>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //
    _child_dialog = new ChildDialog(this);
    connect(ui->showChildButton,&QPushButton::clicked, this, &MainWindow::showChildDialog);
    connect(_child_dialog, &ChildDialog::showMainSig, this, &MainWindow::showMainDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(_child_dialog){
        delete _child_dialog;
        _child_dialog = nullptr;
    }
}

void MainWindow::showChildDialog()
{
    qDebug() << "show child dialog" << endl;
    // auto *childDialog = new QDialog(this);
    _child_dialog->show();
    this->hide();
}
void MainWindow::showMainDialog()
{
    qDebug() << "show child dialog" << endl;
    // auto *childDialog = new QDialog(this);

    this->hide();
    _child_dialog->show();
}
