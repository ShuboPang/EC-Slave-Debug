#ifndef SETTINGDATA_H
#define SETTINGDATA_H

#include<QSettings>

class SettingData
{
public:
    SettingData();
    ~SettingData();

    void Init(const QString& path);

public:
    void SetValue(const QString& key,const QString& value,const QString& group);

    QString GetValue(const QString& key,const QString& group,const QString& default_value);

private:
    QSettings *setting;
};

#endif // SETTINGDATA_H
