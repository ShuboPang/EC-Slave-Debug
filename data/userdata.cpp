#include "userdata.h"

UserData::UserData()
{
    settings = nullptr;
}

UserData::~UserData()
{
    delete settings;
}

bool UserData::Init(const QString& path){
    settings = new SettingData;
    settings->Init(path);
    return true;
}

void UserData::SetUserValue(const QString& key,const QString& value){
    settings->SetValue(key,value,"user");
}

QString UserData::GetUserValue(const QString& key, const QString &default_value){
    return settings->GetValue(key,"user",default_value);
}

