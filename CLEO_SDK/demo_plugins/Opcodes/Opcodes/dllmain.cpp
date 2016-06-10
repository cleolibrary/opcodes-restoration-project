// configuration manager
// active solution configuration: VC or III
// active solution platform: x86

#include "stdafx.h"
#include <math.h>

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  4

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

enum gameVersion{ V1_0, V1_1, VSTEAM, VUNKOWN = -1 };

class GtaGame
{
public:
	gameVersion version;
	GtaGame();
};

GtaGame game;

#if _VC
#pragma comment (lib, "..\\..\\..\\VC.CLEO.lib")
#include "..\\..\\..\\VC.CLEO.h"

tScriptVar *Params;

struct CVector {
	float x, y, z;
};

DWORD(__cdecl *RpAnimBlendClumpGetFirstAssociation)(UINT_PTR); // rpanimblendclumpgetfirstassociation
void(__cdecl *StartFrenzy)(DWORD, INT, USHORT, INT, WCHAR *, INT, INT, INT, BOOLEAN, BOOLEAN); // cdarkel::startfrenzy
void(__cdecl *ResetOnPlayerDeath)(void); // cdarkel::resetonplayerdeath
BYTE(__thiscall *IsEntityEntirelyInside3D)(UINT_PTR, UINT_PTR, FLOAT); // cgarage::isentityentirelyinside3d
void(__cdecl *ChangeGarageType)(SHORT, UCHAR, UINT); // cgarages::changegaragetype
int(__thiscall *ObjectPoolGetHandle)(void *, void *); // cpool_cobject_ccutscenehead::getindex
void(__thiscall *SetPhoneMessage_JustOnce)(UINT_PTR, INT, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *); // cphoneinfo::setphonemessage_justonce
DWORD(__cdecl *GetActualPickupIndex)(INT); // cpickups::getactualpickupindex
void(__cdecl *CreatePickup)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD); // cpickups::generatenewone
void(__cdecl *OverrideNextRestart)(CVector *, FLOAT); // crestart::overridenextrestart
void(__thiscall *RemoveEntityFromList)(UINT_PTR, INT, UCHAR); // cmissioncleanup::removeentityfromlist
UINT_PTR(__thiscall *ObjectPoolGetStruct)(void *, INT); // cpool_cobject_ccutscenehead::getat
UINT_PTR(__thiscall *VehiclePoolGetStruct)(void *, INT); // cpool_cvehicle_cautomobile::getat
UINT_PTR(__thiscall *PedPoolGetStruct)(void *, INT); // cpool_cped_cplayerped::getat
int(__thiscall *PedPoolGetHandle)(void *, void *); // cpool_cped_cplayerped::getindex
void(__cdecl *HighlightImportantArea)(CScript *, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT); // cthescripts::highlightimportantarea
WORD(__thiscall *GetPadState)(CScript *, USHORT, USHORT); // crunningscript::getpadstate
void(__thiscall *CamShake)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT); // ccamera::camshake
BYTE(__thiscall *GetHasCollidedWith)(DWORD, UINT_PTR); // cphysical::gethascollidedwith
BYTE(__thiscall *IsWithinArea_3d)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT); // cplaceable::iswithinarea
BYTE(__thiscall *IsWithinArea_2d)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT); // cplaceable::iswithinarea
DWORD(__cdecl *FindPlayerPed)(void); // findplayerped
void(__cdecl *SetBlipSprite)(INT, INT); // cradar::setblipsprite
void(__cdecl *ChangeBlipScale)(INT, INT); // cradar::changeblipscale
DWORD(__cdecl *SetEntityBlip)(DWORD, INT, INT, DWORD); // cradar::setentityblip
DWORD(__cdecl *SetShortRangeBlip)(DWORD, FLOAT, FLOAT, FLOAT, DWORD, DWORD);
void(__cdecl *GetActualBlipArrayIndex)(INT); // cradar::getactualbliparrayindex
void(__thiscall *RegisterReference)(UINT_PTR, UINT_PTR); // centity::registerreference
void(__cdecl *AnotherKillFrenzyPassed)(void); // cstats::anotherkillfrenzypassed
FLOAT(__cdecl *FindGroundZForCoord)(FLOAT, FLOAT); // cworld::findgroundzforcoord
DWORD(__cdecl *Remove)(DWORD); // cworld::remove
void(__thiscall *SetAmmo)(UINT_PTR, DWORD, UINT); // cped::setammo
void(__thiscall *GrantAmmo)(UINT_PTR, DWORD, UINT); // cped::grantammo
void(__thiscall *GiveWeapon)(UINT_PTR, DWORD, DWORD, DWORD); // cped::giveweapon
BYTE(__thiscall *IsPedInControl)(UINT_PTR); // cped::ispedincontrol
void(__thiscall *Say)(UINT_PTR, WORD); // cped::say
void(__cdecl *SetHelpMessage)(WCHAR *, CHAR, INT); // chud::sethelpmessage
DWORD(__cdecl *GetVehicleTypeId)(DWORD); // cvehiclemodelinfo::getvehicletypeid
void(__cdecl *AddMessageJumpQWithNumber)(WCHAR *, UINT, WORD, INT, INT, INT, INT, INT, INT); // cmessages::addmessagejumpqwithnumber
void(__cdecl *AddMessageWithNumber)(WCHAR *, DWORD, WORD, INT, INT, INT, INT, INT, INT); // cmessages::addmessagewithnumber
void(__cdecl *InsertNumberInString)(WCHAR *, INT, INT, INT, INT, INT, INT, WCHAR *); // cmessages::insertnumberinstring
void(__cdecl *AddBigMessageQ)(WCHAR *, INT, WORD); // cmessages::addbigmessageq
void(__cdecl *AddBigMessage)(WCHAR *, INT, WORD); // cmessages::addbigmessage
WCHAR *(__thiscall *GetText)(UINT_PTR, PCHAR); // ctext::get
DWORD(__cdecl *GetWeaponInfo)(DWORD); // cweaponinfo::getweaponinfo
BYTE *nastyGame = NULL;
DWORD *barrel1 = NULL;
DWORD *barrel2 = NULL;
DWORD *maxWantedLevel = NULL;
BYTE *phoneDisplayMessage = NULL;
DWORD *currentPhone = NULL;
UINT_PTR cexplosion = NULL;
UINT_PTR cprojectileinfo = NULL;
UINT_PTR gangPedModelOverride = NULL;
UINT_PTR ccrane = NULL;
UINT_PTR ccamera = NULL;
UINT_PTR textDraw = NULL;
UINT_PTR infoZoneCarDensity = NULL;
UINT_PTR infoZonePedDensity = NULL;
UINT_PTR cgarage = NULL;
UINT_PTR cphoneinfo = NULL;
WCHAR *numberedText = NULL;
UINT_PTR infoZone = NULL;
UINT_PTR pickupEntity = NULL;
UINT_PTR *playerPedPool = NULL;
UINT_PTR *playerPedState = NULL;
UINT_PTR ctext = NULL;
UINT_PTR *projectileObject = NULL;
UINT_PTR navigZone = NULL;
void **objectPool = NULL;
FLOAT *shootingRangeRank = NULL;
DWORD *gameTimer = NULL;
FLOAT *garbagePickups = NULL;
FLOAT *loanSharkVisits = NULL;
DWORD *numCranes = NULL;
UINT_PTR cmissioncleanup = NULL;
void **pedPool = NULL;
UINT_PTR cfire = NULL;
DWORD *militaryCraneCollected = NULL;
FLOAT *topShootingRangeScore = NULL;
DWORD *levelName = NULL;
UINT_PTR cpedtype = NULL;
FLOAT *movieStunts = NULL;
void **carPool = NULL;
WORD *numInfoZone = NULL;
WORD *currentTextDraw = NULL;
WORD *numNavigZone = NULL;
BYTE *allTaxiLights = NULL;
BYTE *overrideHospital = NULL;
BYTE *overridePolice = NULL;
BYTE *currentPlayer = NULL;
BYTE *freeBombs = NULL;
BYTE *forceRain = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x74FF5064:
		version = V1_0;
		RpAnimBlendClumpGetFirstAssociation = (DWORD(__cdecl *)(UINT_PTR))0x402E20;
		StartFrenzy = (void(__cdecl *)(DWORD, INT, USHORT, INT, WCHAR *, INT, INT, INT, BOOLEAN, BOOLEAN))0x429B60;
		ResetOnPlayerDeath = (void(__cdecl *)(void))0x429F90;
		IsEntityEntirelyInside3D = (BYTE(__thiscall *)(UINT_PTR, UINT_PTR, FLOAT))0x430630;
		ChangeGarageType = (void(__cdecl *)(SHORT, UCHAR, UINT))0x4340F0;
		ObjectPoolGetHandle = (int(__thiscall *)(void *, void *))0x434A10;
		SetPhoneMessage_JustOnce = (void(__thiscall *)(UINT_PTR, INT, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *))0x43C430;
		GetActualPickupIndex = (DWORD(__cdecl *)(INT))0x43D360;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		OverrideNextRestart = (void(__cdecl *)(CVector *, FLOAT))0x4429E0;
		RemoveEntityFromList = (void(__thiscall *)(UINT_PTR, INT, UCHAR))0x4518E0;
		ObjectPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451C30;
		VehiclePoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451C70;
		PedPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451CB0;
		PedPoolGetHandle = (int(__thiscall *)(void *, void *))0x451CF0;
		HighlightImportantArea = (void(__cdecl *)(CScript *, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT))0x45F080;
		GetPadState = (WORD(__thiscall *)(CScript *, USHORT, USHORT))0x460C00;
		CamShake = (void(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, UINT_PTR))0x4B9010;
		IsWithinArea_3d = (BYTE(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT))0x4BB900;
		IsWithinArea_2d = (BYTE(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT))0x4BB9E0;
		FindPlayerPed = (DWORD(__cdecl *)(void))0x4BC120;
		SetBlipSprite = (void(__cdecl *)(INT, INT))0x4C3780;
		ChangeBlipScale = (void(__cdecl *)(INT, INT))0x4C3840;
		SetEntityBlip = (DWORD(__cdecl *)(DWORD, INT, INT, DWORD))0x4C3B40;
		SetShortRangeBlip = (DWORD(__cdecl *)(DWORD, FLOAT, FLOAT, FLOAT, DWORD, DWORD))0x4C3C00;
		GetActualBlipArrayIndex = (void(__cdecl *)(INT))0x4C5D70;
		RegisterReference = (void(__thiscall *)(UINT_PTR, UINT_PTR))0x4C6AC0;
		AnotherKillFrenzyPassed = (void(__cdecl *)(void))0x4CDBA7;
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))0x4D5540;
		Remove = (DWORD(__cdecl *)(DWORD))0x4DB310;
		SetAmmo = (void(__thiscall *)(UINT_PTR, DWORD, UINT))0x4FF780;
		GrantAmmo = (void(__thiscall *)(UINT_PTR, DWORD, UINT))0x4FF840;
		GiveWeapon = (void(__thiscall *)(UINT_PTR, DWORD, DWORD, DWORD))0x4FFA30;
		IsPedInControl = (BYTE(__thiscall *)(UINT_PTR))0x501950;
		Say = (void(__thiscall *)(UINT_PTR, WORD))0x5226B0;
		SetHelpMessage = (void(__cdecl *)(WCHAR *, CHAR, INT))0x55BFC0;
		GetVehicleTypeId = (DWORD(__cdecl *)(DWORD))0x578A70;
		AddMessageJumpQWithNumber = (void(__cdecl *)(WCHAR *, UINT, WORD, INT, INT, INT, INT, INT, INT))0x583440;
		AddMessageWithNumber = (void(__cdecl *)(WCHAR *, DWORD, WORD, INT, INT, INT, INT, INT, INT))0x583560;
		InsertNumberInString = (void(__cdecl *)(WCHAR *, INT, INT, INT, INT, INT, INT, WCHAR *))0x583C80;
		AddBigMessageQ = (void(__cdecl *)(WCHAR *, INT, WORD))0x583F40;
		AddBigMessage = (void(__cdecl *)(WCHAR *, INT, WORD))0x584050;
		GetText = (WCHAR *(__thiscall *)(UINT_PTR, PCHAR))0x584F30;
		GetWeaponInfo = (DWORD(__cdecl *)(DWORD))0x5D5710;
		nastyGame = (BYTE *)0x68DD68;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		phoneDisplayMessage = (BYTE *)0x7030E4;
		currentPhone = (DWORD *)0x7030E8;
		cexplosion = 0x780C88;
		cprojectileinfo = 0x7DB888;
		gangPedModelOverride = 0x7D925C;
		ccrane = 0x7E4040;
		ccamera = 0x7E4688;
		textDraw = 0x7F0EA0;
		infoZoneCarDensity = 0x7FA370;
		infoZonePedDensity = 0x7FA39C;
		cgarage = 0x812668;
		cphoneinfo = 0x817CF0;
		numberedText = (WCHAR *)0x821068;
		infoZone = 0x862508;
		pickupEntity = 0x945D40;
		playerPedPool = (UINT_PTR *)0x94AD28;
		playerPedState = (UINT_PTR *)0x94ADF4;
		ctext = 0x94B220;
		projectileObject = (UINT_PTR *)0x94B708;
		navigZone = 0x94B990;
		objectPool = (void **)0x94DBE0;
		shootingRangeRank = (FLOAT *)0x974B08;
		gameTimer = (DWORD *)0x974B2C;
		garbagePickups = (FLOAT *)0x974C00;
		loanSharkVisits = (FLOAT*)0x974C28;
		numCranes = (DWORD *)0x974C34;
		cmissioncleanup = 0x97F060;
		pedPool = (void **)0x97F2AC;
		cfire = 0x97F8A0;
		militaryCraneCollected = (DWORD *)0x9B6CE4;
		topShootingRangeScore = (FLOAT *)0xA0D8A4;
		levelName = (DWORD *)0xA0D9AC;
		cpedtype = (UINT_PTR)0xA0DA64;
		movieStunts = (FLOAT *)0xA0FC8C;
		carPool = (void **)0xA0FDE4;
		numInfoZone = (WORD *)0xA1096A;
		currentTextDraw = (WORD *)0xA10A48;
		numNavigZone = (WORD *)0xA10A58;
		allTaxiLights = (BYTE *)0xA10ABB;
		overrideHospital = (BYTE *)0xA10AE6;
		overridePolice = (BYTE *)0xA10AEA;
		currentPlayer = (BYTE *)0xA10AFB;
		freeBombs = (BYTE *)0xA10B32;
		forceRain = (BYTE *)0xA10B38;
		break;
	case 0x00408DC0:
		version = V1_1;
		RpAnimBlendClumpGetFirstAssociation = (DWORD(__cdecl *)(UINT_PTR))0x402E20;
		StartFrenzy = (void(__cdecl *)(DWORD, INT, USHORT, INT, WCHAR *, INT, INT, INT, BOOLEAN, BOOLEAN))0x429B60;
		ResetOnPlayerDeath = (void(__cdecl *)(void))0x429F90;
		IsEntityEntirelyInside3D = (BYTE(__thiscall *)(UINT_PTR, UINT_PTR, FLOAT))0x430630;
		ChangeGarageType = (void(__cdecl *)(SHORT, UCHAR, UINT))0x4340F0;
		ObjectPoolGetHandle = (int(__thiscall *)(void *, void *))0x434A10;
		SetPhoneMessage_JustOnce = (void(__thiscall *)(UINT_PTR, INT, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR *))0x43C430;
		GetActualPickupIndex = (DWORD(__cdecl *)(INT))0x43D360;
		CreatePickup = (void(__cdecl *)(FLOAT, FLOAT, FLOAT, DWORD, DWORD, DWORD, DWORD, BYTE, DWORD))0x4418C0;
		OverrideNextRestart = (void(__cdecl *)(CVector *, FLOAT))0x4429E0;
		RemoveEntityFromList = (void(__thiscall *)(UINT_PTR, INT, UCHAR))0x4518E0;
		ObjectPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451C30;
		VehiclePoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451C70;
		PedPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))0x451CB0;
		PedPoolGetHandle = (int(__thiscall *)(void *, void *))0x451CF0;
		HighlightImportantArea = (void(__cdecl *)(CScript *, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT))0x45F080;
		GetPadState = (WORD(__thiscall *)(CScript *, USHORT, USHORT))0x460C00;
		CamShake = (void(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT))0x46FF21;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, UINT_PTR))(0x4B9010 + 0x20);
		IsWithinArea_3d = (BYTE(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT))(0x4BB900 + 0x20);
		IsWithinArea_2d = (BYTE(__thiscall *)(UINT_PTR, FLOAT, FLOAT, FLOAT, FLOAT))(0x4BB9E0 + 0x20);
		FindPlayerPed = (DWORD(__cdecl *)(void))(0x4BC120 + 0x20);
		SetBlipSprite = (void(__cdecl *)(INT, INT))(0x4C3780 + 0x20);
		ChangeBlipScale = (void(__cdecl *)(INT, INT))(0x4C3840 + 0x20);
		SetEntityBlip = (DWORD(__cdecl *)(DWORD, INT, INT, DWORD))(0x4C3B40 + 0x20);
		SetShortRangeBlip = (DWORD(__cdecl *)(DWORD, FLOAT, FLOAT, FLOAT, DWORD, DWORD))(0x4C3C00 + 0x20);
		GetActualBlipArrayIndex = (void(__cdecl *)(INT))(0x4C5D70 + 0x20);
		RegisterReference = (void(__thiscall *)(UINT_PTR, UINT_PTR))(0x4C6AC0 + 0x20);
		AnotherKillFrenzyPassed = (void(__cdecl *)(void))(0x4CDBA7 + 0x20);
		FindGroundZForCoord = (FLOAT(__cdecl *)(FLOAT, FLOAT))(0x4D5540 + 0x20);
		Remove = (DWORD(__cdecl *)(DWORD))(0x4DB310 + 0x20);
		SetAmmo = (void(__thiscall *)(UINT_PTR, DWORD, UINT))(0x4FF780 + 0x20);
		GrantAmmo = (void(__thiscall *)(UINT_PTR, DWORD, UINT))(0x4FF840 + 0x20);
		GiveWeapon = (void(__thiscall *)(UINT_PTR, DWORD, DWORD, DWORD))(0x4FFA30 + 0x20);
		IsPedInControl = (BYTE(__thiscall *)(UINT_PTR))(0x501950 + 0x20);
		Say = (void(__thiscall *)(UINT_PTR, WORD))(0x5226B0 + 0x20);
		SetHelpMessage = (void(__cdecl *)(WCHAR *, CHAR, INT))(0x55BFC0 + 0x20);
		GetVehicleTypeId = (DWORD(__cdecl *)(DWORD))(0x578A70 + 0x20);
		AddMessageJumpQWithNumber = (void(__cdecl *)(WCHAR *, UINT, WORD, INT, INT, INT, INT, INT, INT))(0x583440 + 0x20);
		AddMessageWithNumber = (void(__cdecl *)(WCHAR *, DWORD, WORD, INT, INT, INT, INT, INT, INT))(0x583560 + 0x20);
		InsertNumberInString = (void(__cdecl *)(WCHAR *, INT, INT, INT, INT, INT, INT, WCHAR *))(0x583C80 + 0x20);
		AddBigMessageQ = (void(__cdecl *)(WCHAR *, INT, WORD))(0x583F40 + 0x20);
		AddBigMessage = (void(__cdecl *)(WCHAR *, INT, WORD))(0x584050 + 0x20);
		GetText = (WCHAR *(__thiscall *)(UINT_PTR, PCHAR))(0x584F30 + 0x20);
		GetWeaponInfo = (DWORD(__cdecl *)(DWORD))(0x5D5710 + 0x20);
		nastyGame = (BYTE *)0x68DD68;
		barrel1 = (DWORD *)0x68E8B0;
		barrel2 = (DWORD *)0x68E910;
		maxWantedLevel = (DWORD *)0x6910D8;
		phoneDisplayMessage = (BYTE *)0x7030E4;
		currentPhone = (DWORD *)0x7030E8;
		cexplosion = 0x780C88;
		cprojectileinfo = 0x7DB888 + 8;
		gangPedModelOverride = 0x7D925C + 8;
		ccrane = 0x7E4040 + 8;
		ccamera = 0x7E4688 + 8;
		textDraw = 0x7F0EA0 + 8;
		infoZoneCarDensity = 0x7FA370 + 8;
		infoZonePedDensity = 0x7FA39C + 8;
		cgarage = 0x812668 + 8;
		cphoneinfo = 0x817CF0 + 8;
		numberedText = (WCHAR *)(0x821068 + 8);
		infoZone = 0x862508 + 8;
		pickupEntity = 0x945D40 + 8;
		playerPedPool = (UINT_PTR *)(0x94AD28 + 8);
		playerPedState = (UINT_PTR *)(0x94ADF4 + 8);
		ctext = 0x94B220 + 8;
		projectileObject = (UINT_PTR *)(0x94B708 + 8);
		navigZone = 0x94B990 + 8;
		objectPool = (void **)(0x94DBE0 + 8);
		shootingRangeRank = (FLOAT *)(0x974B08 + 8);
		gameTimer = (DWORD *)(0x974B2C + 8);
		garbagePickups = (FLOAT *)(0x974C00 + 8);
		loanSharkVisits = (FLOAT*)(0x974C28 + 8);
		numCranes = (DWORD *)(0x974C34 + 8);
		cmissioncleanup = 0x97F060 + 8;
		pedPool = (void **)(0x97F2AC + 8);
		cfire = 0x97F8A0 + 8;
		militaryCraneCollected = (DWORD *)(0x9B6CE4 + 8);
		topShootingRangeScore = (FLOAT *)(0xA0D8A4 + 8);
		levelName = (DWORD *)(0xA0D9AC + 8);
		cpedtype = (UINT_PTR)(0xA0DA64 + 8);
		movieStunts = (FLOAT *)(0xA0FC8C + 8);
		carPool = (void **)(0xA0FDE4 + 8);
		numInfoZone = (WORD *)(0xA1096A + 8);
		currentTextDraw = (WORD *)(0xA10A48 + 8);
		numNavigZone = (WORD *)(0xA10A58 + 8);
		allTaxiLights = (BYTE *)(0xA10ABB + 8);
		overrideHospital = (BYTE *)(0xA10AE6 + 8);
		overridePolice = (BYTE *)(0xA10AEA + 8);
		currentPlayer = (BYTE *)(0xA10AFB + 8);
		freeBombs = (BYTE *)(0xA10B32 + 8);
		forceRain = (BYTE *)(0xA10B38 + 8);
		break;
	case 0x00004824:
		version = VSTEAM;
		ObjectPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))(0x451C30 - 0x120);
		VehiclePoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))(0x451C70 - 0x120);
		PedPoolGetStruct = (UINT_PTR(__thiscall *)(void *, INT))(0x451CB0 - 0x120);
		GetText = (WCHAR *(__thiscall *)(UINT_PTR, PCHAR))(0x584F30 - 0x1D0);
		cprojectileinfo = 0x7DB888 - 0xFF8;
		gangPedModelOverride = 0x7D925C - 0xFF8;
		ccrane = 0x7E4040 - 0xFF8;
		ccamera = 0x7E4688 - 0xFF8;
		textDraw = 0x7F0EA0 - 0xFF8;
		infoZoneCarDensity = 0x7FA370 - 0xFF8;
		infoZonePedDensity = 0x7FA39C - 0xFF8;
		cgarage = 0x812668 - 0xFF8;
		cphoneinfo = 0x817CF0 - 0xFF8;
		numberedText = (WCHAR *)(0x821068 - 0xFF8);
		infoZone = 0x862508 - 0xFF8;
		pickupEntity = 0x945D40 - 0xFF8;
		playerPedPool = (UINT_PTR *)(0x94AD28 - 0xFF8);
		playerPedState = (UINT_PTR *)(0x94ADF4 - 0xFF8);
		ctext = 0x94B220 - 0xFF8;
		projectileObject = (UINT_PTR *)(0x94B708 - 0xFF8);
		navigZone = 0x94B990 - 0xFF8;
		objectPool = (void **)(0x94DBE0 - 0xFF8);
		shootingRangeRank = (FLOAT *)(0x974B08 - 0xFF8);
		gameTimer = (DWORD *)(0x974B2C - 0xFF8);
		garbagePickups = (FLOAT *)(0x974C00 - 0xFF8);
		loanSharkVisits = (FLOAT*)(0x974C28 - 0xFF8);
		numCranes = (DWORD *)(0x974C34 - 0xFF8);
		cmissioncleanup = 0x97F060 - 0xFF8;
		pedPool = (void **)(0x97F2AC - 0xFF8);
		cfire = 0x97F8A0 - 0xFF8;
		militaryCraneCollected = (DWORD *)(0x9B6CE4 - 0xFF8);
		topShootingRangeScore = (FLOAT *)(0xA0D8A4 - 0xFF8);
		levelName = (DWORD *)(0xA0D9AC - 0xFF8);
		cpedtype = (UINT_PTR)(0xA0DA64 - 0xFF8);
		movieStunts = (FLOAT *)(0xA0FC8C - 0xFF8);
		carPool = (void **)(0xA0FDE4 - 0xFF8);
		numInfoZone = (WORD *)(0xA1096A - 0xFF8);
		currentTextDraw = (WORD *)(0xA10A48 - 0xFF8);
		numNavigZone = (WORD *)(0xA10A58 - 0xFF8);
		break;
	}
}

bool IsProjectileInRange(FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2, BOOLEAN flag)
{
	UINT_PTR *object = projectileObject;
	for (UINT_PTR info = cprojectileinfo; info < cprojectileinfo + 0x20 * 0x1C; info += 0x1C, object++) {
		if (*object != NULL) {
			if (*(BYTE *)(info + 0xC) != 0 && *(DWORD *)info >= 12 && *(DWORD *)info <= 16) {
				if (*(FLOAT *)(*object + 0x34) >= x1 && *(FLOAT *)(*object + 0x34) <= x2) {
					if (*(FLOAT *)(*object + 0x38) >= y1 && *(FLOAT *)(*object + 0x38) <= y2) {
						if (*(FLOAT *)(*object + 0x3C) >= z1 && *(FLOAT *)(*object + 0x3C) <= z2) {
							if (flag) {
								*(BYTE *)(info + 0xC) = 0;
								auto destroy = (void(__thiscall *)(UINT_PTR, INT))*(UINT_PTR *)((*(UINT_PTR *)(*object)) + 8);
								Remove(*object);
								destroy(*object, 1);
							}
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void SetPhoneMessage_Repeatedly(UINT_PTR cphoneinfo, INT phone, WCHAR *string1, WCHAR *string2, WCHAR *string3, WCHAR *string4, WCHAR *string5, WCHAR *string6)
{
	phone = phone * 0x34 + cphoneinfo;
	*(WCHAR **)(phone + 0x14) = string1;
	*(WCHAR **)(phone + 0x18) = string2;
	*(WCHAR **)(phone + 0x1C) = string3;
	*(WCHAR **)(phone + 0x20) = string4;
	*(WCHAR **)(phone + 0x24) = string5;
	*(WCHAR **)(phone + 0x28) = string6;
	*(DWORD *)(phone + 0x34) = 5;
}

bool TestForExplosionInArea(DWORD explosionType, FLOAT x1, FLOAT x2, FLOAT y1, FLOAT y2, FLOAT z1, FLOAT z2)
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
	for (UINT_PTR address = cexplosion; address < cexplosion + 0x30 * 0x38; address += 0x38) {
		if (*(BYTE *)(address + 0x24) != 0) {
			if (*(DWORD *)(address) == explosionType) {
				if (*(FLOAT *)(address + 4) >= x1 && *(FLOAT *)(address + 4) <= x2) {
					if (*(FLOAT *)(address + 8) >= y1 && *(FLOAT *)(address + 8) <= y2) {
						if (*(FLOAT *)(address + 0xC) >= z1 && *(FLOAT *)(address + 0xC) <= z2) {
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

/* 00A2 */
eOpcodeResult WINAPI IS_CHAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		DWORD state = *(DWORD *)(ped + 0x244);
		if (state != 0x37 && state != 0x36) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 00AC */
eOpcodeResult WINAPI IS_CAR_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	UINT_PTR car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (car != NULL) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag != 5) {
			flag = *(BYTE *)(car + 0x1FD);
			flag &= 1;
			if (flag != 1) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 00C5 */
eOpcodeResult WINAPI RETURN_TRUE(CScript *script)
{
	script->UpdateCompareFlag(true);
	return OR_CONTINUE;
}

/* 00C6 */
eOpcodeResult WINAPI RETURN_FALSE(CScript *script)
{
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 00E2 */
eOpcodeResult WINAPI GET_PAD_STATE(CScript *script)
{
	script->Collect(2);
	Params[0].nVar = (unsigned int)GetPadState(script, Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0113 */
eOpcodeResult WINAPI ADD_AMMO_TO_PLAYER(CScript *script)
{
	script->Collect(3);
	GrantAmmo(playerPedPool[0x2E * Params[0].nVar], Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(playerPedState[0x2E * Params[0].nVar] != 1);
	return OR_CONTINUE;
}

/* 0130 */
eOpcodeResult WINAPI HAS_PLAYER_BEEN_ARRESTED(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(playerPedState[0x2E * Params[0].nVar] == 2);
	return OR_CONTINUE;
}

/* 0135 */
eOpcodeResult WINAPI CHANGE_CAR_LOCK(CScript *script)
{
	script->Collect(2);
	UINT_PTR car;
	if ((car = VehiclePoolGetStruct(*carPool, Params[0].nVar)) != NULL) {
		*(DWORD *)(car + 0x230) = Params[1].nVar;
	}
	return OR_CONTINUE;
}

/* 0136 */
eOpcodeResult WINAPI SHAKE_CAM_WITH_POINT(CScript *script)
{
	script->Collect(4);
	CamShake(ccamera, ((float)Params[0].nVar) * 1e-3f, Params[1].fVar, Params[2].fVar, Params[3].fVar);
	return OR_CONTINUE;
}

/* 013B */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_2D(CScript *script)
{
	script->Collect(6);
	UINT_PTR car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (Params[5].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
	}
	if (car != NULL) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag == 5 && IsWithinArea_2d(car + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 013C */
eOpcodeResult WINAPI IS_CAR_DEAD_IN_AREA_3D(CScript *script)
{
	script->Collect(8);
	UINT_PTR car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	if (Params[7].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[4].fVar, Params[5].fVar, (Params[3].fVar + Params[6].fVar) * 0.5f);
	}
	if (car != NULL) {
		BYTE flag = *(BYTE *)(car + 0x50);
		flag >>= 3;
		if (flag == 5 && IsWithinArea_3d(car + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, Params[6].fVar)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0155 */
eOpcodeResult WINAPI SET_CAR_DENSITY(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(2);
	for (UINT_PTR address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			int offset;
			offset = (BYTE)Params[0].nVar ? *(WORD *)(address + 0x28) : *(WORD *)(address + 0x2A);
			offset *= 0x44;
			offset += infoZoneCarDensity;
			*(WORD *)offset = (WORD)Params[1].nVar;
		}
	}
	return OR_CONTINUE;
}

/* 0156 */
eOpcodeResult WINAPI SET_PED_DENSITY(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(2);
	for (UINT_PTR address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			int offset;
			offset = (BYTE)Params[0].nVar ? *(WORD *)(address + 0x28) : *(WORD *)(address + 0x2A);
			offset *= 0x44;
			offset += infoZonePedDensity;
			*(WORD *)offset = (WORD)Params[1].nVar;
		}
	}
	return OR_CONTINUE;
}

/* 015E */
eOpcodeResult WINAPI IS_CAR_IN_AIR(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x5C5) == 0);
	return OR_CONTINUE;
}

/* 0163 */
eOpcodeResult WINAPI ADD_BLIP_FOR_OBJECT_OLD(CScript *script)
{
	script->Collect(3);
	ObjectPoolGetStruct(*objectPool, Params[0].nVar);
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(3, Params[0].nVar, Params[1].nVar, Params[2].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0178 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	DWORD source = playerPedPool[0x2E * Params[0].nVar];
	UINT_PTR target = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		UINT_PTR car = *(UINT_PTR *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

/* 0179 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT(CScript *script)
{
	script->Collect(2);
	UINT_PTR source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	UINT_PTR target = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		UINT_PTR car = *(UINT_PTR *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

/* 017B */
eOpcodeResult WINAPI SET_CHAR_AMMO(CScript *script)
{
	script->Collect(3);
	SetAmmo(PedPoolGetStruct(*pedPool, Params[0].nVar), Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 01C6 */
eOpcodeResult WINAPI DONT_REMOVE_CAR(CScript *script)
{
	script->Collect(1);
	VehiclePoolGetStruct(*carPool, Params[0].nVar);
	RemoveEntityFromList(cmissioncleanup, Params[0].nVar, 1);
	return OR_CONTINUE;
}

/* 0218 */
eOpcodeResult WINAPI PRINT_WITH_NUMBER_BIG_Q(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(3);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	AddBigMessageQ(numberedText, Params[1].nVar, (WORD)Params[2].nVar - 1);
	return OR_CONTINUE;
}

/* 021D */
eOpcodeResult WINAPI SET_FREE_BOMBS(CScript *script)
{
	script->Collect(1);
	*freeBombs = (BYTE)(Params[0].nVar ? 1 : 0);
	return OR_CONTINUE;
}

/* 021F */
eOpcodeResult WINAPI SET_ALL_TAXI_LIGHTS(CScript *script)
{
	script->Collect(1);
	*allTaxiLights = (BYTE)(Params[0].nVar ? 1 : 0);
	return OR_CONTINUE;
}

/* 0220 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_ANY_BOMB(CScript *script)
{
	script->Collect(1);
	if (*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FE) & 7) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0228 */
eOpcodeResult WINAPI IS_CAR_ARMED_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	if ((*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FE) & 7) == Params[1].nVar) {
		script->UpdateCompareFlag(true);
		return OR_CONTINUE;
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 023A */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	DWORD player = playerPedPool[0x2E * Params[0].nVar];
	UINT_PTR object = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (!*(BYTE *)(player + 0x3AC)) {
		if (GetHasCollidedWith(player, object)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 023B */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_OBJECT_ON_FOOT(CScript *script)
{
	script->Collect(2);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	UINT_PTR object = ObjectPoolGetStruct(*objectPool, Params[1].nVar);
	if (!*(BYTE *)(ped + 0x3AC)) {
		if (GetHasCollidedWith(ped, object)) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0242 */
eOpcodeResult WINAPI ARM_CAR_WITH_BOMB(CScript *script)
{
	script->Collect(2);
	UINT_PTR car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	BYTE input = ((BYTE)Params[1].nVar) & 7;
	BYTE state = (*(BYTE *)(car + 0x1FE) & 0xF8);
	state |= input;
	*(BYTE *)(car + 0x1FE) = state;
	*(DWORD *)(car + 0x210) = FindPlayerPed();
	return OR_CONTINUE;
}

/* 024B */
eOpcodeResult WINAPI SET_REPEATED_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string, NULL, NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 024C */
eOpcodeResult WINAPI SET_PHONE_MESSAGE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string, NULL, NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 024D */
eOpcodeResult WINAPI HAS_PHONE_DISPLAYED_MESSAGE(CScript *script)
{
	script->Collect(1);
	if (*phoneDisplayMessage == 0) {
		DWORD state = *(DWORD *)(Params[0].nVar * 0x34 + cphoneinfo + 0x34);
		if (state == 6 || state == 7 || state == 8) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0255 */
eOpcodeResult WINAPI RESTART_CRITICAL_MISSION(CScript *script)
{
	script->Collect(4);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	CVector pos;
	pos.x = Params[0].fVar;
	pos.y = Params[1].fVar;
	pos.z = Params[2].fVar;
	OverrideNextRestart(&pos, Params[3].fVar);
	DWORD player = *currentPlayer * 0x170 + (DWORD)playerPedPool;
	if (*(BYTE *)(player + 0xCC) == 0) {
		*(BYTE *)(player + 0xCC) = 3;
		*(DWORD *)(player + 0xD0) = *gameTimer;
		ResetOnPlayerDeath();
	}
	return OR_CONTINUE;
}

/* 0295 */
eOpcodeResult WINAPI IS_TAXI(CScript *script)
{
	script->Collect(1);
	WORD model = *(WORD *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x5C);
	script->UpdateCompareFlag(model == 0x96 || model == 0xA8 || model == 0xBC || model == 0xD8);
	return OR_CONTINUE;
}

/* 0299 */
eOpcodeResult WINAPI ACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	BYTE *pgarage = (BYTE *)(Params[0].nVar * 0xA8 + cgarage);
	if (*pgarage == 11) {
		if (*(pgarage + 1) == 0) {
			*(pgarage + 1) = 3;
		}
	}
	*(pgarage + 5) = 0;
	return OR_CONTINUE;
}

/* 029C */
eOpcodeResult WINAPI IS_BOAT(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(*(DWORD *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x29C) == 1);
	return OR_CONTINUE;
}

/* 02A0 */
eOpcodeResult WINAPI IS_CHAR_STOPPED(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (*(BYTE *)(ped + 0x3AC)) {
		UINT_PTR car = *(UINT_PTR *)(ped + 0x3A8);
		if (*(float *)(car + 0x100) == 0.0) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	} else {
		if (*(DWORD *)(ped + 0x24C) <= 1) {
			script->UpdateCompareFlag(true);
			return OR_CONTINUE;
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 02A4 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CAR(CScript *script)
{
	script->Collect(2);
	VehiclePoolGetStruct(*carPool, Params[0].nVar);
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(1, Params[0].nVar, 0, 3);
	SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A5 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_CHAR(CScript *script)
{
	script->Collect(2);
	PedPoolGetStruct(*pedPool, Params[0].nVar);
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(2, Params[0].nVar, 1, 3);
	SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02A6 */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_OBJECT(CScript *script)
{
	script->Collect(2);
	ObjectPoolGetStruct(*objectPool, Params[0].nVar);
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(3, Params[0].nVar, 6, 3);
	SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 02B9 */
eOpcodeResult WINAPI DEACTIVATE_GARAGE(CScript *script)
{
	script->Collect(1);
	*((BYTE *)(Params[0].nVar * 0xA8 + cgarage + 5)) = 1;
	return OR_CONTINUE;
}

/* 02BC */
eOpcodeResult WINAPI SET_SWAT_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE swat = *(BYTE *)(police + 0x1E);
	swat &= 0xFB;
	if (Params[0].nVar) {
		swat |= 4;
	}
	*(BYTE *)(police + 0x1E) = swat;
	return OR_CONTINUE;
}

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE fbi = *(BYTE *)(police + 0x1E);
	fbi &= 0xF7;
	if (Params[0].nVar) {
		fbi |= 8;
	}
	*(BYTE *)(police + 0x1E) = fbi;
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x5F4);
	BYTE army = *(BYTE *)(police + 0x1E);
	army &= 0xEF;
	if (Params[0].nVar) {
		army |= 16;
	}
	*(BYTE *)(police + 0x1E) = army;
	return OR_CONTINUE;
}

/* 02D6 */
eOpcodeResult WINAPI IS_CHAR_SHOOTING_IN_AREA(CScript *script)
{
	script->Collect(6);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	BYTE state = *(BYTE *)(ped + 0x14E);
	state >>= 6;
	state &= 1;
	script->UpdateCompareFlag(state != 0 && IsWithinArea_2d(ped + 4, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar) != 0);
	if (Params[5].nVar) {
		HighlightImportantArea(script, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, -100.0);
	}
	return OR_CONTINUE;
}

/* 02EE */
eOpcodeResult WINAPI IS_PROJECTILE_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (Params[0].fVar > Params[3].fVar) {
		temp = Params[3].fVar;
		Params[3].fVar = Params[0].fVar;
		Params[0].fVar = temp;
	}
	if (Params[1].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[1].fVar;
		Params[1].fVar = temp;
	}
	if (Params[2].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	script->UpdateCompareFlag(IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 0));
	return OR_CONTINUE;
}

/* 02EF */
eOpcodeResult WINAPI DESTROY_PROJECTILES_IN_AREA(CScript *script)
{
	script->Collect(6);
	float temp;
	if (Params[0].fVar > Params[3].fVar) {
		temp = Params[3].fVar;
		Params[3].fVar = Params[0].fVar;
		Params[0].fVar = temp;
	}
	if (Params[1].fVar > Params[4].fVar) {
		temp = Params[4].fVar;
		Params[4].fVar = Params[1].fVar;
		Params[1].fVar = temp;
	}
	if (Params[2].fVar > Params[5].fVar) {
		temp = Params[5].fVar;
		Params[5].fVar = Params[2].fVar;
		Params[2].fVar = temp;
	}
	script->UpdateCompareFlag(IsProjectileInRange(Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].fVar, Params[4].fVar, Params[5].fVar, 1));
	return OR_CONTINUE;
}

/* 02F0 */
eOpcodeResult WINAPI DROP_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CreatePickup(Params[0].fVar, Params[1].fVar, Params[2].fVar, *barrel1, 9, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02F1 */
eOpcodeResult WINAPI DROP_NAUTICAL_MINE(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar) + 0.5f;
	}
	CreatePickup(Params[0].fVar, Params[1].fVar, Params[2].fVar, *barrel2, 11, 0, 0, 0, 0);
	return OR_CONTINUE;
}

/* 02FC */
eOpcodeResult WINAPI PRINT_WITH_2_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(4);
	AddMessageWithNumber(string, Params[2].nVar, (WORD)Params[3].nVar, Params[0].nVar, Params[1].nVar, -1, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0300 */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(5);
	AddMessageJumpQWithNumber(string, Params[3].nVar, (WORD)Params[4].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, -1, -1, -1);
	return OR_CONTINUE;
}

/* 0303 */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(6);
	AddMessageJumpQWithNumber(string, Params[4].nVar, (WORD)Params[5].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, -1, -1);
	return OR_CONTINUE;
}

/* 0305 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(7);
	AddMessageWithNumber(string, Params[5].nVar, (WORD)Params[6].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0306 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(7);
	AddMessageJumpQWithNumber(string, Params[5].nVar, (WORD)Params[6].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, -1);
	return OR_CONTINUE;
}

/* 0309 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_NOW(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(8);
	AddMessageJumpQWithNumber(string, Params[6].nVar, (WORD)Params[7].nVar, Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar);
	return OR_CONTINUE;
}

/* 031B */
eOpcodeResult WINAPI IS_FIRST_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag((DWORD)*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1A0) == Params[1].nVar);
	return OR_CONTINUE;
}

/* 031C */
eOpcodeResult WINAPI IS_SECOND_CAR_COLOUR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag((DWORD)*(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1A1) == Params[1].nVar);
	return OR_CONTINUE;
}

/* 032D */
eOpcodeResult WINAPI SET_CAR_BLOCK_CAR(CScript *script)
{
	script->Collect(2);
	UINT_PTR car = VehiclePoolGetStruct(*carPool, Params[0].nVar);
	UINT_PTR target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	*(DWORD *)(car + 0x19C) = target;
	RegisterReference(target, car + 0x19C);
	*(BYTE *)(car + 0x156) = 0x11;
	BYTE flag = *(BYTE *)(car + 0x1F9);
	flag &= 0xEF;
	flag |= 0x10;
	*(BYTE *)(car + 0x1F9) = flag;
	if (*(BYTE *)(car + 0x160) < 6) {
		*(BYTE *)(car + 0x160) = 6;
	}
	return OR_CONTINUE;
}

/* 0346 */
eOpcodeResult WINAPI SET_TEXT_BACKGROUND_COLOUR(CScript *script)
{
	script->Collect(4);
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x18) = (BYTE)Params[0].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x19) = (BYTE)Params[1].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x1A) = (BYTE)Params[2].nVar;
	*(BYTE *)(*currentTextDraw * 0xF4 + textDraw + 0x1B) = (BYTE)Params[3].nVar;
	return OR_CONTINUE;
}

/* 0351 */
eOpcodeResult WINAPI IS_NASTY_GAME(CScript *script)
{
	script->UpdateCompareFlag(*nastyGame != 0);
	return OR_CONTINUE;
}

/* 0356 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_AREA(CScript *script)
{
	script->Collect(7);
	script->UpdateCompareFlag(TestForExplosionInArea(Params[0].nVar, Params[1].fVar, Params[4].fVar, Params[2].fVar, Params[5].fVar, Params[3].fVar, Params[6].fVar));
	return OR_CONTINUE;
}

/* 0357 */
eOpcodeResult WINAPI IS_EXPLOSION_IN_ZONE(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	script->ReadShortString(gxt);
	for (UINT_PTR address = infoZone; address < infoZone + *numInfoZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			if (TestForExplosionInArea(Params[0].nVar, *(FLOAT *)(address + 8), *(FLOAT *)(address + 0x14), *(FLOAT *)(address + 0xC), *(FLOAT *)(address + 0x18), *(FLOAT *)(address + 0x10), *(FLOAT *)(address + 0x1C))) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	for (UINT_PTR address = navigZone; address < navigZone + *numNavigZone * 0x38; address += 0x38) {
		if (!strncmp(gxt, (char *)address, 8)) {
			if (TestForExplosionInArea(Params[0].nVar, *(FLOAT *)(address + 8), *(FLOAT *)(address + 0x14), *(FLOAT *)(address + 0xC), *(FLOAT *)(address + 0x18), *(FLOAT *)(address + 0x10), *(FLOAT *)(address + 0x1C))) {
				script->UpdateCompareFlag(true);
				return OR_CONTINUE;
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 0367 */
eOpcodeResult WINAPI START_KILL_FRENZY_HEADSHOT(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	WCHAR *string = GetText(ctext, gxt);
	script->Collect(8);
	if (Params[7].nVar) {
		Params[7].nVar = 1;
	}
	StartFrenzy(Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, string, Params[4].nVar, Params[5].nVar, Params[6].nVar, Params[7].nVar, 1);
	return OR_CONTINUE;
}

/* 036E */
eOpcodeResult WINAPI PRINT_WITH_3_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(5);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, 0, 0, 0, numberedText);
	AddBigMessage(numberedText, Params[3].nVar, (WORD)Params[4].nVar - 1);
	return OR_CONTINUE;
}

/* 036F */
eOpcodeResult WINAPI PRINT_WITH_4_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(6);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, 0, 0, numberedText);
	AddBigMessage(numberedText, Params[4].nVar, (WORD)Params[5].nVar - 1);
	return OR_CONTINUE;
}

/* 0370 */
eOpcodeResult WINAPI PRINT_WITH_5_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(7);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, 0, numberedText);
	AddBigMessage(numberedText, Params[5].nVar, (WORD)Params[6].nVar - 1);
	return OR_CONTINUE;
}

/* 0371 */
eOpcodeResult WINAPI PRINT_WITH_6_NUMBERS_BIG(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(8);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, Params[1].nVar, Params[2].nVar, Params[3].nVar, Params[4].nVar, Params[5].nVar, numberedText);
	AddBigMessage(numberedText, Params[6].nVar, (WORD)Params[7].nVar - 1);
	return OR_CONTINUE;
}

/* 0378 */
eOpcodeResult WINAPI SET_2_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[2];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 0379 */
eOpcodeResult WINAPI SET_2_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[2];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], NULL, NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037A */
eOpcodeResult WINAPI SET_3_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[3];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037B */
eOpcodeResult WINAPI SET_3_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[3];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], NULL, NULL, NULL);
	return OR_CONTINUE;
}

/* 037C */
eOpcodeResult WINAPI SET_4_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[4];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], NULL, NULL);
	return OR_CONTINUE;
}

/* 037D */
eOpcodeResult WINAPI SET_4_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[4];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], NULL, NULL);
	return OR_CONTINUE;
}

/* 037F */
eOpcodeResult WINAPI GIVE_PLAYER_DETONATOR(CScript *script)
{
	DWORD player = FindPlayerPed();
	GiveWeapon(player, 0x22, 1, 1);
	int slot = *(DWORD *)(GetWeaponInfo(0x22) + 0x60);
	*(int *)(player + slot * 0x18 + 0x40C) = 0;
	return OR_CONTINUE;
}

/* 0386 */
eOpcodeResult WINAPI SET_5_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[5];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[4] = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], NULL);
	return OR_CONTINUE;
}

/* 0387 */
eOpcodeResult WINAPI SET_5_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[5];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[4] = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], NULL);
	return OR_CONTINUE;
}

/* 0388 */
eOpcodeResult WINAPI SET_6_REPEATED_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[6];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[4] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[5] = GetText(ctext, gxt);
	SetPhoneMessage_Repeatedly(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0389 */
eOpcodeResult WINAPI SET_6_PHONE_MESSAGES(CScript *script)
{
	script->Collect(1);
	char gxt[8];
	WCHAR *string[6];
	script->ReadShortString(gxt);
	string[0] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[1] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[2] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[3] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[4] = GetText(ctext, gxt);
	script->ReadShortString(gxt);
	string[5] = GetText(ctext, gxt);
	SetPhoneMessage_JustOnce(cphoneinfo, Params[0].nVar, string[0], string[1], string[2], string[3], string[4], string[5]);
	return OR_CONTINUE;
}

/* 0393 */
eOpcodeResult WINAPI SET_CHAR_ANIM_SPEED(CScript *script)
{
	script->Collect(2);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	int var = RpAnimBlendClumpGetFirstAssociation(*(UINT_PTR *)(ped + 0x4C));
	if (var != NULL) {
		*(FLOAT *)(var + 0x24) = Params[1].fVar;
	}
	return OR_CONTINUE;
}

/* 03A0 */
eOpcodeResult WINAPI IS_CRANE_LIFTING_CAR(CScript *script)
{
	script->Collect(3);
	for (UINT_PTR address = ccrane; address < ccrane + *numCranes * 0x7C; address += 0x7C) {
		if (sqrt(pow(Params[0].fVar - *(FLOAT *)(address + 0x4C), 2) + pow(Params[1].fVar - *(FLOAT *)(address + 0x50), 2)) < 100.0) {
			if (VehiclePoolGetStruct(*carPool, Params[2].nVar) == *(DWORD *)(address + 0x6C)) {
				if (*(BYTE *)(address + 0x75) == 2 || *(BYTE *)(address + 0x75) == 4) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 03A5 */
eOpcodeResult WINAPI CHANGE_GARAGE_TYPE_WITH_CAR_MODEL(CScript *script)
{
	script->Collect(3);
	ChangeGarageType((WORD)Params[0].nVar, (BYTE)Params[1].nVar, Params[2].nVar);
	return OR_CONTINUE;
}

/* 03C2 */
eOpcodeResult WINAPI IS_PHONE_DISPLAYING_MESSAGE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag((Params[0].nVar * 0x34 + cphoneinfo + 8) == *currentPhone);
	return OR_CONTINUE;
}

/* 03C6 */
eOpcodeResult WINAPI IS_COLLISION_IN_MEMORY(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(Params[0].nVar == *levelName);
	return OR_CONTINUE;
}

/* 03C9 */
eOpcodeResult WINAPI IS_CAR_VISIBLY_DAMAGED(CScript *script)
{
	script->Collect(1);
	BYTE flag = *(BYTE *)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + 0x1FB);
	flag >>= 1;
	flag &= 1;
	script->UpdateCompareFlag(flag == 1);
	return OR_CONTINUE;
}

/* 03DB */
eOpcodeResult WINAPI ADD_BLIP_FOR_PICKUP_OLD(CScript *script)
{
	script->Collect(3);
	int var = GetActualPickupIndex(Params[0].nVar) * 0x34 + pickupEntity;
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(3, ObjectPoolGetHandle(*objectPool, *(void **)var), Params[1].nVar, Params[2].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03DD */
eOpcodeResult WINAPI ADD_SPRITE_BLIP_FOR_PICKUP(CScript *script)
{
	script->Collect(2);
	int var = GetActualPickupIndex(Params[0].nVar) * 0x34 + pickupEntity;
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetEntityBlip(3, ObjectPoolGetHandle(*objectPool, *(void **)var), 6, 3);
	SetBlipSprite(Params[0].nVar, Params[1].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 03E9 */
eOpcodeResult WINAPI IS_CHAR_IN_CONTROL(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(IsPedInControl(PedPoolGetStruct(*pedPool, Params[0].nVar)) != 0);
	return OR_CONTINUE;
}

/* 03EC */
eOpcodeResult WINAPI HAS_MILITARY_CRANE_COLLECTED_ALL_CARS(CScript *script)
{
	script->UpdateCompareFlag((*militaryCraneCollected & 0x7F) == 0x7F);
	return OR_CONTINUE;
}

/* 0410 */
eOpcodeResult WINAPI SET_GANG_PED_MODEL_PREFERENCE(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x18 + gangPedModelOverride) = (BYTE)Params[1].nVar;
	return OR_CONTINUE;
}

/* 0413 */
eOpcodeResult WINAPI SET_GET_OUT_OF_JAIL_FREE(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x170 + (DWORD)playerPedPool + 0x145) = (BYTE)(Params[1].nVar ? 1 : 0);
	return OR_CONTINUE;
}

/* 041B */
eOpcodeResult WINAPI REGISTER_KILL_FRENZY_PASSED(CScript *script)
{
	AnotherKillFrenzyPassed();
	return OR_CONTINUE;
}

/* 041C */
eOpcodeResult WINAPI SET_CHAR_SAY(CScript *script)
{
	script->Collect(2);
	Say(PedPoolGetStruct(*pedPool, Params[0].nVar), Params[1].nVar);
	return OR_CONTINUE;
}

/* 041F */
eOpcodeResult WINAPI OVERRIDE_HOSPITAL_LEVEL(CScript *script)
{
	script->Collect(1);
	*overrideHospital = (BYTE)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0420 */
eOpcodeResult WINAPI OVERRIDE_POLICE_STATION_LEVEL(CScript *script)
{
	script->Collect(1);
	*overridePolice = (BYTE)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0421 */
eOpcodeResult WINAPI FORCE_RAIN(CScript *script)
{
	script->Collect(1);
	*forceRain = (BYTE)(Params[0].nVar ? 1 : 0);
	return OR_CONTINUE;
}

/* 0422 */
eOpcodeResult WINAPI DOES_GARAGE_CONTAIN_CAR(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag(IsEntityEntirelyInside3D(Params[0].nVar * 0xA8 + cgarage, VehiclePoolGetStruct(*carPool, Params[1].nVar), 0.0) != 0);
	return OR_CONTINUE;
}

/* 042A */
eOpcodeResult WINAPI IS_THREAT_FOR_PED_TYPE(CScript *script)
{
	script->Collect(2);
	script->UpdateCompareFlag(Params[1].nVar == (*(DWORD *)(*(DWORD *)(cpedtype + Params[0].nVar * 4) + 0x18) & Params[1].nVar));
	return OR_CONTINUE;
}

/* 0432 */
eOpcodeResult WINAPI GET_CHAR_IN_CAR_PASSENGER_SEAT(CScript *script)
{
	script->Collect(2);
	Params[0].nVar = PedPoolGetHandle(*pedPool, *(void **)(VehiclePoolGetStruct(*carPool, Params[0].nVar) + Params[1].nVar * 4 + 0x1AC));
	script->Store(1);
	return OR_CONTINUE;
}

/* 0444 */
eOpcodeResult WINAPI SET_SCRIPT_FIRE_AUDIO(CScript *script)
{
	script->Collect(2);
	*(BYTE *)(Params[0].nVar * 0x30 + cfire + 7) = (BYTE)(Params[1].nVar ? 1 : 0);
	return OR_CONTINUE;
}

/* 0447 */
eOpcodeResult WINAPI IS_PLAYER_LIFTING_A_PHONE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(*(DWORD *)(playerPedPool[0x2E * Params[0].nVar] + 0x244) == 0x13);
	return OR_CONTINUE;
}

/* 047D */
eOpcodeResult WINAPI GET_NUMBER_OF_SEATS_IN_MODEL(CScript *script)
{
	script->Collect(1);
	Params[0].nVar = GetVehicleTypeId(Params[0].nVar) + 1;
	script->Store(1);
	return OR_CONTINUE;
}

/* 04A7 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_BOAT(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			UINT_PTR car = *(UINT_PTR *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x80000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04A9 */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_HELI(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			UINT_PTR car = *(UINT_PTR *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x20000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04AB */
eOpcodeResult WINAPI IS_CHAR_IN_ANY_PLANE(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			UINT_PTR car = *(UINT_PTR *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x40000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04C8 */
eOpcodeResult WINAPI IS_CHAR_IN_FLYING_VEHICLE(CScript *script)
{
	script->Collect(1);
	UINT_PTR ped = PedPoolGetStruct(*pedPool, Params[0].nVar);
	if (ped != NULL) {
		if (*(BYTE *)(ped + 0x3AC)) {
			UINT_PTR car = *(UINT_PTR *)(ped + 0x3A8);
			if (car != NULL) {
				DWORD handling = *(DWORD *)(car + 0x120);
				DWORD flags = *(DWORD *)(handling + 0xCC);
				flags &= 0xF0000;
				if (flags == 0x20000 || flags == 0x40000) {
					script->UpdateCompareFlag(true);
					return OR_CONTINUE;
				}
			}
		}
	}
	script->UpdateCompareFlag(false);
	return OR_CONTINUE;
}

/* 04CC */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD(CScript *script)
{
	script->Collect(5);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetShortRangeBlip(4, Params[0].fVar, Params[1].fVar, Params[2].fVar, Params[3].nVar, Params[4].nVar);
	script->Store(1);
	return OR_CONTINUE;
}

/* 04CD */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_COORD(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetShortRangeBlip(4, Params[0].fVar, Params[1].fVar, Params[2].fVar, 5, 3);
	ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = *maxWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	SetHelpMessage(numberedText, 0, 0);
	return OR_CONTINUE;
}

/* 0513 */
eOpcodeResult WINAPI PRINT_HELP_FOREVER_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	SetHelpMessage(numberedText, 0, 1);
	return OR_CONTINUE;
}

/* 052D */
eOpcodeResult WINAPI GET_PLAYER_DRUNKENNESS(CScript *script)
{
	script->Collect(1);
	DWORD player = playerPedPool[0x2E * Params[0].nVar];
	Params[0].nVar = (DWORD)*(BYTE *)(player + 0x638);
	script->Store(1);
	return OR_CONTINUE;
}

/* 0530 */
eOpcodeResult WINAPI ADD_LOAN_SHARK_VISITS(CScript *script)
{
	script->Collect(1);
	*loanSharkVisits += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0532 */
eOpcodeResult WINAPI ADD_MOVIE_STUNTS(CScript *script)
{
	script->Collect(1);
	*movieStunts += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0535 */
eOpcodeResult WINAPI ADD_GARBAGE_PICKUPS(CScript *script)
{
	script->Collect(1);
	*garbagePickups += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0537 */
eOpcodeResult WINAPI SET_TOP_SHOOTING_RANGE_SCORE(CScript *script)
{
	script->Collect(1);
	if ((FLOAT)Params[0].nVar > *topShootingRangeScore) {
		*topShootingRangeScore = (FLOAT)Params[0].nVar;
	}
	return OR_CONTINUE;
}

/* 0538 */
eOpcodeResult WINAPI ADD_SHOOTING_RANGE_RANK(CScript *script)
{
	script->Collect(1);
	*shootingRangeRank += (FLOAT)Params[0].nVar;
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	UINT_PTR source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	UINT_PTR target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x3AC)) {
		UINT_PTR car = *(UINT_PTR *)(source + 0x3A8);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

/* 056F */
eOpcodeResult WINAPI ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT(CScript *script)
{
	script->Collect(3);
	if (Params[2].fVar <= -100.0) {
		Params[2].fVar = FindGroundZForCoord(Params[0].fVar, Params[1].fVar);
	}
	GetActualBlipArrayIndex(script->CollectNextWithoutIncreasingPC(script->m_dwIp));
	Params[0].nVar = SetShortRangeBlip(5, Params[0].fVar, Params[1].fVar, Params[2].fVar, 2, 3);
	ChangeBlipScale(Params[0].nVar, 3);
	script->Store(1);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(0x00A2, IS_CHAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x00AC, IS_CAR_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x00C5, RETURN_TRUE);
		Opcodes::RegisterOpcode(0x00C6, RETURN_FALSE);
		Opcodes::RegisterOpcode(0x00E2, GET_PAD_STATE);
		Opcodes::RegisterOpcode(0x0113, ADD_AMMO_TO_PLAYER);
		Opcodes::RegisterOpcode(0x0116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x0130, HAS_PLAYER_BEEN_ARRESTED);
		Opcodes::RegisterOpcode(0x0135, CHANGE_CAR_LOCK);
		Opcodes::RegisterOpcode(0x0136, SHAKE_CAM_WITH_POINT);
		Opcodes::RegisterOpcode(0x013B, IS_CAR_DEAD_IN_AREA_2D);
		Opcodes::RegisterOpcode(0x013C, IS_CAR_DEAD_IN_AREA_3D);
		Opcodes::RegisterOpcode(0x0155, SET_CAR_DENSITY);
		Opcodes::RegisterOpcode(0x0156, SET_PED_DENSITY);
		Opcodes::RegisterOpcode(0x015E, IS_CAR_IN_AIR);
		Opcodes::RegisterOpcode(0x0163, ADD_BLIP_FOR_OBJECT_OLD);
		Opcodes::RegisterOpcode(0x0178, IS_PLAYER_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x0179, IS_CHAR_TOUCHING_OBJECT);
		Opcodes::RegisterOpcode(0x017B, SET_CHAR_AMMO);
		Opcodes::RegisterOpcode(0x01C6, DONT_REMOVE_CAR);
		Opcodes::RegisterOpcode(0x0218, PRINT_WITH_NUMBER_BIG_Q);
		Opcodes::RegisterOpcode(0x021D, SET_FREE_BOMBS);
		Opcodes::RegisterOpcode(0x021F, SET_ALL_TAXI_LIGHTS);
		Opcodes::RegisterOpcode(0x0220, IS_CAR_ARMED_WITH_ANY_BOMB);
		Opcodes::RegisterOpcode(0x0228, IS_CAR_ARMED_WITH_BOMB);
		Opcodes::RegisterOpcode(0x023A, IS_PLAYER_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x023B, IS_CHAR_TOUCHING_OBJECT_ON_FOOT);
		Opcodes::RegisterOpcode(0x0242, ARM_CAR_WITH_BOMB);
		Opcodes::RegisterOpcode(0x024B, SET_REPEATED_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024C, SET_PHONE_MESSAGE);
		Opcodes::RegisterOpcode(0x024D, HAS_PHONE_DISPLAYED_MESSAGE);
		Opcodes::RegisterOpcode(0x0255, RESTART_CRITICAL_MISSION);
		Opcodes::RegisterOpcode(0x0295, IS_TAXI);
		Opcodes::RegisterOpcode(0x0299, ACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x029C, IS_BOAT);
		Opcodes::RegisterOpcode(0x02A0, IS_CHAR_STOPPED);
		Opcodes::RegisterOpcode(0x02A4, ADD_SPRITE_BLIP_FOR_CAR);
		Opcodes::RegisterOpcode(0x02A5, ADD_SPRITE_BLIP_FOR_CHAR);
		Opcodes::RegisterOpcode(0x02A6, ADD_SPRITE_BLIP_FOR_OBJECT);
		Opcodes::RegisterOpcode(0x02B9, DEACTIVATE_GARAGE);
		Opcodes::RegisterOpcode(0x02BC, SET_SWAT_REQUIRED);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x02D6, IS_CHAR_SHOOTING_IN_AREA);
		Opcodes::RegisterOpcode(0x02EE, IS_PROJECTILE_IN_AREA);
		Opcodes::RegisterOpcode(0x02EF, DESTROY_PROJECTILES_IN_AREA);
		Opcodes::RegisterOpcode(0x02F0, DROP_MINE);
		Opcodes::RegisterOpcode(0x02F1, DROP_NAUTICAL_MINE);
		Opcodes::RegisterOpcode(0x02FC, PRINT_WITH_2_NUMBERS);
		Opcodes::RegisterOpcode(0x0300, PRINT_WITH_3_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0303, PRINT_WITH_4_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0305, PRINT_WITH_5_NUMBERS);
		Opcodes::RegisterOpcode(0x0306, PRINT_WITH_5_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x0309, PRINT_WITH_6_NUMBERS_NOW);
		Opcodes::RegisterOpcode(0x031B, IS_FIRST_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x031C, IS_SECOND_CAR_COLOUR);
		Opcodes::RegisterOpcode(0x032D, SET_CAR_BLOCK_CAR);
		Opcodes::RegisterOpcode(0x0346, SET_TEXT_BACKGROUND_COLOUR);
		Opcodes::RegisterOpcode(0x0351, IS_NASTY_GAME);
		Opcodes::RegisterOpcode(0x0356, IS_EXPLOSION_IN_AREA);
		Opcodes::RegisterOpcode(0x0357, IS_EXPLOSION_IN_ZONE);
		Opcodes::RegisterOpcode(0x0367, START_KILL_FRENZY_HEADSHOT);
		Opcodes::RegisterOpcode(0x036E, PRINT_WITH_3_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x036F, PRINT_WITH_4_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0370, PRINT_WITH_5_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0371, PRINT_WITH_6_NUMBERS_BIG);
		Opcodes::RegisterOpcode(0x0378, SET_2_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0379, SET_2_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037A, SET_3_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037B, SET_3_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037C, SET_4_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037D, SET_4_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x037F, GIVE_PLAYER_DETONATOR);
		Opcodes::RegisterOpcode(0x0386, SET_5_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0387, SET_5_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0388, SET_6_REPEATED_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0389, SET_6_PHONE_MESSAGES);
		Opcodes::RegisterOpcode(0x0393, SET_CHAR_ANIM_SPEED);
		Opcodes::RegisterOpcode(0x03A0, IS_CRANE_LIFTING_CAR);
		Opcodes::RegisterOpcode(0x03A5, CHANGE_GARAGE_TYPE_WITH_CAR_MODEL);
		Opcodes::RegisterOpcode(0x03C2, IS_PHONE_DISPLAYING_MESSAGE);
		Opcodes::RegisterOpcode(0x03C6, IS_COLLISION_IN_MEMORY);
		Opcodes::RegisterOpcode(0x03C9, IS_CAR_VISIBLY_DAMAGED);
		Opcodes::RegisterOpcode(0x03DB, ADD_BLIP_FOR_PICKUP_OLD);
		Opcodes::RegisterOpcode(0x03DD, ADD_SPRITE_BLIP_FOR_PICKUP);
		Opcodes::RegisterOpcode(0x03E9, IS_CHAR_IN_CONTROL);
		Opcodes::RegisterOpcode(0x03EC, HAS_MILITARY_CRANE_COLLECTED_ALL_CARS);
		Opcodes::RegisterOpcode(0x0410, SET_GANG_PED_MODEL_PREFERENCE);
		Opcodes::RegisterOpcode(0x0413, SET_GET_OUT_OF_JAIL_FREE);
		Opcodes::RegisterOpcode(0x041B, REGISTER_KILL_FRENZY_PASSED);
		Opcodes::RegisterOpcode(0x041C, SET_CHAR_SAY);
		Opcodes::RegisterOpcode(0x041F, OVERRIDE_HOSPITAL_LEVEL);
		Opcodes::RegisterOpcode(0x0420, OVERRIDE_POLICE_STATION_LEVEL);
		Opcodes::RegisterOpcode(0x0421, FORCE_RAIN);
		Opcodes::RegisterOpcode(0x0422, DOES_GARAGE_CONTAIN_CAR);
		Opcodes::RegisterOpcode(0x042A, IS_THREAT_FOR_PED_TYPE);
		Opcodes::RegisterOpcode(0x0432, GET_CHAR_IN_CAR_PASSENGER_SEAT);
		Opcodes::RegisterOpcode(0x0444, SET_SCRIPT_FIRE_AUDIO);
		Opcodes::RegisterOpcode(0x0447, IS_PLAYER_LIFTING_A_PHONE);
		Opcodes::RegisterOpcode(0x047D, GET_NUMBER_OF_SEATS_IN_MODEL);
		Opcodes::RegisterOpcode(0x04A7, IS_CHAR_IN_ANY_BOAT);
		Opcodes::RegisterOpcode(0x04A9, IS_CHAR_IN_ANY_HELI);
		Opcodes::RegisterOpcode(0x04AB, IS_CHAR_IN_ANY_PLANE);
		Opcodes::RegisterOpcode(0x04C8, IS_CHAR_IN_FLYING_VEHICLE);
		Opcodes::RegisterOpcode(0x04CC, ADD_SHORT_RANGE_BLIP_FOR_COORD_OLD);
		Opcodes::RegisterOpcode(0x04CD, ADD_SHORT_RANGE_BLIP_FOR_COORD);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x0511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x0513, PRINT_HELP_FOREVER_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x052D, GET_PLAYER_DRUNKENNESS);
		Opcodes::RegisterOpcode(0x0530, ADD_LOAN_SHARK_VISITS);
		Opcodes::RegisterOpcode(0x0532, ADD_MOVIE_STUNTS);
		Opcodes::RegisterOpcode(0x0535, ADD_GARBAGE_PICKUPS);
		Opcodes::RegisterOpcode(0x0537, SET_TOP_SHOOTING_RANGE_SCORE);
		Opcodes::RegisterOpcode(0x0538, ADD_SHOOTING_RANGE_RANK);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x056F, ADD_SHORT_RANGE_BLIP_FOR_CONTACT_POINT);
	}
	return TRUE;
}

#else if _III
#pragma comment (lib, "..\\..\\..\\III.CLEO.lib")
#include "..\\..\\..\\III.CLEO.h"

tScriptVar *Params;

uintptr_t(__thiscall *VehiclePoolGetStruct)(void *, INT); // cpool_cvehicle_cautomobile::getat
uintptr_t(__thiscall *PedPoolGetStruct)(void *, INT); // cpool_cped_cplayerped::getat
BYTE(__thiscall *GetHasCollidedWith)(DWORD, uintptr_t); // cphysical::gethascollidedwith
DWORD(__cdecl *FindPlayerPed)(void); // findplayerped
void(__cdecl *SetHelpMessage)(wchar_t *, CHAR, INT); // chud::sethelpmessage
void(__cdecl *InsertNumberInString)(wchar_t *, INT, INT, INT, INT, INT, INT, wchar_t *); // cmessages::insertnumberinstring
wchar_t *(__thiscall *GetText)(uintptr_t, char *); // ctext::get
DWORD *maxWantedLevel = NULL;
wchar_t *numberedText = NULL;
void **pedPool = NULL;
uintptr_t *playerPedPool = NULL;
uintptr_t *playerPedState = NULL;
uintptr_t ctext = NULL;
void **carPool = NULL;

GtaGame::GtaGame()
{
	version = VUNKOWN;
	switch ((*(unsigned int *)0x61C11C)) { // get version signature
	case 0x598B80:
		version = V1_0;
		VehiclePoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x43EAF0;
		PedPoolGetStruct = (uintptr_t(__thiscall *)(void *, INT))0x43EB30;
		GetHasCollidedWith = (BYTE(__thiscall *)(DWORD, uintptr_t))0x497240;
		FindPlayerPed = (DWORD(__cdecl *)(void))0x4A1150;
		SetHelpMessage = (void(__cdecl *)(wchar_t *, CHAR, INT))0x5051E0;
		InsertNumberInString = (void(__cdecl *)(wchar_t *, INT, INT, INT, INT, INT, INT, wchar_t *))0x52A1A0;
		GetText = (wchar_t *(__thiscall *)(uintptr_t, char *))0x52C5A0;
		maxWantedLevel = (DWORD *)0x5F7714;
		numberedText = (wchar_t *)0x74B018;
		pedPool = (void **)0x8F2C60;
		playerPedPool = (uintptr_t *)0x9412F0;
		playerPedState = (uintptr_t *)0x9413C8;
		ctext = 0x941520;
		carPool = (void **)0x9430DC;
	case 0x598E40:
		version = V1_1;
	case 0x646E6957:
		version = VSTEAM;
	}
}

/* 0116 */
eOpcodeResult WINAPI IS_PLAYER_STILL_ALIVE(CScript *script)
{
	script->Collect(1);
	script->UpdateCompareFlag(playerPedState[0x4F * Params[0].nVar] != 1);
	return OR_CONTINUE;
}

/* 02BD */
eOpcodeResult WINAPI SET_FBI_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x53C);
	BYTE fbi = *(BYTE *)(police + 0x16);
	fbi &= 0xF7;
	if (Params[0].nVar) {
		fbi |= 8;
	}
	*(BYTE *)(police + 0x16) = fbi;
	return OR_CONTINUE;
}

/* 02BE */
eOpcodeResult WINAPI SET_ARMY_REQUIRED(CScript *script)
{
	script->Collect(1);
	DWORD player = FindPlayerPed();
	DWORD police = *(DWORD *)(player + 0x53C);
	BYTE army = *(BYTE *)(police + 0x16);
	army &= 0xEF;
	if (Params[0].nVar) {
		army |= 16;
	}
	*(BYTE *)(police + 0x16) = army;
	return OR_CONTINUE;
}

/* 050F */
eOpcodeResult WINAPI GET_MAX_WANTED_LEVEL(CScript *script)
{
	Params[0].nVar = *maxWantedLevel;
	script->Store(1);
	return OR_CONTINUE;
}

/* 0511 */
eOpcodeResult WINAPI PRINT_HELP_WITH_NUMBER(CScript *script)
{
	char gxt[8];
	script->ReadShortString(gxt);
	script->Collect(1);
	InsertNumberInString(GetText(ctext, gxt), Params[0].nVar, 0, 0, 0, 0, 0, numberedText);
	SetHelpMessage(numberedText, 0, 0);
	return OR_CONTINUE;
}

/* 0546 */
eOpcodeResult WINAPI IS_PLAYER_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	DWORD source = playerPedPool[0x4F * Params[0].nVar];
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x314)) {
		uintptr_t car = *(uintptr_t *)(source + 0x310);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

/* 0547 */
eOpcodeResult WINAPI IS_CHAR_TOUCHING_VEHICLE(CScript *script)
{
	script->Collect(2);
	uintptr_t source = PedPoolGetStruct(*pedPool, Params[0].nVar);
	uintptr_t target = VehiclePoolGetStruct(*carPool, Params[1].nVar);
	if (*(BYTE *)(source + 0x314)) {
		uintptr_t car = *(uintptr_t *)(source + 0x310);
		if (car != NULL) {
			source = car;
		}
	}
	script->UpdateCompareFlag(GetHasCollidedWith(source, target) != 0);
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, TEXT("An incorrect version of CLEO was loaded."), TEXT("Opcodes.cleo"), MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(0x0116, IS_PLAYER_STILL_ALIVE);
		Opcodes::RegisterOpcode(0x02BD, SET_FBI_REQUIRED);
		Opcodes::RegisterOpcode(0x02BE, SET_ARMY_REQUIRED);
		Opcodes::RegisterOpcode(0x050F, GET_MAX_WANTED_LEVEL);
		Opcodes::RegisterOpcode(0x0511, PRINT_HELP_WITH_NUMBER);
		Opcodes::RegisterOpcode(0x0546, IS_PLAYER_TOUCHING_VEHICLE);
		Opcodes::RegisterOpcode(0x0547, IS_CHAR_TOUCHING_VEHICLE);
	}
	return TRUE;
}

#endif
