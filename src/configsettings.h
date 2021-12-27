#ifndef  CONFIGSETTINGS_H
#define CONFIGSETTINGS_H


class ConfigSettingsImpl;



class ConfigSettings
{
public:
    ConfigSettings* instance()
    {
        if(_instance == NULL)
        {
            _instance = new ConfigSettings();
        }
        return _instance;
    }

    void init(int implType);

private:
    static ConfigSettings* _instance;

    ConfigSettingsImpl* _impl;

    ConfigSettings();
    ~ConfigSettings();


};





#endif // CONFIGSETTINGS_H
