#include "iiiclasses.h"

static unsigned long g_GetHasCollidedWith = 0x00497240;

__declspec(naked) bool CPhysical::GetHasCollidedWith(CEntity *)
{
	__asm jmp g_GetHasCollidedWith;
}
