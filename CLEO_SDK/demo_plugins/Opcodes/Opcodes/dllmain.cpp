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

enum gameVersion{ V1_0, V1_1, VSTEAM, VSTEAMENC, VUNKOWN = -1 };

int getGameVersion()
{
	int version = -1;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x74FF5064:
		version = V1_0;
		break;
	case 0x00408DC0:
		version = V1_1;
		break;
	case 0x00004824:
		version = VSTEAM;
		break;
	case 0x24E58287:
		version = VSTEAMENC;
		break;
	default:
		version = VUNKOWN;
		break;
	}
	return version;
}

uintptr_t getPedStruct(unsigned int param)
{
	void *func = NULL;
	void *pedPool = NULL;
	uintptr_t ped = NULL;
	switch (getGameVersion()) {
	case V1_0:
		func = (void *)0x451CB0;
		pedPool = *(void **)0x97F2AC;
		break;
	case V1_1:
		func = (void *)0x451CB0;
		pedPool = *(void **)0x97F2B4;
		break;
	case VSTEAM:
		func = (void *)0x97E2B4;
		pedPool = *(void **)0xA0EDEC;
		break;
	}
	__asm push param // ped handle
	__asm mov ecx, pedPool // this ped pool
	__asm call func // cpool_cped_cplayerped::getat
	__asm mov ped, eax // get ped struct
	return ped;
}

uintptr_t getVehicleStruct(unsigned int param)
{
	void *func = NULL;
	void *carPool = NULL;
	uintptr_t car = NULL;
	switch (getGameVersion()) {
	case V1_0:
		func = (void *)0x451C70;
		carPool = *(void **)0xA0FDE4;
		break;
	case V1_1:
		func = (void *)0x451C70;
		carPool = *(void **)0xA0FDEC;
		break;
	case VSTEAM:
		func = (void *)0x451B50;
		carPool = *(void **)0xA0EDEC;
		break;
	}
	__asm push param // vehicle handle
	__asm mov ecx, carPool // this vehicle pool
	__asm call func // cpool_cvehicle_cautomobile::getat
	__asm mov car, eax // get vehicle struct
	return car;
}

DWORD getPlayerPed(unsigned int param)
{
	uintptr_t *address = NULL;
	switch (getGameVersion()) {
	case V1_0:
		address = (uintptr_t *)0x94AD28;
		break;
	case V1_1:
		address = (uintptr_t *)0x94AD30;
		break;
	case VSTEAM: // todo steam address
		break;
	}
	return address[0x2E * param];
}

/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	uintptr_t ped = getPedStruct(Params[0].nVar);
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
	uintptr_t car = getVehicleStruct(Params[0].nVar);
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
	void *func = NULL;
	WORD func_result;
	switch (getGameVersion()) {
	case V1_0:
	case V1_1:
		func = (void *)0x460C00;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	unsigned int param = Params[1].nVar;
	__asm push param // state
	param = Params[0].nVar;
	__asm push param // pad
	__asm call func
	__asm mov func_result, ax
	Params[0].nVar = (unsigned int)func_result;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0113 */
eOpcodeResult WINAPI ADD_AMMO_TO_PLAYER(CScript *script)
{
	script->Collect(3);
	void *func = NULL;
	switch (getGameVersion()) {
	case V1_0:
		func = (void *)0x4FF840;
		break;
	case V1_1:
		func = (void *)0x4FF860;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	unsigned int param = Params[2].nVar;
	__asm push param // ammo
	param = Params[1].nVar;
	__asm push param // weapon
	DWORD player = getPlayerPed(Params[0].nVar);
	__asm mov ecx, player // this player
	__asm call func // cped::grantammo
	return OR_CONTINUE;
}

/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	uintptr_t *address = NULL;
	switch (getGameVersion()) {
	case V1_0:
		address = (uintptr_t *)0x94ADF4;
		break;
	case V1_1:
		address = (uintptr_t *)0x94ADFC;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	if ((address[0x2E * Params[0].nVar]) == 2) {
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
	if ((car = getVehicleStruct(Params[0].nVar)) != NULL) {
		*(DWORD *)(car + 0x230) = Params[1].nVar;
	}
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	void *func = NULL;
	void *camera = NULL;
	switch (getGameVersion()) {
	case V1_0:
		func = (void *)0x46FF21;
		camera = (void *)0x7E4688;
		break;
	case V1_1:
		func = (void *)0x46FF21;
		camera = (void *)0x7E4690;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	float param = Params[3].fVar;
	__asm push param // z
	param = Params[2].fVar;
	__asm push param // y
	param = Params[1].fVar;
	__asm push param // x
	param = (float)Params[0].nVar;
	param *= 1e-3f;
	__asm push param // duration
	__asm mov ecx, camera // this camera
	__asm call func // ccamera::camshake
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	uintptr_t car = getVehicleStruct(Params[0].nVar);
	if (*(BYTE *)(car + 0x5C5)) {
		script->UpdateCompareFlag(false);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(true);
	return OR_CONTINUE;
}

/* 017B */
eOpcodeResult WINAPI SET_CHAR_AMMO(CScript *script)
{
	script->Collect(3);
	void *func = NULL;
	switch (getGameVersion()) {
	case V1_0:
		func = (void *)0x4FF780;
		break;
	case V1_1:
		func = (void *)0x4FF7A0;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	unsigned int param = Params[2].nVar;
	__asm push param // ammo
	param = Params[1].nVar;
	__asm push param // weapon
	uintptr_t ped = getPedStruct(Params[0].nVar);
	__asm mov ecx, ped
	__asm call func // cped::setammo
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	void *address = NULL;
	switch (getGameVersion()) {
	case V1_0:
		address = (void *)0xA10B32;
		break;
	case V1_1:
		address = (void *)0xA10B3A;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	DWORD param = Params[0].nVar;
	if (param) {
		param = 1;
	}
	*(BYTE *)address = (BYTE)param;
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	void *address = NULL;
	switch (getGameVersion()) {
	case V1_0:
		address = (void *)0xA10ABB;
		break;
	case V1_1:
		address = (void *)0xA10AC3;
		break;
	default:
		return OR_CONTINUE; // todo steam address
	}
	DWORD param = Params[0].nVar;
	if (param) {
		param = 1;
	}
	*(BYTE *)address = (BYTE)param;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	uintptr_t car = getVehicleStruct(Params[0].nVar);
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
	uintptr_t car = getVehicleStruct(Params[0].nVar);
	if ((*(BYTE *)(car + 0x1FE) & 7) == Params[1].nVar) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0242 */
eOpcodeResult WINAPI ARM_CAR_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	uintptr_t car = getVehicleStruct(Params[0].nVar);
	BYTE input = ((BYTE)Params[1].nVar) & 7;
	BYTE state = (*(BYTE *)(car + 0x1FE) & 0xF8);
	state |= input;
	*(BYTE *)(car + 0x1FE) = state;
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	if ((DWORD)*(BYTE *)(getVehicleStruct(Params[0].nVar) + 0x1A0) == Params[1].nVar) {
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
	if ((DWORD)*(BYTE *)(getVehicleStruct(Params[0].nVar) + 0x1A1) == Params[1].nVar) {
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
	uintptr_t ped = getPedStruct(Params[0].nVar);
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
		Opcodes::RegisterOpcode(0x0130, HAS_PLAYER_BEEN_ARRESTED);
		Opcodes::RegisterOpcode(0x0135, CHANGE_CAR_LOCK);
		Opcodes::RegisterOpcode(0x0136, SHAKE_CAM_WITH_POINT);
		Opcodes::RegisterOpcode(0x015E, IS_CAR_IN_AIR);
		Opcodes::RegisterOpcode(0x017B, SET_CHAR_AMMO);
		Opcodes::RegisterOpcode(0x021D, SET_FREE_BOMBS);
		Opcodes::RegisterOpcode(0x021F, SET_ALL_TAXI_LIGHTS);
		Opcodes::RegisterOpcode(0x0220, IS_CAR_ARMED_WITH_ANY_BOMB);
		Opcodes::RegisterOpcode(0x0228, IS_CAR_ARMED_WITH_BOMB);
		Opcodes::RegisterOpcode(0x0242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x031B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x031C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x04A7, IS_CHAR_IN_ANY_BOAT);
	}
	return TRUE;
}
