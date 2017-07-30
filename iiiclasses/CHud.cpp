#include "iiiclasses.h"

static unsigned long g_SetHelpMessage = 0x005051E0;

__declspec(naked) void CHud::SetHelpMessage(wchar_t *, bool)
{
	__asm jmp g_SetHelpMessage;
}
