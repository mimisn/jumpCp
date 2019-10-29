#ifndef CONTENTWINDOWN_H
#define CONTENTWINDOWN_H

#include "serverinfo.h"
#include "user.h"

#include <QProcess>
#include <QSystemTrayIcon>
#include <QWidget>
#include <windows.h>
#include <QTimer>
#include <QMap>

namespace Ui {
class ContentWindown;
}

class ContentWindown : public QWidget
{
    Q_OBJECT
public:
    explicit ContentWindown(QWidget *parent = nullptr);
    ~ContentWindown();
     void hwndSetWindowText(QString rdp_old_name,QString rdp_new_name,int in);
private:
    void initTable();
    void paintEvent(QPaintEvent *event);
    void loadStyleSheet(const QString &sheetName);
    void loadServerInfo();
    void changeEvent(QEvent* event);
    void QSystemTrayIconShowWindwon(QSystemTrayIcon::ActivationReason reason);


signals:
    void findHwnd(QString rdp_old_name,QString rdp_new_name);

public slots:
    void onUserButtonClicked(User* user);
    void tableButtonClicked();
    void processOneThing();
    void qlineEditCange(const QString & text);

private:
    Ui::ContentWindown *ui;
    QMenu* menu;
    QAction* exAction;
    QAction* showAction;
    User* user;
    QList<ServerInfo*>* serList;
    bool is_httpClient_over=false;
    QSystemTrayIcon m_systray;
    QList<QMap<QString,QString>> *mstscLoginList;
    QTimer *timer;

};

#endif // CONTENTWINDOWN_H
