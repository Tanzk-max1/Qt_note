# Qt_note





## 对话框

### 非模态对话框

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

因为无论w的父窗口是谁，都会随着MainWindow构造函数的结束而释放。那么好用的办法就是通过new创建对话框，这样对话框的空间在堆上，就不会随着构造函数结束而被释放了。

```
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

再次运行就可以看到会弹出两个界面，一个是主窗口，一个是对话框了。
上面的方式创建的都是非模态对话框，所谓非模态对话框就是无论是否操作这个对话框都可以操作其他窗口。也就是说在非模态窗口弹出后并不阻塞其他窗口的消息传递。

### 模态对话框

模态对话框就是当该对话框弹出后会阻塞其他窗口的响应事件，必须先关闭该对话框，其他窗口才会继续响应事件。
我们改造MainWindow的构造函数，在里边创建一个模态对话框

```
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   auto w = new QDialog(this);
   w->setModal(true);
   w->show();
}
```

点击运行，弹出一个对话框和主窗口，点击主窗口没有任何反应，点击对话框关闭后才能点击主窗口，所以w就是一个模态对话框。

![image-20240626165344287](D:\QTproject\stuhub\${photo}\image-20240626165344287.png)另外一个创建模态对话框的方式是这样的

```
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDialog w(this);
    w.exec();
}
```

这种方式创建的对话框也是模态的，并且不会一闪而逝，主要原因是exec这个函数会阻塞其他界面响应事件。所以直到我们关闭这个对话框后，exec才返回，这样MainWindow的构造函数才继续往下执行。

### 窗口指定

![image-20240626173209255](D:\QTproject\stuhub\${photo}\image-20240626173209255.png)

槽函数的参数个数会小于信号的个数，也就是说slot里面的这个showxxx必须是（）起手，可以无参数，但是一定是小于signal这个函数

这是qt4的链接信号和槽函数的方式

qt4风格的slot和signal只是宏转换，字符串定义不能检测编译错误

![image-20240626174121612](D:\QTproject\stuhub\${photo}\image-20240626174121612.png)

这就错误了

区别不同的联系方式

我们上边用来连接信号和槽的方式是qt4提供的方式，用SIGNAL和SLOT将信号和槽转化为字符串。
但是这种方式会存在一定问题，Qt要求槽函数的参数不能超过信号定义的参数，比如我们用到的信号clicked(bool)参数就是bool，我们定义的槽函数showChildDialog()是不带参数的，可以连接成功，如果我们在连接的时候将showChildDialog的参数写为3个，也可以连接成功

```
//qt4 风格的Slot和Signal 只是宏转换，字符串定义不能检测编译错误
connect(ui->showChildButton, SIGNAL(clicked(bool)), this, SLOT(showChildDialog(1,2,3)));
```

但是点击会没有反应，说明qt4 这种连接信号和槽的方式不做编译检查，只是将信号和槽函数转译成字符串。
所以我推荐使用qt5以上版本的连接方式

```
//推荐qt5 风格
connect(ui->showChildButton, &QPushButton::clicked, this, &MainWindow::showChildDialog);
```

这种方式也可以实现信号和槽函数的连接。

在ChildDialog的实现文件里连接槽函数，并且实现子界面隐藏，主界面显示

```cpp
ChildDialog::ChildDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChildDialog),_parent(parent)
{
    ui->setupUi(this);
    connect(ui->showMainWindow, &QPushButton::clicked, this, &ChildDialog::showMainWindow);
}

void ChildDialog::showMainWindow()
{
    qDebug() << "show main window" << endl;
    _parent->show();
    this->hide();
}
```

修改主界面的槽函数,让主界面隐藏，子界面显示

```cpp
void MainWindow::showChildDialog()
{
    qDebug() << "show child dialog " << endl;
    _child_dialog->show();
    this->hide();
}
```

运行程序后，点击按钮就可以实现界面的切换。 这么做有一个不好的地方就是在ChildDialog类里保存了MainWindow的指针，如果我们ChildDialog类里要实现和多个其他界面的交互，就需要保存多个指针，这样代码的耦合性太大了。所以我们引入信号和槽机制，当我们点击子界面按钮时发送一个信号给主界面，这样主界面收到该信号后就显示主界面隐藏子界面。 那我们先为ChildDialog类声明一个信号，用来通知主界面显示

```cpp
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
    QWidget *_parent;
public slots:
    void showMainWindow();
};
```

showMainSig是一个信号，用来通知主界面，所以主界面MainWindow类要连接这个信号，我们先在主界面类中声明这个函数

```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public slots:
    void showChildDialog();
    void showMainDialog();
private:
    ChildDialog *_child_dialog;
};
```

showMainDialog 是新增的槽函数，用来连接ChildDialog的showMainSig信号。 我们修改ChildDialog的showMainWindow函数

```cpp
void ChildDialog::showMainWindow()
{
    qDebug() << "show main window" << endl;
    this->hide();
    //可以再次发送信号通知主窗口显示
    emit showMainSig();
}
```

然后在MainWindow连接这个信号

```cpp
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _child_dialog = new ChildDialog(this);

    //推荐qt5 风格
    connect(ui->showChildButton, &QPushButton::clicked, this, &MainWindow::showChildDialog);

    connect(_child_dialog, &ChildDialog::showMainSig, this, &MainWindow::showMainDialog);
}

void MainWindow::showChildDialog()
{
    qDebug() << "show child dialog " << endl;
    _child_dialog->show();
    this->hide();
}
```

再次运行程序，点击按钮实现了界面的切换。

### 连接信号

上面的程序还可以进一步优化，因为Qt提供了信号连接信号的方式，也就是说我们可以把子界面的按钮点击信号和showMainSig信号连接起来。

```cpp
ChildDialog::ChildDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChildDialog),_parent(parent)
{
    ui->setupUi(this);
    connect(ui->showMainWindow, &QPushButton::clicked, this, &ChildDialog::showMainSig);
}
```

将clicked和showMainSig两个信号连接起来，也可以实现消息的传递，让代码更简洁了。



## 模态对话框接受和拒绝信息

我们创建一个模态对话框，调用exec函数后可以根据其返回值进行不同的处理，exec的返回值有两种，Qt的官方文档记录的为

```cpp
QDialog::Accepted
QDialog::Rejected
```

Accepted 表示接受消息， Rejected表示拒绝消息。 还是按照之前的操作，选择新建QT Application项目，然后创建类名为MainWindow, 基类选择QDialog, 点击创建生成Qt项目。然后我们添加设计师界面类，类名MyDialog, 基类选择QDialog。然后在这个mydialog.ui中添加两个按钮，一个是进入主界面，一个是退出程序。[![https://cdn.llfc.club/1662194514279.jpg](D:\QTproject\stuhub\${photo}\1662194514279.jpg)](https://cdn.llfc.club/1662194514279.jpg)在设计师界面点击Edit Signal/Slots 信号槽的按钮，进入信号槽编辑界面, 鼠标按住退出程序按钮不松开拖动，将另一端连接到MyDialog对话框，QPushbutton这边信号选择clicked()， MyDialog信号选择reject，这样就将两个信号连接起来了，我们点击退出程序按钮，会触发MyDialog发送reject信号，因为MyDialog调用exec后等待信号返回，此时收到reject信号，exec就会返回Rjected值。 同样道理，MyDialog发送accetp()信号后, exec返回值为Accepted。[![https://cdn.llfc.club/1662194804759.jpg](D:\QTproject\stuhub\${photo}\1662194804759.jpg)](https://cdn.llfc.club/1662194804759.jpg)我们点击Edit Widget 按钮， 然后右键点击键入主界面按钮，点击进入槽，在Qt 为我们生成的槽函数里添加accetp()信号发送逻辑

```cpp
void MyDialog::on_pushButton_clicked()
{
    accept();
}
```

我们在main函数里调用对话框，然后根据返回值为Accepted，调用主窗口显示，否则直接退出

```cpp
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MyDialog dialog;
    if(dialog.exec() == QDialog::Accepted){
        w.show();
         return a.exec();
    }else{
        return 0;
    }
}
```

运行后，我们点击进入主界面按钮，界面跳转至主界面，点击退出按钮程序退出。

### 主界面和登录界面切换

现在我们要实现主界面和登录界面的互相切换，我们刚才创建的对话框界面MyDialog界面是登录界面，主界面是MainWindow类。 我们在mainwindow.ui添加两个按钮，返回登录按钮和退出程序按钮, 再添加一个label表示这是主界面. 然后点击Edit Signal/Slot 进入信号编辑界面，点击退出程序按钮不松开拖动到尾部连接MainWindow的主界面上，选择退出程序的clicked()信号，MainWindow选择reject信号,将两个信号连接起来。[![https://cdn.llfc.club/1662198287430.jpg](D:\QTproject\stuhub\${photo}\1662198287430.jpg)](https://cdn.llfc.club/1662198287430.jpg)然后右击登录按钮转到槽，在槽函数里添加这个逻辑

```cpp
void MainWindow::on_pushButton_clicked()
{
    close();
    MyDialog mydialog;
    if(mydialog.exec() == QDialog::Accepted ){
        this->show();
    }else{
        return;
    }
}
```

点击返回登陆按钮就会close主窗口，但是并不是真的关闭，只是将主窗口隐藏，Qt回收机制是所有窗口都关闭后才回收。 再次运行程序，点击进入主界面按钮就可以进入主界面，点击返回登录按钮就可以返回登录界面，可以实现两个界面的切换了。