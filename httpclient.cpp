#include "httpclient.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextCodec>

HttpClient::HttpClient(QObject *parent):
    QObject(parent),
    is_https(false)
{
        initHttp();
}

HttpClient::~HttpClient(){
    delete this->m_AccessManager;
}


void HttpClient::initHttp(){
    this->m_AccessManager = new QNetworkAccessManager();
}

void HttpClient::setUrl(const QString &url){
    this->m_url.setUrl(url);
    this->m_Request.setUrl(this->m_url);
}

void HttpClient::setIsHttps(bool k){
    this->is_https=k;
}
void HttpClient::setRawHeader(const QByteArray  &key, const QByteArray  &value){
    this->m_Request.setRawHeader(key,value);
}
void HttpClient::setAcceptHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Accept",value);
}

void HttpClient::setAcceptLanguageHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Accept-Language",value);
}

void HttpClient::setContentTypeHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Content-Type",value);
}

void HttpClient::setUserAgentHeader(const QByteArray &value){
    this->m_Request.setRawHeader("User-Agent",value);
}

void HttpClient::setAcceptCharsetHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Accept-Charset",value);
}

void HttpClient::setAcceptRangesHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Accept-Ranges",value);
}

void HttpClient::setCacheControlHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Cache-Control",value);
}

void HttpClient::setAuthorizationHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Authorization",value);
}

void HttpClient::setConnectionHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Connection",value);
}

void HttpClient::setRefererHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Referer",value);
}
void HttpClient::setAcceptEncodingHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Accept-Encoding",value);
}

void HttpClient::setCookieHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Cookie",value);
}
void HttpClient::setContentLengthHeader(const QByteArray &value){
    this->m_Request.setRawHeader("Content-Length",value);
}

void HttpClient::setJumpOverSslAuthenticate(){
    if(is_https){
        this->m_config.setPeerVerifyMode(QSslSocket::VerifyNone);
        this->m_config.setProtocol(QSsl::TlsV1SslV3);
        this->m_Request.setSslConfiguration(this->m_config);
    }
}

void HttpClient::get(){
    this->Reply=this->m_AccessManager->get(this->m_Request);
    QEventLoop eventloop;
    connect( this->Reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec( QEventLoop::ExcludeUserInputEvents);
}

void HttpClient::post(const QString &data){
    this->m_sendData = data.toUtf8();
    this->Reply=this->m_AccessManager->post(this->m_Request,this->m_sendData);
    QEventLoop eventloop;
    connect( this->Reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    eventloop.exec( QEventLoop::ExcludeUserInputEvents);
}

QString HttpClient::responseQString(){
    int NetworkErrors = -1;
    NetworkErrors = this->Reply->error();
    if(NetworkErrors !=QNetworkReply::NoError){
         return "null";
     }
     QTextCodec *codec = QTextCodec::codecForName("utf8");
     QString replyData = codec->toUnicode( this->Reply->readAll() );
     return  replyData;
}

QNetworkReply* HttpClient::responseQNetworkReply(){
    return this->Reply;
}

void HttpClient::close(){
    this->Reply->deleteLater();
    this->Reply->close();
}

User* HttpClient::getUserInfo(){
    User* user =new User();
    int NetworkErrors = -1;
    NetworkErrors = this->Reply->error();


    if(NetworkErrors ==QNetworkReply::NoError){
        //解析json
        QByteArray databuff = this->Reply->readAll();
        QString result = QString::fromStdString(databuff.toStdString()).toUtf8();
        QJsonDocument document;
        QJsonParseError ParseError;
        document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);

        if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
            if(document.isObject()){
                QJsonObject Object = document.object();
                if(Object.contains("msg")){
                    QJsonValue msg = Object.value("msg");
                    user->msg=msg.toString();
                    return user;
                }else {
                    user->msg="null";
                }
                if(Object.contains("token")){
                    QJsonValue token = Object.value("token");
                    user->token=token.toString();
                }
                if(Object.contains("user")){
                    QJsonValue userdata = Object.value("user");
                    if(userdata.isObject()){
                        QJsonObject userObject = userdata.toObject();
                        if(userObject.contains("name")){
                             QJsonValue namedata = userObject.value("name");
                             user->name = namedata.toString();
                        }
                        if(userObject.contains("username")){
                            QJsonValue usernamedata = userObject.value("username");
                            user->username = usernamedata.toString();
                        }
                        if(userObject.contains("email")){
                            QJsonValue emaildata = userObject.value("email");
                            user->email = emaildata.toString();
                        }
                        if(userObject.contains("avatar_url")){
                            QJsonValue avatarurldata = userObject.value("avatar_url");
                            user->avatar_url = avatarurldata.toString();
                        }

                    }
                }
            }
        }
    }else {
        QByteArray databuff = this->Reply->readAll();
        QString result = QString::fromStdString(databuff.toStdString()).toUtf8();
        QJsonDocument document;
        QJsonParseError ParseError;
        document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);

        if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
            if(document.isObject()){
                QJsonObject Object = document.object();
                if(Object.contains("msg")){
                    QJsonValue msg = Object.value("msg");
                    user->msg=msg.toString();
                    return user;
                }
             }
        }
    }
    return user;
}

QList<ServerInfo*>* HttpClient::getServerInfo(){
    QList<ServerInfo*>* serList = new  QList<ServerInfo*>;
    int NetworkErrors = -1;
    NetworkErrors = this->Reply->error();


    if(NetworkErrors ==QNetworkReply::NoError){
        QByteArray databuff = this->Reply->readAll();
        QString result = QString::fromStdString(databuff.toStdString()).toUtf8();
        QJsonDocument document;
        QJsonParseError ParseError;
        document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);

        if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
            if(document.isArray()){
                QJsonArray array=document.array();
                for (int i=0;i<array.size();i++) {
                     QJsonValue value = array.at(i);
                     ServerInfo* serInfo=new ServerInfo();
                     if(value.isObject()){
                         QJsonObject arrayObject = value.toObject();
                         if(arrayObject.contains("iconSkin")){
                             QJsonValue arrayValue = arrayObject.value("iconSkin");
                             if(arrayValue.toString().compare("")==0){
                                 continue;
                             }else {
                                   serInfo->iconSkin=arrayValue.toString();
                             }
                         }
                         if(arrayObject.contains("meta")){
                            QJsonValue metaValue = arrayObject.value("meta");
                            if(metaValue.isObject()){
                                QJsonObject metaObject=metaValue.toObject();
                                if(metaObject.contains("asset")){
                                    QJsonValue assetValue = metaObject.value("asset");
                                    if(assetValue.isObject()){
                                        QJsonObject assetObject=assetValue.toObject();
                                        if(assetObject.contains("hostname")){
                                            QJsonValue hostnameValue = assetObject.value("hostname");
                                            serInfo->hostname=hostnameValue.toString();
                                        }
                                        if(assetObject.contains("ip")){
                                            QJsonValue ipValue = assetObject.value("ip");
                                            serInfo->ip=ipValue.toString();
                                        }
                                        if(assetObject.contains("protocols")){
                                            QJsonValue protocolsValue = assetObject.value("protocols");
                                            if(protocolsValue.isArray()){
                                                QJsonArray protocolsList = protocolsValue.toArray();
                                                serInfo->protocols="["+protocolsList.at(0).toString()+"]";
                                            }
                                            serInfo->username="xwtec";
                                        }
                                    }
                                }
                            }
                         }


                     }
                    //
                     serList->append(serInfo);
                }
            }
        }
    }

    return serList;
}

QString HttpClient::getMstscIp(){
    int NetworkErrors = -1;
    NetworkErrors = this->Reply->error();
    QString ip;
    QString port;


    if(NetworkErrors ==QNetworkReply::NoError){
        QByteArray databuff = this->Reply->readAll();
        QString result = QString::fromStdString(databuff.toStdString()).toUtf8();
        QJsonDocument document;
        QJsonParseError ParseError;
        document =QJsonDocument::fromJson(result.toUtf8(),&ParseError);

        if(!document.isNull() && ParseError.error == QJsonParseError::NoError){
            if(document.isObject()){
                QJsonObject Object = document.object();
                if(Object.contains("status")){
                    QJsonValue statusValue = Object.value("status");
                    if(statusValue.toString().compare("success")!=0){
                        return "null";
                    }
                }
                if(Object.contains("ip")){
                    QJsonValue ipValue = Object.value("ip");
                    ip=ipValue.toString();
                }
                if(Object.contains("port")){
                    QJsonValue portValue = Object.value("port");
                    port = portValue.toString();
                }
            }
        }
    }else {
        return "null";
    }
    return QString("%1:%2").arg(ip).arg(port);
}

