#pragma once



namespace ParticleMQTTWrapper {



class Buffer
{
private:
    char*  _buffer;
    unsigned long _size;
    unsigned long _maxSize;

public:
    Buffer(unsigned long maxSize) : _buffer(NULL), _size(0), _maxSize(maxSize)
    {
        _buffer = new char[maxSize];
    }

    char* buffer()
    {
        return _buffer;
    }

    unsigned long size()
    {
        return _size;
    }

    unsigned long maxSize()
    {
        return _maxSize;
    }
};

}
