#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "basewindow.h"
#include "contentwindown.h"
#include "user.h"

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;
    void initTitleBar();
    ContentWindown* contentWindown;
    QString* token;
    bool is_jizhumima;
    bool is_autoLogin;

signals:
    void singalUserinfo(User* user);
    //void singalUserinfo(QString* token);
public slots:
    void onUserButtonClicked();
    //void setJizhuMima();
};

#endif // LOGINWINDOW_H
