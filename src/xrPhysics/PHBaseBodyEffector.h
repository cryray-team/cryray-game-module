#pragma once
#include "ode/include/ode/common.h"

class CPHBaseBodyEffector
{
protected:
    dBodyID m_body;

public:
    void Init(dBodyID body) { m_body = body; }
};
