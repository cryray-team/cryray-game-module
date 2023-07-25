#pragma once

class CBlender_msaa : public IBlender
{
public:
    virtual LPCSTR getComment() { return "INTERNAL: DX10 msaa blender"; }
    virtual BOOL canBeDetailed() { return FALSE; }
    virtual BOOL canBeLMAPped() { return FALSE; }

    virtual void Compile(CBlender_Compile& C);
};
