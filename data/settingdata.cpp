#include "settingdata.h"


SettingData::SettingData()
{
    setting = nullptr;
}

SettingData::~SettingData()
{
    if(setting != nullptr){
        delete setting;
    }
}

void SettingData::Init(const QString &path){
    setting = new QSettings(path,QSettings::IniFormat);
}

void SettingData::SetValue(const QString& key,const QString& value,const QString& group){
    setting->beginGroup(group);
    setting->setValue(key,value);
    setting->endGroup();
}

QString SettingData::GetValue(const QString& key,const QString& group,const QString& default_value){
    setting->beginGroup(group);
    QString value = default_value;
    if( setting->value(key) != NULL){
        value = setting->value(key).toString();
    }
    else{
        setting->setValue(key,default_value);
    }
    setting->endGroup();
    return value;
}
