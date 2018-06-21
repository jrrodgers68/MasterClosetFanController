#pragma once


namespace MasterClosetFanController {


class Publisher
{
public:
    Publisher();

    static void writeLogMessage(const char* source, const char* text);
};
}
