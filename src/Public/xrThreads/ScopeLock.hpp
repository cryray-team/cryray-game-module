#pragma once
#include "Lock.hpp"

class ScopeLock
{
    Lock* syncObject;

public:
    ScopeLock(Lock* SyncObject);
    ~ScopeLock();
};

ScopeLock::ScopeLock(Lock* SyncObject) : syncObject(SyncObject)
{
    syncObject->Enter();
}

ScopeLock::~ScopeLock() { syncObject->Leave(); }