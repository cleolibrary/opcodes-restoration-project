#pragma once

#include "vcclasses.h"

class CExplosionHack : public CExplosion
{
public:
	static bool TestForExplosionInArea(int explosionType, float x1, float x2, float y1, float y2, float z1, float z2);
};
