#include "CPedHack.h"

void CPedHack::ClearWeapon(int type)
{
	int slot = CWeaponInfo::GetWeaponInfo(type)->slot;
	if (slot != -1 && this->weapons[slot].type == type) {
		if (this->activeWeaponSlot == slot) {
			this->SetCurrentWeapon(CWeaponInfo::GetWeaponInfo(0)->slot);
		}
		this->weapons[slot].Shutdown();
	}
}
