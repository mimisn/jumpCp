#ifndef USER_H
#define USER_H

#include <QObject>

class User
{
public:
    User();
    ~User();
    QString token;
    QString name;
    QString username;
    QString email;
    QString avatar_url;
    QString msg;

public:
    void setToken(const QString &token);
    void setName(const QString &name);
    void setUserName(const QString &username);
    void setEmail(const QString &email);
    void setAvatarUrl(const QString &avatarurl);
    void setMsg(const QString &msg);

    QString getToken();
    QString getName();
    QString getUserName();
    QString getEmail();
    QString getAvatarUrl();
    QString getMsg();

};

#endif // USER_H
