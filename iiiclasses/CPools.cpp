#include "iiiclasses.h"

CPool<CPed, CPlayerPed> *&CPools::ms_pPedPool = *reinterpret_cast<CPool<CPed, CPlayerPed> **>(0x008F2C60);
CPool<CVehicle, CAutomobile> *&CPools::ms_pVehiclePool = *reinterpret_cast<CPool<CVehicle, CAutomobile> **>(0x009430DC);

static unsigned long g_CVehicle_CAutomobile_GetAt = 0x0043EAF0;
static unsigned long g_CPed_CPlayerPed_GetAt = 0x0043EB30;

__declspec(naked) CVehicle *CPool<CVehicle, CAutomobile>::GetAt(int)
{
	__asm jmp g_CVehicle_CAutomobile_GetAt;
}

__declspec(naked) CPed *CPool<CPed, CPlayerPed>::GetAt(int)
{
	__asm jmp g_CPed_CPlayerPed_GetAt;
}
