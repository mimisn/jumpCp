#include "user.h"

User::User()
{

}
User::~User(){

}
void User::setToken(const QString &token){
    this->token = token;

}
void User::setName(const QString &name){
    this->name = name;

}
void User::setUserName(const QString &username){
    this->username = username;
}
void User::setEmail(const QString &email){
    this->email=email;
}
void User::setAvatarUrl(const QString &avatarurl){
    this->avatar_url=avatarurl;
}
void User::setMsg(const QString &msg){
    this->msg=msg;
}

QString User::getToken(){
    return this->token;
}

QString User::getUserName(){
    return this->username;
}
QString User::getEmail(){
    return this->email;
}

QString User::getAvatarUrl(){
    return this->avatar_url;
}
QString User::getName(){
    return this->name;
}

QString User::getMsg(){
    return this->msg;
}
