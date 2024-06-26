# Qt_note





## 对话框

我们在MainWindow的构造函数里创建了QDialog类型的变量w。
然后调用w的show函数展示该对话框。
运行程序后会看到对话框w一闪而逝，然后创建了主窗口。
一闪而逝的而原因是w在MainWindow的构造函数里，构造函数结束w就被释放了。所以窗口就会一闪而逝。
即便是如下设置w的父窗口为mainwindow也无济于事

```c++
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDialog w(this);
    w.show();

}
```

因为无论w的父窗口是谁，都会随着MainWindow构造函数的结束而释放。那么好用的办法就是通过new创建对话框，这样对话框的空间在堆上，就不会随着构造函数结束而被释放了。

```c++
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   auto w = new QDialog(this);
   w->show();
}
```

用new创建QDialog 对象w，并且指明了this(MainWindow)为其父窗口，这样在父窗口关闭或者释放后，其子窗口也会释放。这个原理在之后会讲给大家，QT提供了对象树的机制，保证了父类被释放后子类都会被回收。所以这也是我们指明w的父窗口为MainWindow的意思，如果不指明就需要手动回收w。不回收就会造成内存泄漏。