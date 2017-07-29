#include "CPhoneInfoHack.h"

void CPhoneInfoHack::SetPhoneMessage_Repeatedly(int index, wchar_t *message1, wchar_t *message2, wchar_t *message3, wchar_t *message4, wchar_t *message5, wchar_t *message6)
{
	this->phones[index].message1 = message1;
	this->phones[index].message2 = message2;
	this->phones[index].message3 = message3;
	this->phones[index].message4 = message4;
	this->phones[index].message5 = message5;
	this->phones[index].message6 = message6;
	this->phones[index].state = 5;
}
