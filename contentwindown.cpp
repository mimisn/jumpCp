#include "contentwindown.h"
#include "httpclient.h"

#include <QFile>
#include <ui_contentwindown.h>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QMenu>
#include <QMessageBox>



ContentWindown::ContentWindown(QWidget *parent) : QWidget(parent),ui(new Ui::ContentWindown)
{
   ui->setupUi(this);
   initTable();
   this->loadStyleSheet("ContentWindown");
   mstscLoginList = new QList<QMap<QString,QString>>();
   timer = new QTimer();
   connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
   connect(ui->lineEdit, static_cast<void (QLineEdit::*)(const QString &)>(&QLineEdit::textChanged),this,&ContentWindown::qlineEditCange);
}


ContentWindown::~ContentWindown(){
    delete  mstscLoginList;
    delete  user;
    delete timer;
    delete menu;
    delete exAction;
    delete showAction;
    delete serList;
}

void ContentWindown::initTable(){
    QStringList HStrList;
    HStrList.push_back(QString("主机名"));
    HStrList.push_back(QString("IP"));
    HStrList.push_back(QString("系统用户"));
    HStrList.push_back(QString("系统类型"));
    HStrList.push_back(QString("动作"));
    //设置行列数(只有列存在的前提下，才可以设置列标签)
    int HlableCnt = HStrList.count();
    ui->tableWidget->setColumnCount(HlableCnt);
    //设置表格为只读
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置表头
    ui->tableWidget->setHorizontalHeaderLabels(HStrList);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //单元格自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->tableWidget->setShowGrid(true);//显示表格线
    //禁用水平滚动条
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->tableWidget->setStyleSheet("QHeaderView::section{background:skyblue;}");
    //表格添加控件
//    QComboBox *comBox = new QComboBox();
//    comBox->addItem("F");
//    comBox->addItem("M");
//    ui->tableWidget->setCellWidget(0,3,comBox);
    this->setWindowTitle("jumpserver Client");
    this->setWindowIcon(QIcon(":/logo_text.png"));

    //初始化托盘
    m_systray.setIcon(QIcon(":/logo_text.png"));
    m_systray.setToolTip(QObject::tr("jumpserver"));
    menu = new QMenu();
    exAction = new QAction(QObject::tr("退出"),this);
    showAction = new QAction(QObject::tr("打开主界面"),this);
    connect(exAction,&QAction::triggered,this,&ContentWindown::close);
    connect(showAction,&QAction::triggered,this,&ContentWindown::showNormal);
    menu->addAction(showAction);
    menu->addSeparator();
    menu->addAction(exAction);
    m_systray.setContextMenu(menu);
    connect(&m_systray,&QSystemTrayIcon::activated,this,&ContentWindown::QSystemTrayIconShowWindwon);




}

void ContentWindown::loadServerInfo(){
    HttpClient httpClient;
    httpClient.setUrl("http://192.168.1.248/api/perms/v1/users/nodes-assets/tree/?cache_policy=1");
    httpClient.setContentTypeHeader("application/x-www-form-urlencoded");
    QByteArray dat = ("Bearer "+this->user->token).toUtf8();
    httpClient.setAuthorizationHeader(dat);
    httpClient.get();
    serList = httpClient.getServerInfo();
    ui->tableWidget->setRowCount(serList->size());

    for (int i=0;i<serList->size();i++) {
        ServerInfo* serInfo = serList->at(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(serInfo->hostname));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(serInfo->ip));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(serInfo->username));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(serInfo->iconSkin));
        QPushButton* button = new QPushButton();
        button->setText("连接");
        button->setObjectName("tableButton_"+QString::number(i));
        button->setMaximumWidth(80);
        button->setMinimumWidth(80);
        connect(button,&QPushButton::clicked,this,&ContentWindown::tableButtonClicked);
        ui->tableWidget->setCellWidget(i,4,button);
    }
    is_httpClient_over=true;
    httpClient.close();
}

//重绘列表长高
void ContentWindown::paintEvent(QPaintEvent *event){
    if(!is_httpClient_over){
        loadServerInfo();
    }
    if(ui->tableWidget->width()!=this->width()-10){
        ui->tableWidget->setFixedWidth(this->width()-10);
        ui->tableWidget->setFixedHeight(this->height()-80);
        ui->tableWidget->move(5,40);
        ui->username->setText(this->user->username);
        ui->username->move(this->width()-60,5);
        ui->user_image->move(this->width()-60-31,5);
        ui->label->move(this->width()/2-36,5);
        ui->lineEdit->move(this->width()/2,10);
    }

    QWidget::paintEvent(event);
}


// 加载本地样式文件;
// 可以将样式直接写在文件中，程序运行时直接加载进来;
void ContentWindown::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}


void ContentWindown::onUserButtonClicked(User* user){
    this->user = user;
}

void ContentWindown::tableButtonClicked(){
    QString m_ObjectName = sender()->objectName();
    QStringList list = m_ObjectName.split("_");
    int infox = list[1].toInt();
    HttpClient httpClient;
    httpClient.setUrl("http://192.168.1.248/mstsc/v1/api?ip="+serList->at(infox)->ip+"&port="+serList->at(infox)->protocols.toUtf8().toPercentEncoding()+"&token="+user->getToken());
    httpClient.setContentTypeHeader("application/x-www-form-urlencoded");
    httpClient.get();
    QString Ip=httpClient.getMstscIp();
    httpClient.close();

    if(Ip.compare("null")!=0){
        QProcess process ;
        QString cmd = QString("mstsc /v:%1 /admin /f").arg(Ip);
        process.startDetached(cmd);
        //process.waitForStarted();
        //192.168.1.248:49033 - 远程桌面连接
        //connect(&process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),this,&ContentWindown::processFinished);

        QMap<QString,QString> mstscLoginMap;
        mstscLoginMap.insert("rdp_old_name",Ip);
        mstscLoginMap.insert("rdp_new_name",serList->at(infox)->ip);
        mstscLoginList->append(mstscLoginMap);

        if(!timer->isActive()){
             timer->start(1000);
        }
    }else{
        QMessageBox *messageBox = new QMessageBox(this);
        messageBox->setIcon(QMessageBox::Warning);
        messageBox->setWindowTitle(QObject::tr("登录超时或认证错误"));
        messageBox->setText(QObject::tr("当前登录超时或发生未知错误，请重新登录！"));
        messageBox->addButton("确定",QMessageBox::AcceptRole);
        messageBox->show();
        if(messageBox->exec() == QDialog::Accepted){
            qApp->exec();
        }
    }
}


void ContentWindown::qlineEditCange(const QString & text){
    int rC = ui->tableWidget->rowCount();//获得行号
    if("" == text){
        for (int i=0;i<rC;i++) {
            ui->tableWidget->setRowHidden(i,false); //显示所有行
        }
    }else{
        //获取符合条件的cell索引
        QList <QTableWidgetItem *> item = ui->tableWidget->findItems(text, Qt::MatchContains);
        for (int i = 0; i < rC; i++)
        {
            ui->tableWidget->setRowHidden(i, true);//隐藏所有行
        }

        if (!item.isEmpty())//不为空
        {
            for (int i = 0; i < item.count(); i++)
            {
                ui->tableWidget->setRowHidden(item.at(i)->row(),false);//item.at(i).row()输出行号
            }
        }

    }

}

void ContentWindown::processOneThing(){
    if(!mstscLoginList->isEmpty()){
        for(int i=0;i<mstscLoginList->size();i++){
            QMap<QString,QString> mstsc = mstscLoginList->at(i);
            hwndSetWindowText(mstsc.find("rdp_old_name").value(),mstsc.find("rdp_new_name").value(),i);
        }
    }else {
        if(timer->isActive()){
            timer->stop();
        }
    }
}


void ContentWindown::hwndSetWindowText(QString rdp_old_name,QString rdp_new_name,int in){
    HWND hwnd=NULL;
    QString  rdp_class = "TscShellContainerClass";//"TscShellContainerClass";
    const wchar_t *prdp_class = reinterpret_cast<const wchar_t *>(rdp_class.utf16());
    QString  rdp_name = rdp_old_name+" - 远程桌面连接";
    const wchar_t *prdp_name = reinterpret_cast<const wchar_t *>(rdp_name.utf16());
    hwnd =::FindWindow(prdp_class,prdp_name);
    QString  title = QString("%1").arg(rdp_new_name);
    const wchar_t *title_c = reinterpret_cast<const wchar_t *>(title.utf16());
    ::SetWindowText(hwnd,title_c);
    if(hwnd!=NULL){
        mstscLoginList->removeAt(in);
    }
    //MoveWindow(hwnd,0,0,576,27,TRUE);
}


void ContentWindown::changeEvent(QEvent *event){
    if(event->type()==QEvent::WindowStateChange){
        if(windowState() & Qt::WindowMinimized){
            this->hide();
            m_systray.show();
        }
        QWidget::changeEvent(event);
    }
}



void ContentWindown::QSystemTrayIconShowWindwon(QSystemTrayIcon::ActivationReason reason){
    if(reason & QSystemTrayIcon::DoubleClick){
        this->showNormal();
    }
}







