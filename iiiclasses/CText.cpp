#include "iiiclasses.h"

static unsigned long g_Get = 0x0052C5A0;

__declspec(naked) wchar_t *CText::Get(const char *)
{
	__asm jmp g_Get;
}
