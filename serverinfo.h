#ifndef SERVERINFO_H
#define SERVERINFO_H

#include <QObject>
class ServerInfo
{
public:
    ServerInfo();
    ~ServerInfo();
    QString iconSkin;
    QString ip;
    QString username;
    QString hostname;
    QString protocols;
};

#endif // SERVERINFO_H
