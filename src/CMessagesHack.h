#pragma once

#if _VC
#include "vcclasses.h"
#elif _III
#include "iiiclasses.h"
#endif

class CMessagesHack : public CMessages
{
public:
	static void AddMessageSoon(wchar_t *, unsigned int, unsigned short);
	static void AddMessageSoonWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int);
	static void AddMessageWithString(wchar_t *, unsigned int, unsigned short, wchar_t *);
	static void AddMessageSoonWithString(wchar_t *, unsigned int, unsigned short, wchar_t *);
};
