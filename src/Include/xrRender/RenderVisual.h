#pragma once

class IKinematics;
class IKinematicsAnimated;
class IParticleCustom;
struct vis_data;

class IRenderVisual
{
public:
    IRenderVisual() { _ignore_optimization = false; }

    virtual ~IRenderVisual() {}

    virtual vis_data& getVisData() = 0;
    virtual u32 getType() = 0;

    bool _ignore_optimization;

#ifdef DEBUG
    virtual shared_str getDebugName() = 0;
#endif

    virtual IKinematics* dcast_PKinematics() { return 0; }
    virtual IKinematicsAnimated* dcast_PKinematicsAnimated() { return 0; }
    virtual IParticleCustom* dcast_ParticleCustom() { return 0; }
};
