#include "iiiclasses.h"

static unsigned long g_ClearWeapons = 0x004CFB70;

__declspec(naked) void CPed::ClearWeapons(void)
{
	__asm jmp g_ClearWeapons;
}
