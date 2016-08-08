/*************************************************************************
	Opcodes Restoration Project

	Plugin for CLEO v2.0.0.4 and above for Grand Theft Auto III and Grand
	Theft Auto: Vice City that aims to restore functionality to many
	unsupported opcodes
	http://www.gtamodding.com/wiki/Opcodes_Restoration_Project
*************************************************************************/

// configuration manager
// active solution configuration: VC or III
// active solution platform: x86

#include "stdafx.h"
#include <math.h>

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  4

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

enum gameVersion{ V1_0, V1_1, VSTEAM, VUNKOWN = -1 };

class GtaGame
{
public:
	gameVersion version;
	GtaGame();
};

GtaGame game;

#if _VC
#pragma comment (lib, "..\\..\\III.VC.CLEO\\CLEO_SDK\\VC.CLEO.lib")
#include "..\\..\\III.VC.CLEO\\CLEO_SDK\\VC.CLEO.h"

#pragma comment (lib, "..\\Release (Without WPO)\\vcclasses.lib")
#pragma comment (lib, "..\\Release (Without WPO)\\vcversion.lib")
#include "..\\..\\gtalc-dinput8\\vcclasses\\include\\vcclasses.h"
#include "..\\..\\gtalc-dinput8\\vcclasses\\include\\Globals.h"
#include "..\\..\\gtalc-dinput8\\vcversion\\include\\vcversion.h"

tScriptVar *Params;

class CCranesHack : public CCranes
{
public:
	static bool IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle);
	static void DeActivateCrane(float positionX, float positionY);
	static void ActivateCrane(float pickupX1, float pickupX2, float pickupY1, float pickupY2, float dropoffX, float dropoffY, float dropoffZ, float dropoffHeading, bool isCrusher, bool isMilitary, float positionX, float positionY);
};

class CCraneHack : public CCrane
{
public:
	bool DoesCranePickUpThisCarType(unsigned int model);
};

class CExplosionHack : public CExplosion
{
public:
	static bool TestForExplosionInArea(int explosionType, float x1, float x2, float y1, float y2, float z1, float z2);
};

DWORD(__cdecl *RpAnimBlendClumpGetFirstAssociation)(uintptr_t); // rpanimblendclumpgetfirstassociation
void(__thiscall *SetPhoneMessage_JustOnce)(uintptr_t, int, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *); // cphoneinfo::setphonemessage_justonce
void(__cdecl *OverrideNextRestart)(CVector *, float); // crestart::overridenextrestart
void(__thiscall *RemoveEntityFromList)(uintptr_t, int, unsigned char); // cmissioncleanup::removeentityfromlist
uintptr_t(__thiscall *VehiclePoolGetStruct)(void *, int); // cpool_cvehicle_cautomobile::getat
uintptr_t(__thiscall *PedPoolGetStruct)(void *, int); // cpool_cped_cplayerped::getat
int(__thiscall *PedPoolGetHandle)(void *, void *); // cpool_cped_cplayerped::getindex
void(__cdecl *HighlightImportantArea)(CScript *, float, float, float, float, float); // cthescripts::highlightimportantarea
WORD(__thiscall *GetPadState)(CScript *, unsigned short, unsigned short); // crunningscript::getpadstate
bool(__thiscall *IsWithinArea_3d)(uintptr_t, float, float, float, float, float, float); // cplaceable::iswithinarea
bool(__thiscall *IsWithinArea_2d)(uintptr_t, float, float, float, float); // cplaceable::iswithinarea
DWORD(__cdecl *FindPlayerPed)(void); // findplayerped
void(__thiscall *RegisterReference)(uintptr_t, uintptr_t); // centity::registerreference
void(__cdecl *AnotherKillFrenzyPassed)(void); // cstats::anotherkillfrenzypassed
DWORD(__cdecl *Remove)(DWORD); // cworld::remove
void(__thiscall *GrantAmmo)(uintptr_t, DWORD, unsigned int); // cped::grantammo
void(__thiscall *GiveWeapon)(uintptr_t, DWORD, DWORD, DWORD); // cped::giveweapon
DWORD(__cdecl *GetNumberOfPassengerSeats)(DWORD);
DWORD(__cdecl *GetWeaponInfo)(DWORD); // cweaponinfo::getweaponinfo
bool *isNastyGame = NULL;
DWORD *barrel1 = NULL;
DWORD *barrel2 = NULL;
DWORD *maxWantedLevel = NULL;
BYTE *phoneDisplayMessage = NULL;
DWORD *currentPhone = NULL;
uintptr_t cprojectileinfo = NULL;
uintptr_t gangPedModelOverride = NULL;
uintptr_t textDraw = NULL;
uintptr_t infoZoneCarDensity = NULL;
uintptr_t infoZonePedDensity = NULL;
uintptr_t cgarage = NULL;
uintptr_t cphoneinfo = NULL;
wchar_t *numberedText = NULL;
uintptr_t infoZone = NULL;
uintptr_t pickupEntity = NULL;
uintptr_t *playerPedPool = NULL;
uintptr_t *playerPedState = NULL;
uintptr_t *projectileObject = NULL;
uintptr_t navigZone = NULL;
void **objectPool = NULL;
float *shootingRangeRank = NULL;
float *garbagePickups = NULL;
float *loanSharkVisits = NULL;
uintptr_t cmissioncleanup = NULL;
void **pedPool = NULL;
uintptr_t cfire = NULL;
float *topShootingRangeScore = NULL;
int *levelName = NULL;
uintptr_t cpedtype = NULL;
float *movieStunts = NULL;
void **carPool = NULL;
WORD *numInfoZone = NULL;
WORD *currentTextDraw = NULL;
WORD *numNavigZone = NULL;
BYTE *allTaxiLights = NULL;
BYTE *overrideHospital = NULL;
BYTE *overridePolice = NULL;
BYTE *currentPlayer = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x74FF5064:
		version = V1_0;
		RpAnimBlendClumpGetFirstAssociation = (DWORD(__cdecl *)(uintptr_t))0x402E20;
		SetPhoneMessage_JustOnce = (void(__thiscall *)(uintptr_t, int, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *))0x43C430;
		OverrideNextRestart = (void(__cdecl *)(CVector *, float))0x4429E0;
		RemoveEntityFromList = (void(__thiscall *)(uintptr_t, int, unsigned char))0x4518E0;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x451CB0;
		PedPoolGetHandle = (int(__thiscall *)(void *, void *))0x451CF0;
		HighlightImportantArea = (void(__cdecl *)(CScript *, float, float, float, float, float))0x45F080;
		GetPadState = (WORD(__thiscall *)(CScript *, unsigned short, unsigned short))0x460C00;
		IsWithinArea_3d = (bool(__thiscall *)(uintptr_t, float, float, float, float, float, float))0x4BB900;
		IsWithinArea_2d = (bool(__thiscall *)(uintptr_t, float, float, float, float))0x4BB9E0;
		FindPlayerPed = (DWORD(__cdecl *)(void))0x4BC120;
		RegisterReference = (void(__thiscall *)(uintptr_t, uintptr_t))0x4C6AC0;
		AnotherKillFrenzyPassed = (void(__cdecl *)(void))0x4CDBA7;
		Remove = (DWORD(__cdecl *)(DWORD))0x4DB310;
		GrantAmmo = (void(__thiscall *)(uintptr_t, DWORD, unsigned int))0x4FF840;
		GiveWeapon = (void(__thiscall *)(uintptr_t, DWORD, DWORD, DWORD))0x4FFA30;
		GetNumberOfPassengerSeats = (DWORD(__cdecl *)(DWORD))0x578A70;
		GetWeaponInfo = (DWORD(__cdecl *)(DWORD))0x5D5710;
		isNastyGame = (bool *)0x68DD68;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		phoneDisplayMessage = (BYTE *)0x7030E4;
		currentPhone = (DWORD *)0x7030E8;
		cprojectileinfo = 0x7DB888;
		gangPedModelOverride = 0x7D925C;
		textDraw = 0x7F0EA0;
		infoZoneCarDensity = 0x7FA370;
		infoZonePedDensity = 0x7FA39C;
		cgarage = 0x812668;
		cphoneinfo = 0x817CF0;
		numberedText = (wchar_t *)0x821068;
		infoZone = 0x862508;
		pickupEntity = 0x945D40;
		playerPedPool = (uintptr_t *)0x94AD28;
		playerPedState = (uintptr_t *)0x94ADF4;
		projectileObject = (uintptr_t *)0x94B708;
		navigZone = 0x94B990;
		objectPool = (void **)0x94DBE0;
		shootingRangeRank = (float *)0x974B08;
		garbagePickups = (float *)0x974C00;
		loanSharkVisits = (float*)0x974C28;
		cmissioncleanup = 0x97F060;
		pedPool = (void **)0x97F2AC;
		cfire = 0x97F8A0;
		topShootingRangeScore = (float *)0xA0D8A4;
		levelName = (int *)0xA0D9AC;
		cpedtype = (uintptr_t)0xA0DA64;
		movieStunts = (float *)0xA0FC8C;
		carPool = (void **)0xA0FDE4;
		numInfoZone = (WORD *)0xA1096A;
		currentTextDraw = (WORD *)0xA10A48;
		numNavigZone = (WORD *)0xA10A58;
		allTaxiLights = (BYTE *)0xA10ABB;
		overrideHospital = (BYTE *)0xA10AE6;
		overridePolice = (BYTE *)0xA10AEA;
		currentPlayer = (BYTE *)0xA10AFB;
		break;
	case 0x00408DC0:
		version = V1_1;
		RpAnimBlendClumpGetFirstAssociation = (DWORD(__cdecl *)(uintptr_t))0x402E20;
		SetPhoneMessage_JustOnce = (void(__thiscall *)(uintptr_t, int, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *))0x43C430;
		OverrideNextRestart = (void(__cdecl *)(CVector *, float))0x4429E0;
		RemoveEntityFromList = (void(__thiscall *)(uintptr_t, int, unsigned char))0x4518E0;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x451C70;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x451CB0;
		PedPoolGetHandle = (int(__thiscall *)(void *, void *))0x451CF0;
		HighlightImportantArea = (void(__cdecl *)(CScript *, float, float, float, float, float))0x45F080;
		GetPadState = (WORD(__thiscall *)(CScript *, unsigned short, unsigned short))0x460C00;
		IsWithinArea_3d = (bool(__thiscall *)(uintptr_t, float, float, float, float, float, float))(0x4BB900 + 0x20);
		IsWithinArea_2d = (bool(__thiscall *)(uintptr_t, float, float, float, float))(0x4BB9E0 + 0x20);
		FindPlayerPed = (DWORD(__cdecl *)(void))(0x4BC120 + 0x20);
		RegisterReference = (void(__thiscall *)(uintptr_t, uintptr_t))(0x4C6AC0 + 0x20);
		AnotherKillFrenzyPassed = (void(__cdecl *)(void))(0x4CDBA7 + 0x20);
		Remove = (DWORD(__cdecl *)(DWORD))(0x4DB310 + 0x20);
		GrantAmmo = (void(__thiscall *)(uintptr_t, DWORD, unsigned int))(0x4FF840 + 0x20);
		GiveWeapon = (void(__thiscall *)(uintptr_t, DWORD, DWORD, DWORD))(0x4FFA30 + 0x20);
		GetNumberOfPassengerSeats = (DWORD(__cdecl *)(DWORD))(0x578A70 + 0x20);
		GetWeaponInfo = (DWORD(__cdecl *)(DWORD))(0x5D5710 + 0x20);
		isNastyGame = (bool *)0x68DD68;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		phoneDisplayMessage = (BYTE *)0x7030E4;
		currentPhone = (DWORD *)0x7030E8;
		cprojectileinfo = 0x7DB888 + 8;
		gangPedModelOverride = 0x7D925C + 8;
		textDraw = 0x7F0EA0 + 8;
		infoZoneCarDensity = 0x7FA370 + 8;
		infoZonePedDensity = 0x7FA39C + 8;
		cgarage = 0x812668 + 8;
		cphoneinfo = 0x817CF0 + 8;
		numberedText = (wchar_t *)(0x821068 + 8);
		infoZone = 0x862508 + 8;
		pickupEntity = 0x945D40 + 8;
		playerPedPool = (uintptr_t *)(0x94AD28 + 8);
		playerPedState = (uintptr_t *)(0x94ADF4 + 8);
		projectileObject = (uintptr_t *)(0x94B708 + 8);
		navigZone = 0x94B990 + 8;
		objectPool = (void **)(0x94DBE0 + 8);
		shootingRangeRank = (float *)(0x974B08 + 8);
		garbagePickups = (float *)(0x974C00 + 8);
		loanSharkVisits = (float*)(0x974C28 + 8);
		cmissioncleanup = 0x97F060 + 8;
		pedPool = (void **)(0x97F2AC + 8);
		cfire = 0x97F8A0 + 8;
		topShootingRangeScore = (float *)(0xA0D8A4 + 8);
		levelName = (int *)(0xA0D9AC + 8);
		cpedtype = (uintptr_t)(0xA0DA64 + 8);
		movieStunts = (float *)(0xA0FC8C + 8);
		carPool = (void **)(0xA0FDE4 + 8);
		numInfoZone = (WORD *)(0xA1096A + 8);
		currentTextDraw = (WORD *)(0xA10A48 + 8);
		numNavigZone = (WORD *)(0xA10A58 + 8);
		allTaxiLights = (BYTE *)(0xA10ABB + 8);
		overrideHospital = (BYTE *)(0xA10AE6 + 8);
		overridePolice = (BYTE *)(0xA10AEA + 8);
		currentPlayer = (BYTE *)(0xA10AFB + 8);
		break;
	case 0x00004824:
		version = VSTEAM;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, int))(0x451C70 - 0x120);
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, int))(0x451CB0 - 0x120);
		cprojectileinfo = 0x7DB888 - 0xFF8;
		gangPedModelOverride = 0x7D925C - 0xFF8;
		textDraw = 0x7F0EA0 - 0xFF8;
		infoZoneCarDensity = 0x7FA370 - 0xFF8;
		infoZonePedDensity = 0x7FA39C - 0xFF8;
		cgarage = 0x812668 - 0xFF8;
		cphoneinfo = 0x817CF0 - 0xFF8;
		numberedText = (wchar_t *)(0x821068 - 0xFF8);
		infoZone = 0x862508 - 0xFF8;
		pickupEntity = 0x945D40 - 0xFF8;
		playerPedPool = (uintptr_t *)(0x94AD28 - 0xFF8);
		playerPedState = (uintptr_t *)(0x94ADF4 - 0xFF8);
		projectileObject = (uintptr_t *)(0x94B708 - 0xFF8);
		navigZone = 0x94B990 - 0xFF8;
		objectPool = (void **)(0x94DBE0 - 0xFF8);
		shootingRangeRank = (float *)(0x974B08 - 0xFF8);
		garbagePickups = (float *)(0x974C00 - 0xFF8);
		loanSharkVisits = (float*)(0x974C28 - 0xFF8);
		cmissioncleanup = 0x97F060 - 0xFF8;
		pedPool = (void **)(0x97F2AC - 0xFF8);
		cfire = 0x97F8A0 - 0xFF8;
		topShootingRangeScore = (float *)(0xA0D8A4 - 0xFF8);
		levelName = (int *)(0xA0D9AC - 0xFF8);
		cpedtype = (uintptr_t)(0xA0DA64 - 0xFF8);
		movieStunts = (float *)(0xA0FC8C - 0xFF8);
		carPool = (void **)(0xA0FDE4 - 0xFF8);
		numInfoZone = (WORD *)(0xA1096A - 0xFF8);
		currentTextDraw = (WORD *)(0xA10A48 - 0xFF8);
		numNavigZone = (WORD *)(0xA10A58 - 0xFF8);
		break;
	}
}

bool CCranesHack::IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle)
{
	float craneObjectX, craneObjectY, distance;
	if (numCranes > 0) {
		for (int i = 0; i < numCranes; i++) {
			craneObjectX = *(float *)(cranes[i].object + 0x34);
			craneObjectY = *(float *)(cranes[i].object + 0x38);
			distance = sqrt(pow(positionX - craneObjectX, 2) + pow(positionY - craneObjectY, 2));
			if (distance < 100.0 && cranes[i].vehicle == vehicle && (cranes[i].status == 2 || cranes[i].status == 4)) {
				return true;
			}
		}
	}
	return false;
}

void CCranesHack::DeActivateCrane(float positionX, float positionY)
{
	int index = -1;
	float craneObjectX, craneObjectY, minDistance, distance;
	minDistance = 100.0;
	if (numCranes > 0) {
		for (int i = 0; i < numCranes; i++) {
			craneObjectX = *(float *)(cranes[i].object + 0x34);
			craneObjectY = *(float *)(cranes[i].object + 0x38);
			distance = sqrt(pow(positionX - craneObjectX, 2) + pow(positionY - craneObjectY, 2));
			if (distance < minDistance) {
				index = i;
				minDistance = distance;
			}
		}
	}
	if (index == -1) return;
	cranes[index].activity = 2;
	cranes[index].status = 0;
}

void CCranesHack::ActivateCrane(float pickupX1, float pickupX2, float pickupY1, float pickupY2, float dropoffX, float dropoffY, float dropoffZ, float dropoffHeading, bool isCrusher, bool isMilitary, float positionX, float positionY)
{
	int index = -1;
	float craneObjectX, craneObjectY, minDistance, distance, pickupCenterX, pickupCenterY;
	minDistance = 100.0;
	if (numCranes > 0) {
		for (int i = 0; i < numCranes; i++) {
			craneObjectX = *(float *)(cranes[i].object + 0x34);
			craneObjectY = *(float *)(cranes[i].object + 0x38);
			distance = sqrt(pow(positionX - craneObjectX, 2) + pow(positionY - craneObjectY, 2));
			if (distance < minDistance) {
				index = i;
				minDistance = distance;
			}
		}
	}
	if (index == -1) return;
	cranes[index].pickupX1 = pickupX1;
	cranes[index].pickupX2 = pickupX2;
	cranes[index].pickupY1 = pickupY1;
	cranes[index].pickupY2 = pickupY2;
	cranes[index].dropoffX = dropoffX;
	cranes[index].dropoffY = dropoffY;
	cranes[index].dropoffZ = dropoffZ;
	cranes[index].activity = 1;
	cranes[index].vehicle = NULL;
	cranes[index].countCollected = 0;
	cranes[index].dropoffHeading = dropoffHeading;
	cranes[index].isCrusher = isCrusher;
	cranes[index].isMilitary = isMilitary;
	cranes[index].timer = 0;
	cranes[index].status = 0;
	pickupCenterX = (pickupX1 + pickupX2) / 2;
	pickupCenterY = (pickupY1 + pickupY2) / 2;
	craneObjectX = *(float *)(cranes[index].object + 0x34);
	craneObjectY = *(float *)(cranes[index].object + 0x38);
	/*if (isCrusher) {
		cranes[index].armPickupHeight = -0.95099998f;
	} else if (isMilitary) {
		cranes[index].armPickupHeight = 10.7862f;
	} else {
		cranes[index].armPickupHeight = CWorld::FindGroundZForCoord(pickupCenterX, pickupCenterY);
	}*/
	cranes[index].armPickupHeight = CWorld::FindGroundZForCoord(pickupCenterX, pickupCenterY);
	cranes[index].armPickupRotation = CGeneral::GetATanOfXY(pickupCenterX - craneObjectX, pickupCenterY - craneObjectY);
	cranes[index].armPickupDistance = sqrt(pow(pickupCenterX - craneObjectX, 2) + pow(pickupCenterY - craneObjectY, 2));
	cranes[index].armDropoffRotation = CGeneral::GetATanOfXY(dropoffX - craneObjectX, dropoffY - craneObjectY);
	cranes[index].armDropoffDistance = sqrt(pow(dropoffX - craneObjectX, 2) + pow(dropoffY - craneObjectY, 2));
	cranes[index].armDropoffHeight = dropoffZ;
}

bool IsProjectileInRange(float x1, float y1, float z1, float x2, float y2, float z2, bool flag)
{
	uintptr_t *object = projectileObject;
	for (uintptr_t info = cprojectileinfo; info < cprojectileinfo + 0x20 * 0x1C; info += 0x1C, object++) {
		if (*object != NULL) {
			if (*(BYTE *)(info + 0xC) != 0 && *(DWORD *)info >= 12 && *(DWORD *)info <= 16) {
				if (*(float *)(*object + 0x34) >= x1 && *(float *)(*object + 0x34) <= x2) {
					if (*(float *)(*object + 0x38) >= y1 && *(float *)(*object + 0x38) <= y2) {
						if (*(float *)(*object + 0x3C) >= z1 && *(float *)(*object + 0x3C) <= z2) {
							if (flag) {
								*(BYTE *)(info + 0xC) = 0;
								auto Destroy = (void(__thiscall *)(uintptr_t, int))*(uintptr_t *)((*(uintptr_t *)(*object)) + 8);
								Remove(*object);
								Destroy(*object, 1);
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void SetPhoneMessage_Repeatedly(uintptr_t cphoneinfo, int phone, wchar_t *string1, wchar_t *string2, wchar_t *string3, wchar_t *string4, wchar_t *string5, wchar_t *string6)
{
	phone = phone * 0x34 + cphoneinfo;
	*(wchar_t **)(phone + 0x14) = string1;
	*(wchar_t **)(phone + 0x18) = string2;
	*(wchar_t **)(phone + 0x1C) = string3;
	*(wchar_t **)(phone + 0x20) = string4;
	*(wchar_t **)(phone + 0x24) = string5;
	*(wchar_t **)(phone + 0x28) = string6;
	*(unsigned int *)(phone + 0x34) = 5;
}

bool CExplosionHack::TestForExplosionInArea(int explosionType, float x1, float x2, float y1, float y2, float z1, float z2)
{
	float temp;
	if (x1 > x2) {
		temp = x2;
		x2 = x1;
		x1 = temp;
	}
	if (y1 > y2) {
		temp = y2;
		y2 = y1;
		y1 = temp;
	}
	if (z1 > z2) {
		temp = z2;
		z2 = z1;
		z1 = temp;
	}
	for (int i = 0; i < 0x30; i++) {
		if (explosions[i].counter) {
			if (explosions[i].type == explosionType) {
				if (explosions[i].position.x >= x1 && explosions[i].position.x <= x2) {
					if (explosions[i].position.y >= y1 && explosions[i].position.y <= y2) {
						if (explosions[i].position.z >= z1 && explosions[i].position.z <= z2) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	if (ped) {
		if (ped->state != 0x37 && ped->state != 0x36) {
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
	uintptr_t car = (uintptr_t)CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	if (car) {
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
	Params[0].nVar = (unsigned int)GetPadState(script, (unsigned short)Params[0].nVar, (unsigned short)Params[1].nVar);
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
	script->UpdateCompareFlag(playerPedState[0x2E * Params[0].nVar] != 1);
	return OR_CONTINUE;
}

/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(playerPedState[0x2E * Params[0].nVar] == 2);
	return OR_CONTINUE;
}

/* 0135 */
eOpcodeResult WINAPI CHANGE_CAR_LOCK(CScript *script)
{
	script->Collect(2);
	if (CVehicle *car = CPools::ms_pVehiclePool->GetAt(Params[0].nVar)) {
		car->lock = Params[1].nVar;
	}
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	VCGlobals::TheCamera.CamShake(((float)Params[0].nVar) * 1e-3f, Params[1].fVar, Params[2].fVar, Params[3].fVar);
	return OR_CONTINUE;
}

/* 013B */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_2D(CScript *script)
{
	script->Collect(6);
	uintptr_t car = (uintptr_t)CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	if (Params[5].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
	}
	if (car) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag == 5 && IsWithinArea_2d(car + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar)) {
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
	uintptr_t car = (uintptr_t)CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	if (Params[7].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[4].fVar, Params[5].fVar, (Params[3].fVar + Params[6].fVar) * 0.5f);
	}
	if (car != NULL) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag == 5 && IsWithinArea_3d(car + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, Params[6].fVar)) {
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
	for (uintptr_t address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			int offset;
			offset = (BYTE)Params[0].nVar ? *(WORD *)(address + 0x28) : *(WORD *)(address + 0x2A);
			offset *= 0x44;
			offset += infoZoneCarDensity;
			*(WORD *)offset = (WORD)Params[1].nVar;
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
	for (uintptr_t address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			int offset;
			offset = (BYTE)Params[0].nVar ? *(WORD *)(address + 0x28) : *(WORD *)(address + 0x2A);
			offset *= 0x44;
			offset += infoZonePedDensity;
			*(WORD *)offset = (WORD)Params[1].nVar;
		}
	}
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->numberOfWheelsOnGround == 0);
	return OR_CONTINUE;
}

/* 0163 */
eOpcodeResult WINAPI ADD_BLIP_FOR_OBJECT_OLD(CScript *script)
{
	script->Collect(3);
	CPools::ms_pObjectPool->GetAt(Params[0].nVar);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(3, Params[0].nVar, Params[1].nVar, Params[2].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0178 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[Params[0].nVar].playerEntity;
	CPhysical *source = player;
	if (player->isInAnyVehicle) {
		if (player->vehicle) {
			source = player->vehicle;
		}
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(CPools::ms_pObjectPool->GetAt(Params[1].nVar)));
	return OR_CONTINUE;
}

/* 0179 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle) {
		if (ped->vehicle) {
			source = ped->vehicle;
		}
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(CPools::ms_pObjectPool->GetAt(Params[1].nVar)));
	return OR_CONTINUE;
}

/* 017B */
eOpcodeResult WINAPI SET_CHAR_AMMO(CScript *script)
{
	script->Collect(3);
	CPools::ms_pPedPool->GetAt(Params[0].nVar)->SetAmmo(Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 01C6 */
eOpcodeResult WINAPI DONT_REMOVE_CAR(CScript *script)
{
	script->Collect(1);
	CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	RemoveEntityFromList(cmissioncleanup, Params[0].nVar, 1);
	return OR_CONTINUE;
}

/* 01EE */
eOpcodeResult WINAPI ACTIVATE_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (Params[2].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	if (Params[3].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[3].fVar;
		Params[3].fVar = temp;
	}
	Params[9].fVar = Params[9].fVar * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(Params[2].fVar, Params[4].fVar, Params[3].fVar, Params[5].fVar, Params[6].fVar, Params[7].fVar, Params[8].fVar, Params[9].fVar, false, false, Params[0].fVar, Params[1].fVar);
	return OR_CONTINUE;
}

/* 01EF */
eOpcodeResult WINAPI DEACTIVATE_CRANE(CScript *script)
{
	script->Collect(2);
	CCranesHack::DeActivateCrane(Params[0].fVar, Params[1].fVar);
	return OR_CONTINUE;
}

/* 0218 */
eOpcodeResult WINAPI PRINT_WITH_NUMBER_BIG_Q(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(3);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	CMessages::AddBigMessageQ(numberedText, Params[1].nVar, (unsigned short)Params[2].nVar - 1);
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	CGarages::bombsAreFree = !!Params[0].nVar;
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	*allTaxiLights = !!Params[0].nVar;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(!!(CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->bombState & 7));
	return OR_CONTINUE;
}

/* 0228 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag((CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->bombState & 7) == Params[1].nVar);
	return OR_CONTINUE;
}

/* 023A */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[Params[0].nVar].playerEntity;
	script->UpdateCompareFlag(!player->isInAnyVehicle && player->GetHasCollidedWith(CPools::ms_pObjectPool->GetAt(Params[1].nVar)));
	return OR_CONTINUE;
}

/* 023B */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(!ped->isInAnyVehicle && ped->GetHasCollidedWith(CPools::ms_pObjectPool->GetAt(Params[1].nVar)));
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
	*(DWORD *)(car + 0x210) = FindPlayerPed();
	return OR_CONTINUE;
}

/* 024B */
eOpcodeResult WINAPI SET_REPEATED_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, VCGlobals::TheText.Get(gxt), NULL, NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 024C */
eOpcodeResult WINAPI SET_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, VCGlobals::TheText.Get(gxt), NULL, NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 024D */
eOpcodeResult WINAPI HAS_PHONE_DISPLAYED_MESSAGE(CScript *script)
{
	script->Collect(1);
	if (*phoneDisplayMessage == 0) {
		DWORD state = *(DWORD *)(Params[0].nVar * 0x34 + cphoneinfo + 0x34);
		if (state == 6 || state == 7 || state == 8) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0255 */
eOpcodeResult WINAPI RESTART_CRITICAL_MISSION(CScript *script)
{
	script->Collect(4);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	CVector pos;
	pos.x = Params[0].fVar;
	pos.y = Params[1].fVar;
	pos.z = Params[2].fVar;
	OverrideNextRestart(&pos, Params[3].fVar);
	DWORD player = *currentPlayer * 0x170 + (DWORD)playerPedPool;
	if (*(BYTE *)(player + 0xCC) == 0) {
		*(BYTE *)(player + 0xCC) = 3;
		*(DWORD *)(player + 0xD0) = CTimer::m_snTimeInMilliseconds;
		CDarkel::ResetOnPlayerDeath();
	}
	return OR_CONTINUE;
}

/* 0295 */
eOpcodeResult WINAPI IS_TAXI(CScript *script)
{
	script->Collect(1);
	unsigned short model = CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->modelIndex;
	script->UpdateCompareFlag(model == 0x96 || model == 0xA8 || model == 0xBC || model == 0xD8);
	return OR_CONTINUE;
}

/* 0299 */
eOpcodeResult WINAPI ACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	if (CGarages::garages[Params[0].nVar].type == 11 && CGarages::garages[Params[0].nVar].state == 0) {
		CGarages::garages[Params[0].nVar].state = 3;
	}
	CGarages::garages[Params[0].nVar].isInactive = 0;
	return OR_CONTINUE;
}

/* 029C */
eOpcodeResult WINAPI IS_BOAT(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->type == 1);
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

/* 02A4 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CAR(CScript *script)
{
	script->Collect(2);
	CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(1, Params[0].nVar, 0, 3);
	CRadar::SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A5 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CHAR(CScript *script)
{
	script->Collect(2);
	CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(2, Params[0].nVar, 1, 3);
	CRadar::SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A6 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_OBJECT(CScript *script)
{
	script->Collect(2);
	CPools::ms_pObjectPool->GetAt(Params[0].nVar);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(3, Params[0].nVar, 6, 3);
	CRadar::SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02B9 */
eOpcodeResult WINAPI DEACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	CGarages::garages[Params[0].nVar].isInactive = 1;
	return OR_CONTINUE;
}

/* 02BC */
eOpcodeResult WINAPI SET_SWAT_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = VCGlobals::FindPlayerPed();
	player->wanted->activity &= 0xFB;
	if (Params[0].nVar) {
		player->wanted->activity |= 4;
	}
	return OR_CONTINUE;
}

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = VCGlobals::FindPlayerPed();
	player->wanted->activity &= 0xF7;
	if (Params[0].nVar) {
		player->wanted->activity |= 8;
	}
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	CPlayerPed *player = VCGlobals::FindPlayerPed();
	player->wanted->activity &= 0xEF;
	if (Params[0].nVar) {
		player->wanted->activity |= 16;
	}
	return OR_CONTINUE;
}

/* 02D6 */
eOpcodeResult WINAPI IS_CHAR_SHOOTING_IN_AREA(CScript *script)
{
	script->Collect(6);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	BYTE state = *(BYTE *)(ped + 0x14E);
	state >>= 6;
	state &= 1;
	script->UpdateCompareFlag(state != 0 && IsWithinArea_2d(ped + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar) != 0);
	if (Params[5].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
	}
	return OR_CONTINUE;
}

/* 02EE */
eOpcodeResult WINAPI IS_PROJECTILE_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (Params[0].fVar > Params[3].fVar) {
		temp = Params[3].fVar;
		Params[3].fVar = Params[0].fVar;
		Params[0].fVar = temp;
	}
	if (Params[1].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[1].fVar;
		Params[1].fVar = temp;
	}
	if (Params[2].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	script->UpdateCompareFlag(IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 0));
	return OR_CONTINUE;
}

/* 02EF */
eOpcodeResult WINAPI DESTROY_PROJECTILES_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (Params[0].fVar > Params[3].fVar) {
		temp = Params[3].fVar;
		Params[3].fVar = Params[0].fVar;
		Params[0].fVar = temp;
	}
	if (Params[1].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[1].fVar;
		Params[1].fVar = temp;
	}
	if (Params[2].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	script->UpdateCompareFlag(IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 1));
	return OR_CONTINUE;
}

/* 02F0 */
eOpcodeResult WINAPI DROP_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CVector pos;
	pos.x = Params[0].fVar;
	pos.y = Params[1].fVar;
	pos.z = Params[2].fVar;
	CPickups::GenerateNewOne(pos, *barrel1, 9, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02F1 */
eOpcodeResult WINAPI DROP_NAUTICAL_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CVector pos;
	pos.x = Params[0].fVar;
	pos.y = Params[1].fVar;
	pos.z = Params[2].fVar;
	CPickups::GenerateNewOne(pos, *barrel2, 11, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02FB */
eOpcodeResult WINAPI ACTIVATE_CRUSHER_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (Params[2].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	if (Params[3].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[3].fVar;
		Params[3].fVar = temp;
	}
	Params[9].fVar = Params[9].fVar * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(Params[2].fVar, Params[4].fVar, Params[3].fVar, Params[5].fVar, Params[6].fVar, Params[7].fVar, Params[8].fVar, Params[9].fVar, true, false, Params[0].fVar, Params[1].fVar);
	return OR_CONTINUE;
}

/* 02FC */
eOpcodeResult WINAPI PRINT_WITH_2_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(4);
	CMessages::AddMessageWithNumber(VCGlobals::TheText.Get(gxt), Params[2].nVar, (unsigned short)Params[3].nVar, Params[0].nVar, Params[1].nVar, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0300 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[3].nVar, (unsigned short)Params[4].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0303 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[4].nVar, (unsigned short)Params[5].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, -1, -1);
	return OR_CONTINUE;
}

/* 0305 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageWithNumber(VCGlobals::TheText.Get(gxt), Params[5].nVar, (unsigned short)Params[6].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0306 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[5].nVar, (unsigned short)Params[6].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0309 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[6].nVar, (unsigned short)Params[7].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar);
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag((int)CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->firstColour == Params[1].nVar);
	return OR_CONTINUE;
}

/* 031C */
eOpcodeResult WINAPI IS_SECOND_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag((int)CPools::ms_pVehiclePool->GetAt(Params[0].nVar)->secondColour == Params[1].nVar);
	return OR_CONTINUE;
}

/* 0338 */
eOpcodeResult WINAPI SET_CAR_VISIBLE(CScript *script)
{
	script->Collect(2);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	BYTE flag = *(BYTE *)(car + 0x52);
	flag &= 0xFB;
	if (Params[1].nVar) {
		flag |= 4;
	}
	*(BYTE *)(car + 0x52) = flag;
	return OR_CONTINUE;
}

/* 032D */
eOpcodeResult WINAPI SET_CAR_BLOCK_CAR(CScript *script)
{
	script->Collect(2);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	*(DWORD *)(car + 0x19C) = target;
	RegisterReference(target, car + 0x19C);
	*(BYTE *)(car + 0x156) = 0x11;
	BYTE flag = *(BYTE *)(car + 0x1F9);
	flag &= 0xEF;
	flag |= 0x10;
	*(BYTE *)(car + 0x1F9) = flag;
	if (*(BYTE *)(car + 0x160) < 6) {
		*(BYTE *)(car + 0x160) = 6;
	}
	return OR_CONTINUE;
}

/* 0346 */
eOpcodeResult WINAPI SET_TEXT_BACKGROUND_COLOUR(CScript *script)
{
	script->Collect(4);
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x18) = (BYTE)Params[0].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x19) = (BYTE)Params[1].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x1A) = (BYTE)Params[2].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x1B) = (BYTE)Params[3].nVar;
	return OR_CONTINUE;
}

/* 0351 */
eOpcodeResult WINAPI IS_NASTY_GAME(CScript *script)
{
	script->UpdateCompareFlag(*isNastyGame);
	return OR_CONTINUE;
}

/* 0356 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_AREA(CScript *script)
{
	script->Collect(7);
	script->UpdateCompareFlag(CExplosionHack::TestForExplosionInArea(Params[0].nVar, Params[1].fVar, Params[4].fVar, Params[2].fVar, Params[5].fVar, Params[3].fVar, Params[6].fVar));
	return OR_CONTINUE;
}

/* 0357 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_ZONE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	for (uintptr_t address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			if (CExplosionHack::TestForExplosionInArea(Params[0].nVar, *(float *)(address + 8), *(float *)(address + 0x14), *(float *)(address + 0xC), *(float *)(address + 0x18), *(float *)(address + 0x10), *(float *)(address + 0x1C))) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	for (uintptr_t address = navigZone; address < navigZone + *numNavigZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			if (CExplosionHack::TestForExplosionInArea(Params[0].nVar, *(float *)(address + 8), *(float *)(address + 0x14), *(float *)(address + 0xC), *(float *)(address + 0x18), *(float *)(address + 0x10), *(float *)(address + 0x1C))) {
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
	if (Params[7].nVar) {
		Params[7].nVar = 1;
	}
	CDarkel::StartFrenzy(Params[0].nVar, Params[1].nVar, (unsigned short)Params[2].nVar, Params[3].nVar, VCGlobals::TheText.Get(gxt), Params[4].nVar, Params[5].nVar, Params[6].nVar, !!Params[7].nVar, true);
	return OR_CONTINUE;
}

/* 0368 */
eOpcodeResult WINAPI ACTIVATE_MILITARY_CRANE(CScript *script)
{
	script->Collect(10);
	float temp;
	if (Params[2].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	if (Params[3].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[3].fVar;
		Params[3].fVar = temp;
	}
	Params[9].fVar = Params[9].fVar * 3.1415927f * 5.5555557e-3f;
	CCranesHack::ActivateCrane(Params[2].fVar, Params[4].fVar, Params[3].fVar, Params[5].fVar, Params[6].fVar, Params[7].fVar, Params[8].fVar, Params[9].fVar, false, true, Params[0].fVar, Params[1].fVar);
	return OR_CONTINUE;
}

/* 036E */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, 0, 0, 0, numberedText);
	CMessages::AddBigMessage(numberedText, Params[3].nVar, (unsigned short)Params[4].nVar - 1);
	return OR_CONTINUE;
}

/* 036F */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, 0, 0, numberedText);
	CMessages::AddBigMessage(numberedText, Params[4].nVar, (unsigned short)Params[5].nVar - 1);
	return OR_CONTINUE;
}

/* 0370 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, 0, numberedText);
	CMessages::AddBigMessage(numberedText, Params[5].nVar, (unsigned short)Params[6].nVar - 1);
	return OR_CONTINUE;
}

/* 0371 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar, numberedText);
	CMessages::AddBigMessage(numberedText, Params[6].nVar, (unsigned short)Params[7].nVar - 1);
	return OR_CONTINUE;
}

/* 0378 */
eOpcodeResult WINAPI SET_2_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 0379 */
eOpcodeResult WINAPI SET_2_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[2];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037A */
eOpcodeResult WINAPI SET_3_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[3];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037B */
eOpcodeResult WINAPI SET_3_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[3];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037C */
eOpcodeResult WINAPI SET_4_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[4];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], NULL, NULL);
	return OR_CONTINUE;
}

/* 037D */
eOpcodeResult WINAPI SET_4_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[4];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], NULL, NULL);
	return OR_CONTINUE;
}

/* 037F */
eOpcodeResult WINAPI GIVE_PLAYER_DETONATOR(CScript *)
{
	CStreaming::RequestModel(291, 1);
	CStreaming::LoadAllRequestedModels(false);
	DWORD player = FindPlayerPed();
	GiveWeapon(player, 0x22, 1, 1);
	int slot = *(DWORD *)(GetWeaponInfo(0x22) + 0x60);
	*(int *)(player + slot * 0x18 + 0x40C) = 0;
	return OR_CONTINUE;
}

/* 0386 */
eOpcodeResult WINAPI SET_5_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[5];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], NULL);
	return OR_CONTINUE;
}

/* 0387 */
eOpcodeResult WINAPI SET_5_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[5];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], NULL);
	return OR_CONTINUE;
}

/* 0388 */
eOpcodeResult WINAPI SET_6_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[6];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[5] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0389 */
eOpcodeResult WINAPI SET_6_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	wchar_t *string[6];
	script->ReadShortString(gxt);
	string[0] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[1] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[2] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[3] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[4] = VCGlobals::TheText.Get(gxt);
	script->ReadShortString(gxt);
	string[5] = VCGlobals::TheText.Get(gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0393 */
eOpcodeResult WINAPI SET_CHAR_ANIM_SPEED(CScript *script)
{
	script->Collect(2);
	uintptr_t ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	int var = RpAnimBlendClumpGetFirstAssociation(*(uintptr_t *)(ped + 0x4C));
	if (var != NULL) {
		*(float *)(var + 0x24) = Params[1].fVar;
	}
	return OR_CONTINUE;
}

/* 03A0 */
eOpcodeResult WINAPI IS_CRANE_LIFTING_CAR(CScript *script)
{
	script->Collect(3);
	script->UpdateCompareFlag(CCranesHack::IsThisCarPickedUp(Params[0].fVar, Params[1].fVar, CPools::ms_pVehiclePool->GetAt(Params[2].nVar)));
	return OR_CONTINUE;
}

/* 03A5 */
eOpcodeResult WINAPI CHANGE_GARAGE_TYPE_WITH_CAR_MODEL(CScript *script)
{
	script->Collect(3);
	CGarages::ChangeGarageType((short)Params[0].nVar, (char)Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 03C2 */
eOpcodeResult WINAPI IS_PHONE_DISPLAYING_MESSAGE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag((Params[0].nVar * 0x34 + cphoneinfo + 8) == *currentPhone);
	return OR_CONTINUE;
}

/* 03C6 */
eOpcodeResult WINAPI IS_COLLISION_IN_MEMORY(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(Params[0].nVar == *levelName);
	return OR_CONTINUE;
}

/* 03C9 */
eOpcodeResult WINAPI IS_CAR_VISIBLY_DAMAGED(CScript *script)
{
	script->Collect(1);
	BYTE flag = *(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FB);
	flag >>= 1;
	flag &= 1;
	script->UpdateCompareFlag(flag == 1);
	return OR_CONTINUE;
}

/* 03DB */
eOpcodeResult WINAPI ADD_BLIP_FOR_PICKUP_OLD(CScript *script)
{
	script->Collect(3);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(3, CPools::ms_pObjectPool->GetIndex(CPickups::pickups[CPickups::GetActualPickupIndex(Params[0].nVar)].object), Params[1].nVar, Params[2].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03DD */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_PICKUP(CScript *script)
{
	script->Collect(2);
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetEntityBlip(3, CPools::ms_pObjectPool->GetIndex(CPickups::pickups[CPickups::GetActualPickupIndex(Params[0].nVar)].object), 6, 3);
	CRadar::SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03E9 */
eOpcodeResult WINAPI IS_CHAR_IN_CONTROL(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CPools::ms_pPedPool->GetAt(Params[0].nVar)->IsPedInControl());
	return OR_CONTINUE;
}

/* 03EC */
eOpcodeResult WINAPI HAS_MILITARY_CRANE_COLLECTED_ALL_CARS(CScript *script)
{
	script->UpdateCompareFlag((CCranes::carsCollectedMilitaryCrane & 0x7F) == 0x7F);
	return OR_CONTINUE;
}

/* 0410 */
eOpcodeResult WINAPI SET_GANG_PED_MODEL_PREFERENCE(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x18 + gangPedModelOverride) = (BYTE)Params[1].nVar;
	return OR_CONTINUE;
}

/* 0413 */
eOpcodeResult WINAPI SET_GET_OUT_OF_JAIL_FREE(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x170 + (DWORD)playerPedPool + 0x145) = !!Params[1].nVar;
	return OR_CONTINUE;
}

/* 0415 */
eOpcodeResult WINAPI IS_CAR_DOOR_CLOSED(CScript *script)
{
	script->Collect(2);
	uintptr_t car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	auto IsDoorMissing = (bool(__thiscall *)(uintptr_t, int))*(uintptr_t *)(*(uintptr_t *)car + 0x6C);
	auto IsDoorClosed = (bool(__thiscall *)(uintptr_t, int))*(uintptr_t *)(*(uintptr_t *)car + 0x68);
	script->UpdateCompareFlag(!IsDoorMissing(car, Params[1].nVar) && IsDoorClosed(car, Params[1].nVar));
	return OR_CONTINUE;
}

/* 041B */
eOpcodeResult WINAPI REGISTER_KILL_FRENZY_PASSED(CScript *)
{
	AnotherKillFrenzyPassed();
	return OR_CONTINUE;
}

/* 041C */
eOpcodeResult WINAPI SET_CHAR_SAY(CScript *script)
{
	script->Collect(2);
	CPools::ms_pPedPool->GetAt(Params[0].nVar)->Say((unsigned short)Params[1].nVar);
	return OR_CONTINUE;
}

/* 041F */
eOpcodeResult WINAPI OVERRIDE_HOSPITAL_LEVEL(CScript *script)
{
	script->Collect(1);
	*overrideHospital = (BYTE)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0420 */
eOpcodeResult WINAPI OVERRIDE_POLICE_STATION_LEVEL(CScript *script)
{
	script->Collect(1);
	*overridePolice = (BYTE)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0421 */
eOpcodeResult WINAPI FORCE_RAIN(CScript *script)
{
	script->Collect(1);
	CWeather::bScriptsForceRain = !!Params[0].nVar;
	return OR_CONTINUE;
}

/* 0422 */
eOpcodeResult WINAPI DOES_GARAGE_CONTAIN_CAR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag(CGarages::garages[Params[0].nVar].IsEntityEntirelyInside3D(CPools::ms_pVehiclePool->GetAt(Params[1].nVar), 0.0));
	return OR_CONTINUE;
}

/* 042A */
eOpcodeResult WINAPI IS_THREAT_FOR_PED_TYPE(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag(Params[1].nVar == (*(int *)(*(DWORD *)(cpedtype + Params[0].nVar * 4) + 0x18) & Params[1].nVar));
	return OR_CONTINUE;
}

/* 0432 */
eOpcodeResult WINAPI GET_CHAR_IN_CAR_PASSENGER_SEAT(CScript *script)
{
	script->Collect(2);
	Params[0].nVar = PedPoolGetHandle(*pedPool, *(void **)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + Params[1].nVar * 4 + 0x1AC));
	script->Store(1);
	return OR_CONTINUE;
}

/* 0444 */
eOpcodeResult WINAPI SET_SCRIPT_FIRE_AUDIO(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x30 + cfire + 7) = !!Params[1].nVar;
	return OR_CONTINUE;
}

/* 0447 */
eOpcodeResult WINAPI IS_PLAYER_LIFTING_A_PHONE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[Params[0].nVar].playerEntity->state == 0x13);
	return OR_CONTINUE;
}

/* 047D */
eOpcodeResult WINAPI GET_NUMBER_OF_SEATS_IN_MODEL(CScript *script)
{
	script->Collect(1);
	Params[0].nVar = GetNumberOfPassengerSeats(Params[0].nVar) + 1;
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

/* 04CC */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD(CScript *script)
{
	script->Collect(5);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(4, Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].nVar, Params[4].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 04CD */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(4, Params[0].fVar, Params[1].fVar, Params[2].fVar, 5, 3);
	CRadar::ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = *maxWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	CHud::SetHelpMessage(numberedText, false, false);
	return OR_CONTINUE;
}

/* 0513 */
eOpcodeResult WINAPI PRINT_HELP_FOREVER_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	CHud::SetHelpMessage(numberedText, false, true);
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
	*loanSharkVisits += (float)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0532 */
eOpcodeResult WINAPI ADD_MOVIE_STUNTS(CScript *script)
{
	script->Collect(1);
	*movieStunts += (float)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0535 */
eOpcodeResult WINAPI ADD_GARBAGE_PICKUPS(CScript *script)
{
	script->Collect(1);
	*garbagePickups += (float)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0537 */
eOpcodeResult WINAPI SET_TOP_SHOOTING_RANGE_SCORE(CScript *script)
{
	script->Collect(1);
	if ((float)Params[0].nVar > *topShootingRangeScore) {
		*topShootingRangeScore = (float)Params[0].nVar;
	}
	return OR_CONTINUE;
}

/* 0538 */
eOpcodeResult WINAPI ADD_SHOOTING_RANGE_RANK(CScript *script)
{
	script->Collect(1);
	*shootingRangeRank += (float)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle && ped->vehicle) {
		source = ped->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(CPools::ms_pVehiclePool->GetAt(Params[1].nVar)));
	return OR_CONTINUE;
}

/* 056F */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = CWorld::FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(5, Params[0].fVar, Params[1].fVar, Params[2].fVar, 2, 3);
	CRadar::ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
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
		Opcodes::RegisterOpcode(0x013B, IS_CAR_DEAD_IN_AREA_2D);
		Opcodes::RegisterOpcode(0x013C, IS_CAR_DEAD_IN_AREA_3D);
		Opcodes::RegisterOpcode(0x0155, SET_CAR_DENSITY);
		Opcodes::RegisterOpcode(0x0156, SET_PED_DENSITY);
		Opcodes::RegisterOpcode(0x015E, IS_CAR_IN_AIR);
		Opcodes::RegisterOpcode(0x0163, ADD_BLIP_FOR_OBJECT_OLD);
		Opcodes::RegisterOpcode(0x0178, IS_PLAYER_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x0179, IS_CHAR_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x017B, SET_CHAR_AMMO);
		Opcodes::RegisterOpcode(0x01C6, DONT_REMOVE_CAR);
		Opcodes::RegisterOpcode(0x01EE, ACTIVATE_CRANE);
		Opcodes::RegisterOpcode(0x01EF, DEACTIVATE_CRANE);
		Opcodes::RegisterOpcode(0x0218, PRINT_WITH_NUMBER_BIG_Q);
		Opcodes::RegisterOpcode(0x021D, SET_FREE_BOMBS);
		Opcodes::RegisterOpcode(0x021F, SET_ALL_TAXI_LIGHTS);
		Opcodes::RegisterOpcode(0x0220, IS_CAR_ARMED_WITH_ANY_BOMB);
		Opcodes::RegisterOpcode(0x0228, IS_CAR_ARMED_WITH_BOMB);
		Opcodes::RegisterOpcode(0x023A, IS_PLAYER_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x023B, IS_CHAR_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x0242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x024B, SET_REPEATED_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024C, SET_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024D, HAS_PHONE_DISPLAYED_MESSAGE);
		Opcodes::RegisterOpcode(0x0255, RESTART_CRITICAL_MISSION);
		Opcodes::RegisterOpcode(0x0295, IS_TAXI);
		Opcodes::RegisterOpcode(0x0299, ACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x029C, IS_BOAT);
		Opcodes::RegisterOpcode(0x02A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x02A4, ADD_SPRITE_BLIP_FOR_CAR);
		Opcodes::RegisterOpcode(0x02A5, ADD_SPRITE_BLIP_FOR_CHAR);
		Opcodes::RegisterOpcode(0x02A6, ADD_SPRITE_BLIP_FOR_OBJECT);
		Opcodes::RegisterOpcode(0x02B9, DEACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x02BC, SET_SWAT_REQUIRED);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x02D6, IS_CHAR_SHOOTING_IN_AREA);
		Opcodes::RegisterOpcode(0x02EE, IS_PROJECTILE_IN_AREA);
		Opcodes::RegisterOpcode(0x02EF, DESTROY_PROJECTILES_IN_AREA);
		Opcodes::RegisterOpcode(0x02F0, DROP_MINE);
		Opcodes::RegisterOpcode(0x02F1, DROP_NAUTICAL_MINE);
		Opcodes::RegisterOpcode(0x02FB, ACTIVATE_CRUSHER_CRANE);
		Opcodes::RegisterOpcode(0x02FC, PRINT_WITH_2_NUMBERS);
		Opcodes::RegisterOpcode(0x0300, PRINT_WITH_3_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0303, PRINT_WITH_4_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0305, PRINT_WITH_5_NUMBERS);
		Opcodes::RegisterOpcode(0x0306, PRINT_WITH_5_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0309, PRINT_WITH_6_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x031B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x031C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x032D, SET_CAR_BLOCK_CAR);
		Opcodes::RegisterOpcode(0x0338, SET_CAR_VISIBLE);
		Opcodes::RegisterOpcode(0x0346, SET_TEXT_BACKGROUND_COLOUR);
		Opcodes::RegisterOpcode(0x0351, IS_NASTY_GAME);
		Opcodes::RegisterOpcode(0x0356, IS_EXPLOSION_IN_AREA);
		Opcodes::RegisterOpcode(0x0357, IS_EXPLOSION_IN_ZONE);
		Opcodes::RegisterOpcode(0x0367, START_KILL_FRENZY_HEADSHOT);
		Opcodes::RegisterOpcode(0x0368, ACTIVATE_MILITARY_CRANE);
		Opcodes::RegisterOpcode(0x036E, PRINT_WITH_3_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x036F, PRINT_WITH_4_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0370, PRINT_WITH_5_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0371, PRINT_WITH_6_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0378, SET_2_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0379, SET_2_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037A, SET_3_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037B, SET_3_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037C, SET_4_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037D, SET_4_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037F, GIVE_PLAYER_DETONATOR);
		Opcodes::RegisterOpcode(0x0386, SET_5_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0387, SET_5_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0388, SET_6_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0389, SET_6_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0393, SET_CHAR_ANIM_SPEED);
		Opcodes::RegisterOpcode(0x03A0, IS_CRANE_LIFTING_CAR);
		Opcodes::RegisterOpcode(0x03A5, CHANGE_GARAGE_TYPE_WITH_CAR_MODEL);
		Opcodes::RegisterOpcode(0x03C2, IS_PHONE_DISPLAYING_MESSAGE);
		Opcodes::RegisterOpcode(0x03C6, IS_COLLISION_IN_MEMORY);
		Opcodes::RegisterOpcode(0x03C9, IS_CAR_VISIBLY_DAMAGED);
		Opcodes::RegisterOpcode(0x03DB, ADD_BLIP_FOR_PICKUP_OLD);
		Opcodes::RegisterOpcode(0x03DD, ADD_SPRITE_BLIP_FOR_PICKUP);
		Opcodes::RegisterOpcode(0x03E9, IS_CHAR_IN_CONTROL);
		Opcodes::RegisterOpcode(0x03EC, HAS_MILITARY_CRANE_COLLECTED_ALL_CARS);
		Opcodes::RegisterOpcode(0x0410, SET_GANG_PED_MODEL_PREFERENCE);
		Opcodes::RegisterOpcode(0x0413, SET_GET_OUT_OF_JAIL_FREE);
		Opcodes::RegisterOpcode(0x0415, IS_CAR_DOOR_CLOSED);
		Opcodes::RegisterOpcode(0x041B, REGISTER_KILL_FRENZY_PASSED);
		Opcodes::RegisterOpcode(0x041C, SET_CHAR_SAY);
		Opcodes::RegisterOpcode(0x041F, OVERRIDE_HOSPITAL_LEVEL);
		Opcodes::RegisterOpcode(0x0420, OVERRIDE_POLICE_STATION_LEVEL);
		Opcodes::RegisterOpcode(0x0421, FORCE_RAIN);
		Opcodes::RegisterOpcode(0x0422, DOES_GARAGE_CONTAIN_CAR);
		Opcodes::RegisterOpcode(0x042A, IS_THREAT_FOR_PED_TYPE);
		Opcodes::RegisterOpcode(0x0432, GET_CHAR_IN_CAR_PASSENGER_SEAT);
		Opcodes::RegisterOpcode(0x0444, SET_SCRIPT_FIRE_AUDIO);
		Opcodes::RegisterOpcode(0x0447, IS_PLAYER_LIFTING_A_PHONE);
		Opcodes::RegisterOpcode(0x047D, GET_NUMBER_OF_SEATS_IN_MODEL);
		Opcodes::RegisterOpcode(0x04A7, IS_CHAR_IN_ANY_BOAT);
		Opcodes::RegisterOpcode(0x04A9, IS_CHAR_IN_ANY_HELI);
		Opcodes::RegisterOpcode(0x04AB, IS_CHAR_IN_ANY_PLANE);
		Opcodes::RegisterOpcode(0x04C8, IS_CHAR_IN_FLYING_VEHICLE);
		Opcodes::RegisterOpcode(0x04CC, ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD);
		Opcodes::RegisterOpcode(0x04CD, ADD_SHORT_RANGE_BLIP_FOR_COORD);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x0511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x0513, PRINT_HELP_FOREVER_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x052D, GET_PLAYER_DRUNKENNESS);
		Opcodes::RegisterOpcode(0x0530, ADD_LOAN_SHARK_VISITS);
		Opcodes::RegisterOpcode(0x0532, ADD_MOVIE_STUNTS);
		Opcodes::RegisterOpcode(0x0535, ADD_GARBAGE_PICKUPS);
		Opcodes::RegisterOpcode(0x0537, SET_TOP_SHOOTING_RANGE_SCORE);
		Opcodes::RegisterOpcode(0x0538, ADD_SHOOTING_RANGE_RANK);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x056F, ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT);
	}
	return TRUE;
}

#else if _III
#pragma comment (lib, "..\\..\\III.VC.CLEO\\CLEO_SDK\\III.CLEO.lib")
#include "..\\..\\III.VC.CLEO\\CLEO_SDK\\III.CLEO.h"

tScriptVar *Params;

uintptr_t(__thiscall *VehiclePoolGetStruct)(void *, int); // cpool_cvehicle_cautomobile::getat
uintptr_t(__thiscall *PedPoolGetStruct)(void *, int); // cpool_cped_cplayerped::getat
BYTE(__thiscall *GetHasCollidedWith)(DWORD, uintptr_t); // cphysical::gethascollidedwith
DWORD(__cdecl *FindPlayerPed)(void); // findplayerped
void(__cdecl *SetHelpMessage)(wchar_t *, bool); // chud::sethelpmessage
void(__cdecl *InsertNumberInString)(wchar_t *, int, int, int, int, int, int, wchar_t *); // cmessages::insertnumberinstring
wchar_t *(__thiscall *GetText)(uintptr_t, char *); // ctext::get
DWORD *maxWantedLevel = NULL;
wchar_t *numberedText = NULL;
void **pedPool = NULL;
uintptr_t *playerPedPool = NULL;
uintptr_t *playerPedState = NULL;
uintptr_t ctext = NULL;
void **carPool = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x598B80:
		version = V1_0;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x43EAF0;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, int))0x43EB30;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))0x497240;
		FindPlayerPed = (DWORD(__cdecl *)(void))0x4A1150;
		SetHelpMessage = (void(__cdecl *)(wchar_t *, bool))0x5051E0;
		InsertNumberInString = (void(__cdecl *)(wchar_t *, int, int, int, int, int, int, wchar_t *))0x52A1A0;
		GetText = (wchar_t *(__thiscall *)(uintptr_t, char *))0x52C5A0;
		maxWantedLevel = (DWORD *)0x5F7714;
		numberedText = (wchar_t *)0x74B018;
		pedPool = (void **)0x8F2C60;
		playerPedPool = (uintptr_t *)0x9412F0;
		playerPedState = (uintptr_t *)0x9413C8;
		ctext = 0x941520;
		carPool = (void **)0x9430DC;
	case 0x598E40:
		version = V1_1;
	case 0x646E6957:
		version = VSTEAM;
	}
}

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(playerPedState[0x4F * Params[0].nVar] != 1);
	return OR_CONTINUE;
}

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x53C);
	BYTE fbi = *(BYTE *)(police + 0x16);
	fbi &= 0xF7;
	if (Params[0].nVar) {
		fbi |= 8;
	}
	*(BYTE *)(police + 0x16) = fbi;
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x53C);
	BYTE army = *(BYTE *)(police + 0x16);
	army &= 0xEF;
	if (Params[0].nVar) {
		army |= 16;
	}
	*(BYTE *)(police + 0x16) = army;
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = *maxWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	SetHelpMessage(numberedText, false);
	return OR_CONTINUE;
}

/* 0546 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	DWORD source = playerPedPool[0x4F * Params[0].nVar];
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x314)) {
		uintptr_t car = *(uintptr_t *)(source + 0x310);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	uintptr_t source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x314)) {
		uintptr_t car = *(uintptr_t *)(source + 0x310);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
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
		Opcodes::RegisterOpcode(0x0116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x0511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x0546, IS_PLAYER_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
	}
	return TRUE;
}

#endif
