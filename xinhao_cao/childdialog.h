#ifndef CHILDDIALOG_H
#define CHILDDIALOG_H

#include <QDialog>

namespace Ui {
class ChildDialog;
}

class ChildDialog : public QDialog
{
    Q_OBJECT
signals:
    void showMainSig();
public:
    explicit ChildDialog(QWidget *parent = nullptr);
    ~ChildDialog();


private:
    Ui::ChildDialog *ui;
    QWidget * _parent;


public slots:
    void showMainWindow();
private:
    ChildDialog *_child_dialog;

};

#endif // CHILDDIALOG_H
