#include "Particle.h"
#include "configsettings.h"

ConfigSettings* ConfigSettings::_instance = NULL;
//ConfigSettingsImpl* ConfigSettings::_impl = NULL;


ConfigSettings::ConfigSettings(/* args */)
{
    _impl = NULL;
}


ConfigSettings::~ConfigSettings()
{
}


void ConfigSettings::init(int implType)
{
    
}