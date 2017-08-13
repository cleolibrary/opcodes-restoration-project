#include "iiiclasses.h"

BriefMessage *CMessages::BriefMessages = reinterpret_cast<BriefMessage *>(0x008786E0);

static unsigned long g_AddToPreviousBriefArray = 0x0052A040;
static unsigned long g_InsertNumberInString = 0x0052A1A0;
static unsigned long g_AddMessageSoonWithNumber = 0x0052AAC0;

__declspec(naked) void CMessages::AddToPreviousBriefArray(wchar_t *, int, int, int, int, int, int, wchar_t *)
{
	__asm jmp g_AddToPreviousBriefArray;
}

__declspec(naked) void CMessages::InsertNumberInString(wchar_t *, int, int, int, int, int, int, wchar_t *)
{
	__asm jmp g_InsertNumberInString;
}

__declspec(naked) void CMessages::AddMessageSoonWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int)
{
	__asm jmp g_AddMessageSoonWithNumber;
}
