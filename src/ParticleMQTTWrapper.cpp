#include "application.h"
#include "ParticleMQTTWrapper.h"


void init(const char* connectionName, void (*callback)(const char*,uint8_t*,unsigned int))
{
    MQTTWrapper::instance()->init("MasterClosetFanController", callback);

    TopicMgr::instance()->init();
    JsonMQTTMsgMgr::instance()->init(MQTTWrapper::instance()->client());
}
