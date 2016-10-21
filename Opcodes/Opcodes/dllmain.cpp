/*************************************************************************
    Opcodes Restoration Project

    Plugin for CLEO v2.0.0.4 and above for Grand Theft Auto III and Grand
    Theft Auto: Vice City that aims to restore functionality to many
    unsupported opcodes
    http://www.gtamodding.com/wiki/Opcodes_Restoration_Project
*************************************************************************/

#include "stdafx.h"
#include <math.h>
#include <new>

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  4

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

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

class CExplosionHack : public CExplosion
{
public:
	static bool TestForExplosionInArea(int explosionType, float x1, float x2, float y1, float y2, float z1, float z2);
};

#define MAX_PACMAN_PICKUP 256

class CPacManPickupHack
{
public:
	CVector position;
	CObject *object;
	unsigned char state;
	unsigned char padding[3];

	void Update(void);
};

class CPacManPickupsHack
{
public:
	static int PillsEatenInRace;
	static CPacManPickupHack aPMPickups[MAX_PACMAN_PICKUP];
	static bool bPMActive;

	static void CleanUpPacManStuff(void);
	static void Render(void);
	static void GeneratePMPickUpsForRace(void);
	static void GeneratePMPickUps(CVector, float, short);
	static void Update(void);
	static void Init(void);
};

class CPhoneInfoHack : public CPhoneInfo
{
public:
	void SetPhoneMessage_Repeatedly(int index, wchar_t *string1, wchar_t *string2, wchar_t *string3, wchar_t *string4, wchar_t *string5, wchar_t *string6);
};

class CProjectileInfoHack : public CProjectileInfo
{
public:
	static bool IsProjectileInRange(float x1, float y1, float z1, float x2, float y2, float z2, bool flag);
};

auto RpAnimBlendClumpGetFirstAssociation = (DWORD(__cdecl *)(unsigned long))vcversion::AdjustOffset(0x00402E20);
auto WindowHandler = (void(__cdecl *)(int, int))vcversion::AdjustOffset(0x00602EE0);
auto phoneDisplayMessage = (BYTE *)vcversion::AdjustOffset(0x007030E4);
auto currentPhone = (unsigned long *)vcversion::AdjustOffset(0x007030E8);
auto numberedText = (wchar_t *)vcversion::AdjustOffset(0x00821068);
auto baseModelInfo = (unsigned long *)vcversion::AdjustOffset(0x0092D4C8);
auto projectileObject = (unsigned long *)vcversion::AdjustOffset(0x0094B708);
auto cpedtype = (uintptr_t)vcversion::AdjustOffset(0x00A0DA64);

bool CCranesHack::IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle)
{
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (cranes[i].object) {
				float distance = sqrt(pow(positionX - cranes[i].object->GetX(), 2) + pow(positionY - cranes[i].object->GetY(), 2));
				if (distance < 100.0 && cranes[i].vehicle == vehicle && (cranes[i].status == 2 || cranes[i].status == 4)) {
					return true;
				}
			}
		}
	}
	return false;
}

void CCranesHack::DeActivateCrane(float positionX, float positionY)
{
	int index = -1;
	float minDistance = 100.0;
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (cranes[i].object) {
				float distance = sqrt(pow(positionX - cranes[i].object->GetX(), 2) + pow(positionY - cranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
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
	float craneObjectX, craneObjectY, minDistance, pickupCenterX, pickupCenterY;
	minDistance = 100.0;
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (cranes[i].object) {
				float distance = sqrt(pow(positionX - cranes[i].object->GetX(), 2) + pow(positionY - cranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
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
	cranes[index].vehicle = 0;
	cranes[index].countCollected = 0;
	cranes[index].dropoffHeading = dropoffHeading;
	cranes[index].isCrusher = isCrusher;
	cranes[index].isMilitary = isMilitary;
	cranes[index].timer = 0;
	cranes[index].status = 0;
	pickupCenterX = (pickupX1 + pickupX2) / 2;
	pickupCenterY = (pickupY1 + pickupY2) / 2;
	craneObjectX = cranes[index].object->GetX();
	craneObjectY = cranes[index].object->GetY();
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

int CPacManPickupsHack::PillsEatenInRace;
bool CPacManPickupsHack::bPMActive;
CPacManPickupHack CPacManPickupsHack::aPMPickups[MAX_PACMAN_PICKUP];
unsigned short scrambleModel;
unsigned short raceModel;

CVector aRacePoints1[MAX_PACMAN_PICKUP] =
{
	{ -233.926178f, -420.204987f, 10.995455f },
	{ -254.782120f, -434.359436f, 10.872142f },
	{ -256.466537f, -466.445406f, 10.864866f },
	{ -275.220154f, -481.432922f, 10.828557f },
	{ -321.078857f, -479.096313f, 10.839281f },
	{ -389.883850f, -477.128937f, 10.848102f },
	{ -461.664490f, -480.922363f, 10.843895f },
	{ -544.004761f, -482.025208f, 10.846241f },
	{ -635.769165f, -482.419098f, 10.848301f },
	{ -706.625183f, -484.411377f, 10.867592f },
	{ -762.343628f, -499.139221f, 14.080420f },
	{ -822.559814f, -527.719238f, 10.441530f },
	{ -845.608093f, -566.041870f, 10.692375f },
	{ -837.161804f, -626.011719f, 10.690519f },
	{ -854.579597f, -659.629349f, 11.002276f },
	{ -904.726196f, -651.225281f, 10.964456f },
	{ -958.279053f, -660.088196f, 11.058175f },
	{ -1013.392700f, -668.258362f, 11.373729f },
	{ -1069.584106f, -680.019531f, 11.385255f },
	{ -1130.864341f, -695.816103f, 11.385221f },
	{ -1138.844849f, -725.764343f, 11.640017f },
	{ -1115.353516f, -783.507751f, 11.470589f },
	{ -1086.029663f, -832.858704f, 11.726090f },
	{ -1041.921631f, -895.749268f, 13.382965f },
	{ -988.233826f, -953.362854f, 14.467456f },
	{ -938.162048f, -1014.559204f, 14.467171f },
	{ -906.919861f, -1074.380127f, 14.465546f },
	{ -877.137085f, -1149.252686f, 12.911782f },
	{ -850.260620f, -1206.532349f, 13.382989f },
	{ -799.698303f, -1241.213745f, 10.855042f },
	{ -756.509583f, -1277.165894f, 10.687537f },
	{ -758.260071f, -1332.878784f, 10.854435f },
	{ -787.277649f, -1395.845459f, 11.171658f },
	{ -815.031250f, -1446.596558f, 11.207536f },
	{ -836.611877f, -1467.907471f, 11.532784f },
	{ -887.342651f, -1462.656860f, 11.708844f },
	{ -939.546326f, -1465.730835f, 11.775413f },
	{ -981.882874f, -1464.884766f, 11.656042f },
	{ 0.0f, 0.0f, 0.0f }
};

void CPacManPickupsHack::Init(void)
{
	for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
		aPMPickups[i].state = 0;
		aPMPickups[i].object = 0;
	}
	bPMActive = false;
	PillsEatenInRace = 0;

	// get optional data for pickups
	// create pacman.dat and populate it with two custom models and up to 255 coordinate points
	scrambleModel = 502;
	raceModel = 503;
	int filename = CFileMgr::OpenFile("pacman.dat", "r");
	if (filename) {
		char buffer[200];
		if (CFileMgr::ReadLine(filename, buffer, 200)) VCGlobals::sscanf(buffer, "%d", &scrambleModel);
		if (CFileMgr::ReadLine(filename, buffer, 200)) VCGlobals::sscanf(buffer, "%d", &raceModel);
		int i = 0;
		for (; CFileMgr::ReadLine(filename, buffer, 200) && i < MAX_PACMAN_PICKUP - 1; i++) {
			VCGlobals::sscanf(buffer, "%f %f %f", &aRacePoints1[i].x, &aRacePoints1[i].y, &aRacePoints1[i].z);
		}
		aRacePoints1[i].x = aRacePoints1[i].y = aRacePoints1[i].z = 0;
		CFileMgr::CloseFile(filename);
	}
}

void CPacManPickupsHack::Update(void)
{
	if (bPMActive) {
		for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
			if (aPMPickups[i].state) {
				aPMPickups[i].Update();
			}
		}
	}
}

void CPacManPickupsHack::GeneratePMPickUps(CVector center, float radius, short count)
{
	for (int i = 0; i < count && i < MAX_PACMAN_PICKUP; i++) {
		if (aPMPickups[i].state == 0) {
			CEntity entity;
			CEntity *pentity = &entity;
			CColPoint colpoint = {};
			CVector pos;
			bool t = false;
			int random;
			float randX, randY;
			int counter = 0;

			do {
				// prevents infinite loop in invalid areas
				if (counter++ * CTimer::ms_fTimeStep > 4000.0) {
					colpoint = {};
					break;
				}
				random = VCGlobals::rand();
				randX = static_cast<float>((random & 255) - 128);
				randY = static_cast<float>(((random / 256) & 255) - 128);
				pos.x = center.x + randX * radius / 128;
				pos.y = center.y + randY * radius / 128;
				pos.z = 1000.0;
				t = CWorld::ProcessVerticalLine(pos, -1000.0, colpoint, pentity, true, false, false, false, true, false, 0);
			} while (!t || (pentity->status & 7) != 1 || !(*reinterpret_cast<unsigned short *>(baseModelInfo[pentity->modelIndex] + 0x42) & 4));

			aPMPickups[i].state = 1;
			aPMPickups[i].position.x = colpoint.point.x;
			aPMPickups[i].position.y = colpoint.point.y;
			aPMPickups[i].position.z = colpoint.point.z + 0.7f;

			void *place = CObject::operator new(0x194);
			CObject *object = ::new (place)CObject(scrambleModel, true);
			if (object) {
				object->field_16C = 2;
				object->GetMatrix().SetRotate(0.0, 0.0, -1.5707964f);
				object->GetPos() = aPMPickups[i].position;
				object->GetMatrix().UpdateRW();
				object->UpdateRwFrame();
				object->field_11A &= 0xFD;
				object->field_052 |= 2;
				object->field_051 &= 0xFE;
				object->field_16D &= 0xFE;
				CWorld::Add(object);
				aPMPickups[i].object = object;
			}

		}
	}
	bPMActive = true;
}

void CPacManPickupsHack::GeneratePMPickUpsForRace(void)
{
	for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
		if (aRacePoints1[i].x == 0 &&
			aRacePoints1[i].y == 0 &&
			aRacePoints1[i].z == 0) {
			break;
		}
		if (aPMPickups[i].state == 0) {
			aPMPickups[i].state = 2;
			aPMPickups[i].position.x = aRacePoints1[i].x;
			aPMPickups[i].position.y = aRacePoints1[i].y;
			aPMPickups[i].position.z = aRacePoints1[i].z;
			void *place = CObject::operator new(0x194);
			CObject *object = ::new (place)CObject(raceModel, true);
			if (object) {
				object->field_16C = 2;
				object->GetMatrix().SetRotate(0.0, 0.0, -1.5707964f);
				object->GetPos() = aPMPickups[i].position;
				object->GetMatrix().UpdateRW();
				object->UpdateRwFrame();
				object->field_11A &= 0xFD;
				object->field_052 |= 2;
				object->field_051 &= 0xFE;
				object->field_16D &= 0xFE;
				CWorld::Add(object);
				aPMPickups[i].object = object;
			}
		}
	}
	bPMActive = true;
}

void CPacManPickupsHack::Render(void)
{
	if (bPMActive) {
		RwRenderStateSet(8, 0);
		RwRenderStateSet(12, 1);
		RwRenderStateSet(10, 2);
		RwRenderStateSet(11, 2);
		RwRenderStateSet(1, (int)**(void ***)vcversion::AdjustOffset(0x00695550));
		for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
			if (aPMPickups[i].state) {
				RwV3d pickup = { aPMPickups[i].position.x, aPMPickups[i].position.y, aPMPickups[i].position.z };
				RwV3d pos;
				float width;
				float height;
				if (CSprite::CalcScreenCoors(pickup, &pos, &width, &height, true) && pos.z < 100.0) {
					float time = static_cast<float>(CTimer::m_snTimeInMilliseconds) * 6.1359233e-3f;
					if (aPMPickups[i].object) {
						CMatrix *matrix = &aPMPickups[i].object->GetMatrix();
						matrix->SetRotateZOnly(time);
						matrix->UpdateRW();
						aPMPickups[i].object->UpdateRwFrame();
					}
					CSprite::RenderOneXLUSprite(pos.x, pos.y, pos.z, abs(width * 0.8f * sin(time)), height * 0.8f, 100, 50, 5, 255, 1.0f / pos.z, 255);
				}
			}
		}
	}
}

void CPacManPickupHack::Update(void)
{
	CVehicle *vehicle = VCGlobals::FindPlayerVehicle();
	if (!vehicle ||
		(pow(vehicle->GetY() - position.y, 2) + pow(vehicle->GetX() - position.x, 2) > 100.0) ||
		!vehicle->IsSphereTouchingVehicle(position.x, position.y, position.z, 1.5)) {
		return;
	}
	if (state == 1) {
		vehicle->powerPillsCarried++;
		vehicle->forceX *= 0.65f;
		vehicle->forceY *= 0.65f;
		vehicle->forceZ *= 0.65f;
		float multiplier = (250.0f + vehicle->mass) / vehicle->mass;
		vehicle->mass *= multiplier;
		vehicle->turnResistance *= multiplier;
		vehicle->accelerationResistance *= multiplier;
		VCGlobals::FindPlayerPed()->wanted->counter += 10;
		VCGlobals::FindPlayerPed()->wanted->UpdateWantedLevel();
	} else if (state == 2) {
		CPacManPickupsHack::PillsEatenInRace++;
	}
	VCGlobals::DMAudio.cDMAudio::PlayFrontEndSound(0x54, 0);
	state = 0;
	if (object) {
		CWorld::Remove(object);
		auto Destroy = (void(__thiscall *)(CObject *, int))*(unsigned long *)(object->vtbl + 8);
		Destroy(object, 1);
		object = 0;
	}
}

void CPacManPickupsHack::CleanUpPacManStuff(void)
{
	bPMActive = 0;
	for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
		if (aPMPickups[i].state && aPMPickups[i].object) {
			CWorld::Remove(aPMPickups[i].object);
			auto Destroy = (void(__thiscall *)(CObject *, int))*(unsigned long *)(aPMPickups[i].object->vtbl + 8);
			Destroy(aPMPickups[i].object, 1);
			aPMPickups[i].object = 0;
		}
		aPMPickups[i].state = 0;
	}
}

bool CProjectileInfoHack::IsProjectileInRange(float x1, float y1, float z1, float x2, float y2, float z2, bool flag)
{
	unsigned long *address = projectileObject;
	for (int i = 0; i < 0x20; i++) {
		CObject *object = *(CObject **)&address[i];
		if (object) {
			if (projectiles[i].doesProjectileExist && projectiles[i].weaponType >= 12 && projectiles[i].weaponType <= 16) {
				if (object->GetX() >= x1 && object->GetX() <= x2) {
					if (object->GetY() >= y1 && object->GetY() <= y2) {
						if (object->GetZ() >= z1 && object->GetZ() <= z2) {
							if (flag) {
								projectiles[i].doesProjectileExist = 0;
								auto Destroy = (void(__thiscall *)(CObject *, int))*(unsigned long *)(object->vtbl + 8);
								CWorld::Remove(object);
								Destroy(object, 1);
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

void CPhoneInfoHack::SetPhoneMessage_Repeatedly(int index, wchar_t *string1, wchar_t *string2, wchar_t *string3, wchar_t *string4, wchar_t *string5, wchar_t *string6)
{
	this->phones[index].message1 = string1;
	this->phones[index].message2 = string2;
	this->phones[index].message3 = string3;
	this->phones[index].message4 = string4;
	this->phones[index].message5 = string5;
	this->phones[index].message6 = string6;
	this->phones[index].state = 5;
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

static short CardStack[6 * 52];
static short CardStackPosition;

/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(ped && ped->state != 0x37 && ped->state != 0x36);
	return OR_CONTINUE;
}

/* 00AC */
eOpcodeResult WINAPI IS_CAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag((vehicle && (vehicle->status >> 3) != 5) && (vehicle->type == 1 || !(vehicle->field_11A & 0x10)));
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
	Params[0].nVar = static_cast<int>(reinterpret_cast<CRunningScript *>(script)->GetPadState(static_cast<unsigned short>(Params[0].nVar), static_cast<unsigned short>(Params[1].nVar)));
	script->Store(1);
	return OR_CONTINUE;
}

/* 0113 */
eOpcodeResult WINAPI ADD_AMMO_TO_PLAYER(CScript *script)
{
	script->Collect(3);
	CWorld::Players[Params[0].nVar].playerEntity->GrantAmmo(Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[Params[0].nVar].deathArrestState != 1);
	return OR_CONTINUE;
}

/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[Params[0].nVar].deathArrestState == 2);
	return OR_CONTINUE;
}

/* 0135 */
eOpcodeResult WINAPI CHANGE_CAR_LOCK(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->lock = Params[1].nVar;
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	VCGlobals::TheCamera.CamShake(static_cast<float>(Params[0].nVar) * 1e-3f, Params[1].fVar, Params[2].fVar, Params[3].fVar);
	return OR_CONTINUE;
}

/* 013B */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_2D(CScript *script)
{
	script->Collect(6);
	if (Params[5].nVar) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
	}
	if (CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar)) {
		if ((vehicle->status >> 3) == 5 && vehicle->placeable.IsWithinArea(Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar)) {
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
	if (Params[7].nVar) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, Params[1].fVar, Params[2].fVar, Params[4].fVar, Params[5].fVar, (Params[3].fVar + Params[6].fVar) * 0.5f);
	}
	if (CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar)) {
		if ((vehicle->status >> 3) == 5 && vehicle->placeable.IsWithinArea(Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, Params[6].fVar)) {
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
			int offset = Params[0].nVar ? CTheZones::InfoZoneArray[i].night : CTheZones::InfoZoneArray[i].day;
			CTheZones::ZoneInfoArray[offset].carDensity = static_cast<unsigned short>(Params[1].nVar);
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
			int offset = Params[0].nVar ? CTheZones::InfoZoneArray[i].night : CTheZones::InfoZoneArray[i].day;
			CTheZones::ZoneInfoArray[offset].pedDensity = static_cast<unsigned short>(Params[1].nVar);
		}
	}
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(vehicle->numberOfWheelsOnGround == 0);
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
	CObject *object = CPools::ms_pObjectPool->GetAt(Params[1].nVar);
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CObject *object = CPools::ms_pObjectPool->GetAt(Params[1].nVar);
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	ped->SetAmmo(Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 01C6 */
eOpcodeResult WINAPI DONT_REMOVE_CAR(CScript *script)
{
	script->Collect(1);
	CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	CTheScripts::MissionCleanUp.RemoveEntityFromList(Params[0].nVar, 1);
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
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, -1, -1, -1, -1, -1, numberedText);
	CMessages::AddBigMessageQ(numberedText, Params[1].nVar, static_cast<unsigned short>(Params[2].nVar - 1));
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	CGarages::BombsAreFree = !!Params[0].nVar;
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	CAutomobile::m_sAllTaxiLights = !!Params[0].nVar;
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(!!(vehicle->bombState & 7));
	return OR_CONTINUE;
}

/* 0228 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag((vehicle->bombState & 7) == Params[1].nVar);
	return OR_CONTINUE;
}

/* 016F */
eOpcodeResult WINAPI DRAW_SHADOW(CScript *script)
{
	script->Collect(10);
	unsigned char type = 0;
	RwTexture *texture = 0;
	CVector pos = { Params[1].fVar, Params[2].fVar, Params[3].fVar };
	switch (Params[0].nVar) {
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
		-sin(Params[4].fVar) * Params[5].fVar, cos(Params[4].fVar) * Params[5].fVar, cos(Params[4].fVar) * Params[5].fVar, sin(Params[4].fVar) * Params[5].fVar,
		static_cast<short>(Params[6].nVar), static_cast<unsigned char>(Params[7].nVar), static_cast<unsigned char>(Params[8].nVar), static_cast<unsigned char>(Params[9].nVar), 15.0, false, 1.0, 0, false);
	return OR_CONTINUE;
}

/* 023A */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPlayerPed *player = CWorld::Players[Params[0].nVar].playerEntity;
	CObject *object = CPools::ms_pObjectPool->GetAt(Params[1].nVar);
	script->UpdateCompareFlag(!player->isInAnyVehicle && player->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 023B */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CObject *object = CPools::ms_pObjectPool->GetAt(Params[1].nVar);
	script->UpdateCompareFlag(!ped->isInAnyVehicle && ped->GetHasCollidedWith(object));
	return OR_CONTINUE;
}

/* 023E */
eOpcodeResult WINAPI FLASH_CAR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->field_052 &= 0xDF;
	if (Params[1].nVar) {
		vehicle->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 023F */
eOpcodeResult WINAPI FLASH_CHAR(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	ped->field_052 &= 0xDF;
	if (Params[1].nVar) {
		ped->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 0240 */
eOpcodeResult WINAPI FLASH_OBJECT(CScript *script)
{
	script->Collect(2);
	CObject *object = CPools::ms_pObjectPool->GetAt(Params[0].nVar);
	object->field_052 &= 0xDF;
	if (Params[1].nVar) {
		object->field_052 |= 0x20;
	}
	return OR_CONTINUE;
}

/* 0242 */
eOpcodeResult WINAPI ARM_CAR_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->bombState = (vehicle->bombState & 0xF8) | (static_cast<unsigned char>(Params[1].nVar) & 7);
	vehicle->bombOwner = VCGlobals::FindPlayerPed();
	return OR_CONTINUE;
}

/* 024B */
eOpcodeResult WINAPI SET_REPEATED_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, VCGlobals::TheText.Get(gxt), 0, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 024C */
eOpcodeResult WINAPI SET_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, VCGlobals::TheText.Get(gxt), 0, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 024D */
eOpcodeResult WINAPI HAS_PHONE_DISPLAYED_MESSAGE(CScript *script)
{
	script->Collect(1);
	if (*phoneDisplayMessage == 0) {
		unsigned int state = VCGlobals::gPhoneInfo.phones[Params[0].nVar].state;
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
	CVector pos1 = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	CVector pos2 = { 0, 0, 0 };
	CPointLights::AddLight(0, pos1, pos2, 12.0, Params[3].nVar / 255.0f, Params[4].nVar / 255.0f, Params[5].nVar / 255.0f, 0, true);
	return OR_CONTINUE;
}

/* 0255 */
eOpcodeResult WINAPI RESTART_CRITICAL_MISSION(CScript *script)
{
	script->Collect(4);
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRestart::OverrideNextRestart(pos, Params[3].fVar);
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
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	unsigned short model = vehicle->modelIndex;
	script->UpdateCompareFlag(model == 0x96 || model == 0xA8 || model == 0xBC || model == 0xD8);
	return OR_CONTINUE;
}

/* 0299 */
eOpcodeResult WINAPI ACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	CGarage *garage = &CGarages::garages[Params[0].nVar];
	if (garage->type == 11 && garage->state == 0) {
		garage->state = 3;
	}
	garage->isInactive = 0;
	return OR_CONTINUE;
}

/* 029C */
eOpcodeResult WINAPI IS_BOAT(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(vehicle->type == 1);
	return OR_CONTINUE;
}

/* 02A0 */
eOpcodeResult WINAPI IS_CHAR_STOPPED(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	script->m_dwWakeTime = Params[0].nVar + CTimer::m_snTimeInMilliseconds;
	if (Params[1].nVar) {
		script->m_bAwake = 1;
	}
	return OR_TERMINATE;
}

/* 02A2 */
eOpcodeResult WINAPI ADD_PARTICLE_EFFECT(CScript *script)
{
	script->Collect(5);
	CVector pos = { Params[1].fVar, Params[2].fVar, Params[3].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CParticleObject::AddObject(static_cast<unsigned short>(Params[0].nVar), pos, !!Params[4].nVar);
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
	Params[0].nVar = CPacManPickupsHack::PillsEatenInRace;
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
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CPacManPickupsHack::GeneratePMPickUps(pos, Params[3].fVar, (short)Params[4].nVar);
	return OR_CONTINUE;
}

/* 02C8 */
eOpcodeResult WINAPI GET_NUMBER_OF_POWER_PILLS_CARRIED(CScript *script)
{
	CVehicle *vehicle = VCGlobals::FindPlayerVehicle();
	Params[0].nVar = vehicle ? vehicle->powerPillsCarried : 0;
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
	script->m_dwIp = Params[0].nVar;
	return OR_CONTINUE;
}

/* 02D6 */
eOpcodeResult WINAPI IS_CHAR_SHOOTING_IN_AREA(CScript *script)
{
	script->Collect(6);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag((ped->field_14E & 0x40) && ped->placeable.IsWithinArea(Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar));
	if (Params[5].nVar) {
		CTheScripts::HighlightImportantArea((unsigned long)&script->m_pNext + script->m_dwIp, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
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
	CPacManPickupsHack::aPMPickups[0].position.x = Params[0].fVar;
	CPacManPickupsHack::aPMPickups[0].position.y = Params[1].fVar;
	CPacManPickupsHack::aPMPickups[0].position.z = Params[2].fVar;
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
	script->UpdateCompareFlag(CProjectileInfoHack::IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 0));
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
	script->UpdateCompareFlag(CProjectileInfoHack::IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 1));
	return OR_CONTINUE;
}

/* 02F0 */
eOpcodeResult WINAPI DROP_MINE(CScript *script)
{
	script->Collect(3);
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
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
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
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
	CMessages::AddMessageWithNumber(VCGlobals::TheText.Get(gxt), Params[2].nVar, static_cast<unsigned short>(Params[3].nVar), Params[0].nVar, Params[1].nVar, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0300 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[3].nVar, static_cast<unsigned short>(Params[4].nVar), Params[0].nVar, Params[1].nVar, Params[2].nVar, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0303 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[4].nVar, static_cast<unsigned short>(Params[5].nVar), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, -1, -1);
	return OR_CONTINUE;
}

/* 0305 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageWithNumber(VCGlobals::TheText.Get(gxt), Params[5].nVar, static_cast<unsigned short>(Params[6].nVar), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0306 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[5].nVar, static_cast<unsigned short>(Params[6].nVar), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0309 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::AddMessageJumpQWithNumber(VCGlobals::TheText.Get(gxt), Params[6].nVar, static_cast<unsigned short>(Params[7].nVar), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar);
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag((int)vehicle->firstColour == Params[1].nVar);
	return OR_CONTINUE;
}

/* 031C */
eOpcodeResult WINAPI IS_SECOND_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag((int)vehicle->secondColour == Params[1].nVar);
	return OR_CONTINUE;
}

/* 0338 */
eOpcodeResult WINAPI SET_CAR_VISIBLE(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->field_052 &= 0xFB;
	if (Params[1].nVar) {
		vehicle->field_052 |= 4;
	}
	return OR_CONTINUE;
}

/* 032D */
eOpcodeResult WINAPI SET_CAR_BLOCK_CAR(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	CVehicle *target = CPools::ms_pVehiclePool->GetAt(Params[1].nVar);
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

/* 0346 */
eOpcodeResult WINAPI SET_TEXT_BACKGROUND_COLOUR(CScript *script)
{
	script->Collect(4);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorR = static_cast<unsigned char>(Params[0].nVar);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorG = static_cast<unsigned char>(Params[1].nVar);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorB = static_cast<unsigned char>(Params[2].nVar);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].backgroundColorA = static_cast<unsigned char>(Params[3].nVar);
	return OR_CONTINUE;
}

/* 0349 */
eOpcodeResult WINAPI SET_TEXT_FONT(CScript *script)
{
	script->Collect(1);
	CTheScripts::IntroTextLines[CTheScripts::NumberOfIntroTextLinesThisFrame].font = static_cast<short>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0351 */
eOpcodeResult WINAPI IS_NASTY_GAME(CScript *script)
{
	script->UpdateCompareFlag(!!CGame::nastyGame);
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
	for (int i = 0; i < CTheZones::TotalNumberOfInfoZones; i++) {
		if (!strncmp(gxt, CTheZones::InfoZoneArray[i].name, 8)) {
			if (CExplosionHack::TestForExplosionInArea(Params[0].nVar,
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
			if (CExplosionHack::TestForExplosionInArea(Params[0].nVar,
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
	CDarkel::StartFrenzy(Params[0].nVar, Params[1].nVar, static_cast<unsigned short>(Params[2].nVar), Params[3].nVar, VCGlobals::TheText.Get(gxt), Params[4].nVar, Params[5].nVar, Params[6].nVar, !!Params[7].nVar, true);
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
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, -1, -1, -1, numberedText);
	CMessages::AddBigMessage(numberedText, Params[3].nVar, static_cast<unsigned short>(Params[4].nVar - 1));
	return OR_CONTINUE;
}

/* 036F */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, -1, -1, numberedText);
	CMessages::AddBigMessage(numberedText, Params[4].nVar, static_cast<unsigned short>(Params[5].nVar - 1));
	return OR_CONTINUE;
}

/* 0370 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1, numberedText);
	CMessages::AddBigMessage(numberedText, Params[5].nVar, static_cast<unsigned short>(Params[6].nVar - 1));
	return OR_CONTINUE;
}

/* 0371 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	CMessages::InsertNumberInString(VCGlobals::TheText.Get(gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar, numberedText);
	CMessages::AddBigMessage(numberedText, Params[6].nVar, static_cast<unsigned short>(Params[7].nVar - 1));
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
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, string[0], string[1], 0, 0, 0, 0);
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
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, string[0], string[1], 0, 0, 0, 0);
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
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, string[0], string[1], string[2], 0, 0, 0);
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
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, string[0], string[1], string[2], 0, 0, 0);
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
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, string[0], string[1], string[2], string[3], 0, 0);
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
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, string[0], string[1], string[2], string[3], 0, 0);
	return OR_CONTINUE;
}

/* 037F */
eOpcodeResult WINAPI GIVE_PLAYER_DETONATOR(CScript *)
{
	CStreaming::RequestModel(291, 1);
	CStreaming::LoadAllRequestedModels(false);
	VCGlobals::FindPlayerPed()->GiveWeapon(34, 1, 1);
	int slot = *(int *)(CWeaponInfo::GetWeaponInfo(34) + 0x60);
	VCGlobals::FindPlayerPed()->weapons[slot].state = 0;
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
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, string[0], string[1], string[2], string[3], string[4], 0);
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
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, string[0], string[1], string[2], string[3], string[4], 0);
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
	((CPhoneInfoHack *)&VCGlobals::gPhoneInfo)->SetPhoneMessage_Repeatedly(Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
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
	VCGlobals::gPhoneInfo.SetPhoneMessage_JustOnce(Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0393 */
eOpcodeResult WINAPI SET_CHAR_ANIM_SPEED(CScript *script)
{
	script->Collect(2);
	int var = RpAnimBlendClumpGetFirstAssociation(CPools::ms_pPedPool->GetAt(Params[0].nVar)->RpClump);
	if (var) {
		*(float *)(var + 0x24) = Params[1].fVar;
	}
	return OR_CONTINUE;
}

/* 03A0 */
eOpcodeResult WINAPI IS_CRANE_LIFTING_CAR(CScript *script)
{
	script->Collect(3);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[2].nVar);
	script->UpdateCompareFlag(CCranesHack::IsThisCarPickedUp(Params[0].fVar, Params[1].fVar, vehicle));
	return OR_CONTINUE;
}

/* 03A5 */
eOpcodeResult WINAPI CHANGE_GARAGE_TYPE_WITH_CAR_MODEL(CScript *script)
{
	script->Collect(3);
	CGarages::ChangeGarageType(static_cast<short>(Params[0].nVar), static_cast<unsigned char>(Params[1].nVar), Params[2].nVar);
	return OR_CONTINUE;
}

/* 03C2 */
eOpcodeResult WINAPI IS_PHONE_DISPLAYING_MESSAGE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag((unsigned long)&VCGlobals::gPhoneInfo.phones[Params[0].nVar] == *currentPhone);
	return OR_CONTINUE;
}

/* 03C6 */
eOpcodeResult WINAPI IS_COLLISION_IN_MEMORY(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(Params[0].nVar == CGame::currLevel);
	return OR_CONTINUE;
}

/* 03C9 */
eOpcodeResult WINAPI IS_CAR_VISIBLY_DAMAGED(CScript *script)
{
	script->Collect(1);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	script->UpdateCompareFlag(!!(vehicle->field_1FB & 2));
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->originLevel = Params[1].nVar ? (char)CTheZones::GetLevelFromPosition(&vehicle->GetPos()) : 0;
	return OR_CONTINUE;
}

/* 03FC */
eOpcodeResult WINAPI SET_CHAR_STAYS_IN_CURRENT_LEVEL(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	ped->originLevel = Params[1].nVar ? (char)CTheZones::GetLevelFromPosition(&ped->GetPos()) : 0;
	return OR_CONTINUE;
}

/* 0410 */
eOpcodeResult WINAPI SET_GANG_PED_MODEL_PREFERENCE(CScript *script)
{
	script->Collect(2);
	CGangs::Gang[Params[0].nVar].pedModelPreference = static_cast<char>(Params[1].nVar);
	return OR_CONTINUE;
}

/* 0413 */
eOpcodeResult WINAPI SET_GET_OUT_OF_JAIL_FREE(CScript *script)
{
	script->Collect(2);
	CWorld::Players[Params[0].nVar].getOutOfJailFree = !!Params[1].nVar;
	return OR_CONTINUE;
}

/* 0415 */
eOpcodeResult WINAPI IS_CAR_DOOR_CLOSED(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	auto IsDoorMissing = (bool(__thiscall *)(CVehicle *, int))*(unsigned long *)(vehicle->vtbl + 0x6C);
	auto IsDoorClosed = (bool(__thiscall *)(CVehicle *, int))*(unsigned long *)(vehicle->vtbl + 0x68);
	script->UpdateCompareFlag(!IsDoorMissing(vehicle, Params[1].nVar) && IsDoorClosed(vehicle, Params[1].nVar));
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	ped->Say(static_cast<unsigned short>(Params[1].nVar));
	return OR_CONTINUE;
}

/* 041F */
eOpcodeResult WINAPI OVERRIDE_HOSPITAL_LEVEL(CScript *script)
{
	script->Collect(1);
	CRestart::OverrideHospitalLevel = static_cast<unsigned char>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0420 */
eOpcodeResult WINAPI OVERRIDE_POLICE_STATION_LEVEL(CScript *script)
{
	script->Collect(1);
	CRestart::OverridePoliceStationLevel = static_cast<unsigned char>(Params[0].nVar);
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
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[1].nVar);
	script->UpdateCompareFlag(CGarages::garages[Params[0].nVar].IsEntityEntirelyInside3D(vehicle, 0.0));
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
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	Params[0].nVar = CPools::ms_pPedPool->GetIndex(vehicle->passengers[Params[1].nVar]);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0438 */
eOpcodeResult WINAPI SET_CHAR_IGNORE_LEVEL_TRANSITIONS(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	ped->originLevel = Params[1].nVar ? -1 : (char)CTheZones::GetLevelFromPosition(&ped->GetPos());
	return OR_CONTINUE;
}

/* 0444 */
eOpcodeResult WINAPI SET_SCRIPT_FIRE_AUDIO(CScript *script)
{
	script->Collect(2);
	CFireManager::fires[Params[0].nVar].sfx = !!Params[1].nVar;
	return OR_CONTINUE;
}

/* 0447 */
eOpcodeResult WINAPI IS_PLAYER_LIFTING_A_PHONE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(CWorld::Players[Params[0].nVar].playerEntity->state == 0x13);
	return OR_CONTINUE;
}

/* 044E */
eOpcodeResult WINAPI SET_CAR_IGNORE_LEVEL_TRANSITIONS(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->originLevel = Params[1].nVar ? -1 : (char)CTheZones::GetLevelFromPosition(&vehicle->GetPos());
	return OR_CONTINUE;
}

/* 044F */
eOpcodeResult WINAPI MAKE_CRAIGS_CAR_A_BIT_STRONGER(CScript *script)
{
	script->Collect(2);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[0].nVar);
	vehicle->field_501 &= 0xDF;
	if (Params[1].nVar) {
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

/* 047D */
eOpcodeResult WINAPI GET_NUMBER_OF_SEATS_IN_MODEL(CScript *script)
{
	script->Collect(1);
	Params[0].nVar = CVehicleModelInfo::GetMaximumNumberOfPassengersFromNumberOfDoors(Params[0].nVar) + 1;
	script->Store(1);
	return OR_CONTINUE;
}

/* 04A7 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_BOAT(CScript *script)
{
	script->Collect(1);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
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
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(4, pos, Params[3].nVar, Params[4].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 04CD */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD(CScript *script)
{
	script->Collect(3);
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(4, pos, 5, 3);
	CRadar::ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = CWanted::MaximumWantedLevel;
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
	Params[0].nVar = (int)CWorld::Players[Params[0].nVar].playerEntity->drunkenness;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0530 */
eOpcodeResult WINAPI ADD_LOAN_SHARK_VISITS(CScript *script)
{
	script->Collect(1);
	CStats::LoanSharks += static_cast<float>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0532 */
eOpcodeResult WINAPI ADD_MOVIE_STUNTS(CScript *script)
{
	script->Collect(1);
	CStats::MovieStunts += static_cast<float>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0535 */
eOpcodeResult WINAPI ADD_GARBAGE_PICKUPS(CScript *script)
{
	script->Collect(1);
	CStats::GarbagePickups += static_cast<float>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0537 */
eOpcodeResult WINAPI SET_TOP_SHOOTING_RANGE_SCORE(CScript *script)
{
	script->Collect(1);
	if (static_cast<float>(Params[0].nVar) > CStats::TopShootingRangeScore) {
		CStats::TopShootingRangeScore = static_cast<float>(Params[0].nVar);
	}
	return OR_CONTINUE;
}

/* 0538 */
eOpcodeResult WINAPI ADD_SHOOTING_RANGE_RANK(CScript *script)
{
	script->Collect(1);
	CStats::ShootingRangeRank += static_cast<float>(Params[0].nVar);
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	CPed *ped = CPools::ms_pPedPool->GetAt(Params[0].nVar);
	CVehicle *vehicle = CPools::ms_pVehiclePool->GetAt(Params[1].nVar);
	CPhysical *source = ped;
	if (ped->isInAnyVehicle && ped->vehicle) {
		source = ped->vehicle;
	}
	script->UpdateCompareFlag(source->GetHasCollidedWith(vehicle));
	return OR_CONTINUE;
}

/* 056F */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT(CScript *script)
{
	script->Collect(3);
	CVector pos = { Params[0].fVar, Params[1].fVar, Params[2].fVar };
	if (pos.z <= -100.0) {
		pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
	}
	CRadar::GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = CRadar::SetShortRangeCoordBlip(5, pos, 2, 3);
	CRadar::ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
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
	if (Params[0].nVar) {
		int index = Params[0].nVar * 52;
		for (short card = 1; card < 53; card++) {
			for (int deck = 0; deck < Params[0].nVar; deck++) {
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
	Params[0].nVar = CardStack[CardStackPosition++];
	script->Store(1);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("VC.Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		DWORD flOldProtect;
		unsigned long address;
		// CGame::Initialise
		address = vcversion::AdjustOffset(0x004A4ED8);
		VirtualProtect((void *)(address + 1), 4, PAGE_EXECUTE_READWRITE, &flOldProtect);
		*(unsigned long *)(address + 1) = (unsigned long)&CPacManPickupsHack::Init - address - 5;
		VirtualProtect((void *)(address + 1), 4, flOldProtect, &flOldProtect);
		// CGame::ReInitGameObjectVariables
		address = vcversion::AdjustOffset(0x004A4967);
		VirtualProtect((void *)(address + 1), 4, PAGE_EXECUTE_READWRITE, &flOldProtect);
		*(unsigned long *)(address + 1) = (unsigned long)&CPacManPickupsHack::Init - address - 5;
		VirtualProtect((void *)(address + 1), 4, flOldProtect, &flOldProtect);
		// CGame::Process
		address = vcversion::AdjustOffset(0x004A45D7);
		VirtualProtect((void *)(address + 1), 4, PAGE_EXECUTE_READWRITE, &flOldProtect);
		*(unsigned long *)(address + 1) = (unsigned long)&CPacManPickupsHack::Update - address - 5;
		VirtualProtect((void *)(address + 1), 4, flOldProtect, &flOldProtect);
		// RenderEffects
		address = vcversion::AdjustOffset(0x004A6547);
		VirtualProtect((void *)(address + 1), 4, PAGE_EXECUTE_READWRITE, &flOldProtect);
		*(unsigned long *)(address + 1) = (unsigned long)&CPacManPickupsHack::Render - address - 5;
		VirtualProtect((void *)(address + 1), 4, flOldProtect, &flOldProtect);

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
		Opcodes::RegisterOpcode(0x016F, DRAW_SHADOW);
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
		Opcodes::RegisterOpcode(0x023E, FLASH_CAR);
		Opcodes::RegisterOpcode(0x023F, FLASH_CHAR);
		Opcodes::RegisterOpcode(0x0240, FLASH_OBJECT);
		Opcodes::RegisterOpcode(0x0242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x024B, SET_REPEATED_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024C, SET_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024D, HAS_PHONE_DISPLAYED_MESSAGE);
		Opcodes::RegisterOpcode(0x0250, DRAW_LIGHT);
		Opcodes::RegisterOpcode(0x0255, RESTART_CRITICAL_MISSION);
		Opcodes::RegisterOpcode(0x0295, IS_TAXI);
		Opcodes::RegisterOpcode(0x0299, ACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x029C, IS_BOAT);
		Opcodes::RegisterOpcode(0x02A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x02A1, MESSAGE_WAIT);
		Opcodes::RegisterOpcode(0x02A2, ADD_PARTICLE_EFFECT);
		Opcodes::RegisterOpcode(0x02A4, ADD_SPRITE_BLIP_FOR_CAR);
		Opcodes::RegisterOpcode(0x02A5, ADD_SPRITE_BLIP_FOR_CHAR);
		Opcodes::RegisterOpcode(0x02A6, ADD_SPRITE_BLIP_FOR_OBJECT);
		Opcodes::RegisterOpcode(0x02B9, DEACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x02BC, SET_SWAT_REQUIRED);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x02C3, START_PACMAN_RACE);
		Opcodes::RegisterOpcode(0x02C5, GET_NUMBER_OF_POWER_PILLS_EATEN);
		Opcodes::RegisterOpcode(0x02C6, CLEAR_PACMAN);
		Opcodes::RegisterOpcode(0x02C7, START_PACMAN_SCRAMBLE);
		Opcodes::RegisterOpcode(0x02C8, GET_NUMBER_OF_POWER_PILLS_CARRIED);
		Opcodes::RegisterOpcode(0x02C9, CLEAR_NUMBER_OF_POWER_PILLS_CARRIED);
		Opcodes::RegisterOpcode(0x02CD, GOSUB_FILE);
		Opcodes::RegisterOpcode(0x02D6, IS_CHAR_SHOOTING_IN_AREA);
		Opcodes::RegisterOpcode(0x02D9, CLEAR_NUMBER_OF_POWER_PILLS_EATEN);
		Opcodes::RegisterOpcode(0x02DA, ADD_POWER_PILL);
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
		Opcodes::RegisterOpcode(0x0349, SET_TEXT_FONT);
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
		Opcodes::RegisterOpcode(0x03FB, SET_CAR_STAYS_IN_CURRENT_LEVEL);
		Opcodes::RegisterOpcode(0x03FC, SET_CHAR_STAYS_IN_CURRENT_LEVEL);
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
		Opcodes::RegisterOpcode(0x0438, SET_CHAR_IGNORE_LEVEL_TRANSITIONS);
		Opcodes::RegisterOpcode(0x0444, SET_SCRIPT_FIRE_AUDIO);
		Opcodes::RegisterOpcode(0x0447, IS_PLAYER_LIFTING_A_PHONE);
		Opcodes::RegisterOpcode(0x044E, SET_CAR_IGNORE_LEVEL_TRANSITIONS);
		Opcodes::RegisterOpcode(0x044F, MAKE_CRAIGS_CAR_A_BIT_STRONGER);
		Opcodes::RegisterOpcode(0x0452, ENABLE_PLAYER_CONTROL_CAMERA);
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
		Opcodes::RegisterOpcode(0x059D, SHUFFLE_CARD_DECKS);
		Opcodes::RegisterOpcode(0x059E, FETCH_NEXT_CARD);
	}
	return TRUE;
}

#else if _III
#pragma comment (lib, "..\\..\\III.VC.CLEO\\CLEO_SDK\\III.CLEO.lib")
#include "..\\..\\III.VC.CLEO\\CLEO_SDK\\III.CLEO.h"

enum gameVersion{ V1_0, V1_1, VSTEAM, VUNKOWN = -1 };

class GtaGame
{
public:
	gameVersion version;
	GtaGame();
};

GtaGame game;
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

BOOL APIENTRY DllMain(HMODULE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("III.Opcodes.cleo"), MB_ICONERROR);
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
