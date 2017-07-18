#include "CPhoneInfoHack.h"
#include "vcclasses.h"

void CPhoneInfoHack::SetPhoneMessage_Repeatedly(int index, wchar_t *string1, wchar_t *string2, wchar_t *string3, wchar_t *string4, wchar_t *string5, wchar_t *string6)
{
	this->phones[index].message1 = string1;
	this->phones[index].message2 = string2;
	this->phones[index].message3 = string3;
	this->phones[index].message4 = string4;
	this->phones[index].message5 = string5;
	this->phones[index].message6 = string6;
	this->phones[index].state = 5;
}
