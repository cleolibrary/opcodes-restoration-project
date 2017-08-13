#pragma once

//########################################################################
//# CEntity
//########################################################################

class CEntity // public CPlaceable
{
public:
	unsigned long vtbl; // 0x000
	unsigned char space[0x060];
};

static_assert(sizeof(CEntity) == 0x064, "Size of CEntity is not 0x064 bytes.");

//########################################################################
//# CPhysical
//########################################################################

class CPhysical : public CEntity
{
public:
	// 0x064
	unsigned char space[0x0C4];
	// 0x128

	bool GetHasCollidedWith(CEntity *);
};

static_assert(sizeof(CPhysical) == 0x128, "Size of CPhysical is not 0x128 bytes.");

//########################################################################
//# CPed
//########################################################################

class CPed : public CPhysical
{
public:
	// 0x128
	unsigned char  space1[0x1E8];
	class CVehicle *vehicle;       // 0x310
	unsigned char  isInAnyVehicle; // 0x314
	unsigned char  space2[0x227];
	class CWanted  *wanted;        // 0x53C
	unsigned char  space3[0x0B0];
	// 0x5F0

	void ClearWeapons(void);
};

static_assert(sizeof(CPed) == 0x5F0, "Size of CPed is not 0x5F0 bytes.");

//########################################################################
//# CPlayerPed
//########################################################################

class CPlayerPed : public CPed
{
public:
};

//########################################################################
//# CVehicle
//########################################################################

class CVehicle : public CPhysical
{
public:
};

//########################################################################
//# CAutomobile
//########################################################################

class CAutomobile : public CVehicle
{
public:
};

//########################################################################
//# CHud
//########################################################################

class CHud
{
public:
	static void SetHelpMessage(wchar_t *, bool);
};

//########################################################################
//# CMessages
//########################################################################

struct BriefMessage
{
	wchar_t *message;
	unsigned short flag;
	unsigned char pad[2];
	unsigned int duration;
	unsigned int timeMessageAdded;
	int tokenNumber1;
	int tokenNumber2;
	int tokenNumber3;
	int tokenNumber4;
	int tokenNumber5;
	int tokenNumber6;
	wchar_t *tokenString;
};

static_assert(sizeof(BriefMessage) == 0x2C, "Size of BriefMessage is not 0x2C bytes.");

class CMessages
{
public:
	static void AddToPreviousBriefArray(wchar_t *, int, int, int, int, int, int, wchar_t *);
	static void InsertNumberInString(wchar_t *, int, int, int, int, int, int, wchar_t *);
	static void AddMessageSoonWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int);

	static BriefMessage *BriefMessages;
};

//########################################################################
//# CPlayerInfo
//########################################################################

class CPlayerInfo
{
public:
	CPlayerPed    *playerEntity;    // 0x000
	unsigned char space1[0x0D4];
	unsigned char deathArrestState; // 0x0D8
	unsigned char space2[0x063];
};

static_assert(sizeof(CPlayerInfo) == 0x13C, "Size of CPlayerInfo is not 0x13C bytes.");

//########################################################################
//# CPool
//########################################################################

template <class T1, class T2 = T1>
class CPool
{
public:
	T1 *GetAt(int);
};

//########################################################################
//# CPools
//########################################################################

class CPools
{
public:
	static CPool<CPed, CPlayerPed> *&ms_pPedPool;
	static CPool<CVehicle, CAutomobile> *&ms_pVehiclePool;
};

//########################################################################
//# CText
//########################################################################

class CText
{
public:
	wchar_t *Get(const char *);
};

//########################################################################
//# CTimer
//########################################################################

class CTimer
{
public:
	static unsigned int &m_snTimeInMilliseconds;
};

//########################################################################
//# CWanted
//########################################################################

class CWanted
{
public:
	unsigned char space1[0x016];
	unsigned char activity; // 0x016

	static unsigned int &MaximumWantedLevel;
};

//########################################################################
//# CWorld
//########################################################################

class CWorld
{
public:
	static unsigned char &PlayerInFocus;
	static CPlayerInfo *Players;
};
