#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QRegExpValidator"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString IPEdit = "000.000.000.000;";
    ui->IPEdit->setInputMask(IPEdit);
    QString maskEdit = "HH:HH:HH:HH:HH:HH;_";
    ui->maskEdit->setInputMask(maskEdit);
    // ui->emaillineEdit->setEchoMode(QLineEdit::Normal);
    QRegExp regx("[a-zA-Z0-9_-]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+");
    QValidator *validator = new QRegExpValidator(regx, ui->emaillineEdit );
    ui->pwdlineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}
