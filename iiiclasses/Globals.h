#pragma once

#include "iiiclasses.h"

namespace IIIGlobals
{
	union ScriptParameter
	{
		char int8;
		short int16;
		int int32;
		unsigned char uint8;
		unsigned short uint16;
		unsigned int uint32;
		float float32;
	};
	extern ScriptParameter *ScriptParams;
	extern wchar_t *gUString;
	extern CText &TheText;

	CPlayerPed *FindPlayerPed(void);
}
