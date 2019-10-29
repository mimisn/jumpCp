#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "serverinfo.h"
#include "user.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QEventLoop>


class HttpClient:public QObject
{
    Q_OBJECT
public:
     explicit HttpClient(QObject *parent=nullptr);
    ~HttpClient();
    void initHttp();
    void setIsHttps(bool k);
    void setUrl(const QString &url);
    //设置http头
    void setRawHeader(const QByteArray  &key,const QByteArray  &value );
    //可接受的响应内容类型
    void setAcceptHeader(const QByteArray  &value  );
    //可接受的响应内容语言列表。
    void setAcceptLanguageHeader(const QByteArray  &value);
    //请求体的MIME类型 （用于POST和PUT请求中）
    void setContentTypeHeader(const QByteArray  &value);
    //浏览器表明自己的身份（是哪种浏览器）。
    void setUserAgentHeader(const QByteArray  &value);
    //浏览器申明自己接收的字符集
    void setAcceptCharsetHeader(const QByteArray  &value);
    //WEB服务器表明自己是否接受获取其某个实体的一部分（比如文件的一部分）的请求
    void setAcceptRangesHeader(const QByteArray  &value);
    //缓存
    void setCacheControlHeader(const QByteArray  &value);
    //身份验证信息给
    void setAuthorizationHeader(const QByteArray  &value);
    //请求：close（告诉WEB服务器或者代理服务器，在完成本次请求的响应后，断开连接，不要等待本次连接的后续请求了）。
    //keepalive（告诉WEB服务器或者代理服务器，在完成本次请求的响应后，保持连接，等待本次连接的后续请求）。
    //响应：close（连接已经关闭）。
    //keepalive（连接保持着，在等待本次连接的后续请求）。
    //Keep-Alive：如果浏览器请求保持连接，则该头部表明希望 WEB 服务器保持连接多长时间（秒）。例如：Keep-Alive：300
    void setConnectionHeader(const QByteArray  &value);
    //浏览器向 WEB 服务器表明自己是从哪个网页/URL 获得/点击当前请求中的网址/URL。
    void setRefererHeader(const QByteArray  &value);
    //可接受的响应内容的编码方式。
    void setAcceptEncodingHeader(const QByteArray  &value);
    //设置的一个HTTP协议Cookie
    void setCookieHeader(const QByteArray  &value);
    //以8进制表示的请求体的长度
    void setContentLengthHeader(const QByteArray  &value);
    //设置跳过ssl验证
    void setJumpOverSslAuthenticate();

    void get();
    void post(const QString &data);
    QString responseQString();
    QNetworkReply*  responseQNetworkReply();
    void close();

    User* getUserInfo();
    QList<ServerInfo*>* getServerInfo();
    QString getMstscIp();

public:
    bool is_https = false;

private:
    QUrl m_url;
    QNetworkRequest m_Request;
    QByteArray m_sendData;
    QNetworkAccessManager* m_AccessManager;
    QSslConfiguration m_config;
    QNetworkReply* Reply;
    //QList<ServerInfo>* serList;

};

#endif // HTTPCLIENT_H
