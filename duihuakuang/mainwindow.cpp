#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDialog>
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto s = new QDialog(this);
    s->setWindowFlag(Qt::WindowStaysOnTopHint);
    s->show();
    qDebug()<<"hahah " << Qt::endl;
    // QDialog w(this);
    // w.exec();

}

MainWindow::~MainWindow()
{
    delete ui;
}
