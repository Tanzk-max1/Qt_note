#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    close();
    MyDialog mydialog;
    if(mydialog.exec() == QDialog::Accepted ){
        this->show();
        //作用是显示当前对象（在这里是 MainWindow）的窗口
    }else{
        return;
    }
}
