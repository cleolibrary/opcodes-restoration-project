#pragma once

#include "vcclasses.h"

class CMessagesHack : public CMessages
{
public:
	static void AddMessageSoon(wchar_t *, unsigned int, unsigned short);
	static void AddMessageSoonWithNumber(wchar_t *, unsigned int, unsigned short, int, int, int, int, int, int);
	static void AddMessageWithString(wchar_t *, unsigned int, unsigned short, wchar_t *);
	static void AddMessageSoonWithString(wchar_t *, unsigned int, unsigned short, wchar_t *);
};
