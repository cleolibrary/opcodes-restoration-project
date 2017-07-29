#include "CExplosionHack.h"

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
		if (explosions[i].counter && explosions[i].type == explosionType &&
			explosions[i].position.x >= x1 && explosions[i].position.x <= x2 &&
			explosions[i].position.y >= y1 && explosions[i].position.y <= y2 &&
			explosions[i].position.z >= z1 && explosions[i].position.z <= z2) {
			return true;
		}
	}
	return false;
}
