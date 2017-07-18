#pragma once

#include "vcclasses.h"

class CCranesHack : public CCranes
{
public:
	static bool IsThisCarPickedUp(float positionX, float positionY, CVehicle *vehicle);
	static void DeActivateCrane(float positionX, float positionY);
	static void ActivateCrane(float pickupX1, float pickupX2, float pickupY1, float pickupY2, float dropoffX, float dropoffY, float dropoffZ, float dropoffHeading, bool isCrusher, bool isMilitary, float positionX, float positionY);
};
