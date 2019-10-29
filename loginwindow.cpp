#include "contentwindown.h"
#include "httpclient.h"
#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QMovie>
#include <QStandardItemModel>
#include<QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    initTitleBar();
    this->setWindowIcon(QIcon(":/logo_text.png"));

}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::initTitleBar()
{

    QLabel* pBack = new QLabel(this);
    QMovie *movie = new QMovie();
    movie->setFileName(":/pbg.gif");
    pBack->setMovie(movie);
    movie->start();
    pBack->move(0, 0);

    // 设置标题栏跑马灯效果，可以不设置;
    m_titleBar->setTitleRoll();
    m_titleBar->setTitleIcon(":/logo_text.png");
    m_titleBar->setTitleContent(QStringLiteral("Jumpserver Client 桌面版"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
    m_titleBar->raise();
    //ui->login_pushButton->setShortcut(QKeySequence(QLatin1String("Enter")));
    ui->passwd_lineEdit->setEchoMode(QLineEdit::Password);
    QObject::connect(ui->login_pushButton,&QPushButton::clicked,this,&LoginWindow::onUserButtonClicked);
    //connect(&ui->jizhu_checkBox,&QCheckBox::toggled,this,&LoginWindow::setJizhuMima);

}

void LoginWindow::onUserButtonClicked(){
    //登录验证
    HttpClient httpClient;
    httpClient.setUrl("http://192.168.1.248/api/users/v1/auth/");
    httpClient.setContentTypeHeader("application/x-www-form-urlencoded");
    QString username = ui->user_lineEdit->text().toUtf8();
    QString password = ui->passwd_lineEdit->text().toUtf8();
    QString senddata = "username="+username+"&password="+password;
    httpClient.post(senddata);
    User* user = httpClient.getUserInfo();
    httpClient.close();
    if(user->getMsg().compare("null")==0){
        contentWindown = new ContentWindown();
        QObject::connect(this,&LoginWindow::singalUserinfo,contentWindown,&ContentWindown::onUserButtonClicked);
        this->close();
        contentWindown->show();
        emit singalUserinfo(user);
    }else {
        this->ui->err_msg->setText(user->getMsg());
    }
}

