#include "iiiclasses.h"

static unsigned long g_InsertNumberInString = 0x0052A1A0;

__declspec(naked) void CMessages::InsertNumberInString(wchar_t *, int, int, int, int, int, int, wchar_t *)
{
	__asm jmp g_InsertNumberInString;
}
