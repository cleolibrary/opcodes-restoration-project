#pragma once

#include "vcclasses.h"

namespace VCGlobals
{
	extern CCamera& TheCamera;
	extern CText& TheText;
	extern cDMAudio& DMAudio;
	extern cSampleManager& SampleManager;
	extern cAudioManager &AudioManager;

	extern char *ScrollCharSet;

	extern float &timeScale;
	extern unsigned char &currentPlayer;

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

	struct RsGlobalType
	{
		const char *appName;
		int maximumWidth;
		int maximumHeight;
		int currentWidth;
		int currentHeight;
		int  quit;
		void *ps;
		int keyboard[3];
		int mouse[3];
		int pad[3];
	};
	extern RsGlobalType& Globals;

	CPlayerPed *FindPlayerPed();
	CVehicle *FindPlayerVehicle();
	CVector *FindPlayerCoors(CVector *);
}
