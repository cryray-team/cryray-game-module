#pragma once

namespace GameObject
{
enum ECallbackType
{
    eTradeStart = u32(0),
    eTradeStop,
    eTradeSellBuyItem,
    eTradePerformTradeOperation,

    eZoneEnter,
    eZoneExit,
    eExitLevelBorder,
    eEnterLevelBorder,
    eDeath,

    ePatrolPathInPoint,

    eInventoryPda,
    eInventoryInfo,
    eArticleInfo,
    eTaskStateChange,
    eMapLocationAdded,

    eUseObject,

    eHit,

    eSound,

    eActionTypeMovement,
    eActionTypeWatch,
    eActionTypeRemoved,
    eActionTypeAnimation,
    eActionTypeSound,
    eActionTypeParticle,
    eActionTypeObject,

    eActorSleep,

    eHelicopterOnPoint,
    eHelicopterOnHit,

    eOnItemTake,
    eOnItemDrop,

    eScriptAnimation,

    eTraderGlobalAnimationRequest,
    eTraderHeadAnimationRequest,
    eTraderSoundEnd,

    eInvBoxItemTake,
    eWeaponNoAmmoAvailable,
    // Alundaio: added defines
    eActorHudAnimationEnd,
// AVO: custom callbacks
    // input
    eKeyPress,
    eKeyRelease,
    eKeyHold,
    // inventory
    eItemToBelt,
    eItemToSlot,
    eItemToRuck,
    // weapon
    eOnWeaponZoomIn,
    eOnWeaponZoomOut,
    eOnWeaponJammed,
    eOnWeaponFired,
    eOnWeaponMagazineEmpty,
// actor
    eActorBeforeDeath,
    // vehicle
    eAttachVehicle,
    eDetachVehicle,
    eUseVehicle,
    eOnFootStep,
    //-AVO

    eOnWeaponLowered,
    eOnWeaponRaised,

    eDummy = u32(-1),
};
}; // namespace GameObject
