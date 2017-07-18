#pragma once

#include "vcclasses.h"

class CPhoneInfoHack : public CPhoneInfo
{
public:
	void SetPhoneMessage_Repeatedly(int index, wchar_t *string1, wchar_t *string2, wchar_t *string3, wchar_t *string4, wchar_t *string5, wchar_t *string6);
};
