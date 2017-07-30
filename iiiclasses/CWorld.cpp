#include "iiiclasses.h"

CPlayerInfo *CWorld::Players = reinterpret_cast<CPlayerInfo *>(0x009412F0);
unsigned char &CWorld::PlayerInFocus = *reinterpret_cast<unsigned char *>(0x0095CD61);
