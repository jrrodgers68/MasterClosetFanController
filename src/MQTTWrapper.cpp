#include "application.h"
#include "MQTTWrapper.h"

using namespace ParticleMQTTWrapper;


MQTTWrapper* MQTTWrapper::_instance = NULL;


#define MQTT_BROKER         "pi3_2"
#define MQTT_PORT           1883


MQTTWrapper::MQTTWrapper() : clientCallback(NULL)
{
    _client = new MQTT(MQTT_BROKER, MQTT_PORT, MQTTWrapper::callback);
}

void MQTTWrapper::callback(char* topic, uint8_t* payload, unsigned int length)
{
    if(MQTTWrapper::instance()->clientCallback)
    {
        MQTTWrapper::instance()->clientCallback(topic, payload, length);
    }
}

void MQTTWrapper::init(const char* connectionName, void (*callback)(const char*,uint8_t*,unsigned int))
{
    clientCallback = callback;

    if(_client)
    {
        _client->connect(connectionName);
    }
}

void MQTTWrapper::loop()
{
    if(_client)
    {
        _client->loop();
    }
}
