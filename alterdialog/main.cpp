#include "mainwindow.h"
#include <QApplication>
#include "Mydialog.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MyDialog dialog;
    // mydialog.exec();
    auto res = dialog.exec();
    if(res == QDialog::Accepted){
        w.show();
        return a.exec();
        qDebug()<< "exit?"<<endl;
    }else {
        qDebug()<< "bug"<<endl;
        return 0;
    }
    //w.show();
    // return a.exec();
}
