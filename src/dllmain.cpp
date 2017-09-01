/*************************************************************************
    Opcodes Restoration Project

    Plugin for CLEO v2.0.0.5 and above for Grand Theft Auto III and Grand
    Theft Auto: Vice City that aims to restore functionality to many
    unsupported opcodes
    http://www.gtamodding.com/wiki/Opcodes_Restoration_Project
*************************************************************************/

#include <Windows.h>
#include <cmath>

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  5

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

#if _VC
#include "VC.CLEO.h"
#include "CCranesHack.h"
#include "CExplosionHack.h"
#include "CMessagesHack.h"
#include "CPacManPickupsHack.h"
#include "CPedHack.h"
#include "CPhoneInfoHack.h"
#include "CProjectileInfoHack.h"
#include "CWeatherHack.h"
#include "Globals.h"
#include "SilentCall.h"
#include "vcclasses.h"

using ::VCGlobals::ScriptParams;
using ::VCGlobals::ScriptParameter;
using ::VCGlobals::FindPlayerPed;
using ::VCGlobals::TheText;

void(__cdecl *CallInit)(void);
void(__cdecl *CallUpdate)(void);
void(__cdecl *CallRender)(void);

static auto WindowHandler = (void(__cdecl *)(int, int))vcversion::AdjustOffset(0x00602EE0);
static auto phoneDisplayMessage = (BYTE *)vcversion::AdjustOffset(0x007030E4);
static auto currentPhone = (CPhoneInfo::CPhone **)vcversion::AdjustOffset(0x007030E8);

static short CardStack[6 * 52];
static short CardStackPosition;
#elif _III
#include "III.CLEO.h"
#include "CMessagesHack.h"
#include "Globals.h"
#include "iiiclasses.h"

using ::IIIGlobals::ScriptParams;
using ::IIIGlobals::ScriptParameter;
using ::IIIGlobals::FindPlayerPed;
using ::IIIGlobals::TheText;
#endif

#if _VC
/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(ped && ped->state != 0x37 && ped->state != 0x36);
	return OR_CONTINUE;
}

/* 00AC */
eOpcodeResult WINAPI IS_CAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag((vehicle && (vehicle->status >> 3) != 5) && (vehicle->type == 1 || !(vehicle->field_11A & 0x10)));
	return OR_CONTINUE;
}

/* 00BD */
eOpcodeResult WINAPI PRINT_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(2);
	CMessagesHack::AddMessageSoon(TheText.Get(gxt), ScriptParams[0].uint32, ScriptParams[1].uint16);
	return OR_CONTINUE;
}

/* 00C5 */
eOpcodeResult WINAPI RETURN_TRUE(CScript *script)
{
	script->UpdateCompareFlag(true);
	return OR_CONTINUE;
}

/* 00C6 */
eOpcodeResult WINAPI RETURN_FALSE(CScript *script)
{
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 00E2 */
eOpcodeResult WINAPI GET_PAD_STATE(CScript *script)
{
	script->Collect(2);
	ScriptParams[0].int32 = static_cast<int>(reinterpret_cast<CRunningScript *>(script)->GetPadState(ScriptParams[0].uint16, ScriptParams[1].uint16));
	script->Store(1);
	return OR_CONTINUE;
}

/* 0113 */
eOpcodeResult WINAPI ADD_AMMO_TO_PLAYER(CScript *script)
{
	script->Collect(3);
	CWorld::Players[ScriptParams[0].int32].playerEntity->GrantAmmo(ScriptParams[1].int32, ScriptParams[2].uint32);
	return OR_CONTINUE;
}
#endif

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[ScriptParams[0].int32].deathArrestState != 1);
	return OR_CONTINUE;
}

#if _VC
/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[ScriptParams[0].int32].deathArrestState == 2);
	return OR_CONTINUE;
}

/* 0135 */
eOpcodeResult WINAPI CHANGE_CAR_LOCK(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->lock = ScriptParams[1].uint32;
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	VCGlobals::TheCamera.CamShake(static_cast<float>(ScriptParams[0].int32) * 1e-3f, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32);
	return OR_CONTINUE;
}

/* 013B */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_2D(CScript *script)
{
	script->Collect(6);
	if (ScriptParams[5].int32) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32, -100.0);
	}
	if (CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32)) {
		if ((vehicle->status >> 3) == 5 && vehicle->placeable.IsWithinArea(ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 013C */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_3D(CScript *script)
{
	script->Collect(8);
	if (ScriptParams[7].int32) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[4].float32, ScriptParams[5].float32, (ScriptParams[3].float32 + ScriptParams[6].float32) * 0.5f);
	}
	if (CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32)) {
		if ((vehicle->status >> 3) == 5 && vehicle->placeable.IsWithinArea(ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32, ScriptParams[5].float32, ScriptParams[6].float32)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0155 */
eOpcodeResult WINAPI SET_CAR_DENSITY(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(2);
	for (int i = 0; i < CTheZones::TotalNumberOfInfoZones; i++) {
		if (!strncmp(gxt, CTheZones::InfoZoneArray[i].name, 8)) {
			int offset = ScriptParams[0].int32 ? CTheZones::InfoZoneArray[i].night : CTheZones::InfoZoneArray[i].day;
			CTheZones::ZoneInfoArray[offset].carDensity = ScriptParams[1].uint16;
		}
	}
	return OR_CONTINUE;
}

/* 0156 */
eOpcodeResult WINAPI SET_PED_DENSITY(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(2);
	for (int i = 0; i < CTheZones::TotalNumberOfInfoZones; i++) {
		if (!strncmp(gxt, CTheZones::InfoZoneArray[i].name, 8)) {
			int offset = ScriptParams[0].int32 ? CTheZones::InfoZoneArray[i].night : CTheZones::InfoZoneArray[i].day;
			CTheZones::ZoneInfoArray[offset].pedDensity = ScriptParams[1].uint16;
		}
	}
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	CAutomobile *vehicle = static_cast<CAutomobile *>(CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32));
	script->UpdateCompareFlag(vehicle->numberOfWheelsOnGround == 0);
	return OR_CONTINUE;
}

/* 0163 */
eOpcodeResult WINAPI ADD_BLIP_FOR_OBJECT_OLD(CScript *script)
{
	script->Collect(3);
	CPools::ms_pObjectPool->GetAt(ScriptParams[0].int32);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(3, ScriptParams[0].int32, ScriptParams[1].uint32, ScriptParams[2].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0178 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[ScriptParams[0].int32].playerEntity;
	CObject *object = CPools::ms_pObjectPool->GetAt(ScriptParams[1].int32);
	CPhysical *source = player;
	if (player->isInAnyVehicle && player->vehicle) {
		source = player->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 0179 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	CObject *object = CPools::ms_pObjectPool->GetAt(ScriptParams[1].int32);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle && ped->vehicle) {
		source = ped->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 017B */
eOpcodeResult WINAPI SET_CHAR_AMMO(CScript *script)
{
	script->Collect(3);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->SetAmmo(ScriptParams[1].int32, ScriptParams[2].uint32);
	return OR_CONTINUE;
}

/* 01C6 */
eOpcodeResult WINAPI DONT_REMOVE_CAR(CScript *script)
{
	script->Collect(1);
	CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	CTheScripts::MissionCleanUp.RemoveEntityFromList(ScriptParams[0].int32, 1);
	return OR_CONTINUE;
}

/* 01E6 */
eOpcodeResult WINAPI PRINT_WITH_NUMBER_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(3);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[1].uint32, ScriptParams[2].uint16, ScriptParams[0].int32, -1, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 01EE */
eOpcodeResult WINAPI ACTIVATE_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (ScriptParams[2].float32 > ScriptParams[4].float32) {
		temp = ScriptParams[4].float32;
		ScriptParams[4].float32 = ScriptParams[2].float32;
		ScriptParams[2].float32 = temp;
	}
	if (ScriptParams[3].float32 > ScriptParams[5].float32) {
		temp = ScriptParams[5].float32;
		ScriptParams[5].float32 = ScriptParams[3].float32;
		ScriptParams[3].float32 = temp;
	}
	ScriptParams[9].float32 = ScriptParams[9].float32 * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(ScriptParams[2].float32, ScriptParams[4].float32, ScriptParams[3].float32, ScriptParams[5].float32, ScriptParams[6].float32, ScriptParams[7].float32, ScriptParams[8].float32, ScriptParams[9].float32, false, false, ScriptParams[0].float32, ScriptParams[1].float32);
	return OR_CONTINUE;
}

/* 01EF */
eOpcodeResult WINAPI DEACTIVATE_CRANE(CScript *script)
{
	script->Collect(2);
	CCranesHack::DeActivateCrane(ScriptParams[0].float32, ScriptParams[1].float32);
	return OR_CONTINUE;
}

/* 0218 */
eOpcodeResult WINAPI PRINT_WITH_NUMBER_BIG_Q(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(3);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, -1, -1, -1, -1, -1, VCGlobals::gUString);
	CMessages::AddBigMessageQ(VCGlobals::gUString, ScriptParams[1].uint32, static_cast<unsigned short>(ScriptParams[2].int32 - 1));
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	CGarages::BombsAreFree = !!ScriptParams[0].int32;
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	CAutomobile::m_sAllTaxiLights = !!ScriptParams[0].int32;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(!!(vehicle->bombState & 7));
	return OR_CONTINUE;
}

/* 0228 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag((vehicle->bombState & 7) == ScriptParams[1].int32);
	return OR_CONTINUE;
}

/* 016F */
eOpcodeResult WINAPI DRAW_SHADOW(CScript *script)
{
	script->Collect(10);
	unsigned char type = 0;
	RwTexture *texture = 0;
	CVector pos = { ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32 };
	switch (ScriptParams[0].int32) {
	case 1:
		type = 1;
		texture = VCGlobals::gpShadowCarTex;
		break;
	case 2:
		type = 1;
		texture = VCGlobals::gpShadowPedTex;
		break;
	case 3:
		type = 2;
		texture = VCGlobals::gpShadowExplosionTex;
		break;
	case 4:
		type = 1;
		texture = VCGlobals::gpShadowHeliTex;
		break;
	case 5:
		type = 2;
		texture = VCGlobals::gpShadowHeadLightsTex;
		break;
	case 6:
		type = 1;
		texture = VCGlobals::gpBloodPoolTex;
		break;
	default:
		return OR_CONTINUE;
	}
	CShadows::StoreShadowToBeRendered(type, texture, &pos,
		-sin(ScriptParams[4].float32) * ScriptParams[5].float32, cos(ScriptParams[4].float32) * ScriptParams[5].float32, cos(ScriptParams[4].float32) * ScriptParams[5].float32, sin(ScriptParams[4].float32) * ScriptParams[5].float32,
		ScriptParams[6].int16, ScriptParams[7].uint8, ScriptParams[8].uint8, ScriptParams[9].uint8, 15.0, false, 1.0, 0, false);
	return OR_CONTINUE;
}

/* 023A */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[ScriptParams[0].int32].playerEntity;
	CObject *object = CPools::ms_pObjectPool->GetAt(ScriptParams[1].int32);
	script->UpdateCompareFlag(!player->isInAnyVehicle && player->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 023B */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	CObject *object = CPools::ms_pObjectPool->GetAt(ScriptParams[1].int32);
	script->UpdateCompareFlag(!ped->isInAnyVehicle && ped->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 023E */
eOpcodeResult WINAPI FLASH_CAR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->field_052 &= 0xDF;
	if (ScriptParams[1].int32) {
		vehicle->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 023F */
eOpcodeResult WINAPI FLASH_CHAR(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->field_052 &= 0xDF;
	if (ScriptParams[1].int32) {
		ped->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 0240 */
eOpcodeResult WINAPI FLASH_OBJECT(CScript *script)
{
	script->Collect(2);
	CObject *object = CPools::ms_pObjectPool->GetAt(ScriptParams[0].int32);
	object->field_052 &= 0xDF;
	if (ScriptParams[1].int32) {
		object->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 0242 */
eOpcodeResult WINAPI ARM_CAR_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->bombState = (vehicle->bombState & 0xF8) | (static_cast<unsigned char>(ScriptParams[1].int32) & 7);
	vehicle->bombOwner = FindPlayerPed();
	return OR_CONTINUE;
}

/* 024B */
eOpcodeResult WINAPI SET_REPEATED_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, TheText.Get(gxt), 0, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 024C */
eOpcodeResult WINAPI SET_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, TheText.Get(gxt), 0, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 024D */
eOpcodeResult WINAPI HAS_PHONE_DISPLAYED_MESSAGE(CScript *script)
{
	script->Collect(1);
	if (*phoneDisplayMessage == 0) {
		unsigned int state = VCGlobals::gPhoneInfo.phones[ScriptParams[0].int32].state;
		if (state == 6 || state == 7 || state == 8) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0250 */
eOpcodeResult WINAPI DRAW_LIGHT(CScript *script)
{
	script->Collect(6);
	CVector pos1 = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	CVector pos2 = { 0, 0, 0 };
	CPointLights::AddLight(0, pos1, pos2, 12.0, ScriptParams[3].int32 / 255.0f, ScriptParams[4].int32 / 255.0f, ScriptParams[5].int32 / 255.0f, 0, true);
	return OR_CONTINUE;
}

/* 0251 */
eOpcodeResult WINAPI STORE_WEATHER(CScript *)
{
	CWeatherHack::Stored_InterpolationValue = CWeather::InterpolationValue;
	CWeatherHack::Stored_OldWeatherType = CWeather::OldWeatherType;
	CWeatherHack::Stored_NewWeatherType = CWeather::NewWeatherType;
	CWeatherHack::Stored_Rain = CWeather::Rain;
	return OR_CONTINUE;
}

/* 0252 */
eOpcodeResult WINAPI RESTORE_WEATHER(CScript *)
{
	CWeather::InterpolationValue = CWeatherHack::Stored_InterpolationValue;
	CWeather::OldWeatherType = CWeatherHack::Stored_OldWeatherType;
	CWeather::NewWeatherType = CWeatherHack::Stored_NewWeatherType;
	CWeather::Rain = CWeatherHack::Stored_Rain;
	return OR_CONTINUE;
}

/* 0255 */
eOpcodeResult WINAPI RESTART_CRITICAL_MISSION(CScript *script)
{
	script->Collect(4);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRestart::OverrideNextRestart(pos, ScriptParams[3].float32);
	CPlayerInfo *player = &CWorld::Players[CWorld::PlayerInFocus];
	if (player->deathArrestState == 0) {
		player->deathArrestState = 3;
		player->timeDeathArrest = CTimer::m_snTimeInMilliseconds;
		CDarkel::ResetOnPlayerDeath();
	}
	return OR_CONTINUE;
}

/* 0295 */
eOpcodeResult WINAPI IS_TAXI(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	short model = vehicle->modelIndex;
	script->UpdateCompareFlag(model == 0x96 || model == 0xA8 || model == 0xBC || model == 0xD8);
	return OR_CONTINUE;
}

/* 0299 */
eOpcodeResult WINAPI ACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	CGarage *garage = &CGarages::aGarages[ScriptParams[0].int32];
	if (garage->type == 11 && garage->state == 0) {
		garage->state = 3;
	}
	garage->isInactive = 0;
	return OR_CONTINUE;
}

/* 029A */
eOpcodeResult WINAPI SWITCH_TAXI_TIMER(CScript *script)
{
	script->Collect(1);
	if (ScriptParams[0].int32) {
		CWorld::Players[CWorld::PlayerInFocus].timeTaxiTimer = CTimer::m_snTimeInMilliseconds;
		CWorld::Players[CWorld::PlayerInFocus].isTaxiTimerOn = true;
	} else {
		CWorld::Players[CWorld::PlayerInFocus].isTaxiTimerOn = false;
	}
	return OR_CONTINUE;
}

/* 029C */
eOpcodeResult WINAPI IS_BOAT(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(vehicle->type == 1);
	return OR_CONTINUE;
}

/* 02A0 */
eOpcodeResult WINAPI IS_CHAR_STOPPED(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	if (ped->isInAnyVehicle) {
		CVehicle *vehicle = ped->vehicle;
		if (vehicle && vehicle->speed < CTimer::ms_fTimeStep * 0.01) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	} else {
		if (ped->moveState <= 1) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 02A1 */
eOpcodeResult WINAPI MESSAGE_WAIT(CScript *script)
{
	script->Collect(2);
	script->m_dwWakeTime = ScriptParams[0].int32 + CTimer::m_snTimeInMilliseconds;
	if (ScriptParams[1].int32) {
		script->m_bAwake = 1;
	}
	return OR_TERMINATE;
}

/* 02A2 */
eOpcodeResult WINAPI ADD_PARTICLE_EFFECT(CScript *script)
{
	script->Collect(5);
	CVector pos = { ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CParticleObject::AddObject(ScriptParams[0].uint16, pos, !!ScriptParams[4].int32);
	return OR_CONTINUE;
}

/* 02A4 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CAR(CScript *script)
{
	script->Collect(2);
	CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(1, ScriptParams[0].int32, 0, 3);
	CRadar::SetBlipSprite(ScriptParams[0].int32, ScriptParams[1].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A5 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CHAR(CScript *script)
{
	script->Collect(2);
	CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(2, ScriptParams[0].int32, 1, 3);
	CRadar::SetBlipSprite(ScriptParams[0].int32, ScriptParams[1].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A6 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_OBJECT(CScript *script)
{
	script->Collect(2);
	CPools::ms_pObjectPool->GetAt(ScriptParams[0].int32);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(3, ScriptParams[0].int32, 6, 3);
	CRadar::SetBlipSprite(ScriptParams[0].int32, ScriptParams[1].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02B9 */
eOpcodeResult WINAPI DEACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	CGarages::aGarages[ScriptParams[0].int32].isInactive = 1;
	return OR_CONTINUE;
}

/* 02BC */
eOpcodeResult WINAPI SET_SWAT_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = FindPlayerPed();
	player->wanted->activity &= 0xFB;
	if (ScriptParams[0].int32) {
		player->wanted->activity |= 4;
	}
	return OR_CONTINUE;
}
#endif

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = FindPlayerPed();
	player->wanted->activity &= 0xF7;
	if (ScriptParams[0].int32) {
		player->wanted->activity |= 8;
	}
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = FindPlayerPed();
	player->wanted->activity &= 0xEF;
	if (ScriptParams[0].int32) {
		player->wanted->activity |= 16;
	}
	return OR_CONTINUE;
}

#if _VC
/* 02C3 */
eOpcodeResult WINAPI START_PACMAN_RACE(CScript *script)
{
	script->Collect(1);
	CPacManPickupsHack::GeneratePMPickUpsForRace();
	CPacManPickupsHack::PillsEatenInRace = 0;
	return OR_CONTINUE;
}

/* 02C5 */
eOpcodeResult WINAPI GET_NUMBER_OF_POWER_PILLS_EATEN(CScript *script)
{
	ScriptParams[0].int32 = CPacManPickupsHack::PillsEatenInRace;
	script->Store(1);
	return OR_CONTINUE;
}

/* 02C6 */
eOpcodeResult WINAPI CLEAR_PACMAN(CScript *)
{
	CPacManPickupsHack::CleanUpPacManStuff();
	return OR_CONTINUE;
}

/* 02C7 */
eOpcodeResult WINAPI START_PACMAN_SCRAMBLE(CScript *script)
{
	script->Collect(5);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CPacManPickupsHack::GeneratePMPickUps(pos, ScriptParams[3].float32, ScriptParams[4].int16);
	return OR_CONTINUE;
}

/* 02C8 */
eOpcodeResult WINAPI GET_NUMBER_OF_POWER_PILLS_CARRIED(CScript *script)
{
	CVehicle *vehicle = VCGlobals::FindPlayerVehicle();
	ScriptParams[0].int32 = vehicle ? vehicle->powerPillsCarried : 0;
	script->Store(1);
	return OR_CONTINUE;
}

/* 02C9 */
eOpcodeResult WINAPI CLEAR_NUMBER_OF_POWER_PILLS_CARRIED(CScript *)
{
	if (CVehicle *vehicle = VCGlobals::FindPlayerVehicle()) {
		vehicle->powerPillsCarried = 0;
		vehicle->mass /= vehicle->accelerationResistance;
		vehicle->turnResistance /= vehicle->accelerationResistance;
		vehicle->accelerationResistance = 1.0;
	}
	return OR_CONTINUE;
}

/* 02CD */
eOpcodeResult WINAPI GOSUB_FILE(CScript *script)
{
	script->Collect(2);
	script->m_aGosubAddr[script->m_nCurrentGosub] = script->m_dwIp;
	script->m_nCurrentGosub += 1;
	script->m_dwIp = ScriptParams[0].uint32;
	return OR_CONTINUE;
}

/* 02D2 */
eOpcodeResult WINAPI SET_COMEDY_CONTROLS(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->field_1FA &= 0xEF;
	if (ScriptParams[1].int32) {
		vehicle->field_1FA |= 0x10;
	}
	return OR_CONTINUE;
}

/* 02D6 */
eOpcodeResult WINAPI IS_CHAR_SHOOTING_IN_AREA(CScript *script)
{
	script->Collect(6);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag((ped->field_14E & 0x40) && ped->placeable.IsWithinArea(ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32));
	if (ScriptParams[5].int32) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32, -100.0);
	}
	return OR_CONTINUE;
}

/* 02D9 */
eOpcodeResult WINAPI CLEAR_NUMBER_OF_POWER_PILLS_EATEN(CScript *)
{
	CPacManPickupsHack::PillsEatenInRace = 0;
	return OR_CONTINUE;
}

/* 02DA */
eOpcodeResult WINAPI ADD_POWER_PILL(CScript *script)
{
	script->Collect(3);
	CPacManPickupsHack::bPMActive = true;
	CPacManPickupsHack::aPMPickups[0].state = 2;
	CPacManPickupsHack::aPMPickups[0].position.x = ScriptParams[0].float32;
	CPacManPickupsHack::aPMPickups[0].position.y = ScriptParams[1].float32;
	CPacManPickupsHack::aPMPickups[0].position.z = ScriptParams[2].float32;
	return OR_CONTINUE;
}

/* 02EE */
eOpcodeResult WINAPI IS_PROJECTILE_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (ScriptParams[0].float32 > ScriptParams[3].float32) {
		temp = ScriptParams[3].float32;
		ScriptParams[3].float32 = ScriptParams[0].float32;
		ScriptParams[0].float32 = temp;
	}
	if (ScriptParams[1].float32 > ScriptParams[4].float32) {
		temp = ScriptParams[4].float32;
		ScriptParams[4].float32 = ScriptParams[1].float32;
		ScriptParams[1].float32 = temp;
	}
	if (ScriptParams[2].float32 > ScriptParams[5].float32) {
		temp = ScriptParams[5].float32;
		ScriptParams[5].float32 = ScriptParams[2].float32;
		ScriptParams[2].float32 = temp;
	}
	script->UpdateCompareFlag(CProjectileInfoHack::IsProjectileInRange(ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32, ScriptParams[5].float32, 0));
	return OR_CONTINUE;
}

/* 02EF */
eOpcodeResult WINAPI DESTROY_PROJECTILES_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (ScriptParams[0].float32 > ScriptParams[3].float32) {
		temp = ScriptParams[3].float32;
		ScriptParams[3].float32 = ScriptParams[0].float32;
		ScriptParams[0].float32 = temp;
	}
	if (ScriptParams[1].float32 > ScriptParams[4].float32) {
		temp = ScriptParams[4].float32;
		ScriptParams[4].float32 = ScriptParams[1].float32;
		ScriptParams[1].float32 = temp;
	}
	if (ScriptParams[2].float32 > ScriptParams[5].float32) {
		temp = ScriptParams[5].float32;
		ScriptParams[5].float32 = ScriptParams[2].float32;
		ScriptParams[2].float32 = temp;
	}
	script->UpdateCompareFlag(CProjectileInfoHack::IsProjectileInRange(ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32, ScriptParams[3].float32, ScriptParams[4].float32, ScriptParams[5].float32, 1));
	return OR_CONTINUE;
}

/* 02F0 */
eOpcodeResult WINAPI DROP_MINE(CScript *script)
{
	script->Collect(3);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y) + 0.5f;
	}
	CPickups::GenerateNewOne(pos, VCGlobals::MI_CARMINE, 9, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02F1 */
eOpcodeResult WINAPI DROP_NAUTICAL_MINE(CScript *script)
{
	script->Collect(3);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y) + 0.5f;
	}
	CPickups::GenerateNewOne(pos, VCGlobals::MI_NAUTICALMINE, 11, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02FB */
eOpcodeResult WINAPI ACTIVATE_CRUSHER_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (ScriptParams[2].float32 > ScriptParams[4].float32) {
		temp = ScriptParams[4].float32;
		ScriptParams[4].float32 = ScriptParams[2].float32;
		ScriptParams[2].float32 = temp;
	}
	if (ScriptParams[3].float32 > ScriptParams[5].float32) {
		temp = ScriptParams[5].float32;
		ScriptParams[5].float32 = ScriptParams[3].float32;
		ScriptParams[3].float32 = temp;
	}
	ScriptParams[9].float32 = ScriptParams[9].float32 * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(ScriptParams[2].float32, ScriptParams[4].float32, ScriptParams[3].float32, ScriptParams[5].float32, ScriptParams[6].float32, ScriptParams[7].float32, ScriptParams[8].float32, ScriptParams[9].float32, true, false, ScriptParams[0].float32, ScriptParams[1].float32);
	return OR_CONTINUE;
}

/* 02FC */
eOpcodeResult WINAPI PRINT_WITH_2_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(4);
	CMessages::AddMessageWithNumber(TheText.Get(gxt), ScriptParams[2].uint32, ScriptParams[3].uint16, ScriptParams[0].int32, ScriptParams[1].int32, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 02FE */
eOpcodeResult WINAPI PRINT_WITH_2_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(4);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[2].uint32, ScriptParams[3].uint16, ScriptParams[0].int32, ScriptParams[1].int32, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0300 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::AddMessageJumpQWithNumber(TheText.Get(gxt), ScriptParams[3].uint32, ScriptParams[4].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0301 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[3].uint32, ScriptParams[4].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0303 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::AddMessageJumpQWithNumber(TheText.Get(gxt), ScriptParams[4].uint32, ScriptParams[5].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, -1, -1);
	return OR_CONTINUE;
}

/* 0304 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[4].uint32, ScriptParams[5].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, -1, -1);
	return OR_CONTINUE;
}

/* 0305 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageWithNumber(TheText.Get(gxt), ScriptParams[5].uint32, ScriptParams[6].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, -1);
	return OR_CONTINUE;
}

/* 0306 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageJumpQWithNumber(TheText.Get(gxt), ScriptParams[5].uint32, ScriptParams[6].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, -1);
	return OR_CONTINUE;
}

/* 0307 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[5].uint32, ScriptParams[6].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, -1);
	return OR_CONTINUE;
}

/* 0309 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::AddMessageJumpQWithNumber(TheText.Get(gxt), ScriptParams[6].uint32, ScriptParams[7].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, ScriptParams[5].int32);
	return OR_CONTINUE;
}

/* 030A */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessagesHack::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[6].uint32, ScriptParams[7].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, ScriptParams[5].int32);
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(vehicle->firstColour == ScriptParams[1].uint8);
	return OR_CONTINUE;
}

/* 031C */
eOpcodeResult WINAPI IS_SECOND_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(vehicle->secondColour == ScriptParams[1].uint8);
	return OR_CONTINUE;
}

/* 0338 */
eOpcodeResult WINAPI SET_CAR_VISIBLE(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->field_052 &= 0xFB;
	if (ScriptParams[1].int32) {
		vehicle->field_052 |= 4;
	}
	return OR_CONTINUE;
}

/* 032D */
eOpcodeResult WINAPI SET_CAR_BLOCK_CAR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	CVehicle *target = CPools::ms_pVehiclePool->GetAt(ScriptParams[1].int32);
	vehicle->targetEntity = target;
	target->RegisterReference(&vehicle->targetEntity);
	vehicle->targetBehavior = 0x11;
	vehicle->field_1F9 &= 0xEF;
	vehicle->field_1F9 |= 0x10;
	if (vehicle->cruiseSpeed < 6) {
		vehicle->cruiseSpeed = 6;
	}
	return OR_CONTINUE;
}

/* 0344 */
eOpcodeResult WINAPI SET_TEXT_CENTRE_SIZE(CScript *script)
{
	script->Collect(1);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].centreWidth = ScriptParams[0].float32;
	return OR_CONTINUE;
}

/* 0346 */
eOpcodeResult WINAPI SET_TEXT_BACKGROUND_COLOUR(CScript *script)
{
	script->Collect(4);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorR = ScriptParams[0].uint8;
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorG = ScriptParams[1].uint8;
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorB = ScriptParams[2].uint8;
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorA = ScriptParams[3].uint8;
	return OR_CONTINUE;
}

/* 0349 */
eOpcodeResult WINAPI SET_TEXT_FONT(CScript *script)
{
	script->Collect(1);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].font = ScriptParams[0].int16;
	return OR_CONTINUE;
}

/* 0351 */
eOpcodeResult WINAPI IS_NASTY_GAME(CScript *script)
{
	script->UpdateCompareFlag(CGame::nastyGame);
	return OR_CONTINUE;
}

/* 0356 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_AREA(CScript *script)
{
	script->Collect(7);
	script->UpdateCompareFlag(CExplosionHack::TestForExplosionInArea(ScriptParams[0].int32, ScriptParams[1].float32, ScriptParams[4].float32, ScriptParams[2].float32, ScriptParams[5].float32, ScriptParams[3].float32, ScriptParams[6].float32));
	return OR_CONTINUE;
}

/* 0357 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_ZONE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	for (int i = 0; i < CTheZones::TotalNumberOfInfoZones; i++) {
		if (!strncmp(gxt, CTheZones::InfoZoneArray[i].name, 8)) {
			if (CExplosionHack::TestForExplosionInArea(ScriptParams[0].int32,
				CTheZones::InfoZoneArray[i].corner1.x, CTheZones::InfoZoneArray[i].corner2.x,
				CTheZones::InfoZoneArray[i].corner1.y, CTheZones::InfoZoneArray[i].corner2.y,
				CTheZones::InfoZoneArray[i].corner1.z, CTheZones::InfoZoneArray[i].corner2.z)) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	for (int i = 0; i < CTheZones::TotalNumberOfNavigationZones; i++) {
		if (!strncmp(gxt, CTheZones::NavigationZoneArray[i].name, 8)) {
			if (CExplosionHack::TestForExplosionInArea(ScriptParams[0].int32,
				CTheZones::NavigationZoneArray[i].corner1.x, CTheZones::NavigationZoneArray[i].corner2.x,
				CTheZones::NavigationZoneArray[i].corner1.y, CTheZones::NavigationZoneArray[i].corner2.y,
				CTheZones::NavigationZoneArray[i].corner1.z, CTheZones::NavigationZoneArray[i].corner2.z)) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0367 */
eOpcodeResult WINAPI START_KILL_FRENZY_HEADSHOT(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CDarkel::StartFrenzy(ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].uint16, ScriptParams[3].int32, TheText.Get(gxt), ScriptParams[4].int32, ScriptParams[5].int32, ScriptParams[6].int32, !!ScriptParams[7].int32, true);
	return OR_CONTINUE;
}

/* 0368 */
eOpcodeResult WINAPI ACTIVATE_MILITARY_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (ScriptParams[2].float32 > ScriptParams[4].float32) {
		temp = ScriptParams[4].float32;
		ScriptParams[4].float32 = ScriptParams[2].float32;
		ScriptParams[2].float32 = temp;
	}
	if (ScriptParams[3].float32 > ScriptParams[5].float32) {
		temp = ScriptParams[5].float32;
		ScriptParams[5].float32 = ScriptParams[3].float32;
		ScriptParams[3].float32 = temp;
	}
	ScriptParams[9].float32 = ScriptParams[9].float32 * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(ScriptParams[2].float32, ScriptParams[4].float32, ScriptParams[3].float32, ScriptParams[5].float32, ScriptParams[6].float32, ScriptParams[7].float32, ScriptParams[8].float32, ScriptParams[9].float32, false, true, ScriptParams[0].float32, ScriptParams[1].float32);
	return OR_CONTINUE;
}

/* 036E */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, -1, -1, -1, VCGlobals::gUString);
	CMessages::AddBigMessage(VCGlobals::gUString, ScriptParams[3].uint32, static_cast<unsigned short>(ScriptParams[4].int32 - 1));
	return OR_CONTINUE;
}

/* 036F */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, -1, -1, VCGlobals::gUString);
	CMessages::AddBigMessage(VCGlobals::gUString, ScriptParams[4].uint32, static_cast<unsigned short>(ScriptParams[5].int32 - 1));
	return OR_CONTINUE;
}

/* 0370 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, -1, VCGlobals::gUString);
	CMessages::AddBigMessage(VCGlobals::gUString, ScriptParams[5].uint32, static_cast<unsigned short>(ScriptParams[6].int32 - 1));
	return OR_CONTINUE;
}

/* 0371 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, ScriptParams[5].int32, VCGlobals::gUString);
	CMessages::AddBigMessage(VCGlobals::gUString, ScriptParams[6].uint32, static_cast<unsigned short>(ScriptParams[7].int32 - 1));
	return OR_CONTINUE;
}

/* 0375 */
eOpcodeResult WINAPI PRINT_STRING_IN_STRING(CScript *script)
{
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->Collect(2);
	CMessagesHack::AddMessageWithString(string[0], ScriptParams[0].uint32, ScriptParams[1].uint16, string[1]);
	return OR_CONTINUE;
}

/* 0378 */
eOpcodeResult WINAPI SET_2_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, string[0], string[1], 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 0379 */
eOpcodeResult WINAPI SET_2_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, string[0], string[1], 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 037A */
eOpcodeResult WINAPI SET_3_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[3];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, string[0], string[1], string[2], 0, 0, 0);
	return OR_CONTINUE;
}

/* 037B */
eOpcodeResult WINAPI SET_3_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[3];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, string[0], string[1], string[2], 0, 0, 0);
	return OR_CONTINUE;
}

/* 037C */
eOpcodeResult WINAPI SET_4_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[4];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, string[0], string[1], string[2], string[3], 0, 0);
	return OR_CONTINUE;
}

/* 037D */
eOpcodeResult WINAPI SET_4_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[4];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, string[0], string[1], string[2], string[3], 0, 0);
	return OR_CONTINUE;
}

/* 037F */
eOpcodeResult WINAPI GIVE_PLAYER_DETONATOR(CScript *)
{
	CStreaming::RequestModel(291, 1);
	CStreaming::LoadAllRequestedModels(false);
	FindPlayerPed()->GiveWeapon(34, 1, true);
	int slot = FindPlayerPed()->GetWeaponSlot(34);
	FindPlayerPed()->weapons[slot].state = 0;
	return OR_CONTINUE;
}
#endif

/* 0385 */
eOpcodeResult WINAPI PRINT_STRING_IN_STRING_SOON(CScript *script)
{
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->Collect(2);
	CMessagesHack::AddMessageSoonWithString(string[0], ScriptParams[0].uint32, ScriptParams[1].uint16, string[1]);
	return OR_CONTINUE;
}

#if _VC
/* 0386 */
eOpcodeResult WINAPI SET_5_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[5];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = TheText.Get(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, string[0], string[1], string[2], string[3], string[4], 0);
	return OR_CONTINUE;
}

/* 0387 */
eOpcodeResult WINAPI SET_5_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[5];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = TheText.Get(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, string[0], string[1], string[2], string[3], string[4], 0);
	return OR_CONTINUE;
}

/* 0388 */
eOpcodeResult WINAPI SET_6_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[6];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[5] = TheText.Get(gxt);
	static_cast<CPhoneInfoHack *>(&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(ScriptParams[0].int32, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0389 */
eOpcodeResult WINAPI SET_6_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[6];
	script->ReadShortString(gxt);
	string[0] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[5] = TheText.Get(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(ScriptParams[0].int32, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0393 */
eOpcodeResult WINAPI SET_CHAR_ANIM_SPEED(CScript *script)
{
	script->Collect(2);
	unsigned long var = RpAnimBlendClumpGetFirstAssociation(CPools::ms_pPedPool->GetAt(ScriptParams[0].int32)->RpClump);
	if (var) {
		*(float *)(var + 0x24) = ScriptParams[1].float32;
	}
	return OR_CONTINUE;
}

/* 03A0 */
eOpcodeResult WINAPI IS_CRANE_LIFTING_CAR(CScript *script)
{
	script->Collect(3);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[2].int32);
	script->UpdateCompareFlag(CCranesHack::IsThisCarPickedUp(ScriptParams[0].float32, ScriptParams[1].float32, vehicle));
	return OR_CONTINUE;
}

/* 03A5 */
eOpcodeResult WINAPI CHANGE_GARAGE_TYPE_WITH_CAR_MODEL(CScript *script)
{
	script->Collect(3);
	CGarages::ChangeGarageType(ScriptParams[0].int16, ScriptParams[1].uint8, ScriptParams[2].uint32);
	return OR_CONTINUE;
}

/* 03C2 */
eOpcodeResult WINAPI IS_PHONE_DISPLAYING_MESSAGE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(&VCGlobals::gPhoneInfo.phones[ScriptParams[0].int32] == *currentPhone);
	return OR_CONTINUE;
}

/* 03C6 */
eOpcodeResult WINAPI IS_COLLISION_IN_MEMORY(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(ScriptParams[0].int32 == CGame::currLevel);
	return OR_CONTINUE;
}

/* 03C9 */
eOpcodeResult WINAPI IS_CAR_VISIBLY_DAMAGED(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(!!(vehicle->field_1FB & 2));
	return OR_CONTINUE;
}

/* 03DB */
eOpcodeResult WINAPI ADD_BLIP_FOR_PICKUP_OLD(CScript *script)
{
	script->Collect(3);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(3, CPools::ms_pObjectPool->GetIndex(CPickups::pickups[CPickups::GetActualPickupIndex(ScriptParams[0].int32)].object), ScriptParams[1].uint32, ScriptParams[2].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03DD */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_PICKUP(CScript *script)
{
	script->Collect(2);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetEntityBlip(3, CPools::ms_pObjectPool->GetIndex(CPickups::pickups[CPickups::GetActualPickupIndex(ScriptParams[0].int32)].object), 6, 3);
	CRadar::SetBlipSprite(ScriptParams[0].int32, ScriptParams[1].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03E9 */
eOpcodeResult WINAPI IS_CHAR_IN_CONTROL(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(ped->IsPedInControl());
	return OR_CONTINUE;
}

/* 03EC */
eOpcodeResult WINAPI HAS_MILITARY_CRANE_COLLECTED_ALL_CARS(CScript *script)
{
	script->UpdateCompareFlag((CCranes::CarsCollectedMilitaryCrane & 0x7F) == 0x7F);
	return OR_CONTINUE;
}

/* 03FB */
eOpcodeResult WINAPI SET_CAR_STAYS_IN_CURRENT_LEVEL(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->originLevel = ScriptParams[1].int32 ? (char)CTheZones::GetLevelFromPosition(&vehicle->GetPos()) : 0;
	return OR_CONTINUE;
}

/* 03FC */
eOpcodeResult WINAPI SET_CHAR_STAYS_IN_CURRENT_LEVEL(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->originLevel = ScriptParams[1].int32 ? (char)CTheZones::GetLevelFromPosition(&ped->GetPos()) : 0;
	return OR_CONTINUE;
}

/* 0410 */
eOpcodeResult WINAPI SET_GANG_PED_MODEL_PREFERENCE(CScript *script)
{
	script->Collect(2);
	CGangs::Gang[ScriptParams[0].int32].pedModelPreference = ScriptParams[1].int8;
	return OR_CONTINUE;
}

/* 0413 */
eOpcodeResult WINAPI SET_GET_OUT_OF_JAIL_FREE(CScript *script)
{
	script->Collect(2);
	CWorld::Players[ScriptParams[0].int32].getOutOfJailFree = !!ScriptParams[1].int32;
	return OR_CONTINUE;
}

/* 0415 */
eOpcodeResult WINAPI IS_CAR_DOOR_CLOSED(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	auto IsDoorMissing = (bool(__thiscall *)(CVehicle *, int))*(unsigned long *)(vehicle->vtbl + 0x6C);
	auto IsDoorClosed = (bool(__thiscall *)(CVehicle *, int))*(unsigned long *)(vehicle->vtbl + 0x68);
	script->UpdateCompareFlag(!IsDoorMissing(vehicle, ScriptParams[1].int32) && IsDoorClosed(vehicle, ScriptParams[1].int32));
	return OR_CONTINUE;
}

/* 041A */
eOpcodeResult WINAPI GET_AMMO_IN_CHAR_WEAPON(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ScriptParams[0].int32 = 0;
	for (int i = 0; i < 10; i++) {
		if (ped->weapons[i].type == ScriptParams[1].int32) {
			ScriptParams[0].int32 = ped->weapons[i].ammo;
			break;
		}
	}
	script->Store(1);
	return OR_CONTINUE;
}

/* 041B */
eOpcodeResult WINAPI REGISTER_KILL_FRENZY_PASSED(CScript *)
{
	CStats::AnotherKillFrenzyPassed();
	return OR_CONTINUE;
}

/* 041C */
eOpcodeResult WINAPI SET_CHAR_SAY(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->Say(ScriptParams[1].uint16);
	return OR_CONTINUE;
}

/* 041F */
eOpcodeResult WINAPI OVERRIDE_HOSPITAL_LEVEL(CScript *script)
{
	script->Collect(1);
	CRestart::OverrideHospitalLevel = ScriptParams[0].uint8;
	return OR_CONTINUE;
}

/* 0420 */
eOpcodeResult WINAPI OVERRIDE_POLICE_STATION_LEVEL(CScript *script)
{
	script->Collect(1);
	CRestart::OverridePoliceStationLevel = ScriptParams[0].uint8;
	return OR_CONTINUE;
}

/* 0421 */
eOpcodeResult WINAPI FORCE_RAIN(CScript *script)
{
	script->Collect(1);
	CWeather::bScriptsForceRain = !!ScriptParams[0].int32;
	return OR_CONTINUE;
}

/* 0422 */
eOpcodeResult WINAPI DOES_GARAGE_CONTAIN_CAR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[1].int32);
	script->UpdateCompareFlag(CGarages::aGarages[ScriptParams[0].int32].IsEntityEntirelyInside3D(vehicle, 0.0));
	return OR_CONTINUE;
}

/* 042A */
eOpcodeResult WINAPI IS_THREAT_FOR_PED_TYPE(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag(!!(CPedType::ms_apPedType[ScriptParams[0].int32]->threat & ScriptParams[1].int32));
	return OR_CONTINUE;
}

/* 0432 */
eOpcodeResult WINAPI GET_CHAR_IN_CAR_PASSENGER_SEAT(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	ScriptParams[0].int32 = CPools::ms_pPedPool->GetIndex(vehicle->passengers[ScriptParams[1].int32]);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0438 */
eOpcodeResult WINAPI SET_CHAR_IGNORE_LEVEL_TRANSITIONS(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->originLevel = ScriptParams[1].int32 ? -1 : (char)CTheZones::GetLevelFromPosition(&ped->GetPos());
	return OR_CONTINUE;
}

/* 0444 */
eOpcodeResult WINAPI SET_SCRIPT_FIRE_AUDIO(CScript *script)
{
	script->Collect(2);
	CFireManager::fires[ScriptParams[0].int32].sfx = !!ScriptParams[1].int32;
	return OR_CONTINUE;
}

/* 0447 */
eOpcodeResult WINAPI IS_PLAYER_LIFTING_A_PHONE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[ScriptParams[0].int32].playerEntity->state == 0x13);
	return OR_CONTINUE;
}

/* 044E */
eOpcodeResult WINAPI SET_CAR_IGNORE_LEVEL_TRANSITIONS(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	vehicle->originLevel = ScriptParams[1].int32 ? -1 : (char)CTheZones::GetLevelFromPosition(&vehicle->GetPos());
	return OR_CONTINUE;
}

/* 044F */
eOpcodeResult WINAPI MAKE_CRAIGS_CAR_A_BIT_STRONGER(CScript *script)
{
	script->Collect(2);
	CAutomobile *vehicle = static_cast<CAutomobile *>(CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32));
	vehicle->field_501 &= 0xDF;
	if (ScriptParams[1].int32) {
		vehicle->field_501 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 0452 */
eOpcodeResult WINAPI ENABLE_PLAYER_CONTROL_CAMERA(CScript *)
{
	*(unsigned short *)(CPad::GetPad(0) + 0xF0) &= 0xFFFE;
	return OR_CONTINUE;
}

/* 047B */
eOpcodeResult WINAPI LOCATE_SNIPER_BULLET_2D(CScript *script)
{
	script->Collect(5);
	script->UpdateCompareFlag(CBulletInfo::TestForSniperBullet(ScriptParams[0].float32 - ScriptParams[2].float32, ScriptParams[0].float32 + ScriptParams[2].float32, ScriptParams[1].float32 - ScriptParams[3].float32, ScriptParams[1].float32 + ScriptParams[3].float32, -1000.0, 1000.0));
	if (ScriptParams[4].int32) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, ScriptParams[0].float32 - ScriptParams[2].float32, ScriptParams[1].float32 - ScriptParams[3].float32, ScriptParams[0].float32 + ScriptParams[2].float32, ScriptParams[1].float32 + ScriptParams[3].float32, -100.0);
	}
	return OR_CONTINUE;
}

/* 047C */
eOpcodeResult WINAPI LOCATE_SNIPER_BULLET_3D(CScript *script)
{
	script->Collect(7);
	script->UpdateCompareFlag(CBulletInfo::TestForSniperBullet(ScriptParams[0].float32 - ScriptParams[3].float32, ScriptParams[0].float32 + ScriptParams[3].float32, ScriptParams[1].float32 - ScriptParams[4].float32, ScriptParams[1].float32 + ScriptParams[4].float32, ScriptParams[2].float32 - ScriptParams[5].float32, ScriptParams[2].float32 + ScriptParams[5].float32));
	if (ScriptParams[6].int32) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, ScriptParams[0].float32 - ScriptParams[3].float32, ScriptParams[1].float32 - ScriptParams[4].float32, ScriptParams[0].float32 + ScriptParams[3].float32, ScriptParams[1].float32 + ScriptParams[4].float32, ScriptParams[2].float32);
	}
	return OR_CONTINUE;
}

/* 047D */
eOpcodeResult WINAPI GET_NUMBER_OF_SEATS_IN_MODEL(CScript *script)
{
	script->Collect(1);
	ScriptParams[0].uint32 = CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors(ScriptParams[0].int32) + 1;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0491 */
eOpcodeResult WINAPI HAS_CHAR_GOT_WEAPON(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	for (int i = 0; i < 10; i++) {
		if ((int)ped->weapons[i].type == ScriptParams[1].int32) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04A0 */
eOpcodeResult WINAPI GET_OBJECT_DISTANCE_ALONG_PATH(CScript *script)
{
	script->Collect(1);
	ScriptParams[0].float32 = CScriptPaths::ScriptPath[ScriptParams[0].int32].distanceAlongPath;
	script->Store(1);
	return OR_CONTINUE;
}

/* 04A7 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_BOAT(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	if (ped->isInAnyVehicle) {
		if (CVehicle *vehicle = ped->vehicle) {
			unsigned long handling = vehicle->handlingData;
			unsigned int flags = *(unsigned int *)(handling + 0xCC);
			flags &= 0xF0000;
			if (flags == 0x80000) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04A9 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_HELI(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	if (ped->isInAnyVehicle) {
		if (CVehicle *vehicle = ped->vehicle) {
			unsigned long handling = vehicle->handlingData;
			unsigned int flags = *(unsigned int *)(handling + 0xCC);
			flags &= 0xF0000;
			if (flags == 0x20000) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04AB */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_PLANE(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	if (ped->isInAnyVehicle) {
		if (CVehicle *vehicle = ped->vehicle) {
			unsigned long handling = vehicle->handlingData;
			unsigned int flags = *(unsigned int *)(handling + 0xCC);
			flags &= 0xF0000;
			if (flags == 0x40000) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04C8 */
eOpcodeResult WINAPI IS_CHAR_IN_FLYING_VEHICLE(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	if (ped->isInAnyVehicle) {
		if (CVehicle *vehicle = ped->vehicle) {
			unsigned long handling = vehicle->handlingData;
			unsigned int flags = *(unsigned int *)(handling + 0xCC);
			flags &= 0xF0000;
			if (flags == 0x20000 || flags == 0x40000) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04CC */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD(CScript *script)
{
	script->Collect(5);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetShortRangeCoordBlip(4, pos, ScriptParams[3].uint32, ScriptParams[4].int32);
	script->Store(1);
	return OR_CONTINUE;
}

/* 04CD */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD(CScript *script)
{
	script->Collect(3);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetShortRangeCoordBlip(4, pos, 5, 3);
	CRadar::ChangeBlipScale(ScriptParams[0].int32, 3);
	script->Store(1);
	return OR_CONTINUE;
}

/* 04D4 */
eOpcodeResult WINAPI GET_NTH_CLOSEST_CHAR_NODE(CScript *script)
{
	script->Collect(4);
	CVector pos1 = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	CVector pos2 = { 0, 0, 0 };
	if (pos1.z <= -100.0) {
		pos1.z = CWorld::FindGroundZForCoord(pos1.x, pos1.y);
	}
	int node = VCGlobals::ThePaths.FindNthNodeClosestToCoors(pos1, 1, 999999.9f, true, true, ScriptParams[3].int32 - 1, false);
	VCGlobals::ThePaths.FindNodeCoorsForScript(pos2, node);
	ScriptParams[0].float32 = pos2.x;
	ScriptParams[1].float32 = pos2.y;
	ScriptParams[2].float32 = pos2.z;
	script->Store(3);
	return OR_CONTINUE;
}

/* 04DC */
eOpcodeResult WINAPI HAS_PHOTOGRAPH_BEEN_TAKEN(CScript *script)
{
	script->UpdateCompareFlag(CWeapon::bPhotographHasBeenTaken);
	CWeapon::bPhotographHasBeenTaken = false;
	return OR_CONTINUE;
}

/* 0505 */
eOpcodeResult WINAPI GET_NEAREST_TYRE_TO_POINT(CScript *script)
{
	script->Collect(3);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	ScriptParams[0].int32 = vehicle->FindTyreNearestPoint(ScriptParams[1].float32, ScriptParams[2].float32);
	script->Store(1);
	return OR_CONTINUE;
}
#endif

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	ScriptParams[0].uint32 = CWanted::MaximumWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

#if _VC
/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, -1, -1, -1, -1, -1, VCGlobals::gUString);
	CHud::SetHelpMessage(VCGlobals::gUString, false, false);
	return OR_CONTINUE;
}

/* 0513 */
eOpcodeResult WINAPI PRINT_HELP_FOREVER_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, -1, -1, -1, -1, -1, VCGlobals::gUString);
	CHud::SetHelpMessage(VCGlobals::gUString, false, true);
	return OR_CONTINUE;
}

/* 051F */
eOpcodeResult WINAPI DISPLAY_TEXT_WITH_3_NUMBERS(CScript *script)
{
	char gxt[8];
	script->Collect(2);
	script->ReadShortString(gxt);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].positionX = ScriptParams[0].float32;
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].positionY = ScriptParams[1].float32;
	script->Collect(3);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, -1, -1, -1, CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].text);
	CTheScripts::NumberOfIntroTextLinesThisFrame++;
	return OR_CONTINUE;
}

/* 052A */
eOpcodeResult WINAPI ADD_MONEY_SPENT_ON_AUTO_PAINTING(CScript *script)
{
	script->Collect(1);
	CStats::AutoPaintingBudget += static_cast<float>(ScriptParams[0].int32);
	return OR_CONTINUE;
}

/* 052D */
eOpcodeResult WINAPI GET_PLAYER_DRUNKENNESS(CScript *script)
{
	script->Collect(1);
	ScriptParams[0].uint8 = CWorld::Players[ScriptParams[0].int32].playerEntity->drunkenness;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0530 */
eOpcodeResult WINAPI ADD_LOAN_SHARK_VISITS(CScript *script)
{
	script->Collect(1);
	CStats::LoanSharks += static_cast<float>(ScriptParams[0].int32);
	return OR_CONTINUE;
}

/* 0532 */
eOpcodeResult WINAPI ADD_MOVIE_STUNTS(CScript *script)
{
	script->Collect(1);
	CStats::MovieStunts += static_cast<float>(ScriptParams[0].int32);
	return OR_CONTINUE;
}

/* 0535 */
eOpcodeResult WINAPI ADD_GARBAGE_PICKUPS(CScript *script)
{
	script->Collect(1);
	CStats::GarbagePickups += static_cast<float>(ScriptParams[0].int32);
	return OR_CONTINUE;
}

/* 0537 */
eOpcodeResult WINAPI SET_TOP_SHOOTING_RANGE_SCORE(CScript *script)
{
	script->Collect(1);
	if (static_cast<float>(ScriptParams[0].int32) > CStats::TopShootingRangeScore) {
		CStats::TopShootingRangeScore = static_cast<float>(ScriptParams[0].int32);
	}
	return OR_CONTINUE;
}

/* 0538 */
eOpcodeResult WINAPI ADD_SHOOTING_RANGE_RANK(CScript *script)
{
	script->Collect(1);
	CStats::ShootingRank += static_cast<float>(ScriptParams[0].int32);
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[1].int32);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle && ped->vehicle) {
		source = ped->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(vehicle));
	return OR_CONTINUE;
}

/* 0555 */
eOpcodeResult WINAPI REMOVE_WEAPON_FROM_CHAR(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	static_cast<CPedHack *>(ped)->ClearWeapon(ScriptParams[1].int32);
	return OR_CONTINUE;
}

/* 056E */
eOpcodeResult WINAPI DOES_VEHICLE_EXIST(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[0].int32);
	script->UpdateCompareFlag(!!vehicle);
	return OR_CONTINUE;
}

/* 056F */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT(CScript *script)
{
	script->Collect(3);
	CVector pos = { ScriptParams[0].float32, ScriptParams[1].float32, ScriptParams[2].float32 };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	ScriptParams[0].int32 = CRadar::SetShortRangeCoordBlip(5, pos, 2, 3);
	CRadar::ChangeBlipScale(ScriptParams[0].int32, 3);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0577 */
eOpcodeResult WINAPI SET_FADE_AND_JUMPCUT_AFTER_RC_EXPLOSION(CScript *script)
{
	script->Collect(1);
	CWorld::Players[CWorld::PlayerInFocus].fadeJumpCutRcExplode = !!ScriptParams[0].int32;
	return OR_CONTINUE;
}

/* 059D */
eOpcodeResult WINAPI SHUFFLE_CARD_DECKS(CScript *script)
{
	script->Collect(1);
	memset(CardStack, 0, sizeof(CardStack[0]) * 312);
	short seqarr[312];
	for (short i = 0; i < 312; i++) {
		seqarr[i] = i;
	}
	if (ScriptParams[0].int32) {
		int index = ScriptParams[0].int32 * 52;
		for (short card = 1; card < 53; card++) {
			for (int deck = 0; deck < ScriptParams[0].int32; deck++) {
				int rand = VCGlobals::rand();
				int seqindex = static_cast<int>(index * static_cast<float>(rand & 0xFFFF) / 0x8000);
				int seqval = seqarr[seqindex];
				CardStack[seqval] = card;
				while (seqindex < index) {
					if (seqindex + 1 < 312) {
						seqarr[seqindex] = seqarr[seqindex + 1];
					} else {
						seqarr[seqindex] = 0;
					}
					seqindex++;
				}
				index--;
			}
		}
	}
	CardStackPosition = 0;
	return OR_CONTINUE;
}


/* 059E */
eOpcodeResult WINAPI FETCH_NEXT_CARD(CScript *script)
{
	if (!CardStack[CardStackPosition] || CardStackPosition > 6 * 52) {
		CardStackPosition = 0;
	}
	ScriptParams[0].int32 = CardStack[CardStackPosition++];
	script->Store(1);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH) {
		if (CLEO_GetVersion() < CLEO_VERSION) {
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("VC.Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		DWORD flOldProtect;
		unsigned long address;
		// CGame::Initialise
		address = vcversion::AdjustOffset(0x004A4ED8);
		VirtualProtect(reinterpret_cast<void *>(address), 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
		ReadCall(address, CallInit);
		InjectHook(address, &CPacManPickupsHack::Init);
		VirtualProtect(reinterpret_cast<void *>(address), 5, flOldProtect, &flOldProtect);
		// CGame::ReInitGameObjectVariables
		address = vcversion::AdjustOffset(0x004A4967);
		VirtualProtect(reinterpret_cast<void *>(address), 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
		InjectHook(address, &CPacManPickupsHack::Init);
		VirtualProtect(reinterpret_cast<void *>(address), 5, flOldProtect, &flOldProtect);
		// CGame::Process
		address = vcversion::AdjustOffset(0x004A45D7);
		VirtualProtect(reinterpret_cast<void *>(address), 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
		ReadCall(address, CallUpdate);
		InjectHook(address, &CPacManPickupsHack::Update);
		VirtualProtect(reinterpret_cast<void *>(address), 5, flOldProtect, &flOldProtect);
		// RenderEffects
		address = vcversion::AdjustOffset(0x004A6547);
		VirtualProtect(reinterpret_cast<void *>(address), 5, PAGE_EXECUTE_READWRITE, &flOldProtect);
		ReadCall(address, CallRender);
		InjectHook(address, &CPacManPickupsHack::Render);
		VirtualProtect(reinterpret_cast<void *>(address), 5, flOldProtect, &flOldProtect);

		Opcodes::RegisterOpcode(0x0A2, IS_CHAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x0AC, IS_CAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x0BD, PRINT_SOON);
		Opcodes::RegisterOpcode(0x0C5, RETURN_TRUE);
		Opcodes::RegisterOpcode(0x0C6, RETURN_FALSE);
		Opcodes::RegisterOpcode(0x0E2, GET_PAD_STATE);
		Opcodes::RegisterOpcode(0x113, ADD_AMMO_TO_PLAYER);
		Opcodes::RegisterOpcode(0x116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x130, HAS_PLAYER_BEEN_ARRESTED);
		Opcodes::RegisterOpcode(0x135, CHANGE_CAR_LOCK);
		Opcodes::RegisterOpcode(0x136, SHAKE_CAM_WITH_POINT);
		Opcodes::RegisterOpcode(0x13B, IS_CAR_DEAD_IN_AREA_2D);
		Opcodes::RegisterOpcode(0x13C, IS_CAR_DEAD_IN_AREA_3D);
		Opcodes::RegisterOpcode(0x155, SET_CAR_DENSITY);
		Opcodes::RegisterOpcode(0x156, SET_PED_DENSITY);
		Opcodes::RegisterOpcode(0x15E, IS_CAR_IN_AIR);
		Opcodes::RegisterOpcode(0x163, ADD_BLIP_FOR_OBJECT_OLD);
		Opcodes::RegisterOpcode(0x16F, DRAW_SHADOW);
		Opcodes::RegisterOpcode(0x178, IS_PLAYER_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x179, IS_CHAR_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x17B, SET_CHAR_AMMO);
		Opcodes::RegisterOpcode(0x1C6, DONT_REMOVE_CAR);
		Opcodes::RegisterOpcode(0x1E6, PRINT_WITH_NUMBER_SOON);
		Opcodes::RegisterOpcode(0x1EE, ACTIVATE_CRANE);
		Opcodes::RegisterOpcode(0x1EF, DEACTIVATE_CRANE);
		Opcodes::RegisterOpcode(0x218, PRINT_WITH_NUMBER_BIG_Q);
		Opcodes::RegisterOpcode(0x21D, SET_FREE_BOMBS);
		Opcodes::RegisterOpcode(0x21F, SET_ALL_TAXI_LIGHTS);
		Opcodes::RegisterOpcode(0x220, IS_CAR_ARMED_WITH_ANY_BOMB);
		Opcodes::RegisterOpcode(0x228, IS_CAR_ARMED_WITH_BOMB);
		Opcodes::RegisterOpcode(0x23A, IS_PLAYER_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x23B, IS_CHAR_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x23E, FLASH_CAR);
		Opcodes::RegisterOpcode(0x23F, FLASH_CHAR);
		Opcodes::RegisterOpcode(0x240, FLASH_OBJECT);
		Opcodes::RegisterOpcode(0x242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x24B, SET_REPEATED_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x24C, SET_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x24D, HAS_PHONE_DISPLAYED_MESSAGE);
		Opcodes::RegisterOpcode(0x250, DRAW_LIGHT);
		Opcodes::RegisterOpcode(0x251, STORE_WEATHER);
		Opcodes::RegisterOpcode(0x252, RESTORE_WEATHER);
		Opcodes::RegisterOpcode(0x255, RESTART_CRITICAL_MISSION);
		Opcodes::RegisterOpcode(0x295, IS_TAXI);
		Opcodes::RegisterOpcode(0x299, ACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x29A, SWITCH_TAXI_TIMER);
		Opcodes::RegisterOpcode(0x29C, IS_BOAT);
		Opcodes::RegisterOpcode(0x2A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x2A1, MESSAGE_WAIT);
		Opcodes::RegisterOpcode(0x2A2, ADD_PARTICLE_EFFECT);
		Opcodes::RegisterOpcode(0x2A4, ADD_SPRITE_BLIP_FOR_CAR);
		Opcodes::RegisterOpcode(0x2A5, ADD_SPRITE_BLIP_FOR_CHAR);
		Opcodes::RegisterOpcode(0x2A6, ADD_SPRITE_BLIP_FOR_OBJECT);
		Opcodes::RegisterOpcode(0x2B9, DEACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x2BC, SET_SWAT_REQUIRED);
		Opcodes::RegisterOpcode(0x2BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x2BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x2C3, START_PACMAN_RACE);
		Opcodes::RegisterOpcode(0x2C5, GET_NUMBER_OF_POWER_PILLS_EATEN);
		Opcodes::RegisterOpcode(0x2C6, CLEAR_PACMAN);
		Opcodes::RegisterOpcode(0x2C7, START_PACMAN_SCRAMBLE);
		Opcodes::RegisterOpcode(0x2C8, GET_NUMBER_OF_POWER_PILLS_CARRIED);
		Opcodes::RegisterOpcode(0x2C9, CLEAR_NUMBER_OF_POWER_PILLS_CARRIED);
		Opcodes::RegisterOpcode(0x2CD, GOSUB_FILE);
		Opcodes::RegisterOpcode(0x2D2, SET_COMEDY_CONTROLS);
		Opcodes::RegisterOpcode(0x2D6, IS_CHAR_SHOOTING_IN_AREA);
		Opcodes::RegisterOpcode(0x2D9, CLEAR_NUMBER_OF_POWER_PILLS_EATEN);
		Opcodes::RegisterOpcode(0x2DA, ADD_POWER_PILL);
		Opcodes::RegisterOpcode(0x2EE, IS_PROJECTILE_IN_AREA);
		Opcodes::RegisterOpcode(0x2EF, DESTROY_PROJECTILES_IN_AREA);
		Opcodes::RegisterOpcode(0x2F0, DROP_MINE);
		Opcodes::RegisterOpcode(0x2F1, DROP_NAUTICAL_MINE);
		Opcodes::RegisterOpcode(0x2FB, ACTIVATE_CRUSHER_CRANE);
		Opcodes::RegisterOpcode(0x2FC, PRINT_WITH_2_NUMBERS);
		Opcodes::RegisterOpcode(0x2FE, PRINT_WITH_2_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x300, PRINT_WITH_3_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x301, PRINT_WITH_3_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x303, PRINT_WITH_4_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x304, PRINT_WITH_4_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x305, PRINT_WITH_5_NUMBERS);
		Opcodes::RegisterOpcode(0x306, PRINT_WITH_5_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x307, PRINT_WITH_5_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x309, PRINT_WITH_6_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x30A, PRINT_WITH_6_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x31B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x31C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x32D, SET_CAR_BLOCK_CAR);
		Opcodes::RegisterOpcode(0x338, SET_CAR_VISIBLE);
		Opcodes::RegisterOpcode(0x344, SET_TEXT_CENTRE_SIZE);
		Opcodes::RegisterOpcode(0x346, SET_TEXT_BACKGROUND_COLOUR);
		Opcodes::RegisterOpcode(0x349, SET_TEXT_FONT);
		Opcodes::RegisterOpcode(0x351, IS_NASTY_GAME);
		Opcodes::RegisterOpcode(0x356, IS_EXPLOSION_IN_AREA);
		Opcodes::RegisterOpcode(0x357, IS_EXPLOSION_IN_ZONE);
		Opcodes::RegisterOpcode(0x367, START_KILL_FRENZY_HEADSHOT);
		Opcodes::RegisterOpcode(0x368, ACTIVATE_MILITARY_CRANE);
		Opcodes::RegisterOpcode(0x36E, PRINT_WITH_3_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x36F, PRINT_WITH_4_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x370, PRINT_WITH_5_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x371, PRINT_WITH_6_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x375, PRINT_STRING_IN_STRING);
		Opcodes::RegisterOpcode(0x378, SET_2_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x379, SET_2_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x37A, SET_3_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x37B, SET_3_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x37C, SET_4_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x37D, SET_4_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x37F, GIVE_PLAYER_DETONATOR);
		Opcodes::RegisterOpcode(0x385, PRINT_STRING_IN_STRING_SOON);
		Opcodes::RegisterOpcode(0x386, SET_5_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x387, SET_5_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x388, SET_6_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x389, SET_6_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x393, SET_CHAR_ANIM_SPEED);
		Opcodes::RegisterOpcode(0x3A0, IS_CRANE_LIFTING_CAR);
		Opcodes::RegisterOpcode(0x3A5, CHANGE_GARAGE_TYPE_WITH_CAR_MODEL);
		Opcodes::RegisterOpcode(0x3C2, IS_PHONE_DISPLAYING_MESSAGE);
		Opcodes::RegisterOpcode(0x3C6, IS_COLLISION_IN_MEMORY);
		Opcodes::RegisterOpcode(0x3C9, IS_CAR_VISIBLY_DAMAGED);
		Opcodes::RegisterOpcode(0x3DB, ADD_BLIP_FOR_PICKUP_OLD);
		Opcodes::RegisterOpcode(0x3DD, ADD_SPRITE_BLIP_FOR_PICKUP);
		Opcodes::RegisterOpcode(0x3E9, IS_CHAR_IN_CONTROL);
		Opcodes::RegisterOpcode(0x3EC, HAS_MILITARY_CRANE_COLLECTED_ALL_CARS);
		Opcodes::RegisterOpcode(0x3FB, SET_CAR_STAYS_IN_CURRENT_LEVEL);
		Opcodes::RegisterOpcode(0x3FC, SET_CHAR_STAYS_IN_CURRENT_LEVEL);
		Opcodes::RegisterOpcode(0x410, SET_GANG_PED_MODEL_PREFERENCE);
		Opcodes::RegisterOpcode(0x413, SET_GET_OUT_OF_JAIL_FREE);
		Opcodes::RegisterOpcode(0x415, IS_CAR_DOOR_CLOSED);
		Opcodes::RegisterOpcode(0x41A, GET_AMMO_IN_CHAR_WEAPON);
		Opcodes::RegisterOpcode(0x41B, REGISTER_KILL_FRENZY_PASSED);
		Opcodes::RegisterOpcode(0x41C, SET_CHAR_SAY);
		Opcodes::RegisterOpcode(0x41F, OVERRIDE_HOSPITAL_LEVEL);
		Opcodes::RegisterOpcode(0x420, OVERRIDE_POLICE_STATION_LEVEL);
		Opcodes::RegisterOpcode(0x421, FORCE_RAIN);
		Opcodes::RegisterOpcode(0x422, DOES_GARAGE_CONTAIN_CAR);
		Opcodes::RegisterOpcode(0x42A, IS_THREAT_FOR_PED_TYPE);
		Opcodes::RegisterOpcode(0x432, GET_CHAR_IN_CAR_PASSENGER_SEAT);
		Opcodes::RegisterOpcode(0x438, SET_CHAR_IGNORE_LEVEL_TRANSITIONS);
		Opcodes::RegisterOpcode(0x444, SET_SCRIPT_FIRE_AUDIO);
		Opcodes::RegisterOpcode(0x447, IS_PLAYER_LIFTING_A_PHONE);
		Opcodes::RegisterOpcode(0x44E, SET_CAR_IGNORE_LEVEL_TRANSITIONS);
		Opcodes::RegisterOpcode(0x44F, MAKE_CRAIGS_CAR_A_BIT_STRONGER);
		Opcodes::RegisterOpcode(0x452, ENABLE_PLAYER_CONTROL_CAMERA);
		Opcodes::RegisterOpcode(0x47B, LOCATE_SNIPER_BULLET_2D);
		Opcodes::RegisterOpcode(0x47C, LOCATE_SNIPER_BULLET_3D);
		Opcodes::RegisterOpcode(0x47D, GET_NUMBER_OF_SEATS_IN_MODEL);
		Opcodes::RegisterOpcode(0x491, HAS_CHAR_GOT_WEAPON);
		Opcodes::RegisterOpcode(0x4A0, GET_OBJECT_DISTANCE_ALONG_PATH);
		Opcodes::RegisterOpcode(0x4A7, IS_CHAR_IN_ANY_BOAT);
		Opcodes::RegisterOpcode(0x4A9, IS_CHAR_IN_ANY_HELI);
		Opcodes::RegisterOpcode(0x4AB, IS_CHAR_IN_ANY_PLANE);
		Opcodes::RegisterOpcode(0x4C8, IS_CHAR_IN_FLYING_VEHICLE);
		Opcodes::RegisterOpcode(0x4CC, ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD);
		Opcodes::RegisterOpcode(0x4CD, ADD_SHORT_RANGE_BLIP_FOR_COORD);
		Opcodes::RegisterOpcode(0x4D4, GET_NTH_CLOSEST_CHAR_NODE);
		Opcodes::RegisterOpcode(0x4DC, HAS_PHOTOGRAPH_BEEN_TAKEN);
		Opcodes::RegisterOpcode(0x505, GET_NEAREST_TYRE_TO_POINT);
		Opcodes::RegisterOpcode(0x50F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x513, PRINT_HELP_FOREVER_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x51F, DISPLAY_TEXT_WITH_3_NUMBERS);
		Opcodes::RegisterOpcode(0x52A, ADD_MONEY_SPENT_ON_AUTO_PAINTING);
		Opcodes::RegisterOpcode(0x52D, GET_PLAYER_DRUNKENNESS);
		Opcodes::RegisterOpcode(0x530, ADD_LOAN_SHARK_VISITS);
		Opcodes::RegisterOpcode(0x532, ADD_MOVIE_STUNTS);
		Opcodes::RegisterOpcode(0x535, ADD_GARBAGE_PICKUPS);
		Opcodes::RegisterOpcode(0x537, SET_TOP_SHOOTING_RANGE_SCORE);
		Opcodes::RegisterOpcode(0x538, ADD_SHOOTING_RANGE_RANK);
		Opcodes::RegisterOpcode(0x547, IS_CHAR_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x555, REMOVE_WEAPON_FROM_CHAR);
		Opcodes::RegisterOpcode(0x56E, DOES_VEHICLE_EXIST);
		Opcodes::RegisterOpcode(0x56F, ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT);
		Opcodes::RegisterOpcode(0x577, SET_FADE_AND_JUMPCUT_AFTER_RC_EXPLOSION);
		Opcodes::RegisterOpcode(0x59D, SHUFFLE_CARD_DECKS);
		Opcodes::RegisterOpcode(0x59E, FETCH_NEXT_CARD);
	}
	return TRUE;
}

#elif _III

/* 01E6 */
eOpcodeResult WINAPI PRINT_WITH_NUMBER_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(3);
	CMessages::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[1].uint32, ScriptParams[2].uint16, ScriptParams[0].int32, -1, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0301 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[3].uint32, ScriptParams[4].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0304 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[4].uint32, ScriptParams[5].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, -1, -1);
	return OR_CONTINUE;
}

/* 0307 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[5].uint32, ScriptParams[6].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, -1);
	return OR_CONTINUE;
}

/* 030A */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_SOON(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::AddMessageSoonWithNumber(TheText.Get(gxt), ScriptParams[6].uint32, ScriptParams[7].uint16, ScriptParams[0].int32, ScriptParams[1].int32, ScriptParams[2].int32, ScriptParams[3].int32, ScriptParams[4].int32, ScriptParams[5].int32);
	return OR_CONTINUE;
}

/* 048F */
eOpcodeResult WINAPI REMOVE_ALL_CHAR_WEAPONS(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	ped->ClearWeapons();
	return OR_CONTINUE;
}

/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	CMessages::InsertNumberInString(TheText.Get(gxt), ScriptParams[0].int32, -1, -1, -1, -1, -1, IIIGlobals::gUString);
	CHud::SetHelpMessage(IIIGlobals::gUString, false);
	return OR_CONTINUE;
}

/* 0546 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[ScriptParams[0].int32].playerEntity;
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[1].int32);
	CPhysical *source = player;
	if (player->isInAnyVehicle && player->vehicle) {
		source = player->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(vehicle));
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(ScriptParams[0].int32);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(ScriptParams[1].int32);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle && ped->vehicle) {
		source = ped->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(vehicle));
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH) {
		if (CLEO_GetVersion() < CLEO_VERSION) {
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("III.Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		Opcodes::RegisterOpcode(0x116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x1E6, PRINT_WITH_NUMBER_SOON);
		Opcodes::RegisterOpcode(0x2BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x2BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x301, PRINT_WITH_3_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x304, PRINT_WITH_4_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x307, PRINT_WITH_5_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x30A, PRINT_WITH_6_NUMBERS_SOON);
		Opcodes::RegisterOpcode(0x385, PRINT_STRING_IN_STRING_SOON);
		Opcodes::RegisterOpcode(0x48F, REMOVE_ALL_CHAR_WEAPONS);
		Opcodes::RegisterOpcode(0x50F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x546, IS_PLAYER_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x547, IS_CHAR_TOUCHING_VEHICLE);
	}
	return TRUE;
}

#endif
