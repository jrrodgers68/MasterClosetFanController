#pragma once



namespace MasterClosetFanController {


class SystemStatusMgr
{
private:
    static SystemStatusMgr* _instance;

    SystemStatusMgr();

public:
    static SystemStatusMgr* instance()
    {
        if(_instance == NULL)
        {
            _instance = new SystemStatusMgr();
        }
        return _instance;
    }
};

}
