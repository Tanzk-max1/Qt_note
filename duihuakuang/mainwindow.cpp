#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDialog w;
    w.show();


}

MainWindow::~MainWindow()
{
    delete ui;
}
