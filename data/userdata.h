#ifndef USERDATA_H
#define USERDATA_H

#include "settingdata.h"

class UserData
{
public:
    UserData();

    ~UserData();

    bool Init(const QString& path);

    void SetUserValue(const QString& key,const QString& value);

    QString GetUserValue(const QString& key, const QString &default_value);

private:
  SettingData *settings;
};

#endif // USERDATA_H
