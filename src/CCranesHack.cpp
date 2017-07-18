#include "CCranesHack.h"
#include <cmath>
#include "vcclasses.h"

bool CCranesHack::IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle)
{
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < 100.0 && aCranes[i].vehicle == vehicle && (aCranes[i].status == 2 || aCranes[i].status == 4)) {
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
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
			}
		}
	}
	if (index == -1) return;
	aCranes[index].activity = 2;
	aCranes[index].status = 0;
}

void CCranesHack::ActivateCrane(float pickupX1, float pickupX2, float pickupY1, float pickupY2, float dropoffX, float dropoffY, float dropoffZ, float dropoffHeading, bool isCrusher, bool isMilitary, float positionX, float positionY)
{
	int index = -1;
	float craneObjectX, craneObjectY, minDistance, pickupCenterX, pickupCenterY;
	minDistance = 100.0;
	if (CCranes::NumCranes > 0) {
		for (int i = 0; i < CCranes::NumCranes; i++) {
			if (aCranes[i].object) {
				float distance = sqrt(pow(positionX - aCranes[i].object->GetX(), 2) + pow(positionY - aCranes[i].object->GetY(), 2));
				if (distance < minDistance) {
					index = i;
					minDistance = distance;
				}
			}
		}
	}
	if (index == -1) return;
	aCranes[index].pickupX1 = pickupX1;
	aCranes[index].pickupX2 = pickupX2;
	aCranes[index].pickupY1 = pickupY1;
	aCranes[index].pickupY2 = pickupY2;
	aCranes[index].dropoffX = dropoffX;
	aCranes[index].dropoffY = dropoffY;
	aCranes[index].dropoffZ = dropoffZ;
	aCranes[index].activity = 1;
	aCranes[index].vehicle = 0;
	aCranes[index].countCollected = 0;
	aCranes[index].dropoffHeading = dropoffHeading;
	aCranes[index].isCrusher = isCrusher;
	aCranes[index].isMilitary = isMilitary;
	aCranes[index].timer = 0;
	aCranes[index].status = 0;
	pickupCenterX = (pickupX1 + pickupX2) / 2;
	pickupCenterY = (pickupY1 + pickupY2) / 2;
	craneObjectX = aCranes[index].object->GetX();
	craneObjectY = aCranes[index].object->GetY();
	/*if (isCrusher) {
		aCranes[index].armPickupHeight = -0.95099998f;
	} else if (isMilitary) {
		aCranes[index].armPickupHeight = 10.7862f;
	} else {
		aCranes[index].armPickupHeight = CWorld::FindGroundZForCoord(pickupCenterX, pickupCenterY);
	}*/
	aCranes[index].armPickupHeight = CWorld::FindGroundZForCoord(pickupCenterX, pickupCenterY);
	aCranes[index].armPickupRotation = CGeneral::GetATanOfXY(pickupCenterX - craneObjectX, pickupCenterY - craneObjectY);
	aCranes[index].armPickupDistance = sqrt(pow(pickupCenterX - craneObjectX, 2) + pow(pickupCenterY - craneObjectY, 2));
	aCranes[index].armDropoffRotation = CGeneral::GetATanOfXY(dropoffX - craneObjectX, dropoffY - craneObjectY);
	aCranes[index].armDropoffDistance = sqrt(pow(dropoffX - craneObjectX, 2) + pow(dropoffY - craneObjectY, 2));
	aCranes[index].armDropoffHeight = dropoffZ;
}
