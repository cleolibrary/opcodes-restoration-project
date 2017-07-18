#include "CPacManPickupsHack.h"
#include <cmath>
#include "Globals.h"
#include "vcclasses.h"
#include "vcversion.h"

extern void(__cdecl *CallInit)(void);
extern void(__cdecl *CallUpdate)(void);
extern void(__cdecl *CallRender)(void);

static unsigned short scrambleModel;
static unsigned short raceModel;
static CVector aRacePoints1[MAX_PACMAN_PICKUP] =
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

int CPacManPickupsHack::PillsEatenInRace;
bool CPacManPickupsHack::bPMActive;
CPacManPickupHack CPacManPickupsHack::aPMPickups[MAX_PACMAN_PICKUP];

void CPacManPickupsHack::Init(void)
{
	for (int i = 0; i < MAX_PACMAN_PICKUP; i++) {
		aPMPickups[i].state = 0;
		aPMPickups[i].object = 0;
	}
	bPMActive = false;
	PillsEatenInRace = 0;

	// get optional data for pickups
	// create pacman.dat and populate it with two custom models and up to 255 coordinate points on each line
	scrambleModel = 502;
	raceModel = 503;
	if (int filename = CFileMgr::OpenFile("pacman.dat", "r")) {
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
	CallInit();
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
	CallUpdate();
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
			} while (!t || (pentity->status & 7) != 1 || !(*reinterpret_cast<unsigned short *>(CModelInfo::ms_modelInfoPtrs[pentity->modelIndex] + 0x42) & 4));

			aPMPickups[i].state = 1;
			aPMPickups[i].position.x = colpoint.point.x;
			aPMPickups[i].position.y = colpoint.point.y;
			aPMPickups[i].position.z = colpoint.point.z + 0.7f;

			CObject *object = new CObject(scrambleModel, true);
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
			CObject *object = new CObject(raceModel, true);
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
	CallRender();
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
		vehicle->moveSpeed.x *= 0.65f;
		vehicle->moveSpeed.y *= 0.65f;
		vehicle->moveSpeed.z *= 0.65f;
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
