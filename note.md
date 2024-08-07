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

## 封装好的对话框

### 颜色对话框

颜色对话框用来选择颜色，创建后会显示各种颜色和透明度信息

```cpp
void MainWindow::on_pushButton_clicked()
{
//    QColorDialog colorDlg(Qt::blue, this);
//    colorDlg.setOption(QColorDialog::ShowAlphaChannel);
//    colorDlg.exec();
//    QColor color = colorDlg.currentColor();
//    qDebug() << "color is " << color;

   QColor color = QColorDialog::getColor(Qt::blue, this,tr("选择颜色"), QColorDialog::ShowAlphaChannel );
    qDebug() << "color is " << color;
}
```

注释中也是创建颜色对话框的一种方式，两种方式都可以。

### 文本对话框

创建一个选择文件的对话框，选择后，可以根据返回值打印选择的文件的路径信息

```cpp
void MainWindow::on_pushButton_2_clicked()
{
    QString path = QDir::currentPath();
    QString title = tr("文件对话框");
    QString filter = tr("文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)");
    QString aFileName=QFileDialog::getOpenFileName(this,title,path,filter);

    qDebug() << aFileName << endl;
}
```

### 输入对话框

输入对话框分几种，包括文本输入对话框，整数输入对话框，浮点数输入对话框，条目输入对话框。 先看看文本输入对话框

```cpp
void MainWindow::on_pushButton_3_clicked()
{
    bool ok = false;
    auto text = QInputDialog::getText(this, tr("文字输入对话框"), tr("请输入用户的姓名"), QLineEdit::Normal, tr("admin"), &ok);
    if(ok){
        qDebug() << text << endl;
    }
}
```

整形输入对话框

```cpp
void MainWindow::on_pushButton_4_clicked()
{
     bool ok = false;
    auto intdata = QInputDialog::getInt(this,tr("数字输入对话框"),tr("请输入数字"),200,-200,400,10,&ok);
    if(ok){
        qDebug() << intdata << endl;
    }
}
```

浮点输入对话框

```cpp
void MainWindow::on_pushButton_5_clicked()
{
    bool ok = false;
    auto floatdata = QInputDialog::getDouble(this,tr("浮点数输入对话框"),tr("输入浮点数"),0.1,-1,1,2,&ok);
    if(ok){
        qDebug() << floatdata << endl;
    }
}
```

条目输入对话框

```cpp
void MainWindow::on_pushButton_6_clicked()
{
    QStringList items;
    items << tr("条目1") << tr("条目2");
    bool ok = false;
    auto itemData = QInputDialog::getItem(this,tr("条目输入对话框"),tr("输入或选择条目"),items,0,true, &ok);
    if(ok){
        qDebug() << "item is " << itemData << endl;
    }
}
```

提示对话框

```cpp
void MainWindow::on_pushButton_7_clicked()
{
    auto ret = QMessageBox::question(this,tr("提问对话框"),tr("你是单身吗"),QMessageBox::Yes, QMessageBox::No);
    if(ret == QMessageBox::Yes || ret == QMessageBox::No){
        qDebug() << "ret is " << ret << endl;
    }

    auto ret2 = QMessageBox::information(this,tr("通知对话框"),tr("你好单身狗"),QMessageBox::Ok);
    if(ret2 == QMessageBox::Ok){
        qDebug() << "ret2 is " << ret2 << endl;
    }

    auto ret3 = QMessageBox::warning(this,tr("警告对话框"),tr("你最好找个地方发泄一下"),QMessageBox::Ok);
    if(ret3 == QMessageBox::Ok){
        qDebug() << "ret3 is " << ret3 << endl;
    }

    auto ret4 = QMessageBox::critical(this,tr("关键提示对话框"), tr("我梦寐以求是真爱和自由"),QMessageBox::Ok);
    if(ret4 == QMessageBox::Ok){
        qDebug() << "ret4 is " << ret4 << endl;
    }
}
```

### 进度对话框

当我们复制或进行文件传输时，会显示一个进度条的对话框，我们可以使用QProcessDialog来操作。

```cpp
void MainWindow::on_pushButton_8_clicked()
{
    // 进度提示框一闪而逝，不是逻辑问题，是5000量级太小了
    QProgressDialog progressDialog(tr("正在复制"),tr("取消复制"),0,5000,this);
    progressDialog.setWindowTitle(tr("文件复制进度对话框"));
    progressDialog.setWindowModality(Qt::ApplicationModal);
    progressDialog.show();

    for(int i = 0; i < 5000; i++){
        progressDialog.setValue(i);
        QApplication::processEvents();
        if(progressDialog.wasCanceled()){
            break;
        }
    }
    progressDialog.setValue(5000);
}
```

因为循环很快完成，所以进度条一闪而逝，那么我们可以通过定时器的方式定时更新进度，这样就可以更清楚的看到进度条对话框更新情况了

```cpp
void MainWindow::on_pushButton_8_clicked()
{
    //改用定时器
    _progressDialog = new QProgressDialog(tr("正在复制"),tr("取消复制"),0,5000,this);
    _progressDialog->setWindowTitle(tr("文件复制进度对话框"));
    _progressDialog->setWindowModality(Qt::ApplicationModal);
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &MainWindow::on_updateProgressDialog);
    connect(_progressDialog, &QProgressDialog::canceled, this, &MainWindow::on_cancelProgressDialog);
    _timer->start(2);
    qDebug("复制结束");
}
```

上面代码连接了定时器的timeout信号和主窗口的on_updateProgressDialog函数, 该函数的主要作用就是定时更新进度条，逻辑如下

```cpp
void MainWindow::on_updateProgressDialog()
{
    _count++;
    if(_count > 5000){
        _timer->stop();
        delete  _timer;
        _timer = nullptr;
        delete _progressDialog;
        _progressDialog = nullptr;
        _count = 0;
        return;
    }

    _progressDialog->setValue(_count);
}
```

另外我们需要在进度条满了的时候发消息通知主窗口关闭

```cpp
void MainWindow::on_cancelProgressDialog(){
    _timer->stop();
    delete  _timer;
    _timer = nullptr;
    delete _progressDialog;
    _progressDialog = nullptr;
    _count = 0;
    return;
}yi
```

这样进度条满了之后我们就可以回收定时器资源了。

### 向导对话框

当我们想引导用户操作时可以采用向导对话框，比如安装软件时就会根据向导对话框一步一步操作，我们也实现一个向导对话框

```cpp
void MainWindow::on_pushButton_10_clicked()
{
    QWizard wizard(this);
    wizard.setWindowTitle(tr("全城热恋"));
    QWizardPage* page1 = new QWizardPage();
    page1->setTitle(tr("婚恋介绍引导程序"));
    auto label1 = new QLabel();
    label1->setText(tr("该程序帮助您找到人生伴侣"));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label1);
    page1->setLayout(layout);
    wizard.addPage(page1);
    QWizardPage* page2 = new QWizardPage();
    page2->setTitle("选择心动类型");

    QButtonGroup *group = new QButtonGroup(page2);
    QRadioButton * btn1 = new QRadioButton();
    btn1->setText("白富美");
    group->addButton(btn1);
    QRadioButton * btn2 = new QRadioButton();
    btn2->setText("萝莉");
    group->addButton(btn2);
    QRadioButton * btn3 = new QRadioButton();
    btn3->setText("御姐");
    group->addButton(btn3);
    QRadioButton * btn4 = new QRadioButton();
    btn4->setText("小家碧玉");
    group->addButton(btn4);
    QRadioButton * btn5 = new QRadioButton();
    btn5->setText("女汉子");
    group->addButton(btn5);

    QRadioButton * btn6 = new QRadioButton();
    btn6->setText("成年人不做选择，全选!");
    group->addButton(btn6);
    QVBoxLayout *vboxLayout2 = new QVBoxLayout();
    for(int i = 0; i < group->buttons().size(); i++){
        vboxLayout2->addWidget(group->buttons()[i]);
    }

    page2->setLayout(vboxLayout2);
    wizard.addPage(page2);

    QWizardPage* page3 = new QWizardPage();
    page3->setTitle(tr("你的缘分即将到来"));
    auto label3 = new QLabel();
    label3->setText(tr("感谢您的参与，接下来的一个月会遇到对的人"));
    QVBoxLayout *layout3 = new QVBoxLayout();
    layout3->addWidget(label3);
    page3->setLayout(layout3);
    wizard.addPage(page3);
    wizard.show();
    wizard.exec();
}
```

## QT QlineEdit

QLineEdit属于输入插件，用来实现单行录入。支持几种录入模式。

Normal表示正常录入,录入的信息会显示在QLineEdit上。

Password表示密码录入的方式，录入的信息不显示QLineEdit，只是通过黑色圆点显示。

NoEcho 表示不显示录入信息，类似于Linux输入密码时，显示的是一片空白。

PasswordEchoOnEdit 表示在输入的一刹那可以看到字符，但是立刻变为不可见的黑色圆点显示。





### 实战

我们创建一个QApplication项目，名字叫editline。项目主界面取名MainWindow类，继承于QMainWindow。Qt为我们自动生成ui文件。
然后在ui界面里添加四个QLineEdit和Label



![https://cdn.llfc.club/2FqBuQlOvQZm3EIYuEkHHFQvSKV.png](D:\QTproject\stuhub\${photo}\2FqBuQlOvQZm3EIYuEkHHFQvSKV.png)



我将ip的QLineEdit名字改为ipedit,设置ipedit的输入方式为Normal，这样可以在输入时看到输入的字符。
我们可以给ipedit设置mask规定输入的字符只能为0~9的数字

```
ui->ipedit->setEchoMode(QLineEdit::Normal);
QString ip_mask = "000.000.000.000;_";
ui->ipedit->setInputMask(ip_mask);
```

具体的mask规则可以参考Qt官方文档，这里列举一下常见的mask规则![https://cdn.llfc.club/2FqBzfexfBgdO4NzdZa2pLtd4nO.png](D:\QTproject\stuhub\${photo}\2FqBzfexfBgdO4NzdZa2pLtd4nO.png)



利用上面的规则我们可以编写一些常用的mask

![https://cdn.llfc.club/2FqC2I8jSBa5gkKZJyaNS0SFDTy.png](D:\QTproject\stuhub\${photo}\2FqC2I8jSBa5gkKZJyaNS0SFDTy.png)

接下来我们设置mac地址的mask和录入模式,mac地址的edit被命名为macedit

```
ui->macedit->setEchoMode(QLineEdit::Normal);
QString mac_mask = "HH:HH:HH:HH;_";w
ui->macedit->setInputMask(mac_mask);
```

通过设置mask，规定mac的输入内容只能为A~F以及0~9的字符。

其实除了通过mask限制edit录入的内容外，还可以通过正则表达式限制,比如我们设置邮箱edit的录入内容

```
ui->emailedit->setEchoMode(QLineEdit::Normal);
//设置正则表达式过滤  secondtonone23@163.com
QRegExp regx("[a-zA-Z0-9_-]+@[a-zA-Z0-9]+\.[a-zA-Z0-9]+");
QValidator *validator = new QRegExpValidator(regx, ui->emailedit );
//为什么使用对象树呢，因为qt的规则，放在父窗口，当结束函数之后这个对象就会被回收
ui->emailedit->setValidator(validator);
```

我们通过正则表达式限定了有限的规则为 “字符串和数子组合 + @ + 字符串和数子组合 + . + 字符串和数子组合”。
具体的正则表达式可以去Qt文档查看，也可以看看下边这个图

![https://cdn.llfc.club/2FqC9e5dqJXCsFom1y8vHkCI1Jd.png](D:\QTproject\stuhub\${photo}\2FqC9e5dqJXCsFom1y8vHkCI1Jd.png)



接下来我们可以设置密码输入框的输入模式为Password，保证录入时不显示密码

```
ui->passwdedit->setEchoMode(QLineEdit::Password);
```

## **QT 布局**

Qt 中的布局有三种方式，水平布局，垂直布局，栅格布局。

## 通过ui设置布局

我们先创建一个窗口应用程序，程序名叫layout，基类选择QMainWindow。但我们不使用这个mainwindow，我们创建一个Qt应用程序类Login，Qt会为我们自动生成login.ui文件。我们进入ui文件编辑，添加一个label，提示改为用户: ， 在后边添加一个lineedit控件，按住ctrl鼠标依次点击这两个控件选中后，再点击工具栏的水平布局按钮就可以看到用户label和输入框处于同一水平线了。但是输入框会被拉长，而且label和输入框占满了整个水平空间。这时我们可以通过拖动左侧控件列表中的Horizonal Spacer，将其放入用户标签的左侧，再拖动一个Horizonal Spacer将其放在输入框的右侧，就可以看到用户标签和输入框被挤在中间了，并且两侧留有空间了。Spacer可以设置几种模式，包括fixed，expanding, maximum, minimum等模式。
依次类推，我们在添加密码标签和输入框，以及登录和注册按钮，通过ui界面的控件调整布局。

![https://cdn.llfc.club/2FqCTk5nDtdOD4uUtFLCa47RMQN.png](D:\QTproject\stuhub\${photo}\2FqCTk5nDtdOD4uUtFLCa47RMQN.png)



## 通过代码设置布局

上面我们通过ui设置了布局，接下来我们通过代码设置布局，设置注册界面的布局
注册类的声明如下

```c++
#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include <memory>
using namespace std;
class  Login;
namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void set_login(const  weak_ptr<Login> &_login);
private:
    Ui::Register *ui;
    weak_ptr<Login> _login;
    QPushButton* _reg_btn;
public slots:
    void ShowLogin();
};

#endif // REGISTER_H
```

因为要实现登录和注册界面之间的切换，所以Register类包含了Login类的弱指针，Register类的具体实现如下

```c++
#include "register.h"
#include "ui_register.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setMaximumSize(QSize(300,350));
    this->setMinimumSize(QSize(300,350));
    auto vbox_layout = new QVBoxLayout();

    auto verticalSpacer1 = new QSpacerItem(40,20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vbox_layout->addItem(verticalSpacer1);
    QSpacerItem *name_item1 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    QLabel * name_label = new QLabel();
    name_label->setText("邮箱:");
    QLineEdit * name_edit = new QLineEdit();
    auto name_layout = new QHBoxLayout();
    name_layout->addItem(name_item1);
    name_layout->addWidget(name_label);
    name_layout->addWidget(name_edit);
    QSpacerItem *name_item2 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    name_layout->addItem(name_item2);
    vbox_layout->addLayout(name_layout);

    QLabel * pwd_label = new QLabel();
    pwd_label->setText("密码:");
    QLineEdit * pwd_edit = new QLineEdit();

    auto verticalSpacer2 = new QSpacerItem(40,20, QSizePolicy::Maximum, QSizePolicy::Maximum);
    vbox_layout->addItem(verticalSpacer2);
    auto pwd_layout = new QHBoxLayout();

    QSpacerItem *pwd_item2 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    QSpacerItem *pwd_item1 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    pwd_layout->addItem(pwd_item1);
    pwd_layout->addWidget(pwd_label);
    pwd_layout->addWidget(pwd_edit);
    pwd_layout->addItem(pwd_item2);
    vbox_layout->addLayout(pwd_layout);

    auto verticalSpacer3 = new QSpacerItem(40,30, QSizePolicy::Fixed, QSizePolicy::Maximum);
    vbox_layout->addItem(verticalSpacer3);

    QSpacerItem* reg_btn_item1 = new QSpacerItem(150,20, QSizePolicy::Fixed, QSizePolicy::Minimum);
    _reg_btn = new QPushButton();
    _reg_btn->setText("注册");
    auto regbtn_layout = new QHBoxLayout();
    regbtn_layout->addItem(reg_btn_item1);
    regbtn_layout->addWidget(_reg_btn,5);
    QSpacerItem* reg_btn_item2 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    regbtn_layout->addItem(reg_btn_item2);
    vbox_layout->addLayout(regbtn_layout);

    auto verticalSpacer4 = new QSpacerItem(40,20, QSizePolicy::Fixed, QSizePolicy::Expanding);
    vbox_layout->addItem(verticalSpacer4);
    this->setLayout(vbox_layout);
}

Register::~Register()
{
    delete ui;
}

void Register::set_login(const  weak_ptr<Login> &login){
    _login = login;
}

void Register::ShowLogin()
{

}
```

**ps:在布局里面加控件我们用**addWidget



Register的构造函数中用代码的方式创建了一个垂直布局，垂直布局中增加了两个spacer，分别是verticalSpacer1和verticalSpacer4，以及三个水平布局pwd_layout，name_layout以及regbtn_layout，然后分别用代码的方式在三个布局中添加spacer和控件。
Login类的声明如下

```
#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <memory>
class Register;
using namespace std;
namespace Ui {
class Login;
}

class Login : public QDialog, public std::enable_shared_from_this<Login>
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void initSignals();

private slots:
    void on_regBtn_clicked();

private:
    Ui::Login *ui;
    std::shared_ptr<Register> _register;
};

#endif // LOGIN_H
```

Login实现如下

```
#include "ui_login.h"
#include <QBitmap>
#include <QPainter>
#include "register.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

void Login::initSignals(){
     _register = make_shared<Register>();
      //从本类转化为共享的智能指针给register类
     _register->set_login(shared_from_this());
}

Login::~Login()
{
    delete ui;
}

void Login::on_regBtn_clicked()
{

    this->close();
    _register->show();
}
```

main函数的实现如下

```
#include "login.h"
#include <QApplication>
#include <memory>
using namespace  std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<Login> w = make_shared<Login>();
    w->initSignals();
    w->show();

    return a.exec();
}
```

点击运行按钮，程序运行起来就可以从登录界面切换到注册界面了