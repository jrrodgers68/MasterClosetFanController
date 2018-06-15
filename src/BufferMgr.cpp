#include "application.h"
#include "BufferMgr.h"
#include "Buffer.h"

using namespace ParticleMQTTWrapper;

BufferMgr* BufferMgr::_instance = NULL;

BufferMgr::BufferMgr()
{
    // prefill with some buffers
    for(int i=0; i < 12; ++i)
    {
        Buffer* p = new Buffer(64);
        _freeList.push_back(p);

        Buffer* p2 = new Buffer(128);
        _freeList.push_back(p2);
    }
}

Buffer*  BufferMgr::allocate(unsigned long size)
{
    // try to find a matching buffer in the freeList
    // if not, create one and add to inuseList
    std::list<Buffer*>::iterator it = _freeList.begin();
    while(it != _freeList.end())
    {
        Buffer* p = *it;
        if(p->maxSize() >= size)
        {
            _freeList.erase(it);
            _inuseList.push_back(p);
            return p;
        }
    }

    // fixme: do some sanity checking on size!!
    Buffer* p = new Buffer(size);
    _inuseList.push_back(p);
    return p;
}

void BufferMgr::deallocate(Buffer* buffer)
{
    // find in inuseList and move to freeList
    bool removed = false;
    std::list<Buffer*>::iterator it = _inuseList.begin();
    while(it != _inuseList.end())
    {
        Buffer* p = *it;
        if(buffer == p)
        {
            _inuseList.erase(it);
            removed = true;
            break;
        }
        it++;
    }

    if(!removed)
    {
        // arg, what buffer is this!!!
        // fixme: we are leaking the buffer - log this
        return;
    }

    _freeList.push_back(buffer);
}
