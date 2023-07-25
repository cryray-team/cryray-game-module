#pragma once

class CEffect_Rain;
struct SRainParams;

#include "../xrCore/_sphere.h"

class IRainRender
{
public:
    virtual ~IRainRender() { ; }
    virtual void Copy(IRainRender& _in) = 0;

    virtual void Render(CEffect_Rain& owner, SRainParams* p) = 0;

    virtual const Fsphere& GetDropBounds() const = 0;
};
