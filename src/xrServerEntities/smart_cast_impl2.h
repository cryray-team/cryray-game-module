////////////////////////////////////////////////////////////////////////////
//	Module 		: smart_cast_impl2.h
//	Created 	: 17.09.2004
//  Modified 	: 17.09.2004
//	Author		: Dmitriy Iassenev
//	Description : Smart dynamic cast implementation
////////////////////////////////////////////////////////////////////////////

#pragma once

#undef DECLARE_SPECIALIZATION

#define DECLARE_SPECIALIZATION(B, A, C)                                                                                \
    template <>                                                                                                        \
    B* SmartDynamicCast::dynamic_cast<B, A>(A * p)                                                                       \
    {                                                                                                                  \
        return p->C();                                                                                                 \
    };
#ifdef XRGAME_EXPORTS
template <>
CGameObject* SmartDynamicCast::dynamic_cast<CGameObject, CObject>(CObject* p)
{
    return static_cast<CGameObject*>(p);
}

template <>
Feel::Sound* SmartDynamicCast::dynamic_cast<Feel::Sound, ISpatial>(ISpatial* p)
{
    return (p->dcast_FeelSound());
}
#endif