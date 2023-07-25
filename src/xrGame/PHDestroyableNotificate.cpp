#include "stdafx.h"
#include "PHDestroyableNotificate.h"
#include "../xrGameAPI\alife_space.h"
#include "hit.h"
#include "PHDestroyable.h"
#include "Level.h"
#include "xrServer_Object_Base.h"
#include "../xrEngine/xr_object.h"
#include "PhysicsShellHolder.h"
#include "xrServer_Objects.h"

void CPHDestroyableNotificate::spawn_notificate(CSE_Abstract* so)
{
    CPHDestroyableNotificator* D = NULL;
    CSE_PHSkeleton* po = dynamic_cast<CSE_PHSkeleton*>(so);
    u16 id = u16(-1);
    if (po)
        id = po->get_source_id();
    if (id != u16(-1))
        D = dynamic_cast<CPHDestroyableNotificator*>(Level().Objects.net_Find(id));
    if (D)
        D->NotificateDestroy(this);
    po->source_id = BI_NONE;
}