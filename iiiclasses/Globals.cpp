#include "Globals.h"

namespace IIIGlobals
{
	ScriptParameter *ScriptParams = reinterpret_cast<ScriptParameter *>(0x006ED460);
	wchar_t *gUString = reinterpret_cast<wchar_t *>(0x0074B018);
	CText &TheText = *reinterpret_cast<CText *>(0x00941520);

	static unsigned long g_FindPlayerPed = 0x004A1150;

	__declspec(naked) CPlayerPed *FindPlayerPed(void)
	{
		__asm jmp g_FindPlayerPed;
	}
}
