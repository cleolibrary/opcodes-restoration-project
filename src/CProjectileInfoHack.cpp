#include "CProjectileInfoHack.h"
#include "Globals.h"
#include "vcclasses.h"

bool CProjectileInfoHack::IsProjectileInRange(float x1, float y1, float z1, float x2, float y2, float z2, bool destroy)
{
	for (int i = 0; i < 0x20; i++) {
		CObject *object = ms_apProjectile[i];
		if (object) {
			if (VCGlobals::gaProjectileInfo[i].doesProjectileExist &&
				VCGlobals::gaProjectileInfo[i].weaponType >= 12 &&
				VCGlobals::gaProjectileInfo[i].weaponType <= 16 &&
				object->GetX() >= x1 && object->GetX() <= x2 &&
				object->GetY() >= y1 && object->GetY() <= y2 &&
				object->GetZ() >= z1 && object->GetZ() <= z2) {
				if (destroy) {
					VCGlobals::gaProjectileInfo[i].doesProjectileExist = 0;
					auto Destroy = (void(__thiscall *)(CObject *, int))*(unsigned long *)(object->vtbl + 8);
					CWorld::Remove(object);
					Destroy(object, 1);
				}
				return true;
			}
		}
	}
	return false;
}
