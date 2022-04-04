#include "systemdata.h"
#include <QFile>

SystemData::SystemData()
{
    settings = new SettingData;
    settings->Init("./setting/config.ini");
}

SystemData::~SystemData()
{
    if(settings != nullptr){
        delete  settings;
    }
}

void SystemData::SetSystemValue(const QString& key,const QString& value){
    settings->SetValue(key,value,"system");
}

QString SystemData::GetSystemValue(const QString& key,const QString& default_value){
    return settings->GetValue(key,"system",default_value);
}

QString SystemData::Init(){
    last_path = GetSystemValue("last_path","");
    return last_path;
}

void SystemData::SetCurrentPath(const QString& path){
    last_path = path;
    SetSystemValue("last_path",path);
}

QString SystemData::GetLastPath(){
    QString str;
    return str;
}

QStringList SystemData::GetHistoryList(){
    QStringList str_list;
    return str_list;
}
