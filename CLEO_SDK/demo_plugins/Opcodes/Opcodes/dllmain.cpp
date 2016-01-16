// configuration manager
// active solution configuration: VC
// active solution platform: x86

#include "stdafx.h"

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  3

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

#if _VC
#pragma comment (lib, "..\\..\\..\\VC.CLEO.lib")
#include "..\\..\\..\\VC.CLEO.h"

tScriptVar *Params;

enum gameVersion{ V1_0, V1_1, VSTEAM, VUNKOWN = -1 };

class GtaGame
{
public:
	gameVersion version;
	GtaGame();
};

GtaGame game;

void(__cdecl *StartFrenzy)(DWORD, INT, WORD, DWORD, CHAR *, INT, INT, INT, BYTE, BYTE); // cdarkel::startfrenzy
void(__cdecl *CreatePickup)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD); // cpickups::generatenewone
uintptr_t(__thiscall *ObjectPoolGetStruct)(void *, INT); // cpool_cobject_ccutscenehead::getat
uintptr_t(__thiscall *VehiclePoolGetStruct)(void *, INT); // cpool_cvehicle_cautomobile::getat
uintptr_t(__thiscall *PedPoolGetStruct)(void *, INT); // cpool_cped_cplayerped::getat
WORD(__thiscall *GetPadState)(CScript *, DWORD, DWORD); // crunningscript::getpadstate
void(__thiscall *CamShake)(uintptr_t, FLOAT, FLOAT, FLOAT, FLOAT); // ccamera::camshake
BYTE(__thiscall *GetHasCollidedWith)(DWORD, uintptr_t); // cphysical::gethascollidedwith
DWORD(__cdecl *FindPlayerPed)(void); // findplayerped
FLOAT(__cdecl *FindGroundZForCoord)(FLOAT, FLOAT); // cworld::findgroundzforcoord
void(__thiscall *SetAmmo)(uintptr_t, DWORD, DWORD); // cped::setammo
void(__thiscall *GrantAmmo)(DWORD, DWORD, DWORD); // cped::grantammo
DWORD(__cdecl *GetVehicleTypeId)(DWORD); // cvehiclemodelinfo::getvehicletypeid
char *(__thiscall *GetText)(uintptr_t, char *); // ctext::get
DWORD *barrel1 = NULL;
DWORD *barrel2 = NULL;
DWORD *maxWantedLevel = NULL;
uintptr_t ccamera = NULL;
uintptr_t garage = NULL;
uintptr_t *playerPedPool = NULL;
uintptr_t *playerPedState = NULL;
uintptr_t ctext = NULL;
void **objectPool = NULL;
FLOAT *shootingRangeRank = NULL;
FLOAT *garbagePickups = NULL;
FLOAT *loanSharkVisits = NULL;
void **pedPool = NULL;
FLOAT *topShootingRangeScore = NULL;
DWORD *levelName = NULL;
FLOAT *movieStunts = NULL;
void **carPool = NULL;
BYTE *allTaxiLights = NULL;
BYTE *freeBombs = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x74FF5064:
		version = V1_0;
		StartFrenzy = (void(__cdecl *)(DWORD, INT, WORD, DWORD, CHAR *, INT, INT, INT, BYTE, BYTE))0x429B60;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451C30;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451CB0;
		GetPadState = (WORD(__thiscall *)(CScript *, DWORD, DWORD))0x460C00;
		CamShake = (void(__thiscall *)(uintptr_t, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))0x4B9010;
		FindPlayerPed = (DWORD(__cdecl *)(void))0x4BC120;
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))0x4D5540;
		SetAmmo = (void(__thiscall *)(uintptr_t, DWORD, DWORD))0x4FF780;
		GrantAmmo = (void(__thiscall *)(DWORD, DWORD, DWORD))0x4FF840;
		GetVehicleTypeId = (DWORD(__cdecl *)(DWORD))0x578A70;
		GetText = (char *(__thiscall *)(uintptr_t, char *))0x584F30;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		ccamera = 0x7E4688;
		garage = 0x812668;
		playerPedPool = (uintptr_t *)0x94AD28;
		playerPedState = (uintptr_t *)0x94ADF4;
		ctext = 0x94B220;
		objectPool = (void **)0x94DBE0;
		shootingRangeRank = (FLOAT *)0x974B08;
		garbagePickups = (FLOAT *)0x974C00;
		loanSharkVisits = (FLOAT*)0x974C28;
		pedPool = (void **)0x97F2AC;
		topShootingRangeScore = (FLOAT *)0xA0D8A4;
		levelName = (DWORD *)0xA0D9AC;
		movieStunts = (FLOAT *)0xA0FC8C;
		carPool = (void **)0xA0FDE4;
		allTaxiLights = (BYTE *)0xA10ABB;
		freeBombs = (BYTE *)0xA10B32;
		break;
	case 0x00408DC0:
		version = V1_1;
		StartFrenzy = (void(__cdecl *)(DWORD, INT, WORD, DWORD, CHAR *, INT, INT, INT, BYTE, BYTE))0x429B60;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451C30;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x451CB0;
		GetPadState = (WORD(__thiscall *)(CScript *, DWORD, DWORD))0x460C00;
		CamShake = (void(__thiscall *)(uintptr_t, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))(0x4B9010 + 0x20);
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))(0x4D5540 + 0x20);
		SetAmmo = (void(__thiscall *)(uintptr_t, DWORD, DWORD))(0x4FF780 + 0x20);
		GrantAmmo = (void(__thiscall *)(DWORD, DWORD, DWORD))(0x4FF840 + 0x20);
		GetVehicleTypeId = (DWORD(__cdecl *)(DWORD))(0x578A70 + 0x20);
		GetText = (char *(__thiscall *)(uintptr_t, char *))(0x584F30 + 0x20);
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		ccamera = 0x7E4688 + 8;
		garage = 0x812668 + 8;
		playerPedPool = (uintptr_t *)(0x94AD28 + 8);
		playerPedState = (uintptr_t *)(0x94ADF4 + 8);
		ctext = 0x94B220 + 8;
		objectPool = (void **)(0x94DBE0 + 8);
		shootingRangeRank = (FLOAT *)(0x974B08 + 8);
		garbagePickups = (FLOAT *)(0x974C00 + 8);
		loanSharkVisits = (FLOAT*)(0x974C28 + 8);
		pedPool = (void **)(0x97F2AC + 8);
		topShootingRangeScore = (FLOAT *)(0xA0D8A4 + 8);
		levelName = (DWORD *)(0xA0D9AC + 8);
		movieStunts = (FLOAT *)(0xA0FC8C + 8);
		carPool = (void **)(0xA0FDE4 + 8);
		allTaxiLights = (BYTE *)(0xA10ABB + 8);
		freeBombs = (BYTE *)(0xA10B32 + 8);
		break;
	case 0x00004824:
		version = VSTEAM;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))(0x451C30 - 0x120);
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))(0x451C70 - 0x120);
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))(0x451CB0 - 0x120);
		objectPool = (void **)(0x94DBE0 - 0xFF8);
		shootingRangeRank = (FLOAT *)(0x974B08 - 0xFF8);
		garbagePickups = (FLOAT *)(0x974C00 - 0xFF8);
		loanSharkVisits = (FLOAT*)(0x974C28 - 0xFF8);
		pedPool = (void **)(0x97F2AC - 0xFF8);
		topShootingRangeScore = (FLOAT *)(0xA0D8A4 - 0xFF8);
		levelName = (DWORD *)(0xA0D9AC - 0xFF8);
		movieStunts = (FLOAT *)(0xA0FC8C - 0xFF8);
		carPool = (void **)(0xA0FDE4 - 0xFF8);
		break;
	}
}

/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		DWORD state = *(DWORD *)(ped + 0x244);
		if (state != 0x37 && state != 0x36) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 00AC */
eOpcodeResult WINAPI IS_CAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (car != NULL) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag != 5) {
			flag = *(BYTE *)(car + 0x1FD);
			flag &= 1;
			if (flag != 1) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
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
	Params[0].nVar = (unsigned int)GetPadState(script, Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0113 */
eOpcodeResult WINAPI ADD_AMMO_TO_PLAYER(CScript *script)
{
	script->Collect(3);
	GrantAmmo(playerPedPool[0x2E * Params[0].nVar], Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	if (playerPedState[0x2E * Params[0].nVar] == 1) {
		script->UpdateCompareFlag(false);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(true);
	return OR_CONTINUE;
}

/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	if (playerPedState[0x2E * Params[0].nVar] == 2) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0135 */
eOpcodeResult WINAPI CHANGE_CAR_LOCK(CScript *script)
{
	script->Collect(2);
	uintptr_t car;
	if ((car = VehiclePoolGetStruct(*carPool, Params[0].nVar)) != NULL) {
		*(DWORD *)(car + 0x230) = Params[1].nVar;
	}
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	CamShake(ccamera, ((float)Params[0].nVar) * 1e-3f, Params[1].fVar, Params[2].fVar, Params[3].fVar);
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	if (*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x5C5)) {
		script->UpdateCompareFlag(false);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(true);
	return OR_CONTINUE;
}

/* 0178 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	DWORD source = playerPedPool[0x2E * Params[0].nVar];
	uintptr_t target = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		uintptr_t car = *(uintptr_t *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	if (GetHasCollidedWith(source, target)) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0179 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	uintptr_t source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	uintptr_t target = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		uintptr_t car = *(uintptr_t *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	if (GetHasCollidedWith(source, target)) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 017B */
eOpcodeResult WINAPI SET_CHAR_AMMO(CScript *script)
{
	script->Collect(3);
	SetAmmo(PedPoolGetStruct(*pedPool, Params[0].nVar), Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	DWORD param = Params[0].nVar;
	if (param) {
		param = 1;
	}
	*freeBombs = (BYTE)param;
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	DWORD param = Params[0].nVar;
	if (param) {
		param = 1;
	}
	*allTaxiLights = (BYTE)param;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	if (*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FE) & 7) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0228 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	if ((*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FE) & 7) == Params[1].nVar) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 023A */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	DWORD player = playerPedPool[0x2E * Params[0].nVar];
	uintptr_t object = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (!*(BYTE *)(player + 0x3AC)) {
		if (GetHasCollidedWith(player, object)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 023B */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	uintptr_t object = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (!*(BYTE *)(ped + 0x3AC)) {
		if (GetHasCollidedWith(ped, object)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0242 */
eOpcodeResult WINAPI ARM_CAR_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	BYTE input = ((BYTE)Params[1].nVar) & 7;
	BYTE state = (*(BYTE *)(car + 0x1FE) & 0xF8);
	state |= input;
	*(BYTE *)(car + 0x1FE) = state;
	return OR_CONTINUE;
}

/* 029C */
eOpcodeResult WINAPI IS_BOAT(CScript *script)
{
	script->Collect(1);
	if (*(DWORD *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x29C) == 1) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0299 */
eOpcodeResult WINAPI ACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	BYTE *pgarage = (BYTE *)(Params[0].nVar * 0xA8 + garage);
	if (*pgarage == 11) {
		if (*(pgarage + 1) == 0) {
			*(pgarage + 1) = 3;
		}
	}
	*(pgarage + 5) = 0;
	return OR_CONTINUE;
}

/* 02B9 */
eOpcodeResult WINAPI DEACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	*((BYTE *)(Params[0].nVar * 0xA8 + garage + 5)) = 1;
	return OR_CONTINUE;
}

/* 02BC */
eOpcodeResult WINAPI SET_SWAT_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE swat = *(BYTE *)(police + 0x1E);
	swat &= 0xFB;
	if (Params[0].nVar) {
		swat |= 4;
	}
	*(BYTE *)(police + 0x1E) = swat;
	return OR_CONTINUE;
}

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE fbi = *(BYTE *)(police + 0x1E);
	fbi &= 0xF7;
	if (Params[0].nVar) {
		fbi |= 8;
	}
	*(BYTE *)(police + 0x1E) = fbi;
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE army = *(BYTE *)(police + 0x1E);
	army &= 0xEF;
	if (Params[0].nVar) {
		army |= 16;
	}
	*(BYTE *)(police + 0x1E) = army;
	return OR_CONTINUE;
}

/* 02A0 */
eOpcodeResult WINAPI IS_CHAR_STOPPED(CScript *script)
{
	script->Collect(1);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (*(BYTE *)(ped + 0x3AC)) {
		uintptr_t car = *(uintptr_t *)(ped + 0x3A8);
		if (*(float *)(car + 0x100) == 0.0) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	} else {
		if (*(DWORD *)(ped + 0x24C) <= 1) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 02F0 */
eOpcodeResult WINAPI DROP_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CreatePickup(Params[0].fVar, Params[1].fVar, Params[2].fVar, *barrel1, 9, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02F1 */
eOpcodeResult WINAPI DROP_NAUTICAL_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CreatePickup(Params[0].fVar, Params[1].fVar, Params[2].fVar, *barrel2, 11, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	if ((DWORD)*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1A0) == Params[1].nVar) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 031C */
eOpcodeResult WINAPI IS_SECOND_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	if ((DWORD)*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1A1) == Params[1].nVar) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0367 */
eOpcodeResult WINAPI START_KILL_FRENZY_HEADSHOT(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	char *string = GetText(ctext, (char *)gxt);
	script->Collect(8);
	if (Params[7].nVar) {
		Params[7].nVar = 1;
	}
	StartFrenzy(Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, string, Params[4].nVar, Params[5].nVar, Params[6].nVar, Params[7].nVar, 1);
	return OR_CONTINUE;
}

/* 03C6 */
eOpcodeResult WINAPI IS_COLLISION_IN_MEMORY(CScript *script)
{
	script->Collect(1);
	if (Params[0].nVar == *levelName) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 03C9 */
eOpcodeResult WINAPI IS_CAR_VISIBLY_DAMAGED(CScript *script)
{
	script->Collect(1);
	BYTE flag = *(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FB);
	flag >>= 1;
	flag &= 1;
	if (flag == 1) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0413 */
eOpcodeResult WINAPI SET_GET_OUT_OF_JAIL_FREE(CScript *script)
{
	script->Collect(2);
	if (Params[1].nVar) {
		Params[1].nVar = 1;
	}
	*(BYTE *)(Params[0].nVar * 0x170 + (DWORD)playerPedPool + 0x145) = (BYTE)Params[1].nVar;
	return OR_CONTINUE;
}

/* 047D */
eOpcodeResult WINAPI GET_NUMBER_OF_SEATS_IN_MODEL(CScript *script)
{
	script->Collect(1);
	Params[0].nVar = GetVehicleTypeId(Params[0].nVar) + 1;
	script->Store(1);
	return OR_CONTINUE;
}

/* 04A7 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_BOAT(CScript *script)
{
	script->Collect(1);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			uintptr_t car = *(uintptr_t *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x80000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
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
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			uintptr_t car = *(uintptr_t *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x20000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
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
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			uintptr_t car = *(uintptr_t *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x40000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
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
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			uintptr_t car = *(uintptr_t *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x20000 || flags == 0x40000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = *maxWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

/* 052D */
eOpcodeResult WINAPI GET_PLAYER_DRUNKENNESS(CScript *script)
{
	script->Collect(1);
	DWORD player = playerPedPool[0x2E * Params[0].nVar];
	Params[0].nVar = (DWORD)*(BYTE *)(player + 0x638);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0530 */
eOpcodeResult WINAPI ADD_LOAN_SHARK_VISITS(CScript *script)
{
	script->Collect(1);
	*loanSharkVisits += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0532 */
eOpcodeResult WINAPI ADD_MOVIE_STUNTS(CScript *script)
{
	script->Collect(1);
	*movieStunts += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0535 */
eOpcodeResult WINAPI ADD_GARBAGE_PICKUPS(CScript *script)
{
	script->Collect(1);
	*garbagePickups += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0537 */
eOpcodeResult WINAPI SET_TOP_SHOOTING_RANGE_SCORE(CScript *script)
{
	script->Collect(1);
	if ((FLOAT)Params[0].nVar > *topShootingRangeScore) {
		*topShootingRangeScore = (FLOAT)Params[0].nVar;
	}
	return OR_CONTINUE;
}

/* 0538 */
eOpcodeResult WINAPI ADD_SHOOTING_RANGE_RANK(CScript *script)
{
	script->Collect(1);
	*shootingRangeRank += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	uintptr_t source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		uintptr_t car = *(uintptr_t *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	if (GetHasCollidedWith(source, target)) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(0x00A2, IS_CHAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x00AC, IS_CAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x00C5, RETURN_TRUE);
		Opcodes::RegisterOpcode(0x00C6, RETURN_FALSE);
		Opcodes::RegisterOpcode(0x00E2, GET_PAD_STATE);
		Opcodes::RegisterOpcode(0x0113, ADD_AMMO_TO_PLAYER);
		Opcodes::RegisterOpcode(0x0116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x0130, HAS_PLAYER_BEEN_ARRESTED);
		Opcodes::RegisterOpcode(0x0135, CHANGE_CAR_LOCK);
		Opcodes::RegisterOpcode(0x0136, SHAKE_CAM_WITH_POINT);
		Opcodes::RegisterOpcode(0x015E, IS_CAR_IN_AIR);
		Opcodes::RegisterOpcode(0x0178, IS_PLAYER_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x0179, IS_CHAR_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x017B, SET_CHAR_AMMO);
		Opcodes::RegisterOpcode(0x021D, SET_FREE_BOMBS);
		Opcodes::RegisterOpcode(0x021F, SET_ALL_TAXI_LIGHTS);
		Opcodes::RegisterOpcode(0x0220, IS_CAR_ARMED_WITH_ANY_BOMB);
		Opcodes::RegisterOpcode(0x0228, IS_CAR_ARMED_WITH_BOMB);
		Opcodes::RegisterOpcode(0x023A, IS_PLAYER_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x023B, IS_CHAR_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x0242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x029C, IS_BOAT);
		Opcodes::RegisterOpcode(0x0299, ACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x02B9, DEACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x02BC, SET_SWAT_REQUIRED);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x02A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x02F0, DROP_MINE);
		Opcodes::RegisterOpcode(0x02F1, DROP_NAUTICAL_MINE);
		Opcodes::RegisterOpcode(0x031B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x031C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x0367, START_KILL_FRENZY_HEADSHOT);
		Opcodes::RegisterOpcode(0x03C6, IS_COLLISION_IN_MEMORY);
		Opcodes::RegisterOpcode(0x03C9, IS_CAR_VISIBLY_DAMAGED);
		Opcodes::RegisterOpcode(0x0413, SET_GET_OUT_OF_JAIL_FREE);
		Opcodes::RegisterOpcode(0x047D, GET_NUMBER_OF_SEATS_IN_MODEL);
		Opcodes::RegisterOpcode(0x04A7, IS_CHAR_IN_ANY_BOAT);
		Opcodes::RegisterOpcode(0x04A9, IS_CHAR_IN_ANY_HELI);
		Opcodes::RegisterOpcode(0x04AB, IS_CHAR_IN_ANY_PLANE);
		Opcodes::RegisterOpcode(0x04C8, IS_CHAR_IN_FLYING_VEHICLE);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x052D, GET_PLAYER_DRUNKENNESS);
		Opcodes::RegisterOpcode(0x0530, ADD_LOAN_SHARK_VISITS);
		Opcodes::RegisterOpcode(0x0532, ADD_MOVIE_STUNTS);
		Opcodes::RegisterOpcode(0x0535, ADD_GARBAGE_PICKUPS);
		Opcodes::RegisterOpcode(0x0537, SET_TOP_SHOOTING_RANGE_SCORE);
		Opcodes::RegisterOpcode(0x0538, ADD_SHOOTING_RANGE_RANK);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
	}
	return TRUE;
}

#else if _III
#pragma comment (lib, "..\\..\\..\\III.CLEO.lib")
#include "..\\..\\..\\III.CLEO.h"

tScriptVar *Params;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
	}
	return TRUE;
}

#endif
