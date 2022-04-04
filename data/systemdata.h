#ifndef SYSTEMDATA_H
#define SYSTEMDATA_H

#include "settingdata.h"


class SystemData
{
public:
    SystemData();

    ~SystemData();

    void SetSystemValue(const QString& key,const QString& value);

    QString GetSystemValue(const QString& key, const QString &default_value);

    QString Init();

    void SetCurrentPath(const QString& path);

    QString GetLastPath();

    QStringList GetHistoryList();

    static SystemData*  Instance(){
        static SystemData system_data;
        return &system_data;
    }


private:
    QString last_path;

private:
  SettingData *settings;
};

#endif // SYSTEMDATA_H
