// configuration manager
// active solution configuration: VC
// active solution platform: x86

#include "stdafx.h"
#if _VC
#pragma comment (lib, "..\\..\\..\\VC.CLEO.lib")
#include "..\\..\\..\\VC.CLEO.h"
#else if _III
#pragma comment (lib, "..\\..\\..\\III.CLEO.lib")
#include "..\\..\\..\\III.CLEO.h"
#endif

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  3

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

enum gameVersion{ V1_0, V1_1, VSTEAM, VUNKOWN = -1 };

class GtaGame
{
public:
	gameVersion version;
	GtaGame();
};

GtaGame game;

void(__cdecl *CreatePickup)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD); // cpickups::generatenewone
uintptr_t(__thiscall *ObjectPoolGetStruct)(void *, DWORD); // cpool_cobject_ccutscenehead::getat
uintptr_t(__thiscall *VehiclePoolGetStruct)(void *, DWORD); // cpool_cvehicle_cautomobile::getat
uintptr_t(__thiscall *PedPoolGetStruct)(void *, DWORD); // cpool_cped_cplayerped::getat
WORD(__thiscall *GetPadState)(CScript *, DWORD, DWORD); // crunningscript::getpadstate
void(__thiscall *CamShake)(void *, FLOAT, FLOAT, FLOAT, FLOAT); // ccamera::camshake
BYTE(__thiscall *GetHasCollidedWith)(DWORD, uintptr_t); // cphysical::gethascollidedwith
FLOAT(__cdecl *FindGroundZForCoord)(FLOAT, FLOAT); // cworld::findgroundzforcoord
void(__thiscall *SetAmmo)(uintptr_t, DWORD, DWORD); // cped::setammo
void(__thiscall *GrantAmmo)(DWORD, DWORD, DWORD); // cped::grantammo
DWORD *barrel1 = NULL;
DWORD *barrel2 = NULL;
DWORD *maxWantedLevel = NULL;
void *camera = NULL;
uintptr_t *playerPedPool = NULL;
uintptr_t *playerPedState = NULL;
void **objectPool = NULL;
void **pedPool = NULL;
void *allTaxiLights = NULL;
void *freeBombs = NULL;
void **carPool = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x74FF5064:
		version = V1_0;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451C30;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451CB0;
		GetPadState = (WORD(__thiscall *)(CScript *, DWORD, DWORD))0x460C00;
		CamShake = (void(__thiscall *)(void *, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))0x4B9010;
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))0x4D5540;
		SetAmmo = (void(__thiscall *)(uintptr_t, DWORD, DWORD))0x4FF780;
		GrantAmmo = (void(__thiscall *)(DWORD, DWORD, DWORD))0x4FF840;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		camera = (void *)0x7E4688;
		playerPedPool = (uintptr_t *)0x94AD28;
		playerPedState = (uintptr_t *)0x94ADF4;
		objectPool = (void **)0x94DBE0;
		pedPool = (void **)0x97F2AC;
		allTaxiLights = (void *)0xA10ABB;
		freeBombs = (void *)0xA10B32;
		carPool = (void **)0xA0FDE4;
		break;
	case 0x00408DC0:
		version = V1_1;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451C30;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451CB0;
		GetPadState = (WORD(__thiscall *)(CScript *, DWORD, DWORD))0x460C00;
		CamShake = (void(__thiscall *)(void *, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))(0x4B9010 + 0x20);
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))(0x4D5540 + 0x20);
		SetAmmo = (void(__thiscall *)(uintptr_t, DWORD, DWORD))(0x4FF780 + 0x20);
		GrantAmmo = (void(__thiscall *)(DWORD, DWORD, DWORD))(0x4FF840 + 0x20);
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		camera = (void *)(0x7E4688 + 8);
		playerPedPool = (uintptr_t *)(0x94AD28 + 8);
		playerPedState = (uintptr_t *)(0x94ADF4 + 8);
		objectPool = (void **)(0x94DBE0 + 8);
		pedPool = (void **)(0x97F2AC + 8);
		allTaxiLights = (void *)(0xA10ABB + 8);
		freeBombs = (void *)(0xA10B32 + 8);
		carPool = (void **)(0xA0FDE4 + 8);
		break;
	case 0x00004824:
		version = VSTEAM;
		ObjectPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451B10;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451B50;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, DWORD))0x451B90;
		objectPool = (void **)0x94CBE8;
		pedPool = (void **)0x97E2B4;
		carPool = (void **)0xA0EDEC;
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
	CamShake(camera, ((float)Params[0].nVar) * 1e-3f, Params[1].fVar, Params[2].fVar, Params[3].fVar);
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (*(BYTE *)(car + 0x5C5)) {
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
	*(BYTE *)freeBombs = (BYTE)param;
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
	*(BYTE *)allTaxiLights = (BYTE)param;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (*(BYTE *)(car + 0x1FE) & 7) {
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
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if ((*(BYTE *)(car + 0x1FE) & 7) == Params[1].nVar) {
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
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (*(DWORD *)(car + 0x29C) == 1) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
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
	if (Params[2].fVar == -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CreatePickup(Params[0].fVar, Params[1].fVar, Params[2].fVar, *barrel1, 9, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02F1 */
eOpcodeResult WINAPI DROP_NAUTICAL_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar == -100.0) {
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
		Opcodes::RegisterOpcode(0x02A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x02F0, DROP_MINE);
		Opcodes::RegisterOpcode(0x02F1, DROP_NAUTICAL_MINE);
		Opcodes::RegisterOpcode(0x031B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x031C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x04A7, IS_CHAR_IN_ANY_BOAT);
		Opcodes::RegisterOpcode(0x04A9, IS_CHAR_IN_ANY_HELI);
		Opcodes::RegisterOpcode(0x04AB, IS_CHAR_IN_ANY_PLANE);
		Opcodes::RegisterOpcode(0x04C8, IS_CHAR_IN_FLYING_VEHICLE);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
	}
	return TRUE;
}
