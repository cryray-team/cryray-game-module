#pragma once
#include "../xrCore/clsid.h"
#include "../xrServerEntities/clsid_game.h"

namespace XRay
{
	public enum class GameClassIDs : unsigned long long
	{
		// basic
		Actor			= CLSID_OBJECT_ACTOR,
		Actor2			= CLSID_OBJECT_ACTOR2,
		HemiLamp		= CLSID_OBJECT_HLAMP,
		Entity			= CLSID_ENTITY,
		Spectator		= CLSID_SPECTATOR,
		ObjectProjector = CLSID_OBJECT_PROJECTOR,
		LevelPoint		= CLSID_LEVEL_POINT,
		ScriptObject	= CLSID_SCRIPT_OBJECT,
		AIGraph			= CLSID_AI_GRAPH,
		AICrow			= CLSID_AI_CROW,

		// ai creatures
		Zombie			= CLSID_AI_ZOMBIE,
		Poltergeist		= CLSID_AI_POLTERGEIST,
		Flesh			= CLSID_AI_FLESH,
		FleshGroup		= CLSID_AI_FLESH_GROUP,
		Phantom			= CLSID_AI_PHANTOM,
		Sponger			= CLSID_AI_SPONGER,
		Controller		= CLSID_AI_CONTROLLER,
		Bloodsucker		= CLSID_AI_BLOODSUCKER,
		Stalker			= CLSID_AI_STALKER,
		Burer			= CLSID_AI_BURER,
		PseudoGiant		= CLSID_AI_GIANT,
		Chimera			= CLSID_AI_CHIMERA,
		Fracture		= CLSID_AI_FRACTURE,
		DogBlack		= CLSID_AI_DOG_BLACK,
		DogRed			= CLSID_AI_DOG_RED,
		DogPsy			= CLSID_AI_DOG_PSY,
		DogPsyPhantom	= CLSID_AI_DOG_PSY_PHANTOM,
		Trader			= CLSID_AI_TRADER,
		Boar			= CLSID_AI_BOAR,
		Snork			= CLSID_AI_SNORK,
		Cat				= CLSID_AI_CAT,
		Tushkano		= CLSID_AI_TUSHKANO,
		//Rat				= CLSID_AI_RAT,
		
		// inventory
        Ruck			= CLSID_EQUIPMENT_BACKPACK,

		// venichles
		Car				= CLSID_CAR,
		Helicopter		= CLSID_VEHICLE_HELICOPTER,

		// misc
		Event			= CLSID_EVENT,
		Artefact		= CLSID_ARTEFACT,

		// weapons
		WeaponBinocular		= CLSID_OBJECT_W_BINOCULAR,
		WeaponShotgun		= CLSID_OBJECT_W_SHOTGUN,
		WeaponAutoShotgun	= CLSID_OBJECT_W_ASHOTGUN,
		WeaponMaganized		= CLSID_OBJECT_W_MAGAZINED,
		WeaponMaganizedWithGrenade = CLSID_OBJECT_W_MAGAZWGL,
		WeaponSVD			= CLSID_OBJECT_W_SVD,
		WeaponRPG7			= CLSID_OBJECT_W_RPG7,
		WeaponVintorez		= CLSID_OBJECT_W_VINTOREZ,
		WeaponKnife			= CLSID_OBJECT_W_KNIFE,
		WeaponBM6			= CLSID_OBJECT_W_BM16,
		WeaponRG6			= CLSID_OBJECT_W_RG6,

		WeaponStationaryMinigun = CLSID_OBJECT_W_STATMGUN,

		// ammo
		Ammo		= CLSID_OBJECT_AMMO,
		AmmoVOG25	= CLSID_OBJECT_A_VOG25,
		AmmoOG7B	= CLSID_OBJECT_A_OG7B,
		AmmoM209	= CLSID_OBJECT_A_M209,

		// weapon add-ons
		WeaponAddOnScope			= CLSID_OBJECT_W_SCOPE,
		WeaponAddOnSilencer			= CLSID_OBJECT_W_SILENCER,
		WeaponAddOnGrenadeLauncher	= CLSID_OBJECT_W_GLAUNCHER,

		// level objects
		ObjectStandart		= CLSID_OBJECT_ITEM_STD,
		ObjectBreakable		= CLSID_OBJECT_BREAKABLE,
		ObjectClimable		= CLSID_OBJECT_CLIMABLE,
		ObjectRagdoll		= CLSID_PH_SKELETON_OBJECT,
		ObjectPhysics		= CLSID_OBJECT_PHYSIC,
		ObjectPhysicsDestroyable = CLSID_PHYSICS_DESTROYABLE,
		ObjectInventoryBox	= CLSID_INVENTORY_BOX,

		// zones
		Zone				= CLSID_ZONE,
		ZoneMosquitoBald	= CLSID_Z_MBALD,
		ZoneMincer			= CLSID_Z_MINCER,
		ZoneAcidFog			= CLSID_Z_ACIDF,
		ZoneGalantine		= CLSID_Z_GALANT,
		ZoneRadio			= CLSID_Z_RADIO,
		ZoneBFuzz			= CLSID_Z_BFUZZ,
		ZoneRustyHair		= CLSID_Z_RUSTYH,
		ZoneAmeba			= CLSID_Z_AMEBA,
		ZoneNoGravity		= CLSID_Z_NOGRAVITY,
		ZoneFryUp			= CLSID_Z_FRYUP,
		ZoneDead			= CLSID_Z_DEAD,
		ZoneLevelChanger	= CLSID_LEVEL_CHANGER,
		ZoneScriptZone		= CLSID_SCRIPT_ZONE,
		ZoneTorrid			= CLSID_Z_TORRID,
		ZoneSpaceRestrictor = CLSID_SPACE_RESTRICTOR,
		ZoneSmart			= CLSID_SMART_ZONE,
		ZoneCampfire		= CLSID_Z_CAMPFIRE,


		// detectors
		DetectorSimple		= CLSID_DETECTOR_SIMPLE,
		DetectorAdvanced	= CLSID_DETECTOR_ADVANCED,
		DetectorElite		= CLSID_DETECTOR_ELITE,
		DetectorScientific	= CLSID_DETECTOR_SCIENTIFIC,

		// devices
		DeviceDosimeter		= CLSID_DEVICE_DOSIMETER,
		DevicePDA			= CLSID_DEVICE_PDA,
		DeviceTorch			= CLSID_DEVICE_TORCH,
		DeviceArtefactMerger = CLSID_DEVICE_AF_MERGER,
		DeviceFlare			= CLSID_DEVICE_FLARE,

		// inventory items
		InventoryItemBolt		= CLSID_IITEM_BOLT,
		InventoryItemFood		= CLSID_IITEM_FOOD,
		InventoryItemExplosive	= CLSID_IITEM_EXPLOSIVE,
		InventoryItemDocument	= CLSID_IITEM_DOCUMENT,
		InventoryItemAttachable = CLSID_IITEM_ATTACH,

		// grenades
        GrenadeF1 = CLSID_GRENADE_F1,
        GrenadeRGD5 = CLSID_GRENADE_RGD5,
		GrenadeRPG7 = CLSID_OBJECT_G_RPG7,
		GrenadeFake = CLSID_OBJECT_G_FAKE,

		// Equipment
		EquipmentSimple		= CLSID_EQUIPMENT_SIMPLE,
		EquipmentStalker	= CLSID_EQUIPMENT_STALKER,
		EquipmentHelmet		= CLSID_EQUIPMENT_HELMET,

		ServerGameSingle	= CLSID_SV_GAME_SINGLE,
		ClientGameSingle	= CLSID_CL_GAME_SINGLE,
		UISingle			= CLSID_GAME_UI_SINGLE
	};
}