#include "CMessagesHack.h"

void CMessagesHack::AddMessageSoon(wchar_t *message, unsigned int duration, unsigned short flag)
{
	if (CMessages::BriefMessages[0].message) {
		int i;
		for (i = 7; i > 1; i--) {
			CMessages::BriefMessages[i].message = CMessages::BriefMessages[i - 1].message;
			CMessages::BriefMessages[i].flag = CMessages::BriefMessages[i - 1].flag;
			CMessages::BriefMessages[i].duration = CMessages::BriefMessages[i - 1].duration;
			CMessages::BriefMessages[i].timeMessageAdded = CMessages::BriefMessages[i - 1].timeMessageAdded;
			CMessages::BriefMessages[i].tokenNumber1 = CMessages::BriefMessages[i - 1].tokenNumber1;
			CMessages::BriefMessages[i].tokenNumber2 = CMessages::BriefMessages[i - 1].tokenNumber2;
			CMessages::BriefMessages[i].tokenNumber3 = CMessages::BriefMessages[i - 1].tokenNumber3;
			CMessages::BriefMessages[i].tokenNumber4 = CMessages::BriefMessages[i - 1].tokenNumber4;
			CMessages::BriefMessages[i].tokenNumber5 = CMessages::BriefMessages[i - 1].tokenNumber5;
			CMessages::BriefMessages[i].tokenNumber6 = CMessages::BriefMessages[i - 1].tokenNumber6;
			CMessages::BriefMessages[i].tokenString = CMessages::BriefMessages[i - 1].tokenString;
		}
		CMessages::BriefMessages[i].message = message;
		CMessages::BriefMessages[i].flag = flag;
		CMessages::BriefMessages[i].duration = duration;
		CMessages::BriefMessages[i].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[i].tokenNumber1 = -1;
		CMessages::BriefMessages[i].tokenNumber2 = -1;
		CMessages::BriefMessages[i].tokenNumber3 = -1;
		CMessages::BriefMessages[i].tokenNumber4 = -1;
		CMessages::BriefMessages[i].tokenNumber5 = -1;
		CMessages::BriefMessages[i].tokenNumber6 = -1;
		CMessages::BriefMessages[i].tokenString = 0;
	} else {
		CMessages::BriefMessages[0].message = message;
		CMessages::BriefMessages[0].flag = flag;
		CMessages::BriefMessages[0].duration = duration;
		CMessages::BriefMessages[0].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[0].tokenNumber1 = -1;
		CMessages::BriefMessages[0].tokenNumber2 = -1;
		CMessages::BriefMessages[0].tokenNumber3 = -1;
		CMessages::BriefMessages[0].tokenNumber4 = -1;
		CMessages::BriefMessages[0].tokenNumber5 = -1;
		CMessages::BriefMessages[0].tokenNumber6 = -1;
		CMessages::BriefMessages[0].tokenString = 0;
		CMessages::AddToPreviousBriefArray(
			CMessages::BriefMessages[0].message,
			CMessages::BriefMessages[0].tokenNumber1,
			CMessages::BriefMessages[0].tokenNumber2,
			CMessages::BriefMessages[0].tokenNumber3,
			CMessages::BriefMessages[0].tokenNumber4,
			CMessages::BriefMessages[0].tokenNumber5,
			CMessages::BriefMessages[0].tokenNumber6,
			CMessages::BriefMessages[0].tokenString);
	}
}

void CMessagesHack::AddMessageSoonWithNumber(wchar_t *message, unsigned int duration, unsigned short flag, int number1, int number2, int number3, int number4, int number5, int number6)
{
	if (CMessages::BriefMessages[0].message) {
		int i;
		for (i = 7; i > 1; i--) {
			CMessages::BriefMessages[i].message = CMessages::BriefMessages[i - 1].message;
			CMessages::BriefMessages[i].flag = CMessages::BriefMessages[i - 1].flag;
			CMessages::BriefMessages[i].duration = CMessages::BriefMessages[i - 1].duration;
			CMessages::BriefMessages[i].timeMessageAdded = CMessages::BriefMessages[i - 1].timeMessageAdded;
			CMessages::BriefMessages[i].tokenNumber1 = CMessages::BriefMessages[i - 1].tokenNumber1;
			CMessages::BriefMessages[i].tokenNumber2 = CMessages::BriefMessages[i - 1].tokenNumber2;
			CMessages::BriefMessages[i].tokenNumber3 = CMessages::BriefMessages[i - 1].tokenNumber3;
			CMessages::BriefMessages[i].tokenNumber4 = CMessages::BriefMessages[i - 1].tokenNumber4;
			CMessages::BriefMessages[i].tokenNumber5 = CMessages::BriefMessages[i - 1].tokenNumber5;
			CMessages::BriefMessages[i].tokenNumber6 = CMessages::BriefMessages[i - 1].tokenNumber6;
			CMessages::BriefMessages[i].tokenString = CMessages::BriefMessages[i - 1].tokenString;
		}
		CMessages::BriefMessages[i].message = message;
		CMessages::BriefMessages[i].flag = flag;
		CMessages::BriefMessages[i].duration = duration;
		CMessages::BriefMessages[i].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[i].tokenNumber1 = number1;
		CMessages::BriefMessages[i].tokenNumber2 = number2;
		CMessages::BriefMessages[i].tokenNumber3 = number3;
		CMessages::BriefMessages[i].tokenNumber4 = number4;
		CMessages::BriefMessages[i].tokenNumber5 = number5;
		CMessages::BriefMessages[i].tokenNumber6 = number6;
		CMessages::BriefMessages[i].tokenString = 0;
	} else {
		CMessages::BriefMessages[0].message = message;
		CMessages::BriefMessages[0].flag = flag;
		CMessages::BriefMessages[0].duration = duration;
		CMessages::BriefMessages[0].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[0].tokenNumber1 = number1;
		CMessages::BriefMessages[0].tokenNumber2 = number2;
		CMessages::BriefMessages[0].tokenNumber3 = number3;
		CMessages::BriefMessages[0].tokenNumber4 = number4;
		CMessages::BriefMessages[0].tokenNumber5 = number5;
		CMessages::BriefMessages[0].tokenNumber6 = number6;
		CMessages::BriefMessages[0].tokenString = 0;
		CMessages::AddToPreviousBriefArray(
			CMessages::BriefMessages[0].message,
			CMessages::BriefMessages[0].tokenNumber1,
			CMessages::BriefMessages[0].tokenNumber2,
			CMessages::BriefMessages[0].tokenNumber3,
			CMessages::BriefMessages[0].tokenNumber4,
			CMessages::BriefMessages[0].tokenNumber5,
			CMessages::BriefMessages[0].tokenNumber6,
			CMessages::BriefMessages[0].tokenString);
	}
}

void CMessagesHack::AddMessageWithString(wchar_t *message, unsigned int duration, unsigned short flag, wchar_t *tokenString)
{
	for (int i = 0; i < 8; i++) {
		if (!CMessages::BriefMessages[i].message) {
			CMessages::BriefMessages[i].message = message;
			CMessages::BriefMessages[i].flag = flag;
			CMessages::BriefMessages[i].duration = duration;
			CMessages::BriefMessages[i].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
			CMessages::BriefMessages[i].tokenNumber1 = -1;
			CMessages::BriefMessages[i].tokenNumber2 = -1;
			CMessages::BriefMessages[i].tokenNumber3 = -1;
			CMessages::BriefMessages[i].tokenNumber4 = -1;
			CMessages::BriefMessages[i].tokenNumber5 = -1;
			CMessages::BriefMessages[i].tokenNumber6 = -1;
			CMessages::BriefMessages[i].tokenString = tokenString;
			if (i == 0) {
				CMessages::AddToPreviousBriefArray(
					CMessages::BriefMessages[0].message,
					CMessages::BriefMessages[0].tokenNumber1,
					CMessages::BriefMessages[0].tokenNumber2,
					CMessages::BriefMessages[0].tokenNumber3,
					CMessages::BriefMessages[0].tokenNumber4,
					CMessages::BriefMessages[0].tokenNumber5,
					CMessages::BriefMessages[0].tokenNumber6,
					CMessages::BriefMessages[0].tokenString);
			}
			break;
		}
	}
}

void CMessagesHack::AddMessageSoonWithString(wchar_t *message, unsigned int duration, unsigned short flag, wchar_t *tokenString)
{
	if (CMessages::BriefMessages[0].message) {
		int i;
		for (i = 7; i > 1; i--) {
			CMessages::BriefMessages[i].message = CMessages::BriefMessages[i - 1].message;
			CMessages::BriefMessages[i].flag = CMessages::BriefMessages[i - 1].flag;
			CMessages::BriefMessages[i].duration = CMessages::BriefMessages[i - 1].duration;
			CMessages::BriefMessages[i].timeMessageAdded = CMessages::BriefMessages[i - 1].timeMessageAdded;
			CMessages::BriefMessages[i].tokenNumber1 = CMessages::BriefMessages[i - 1].tokenNumber1;
			CMessages::BriefMessages[i].tokenNumber2 = CMessages::BriefMessages[i - 1].tokenNumber2;
			CMessages::BriefMessages[i].tokenNumber3 = CMessages::BriefMessages[i - 1].tokenNumber3;
			CMessages::BriefMessages[i].tokenNumber4 = CMessages::BriefMessages[i - 1].tokenNumber4;
			CMessages::BriefMessages[i].tokenNumber5 = CMessages::BriefMessages[i - 1].tokenNumber5;
			CMessages::BriefMessages[i].tokenNumber6 = CMessages::BriefMessages[i - 1].tokenNumber6;
			CMessages::BriefMessages[i].tokenString = CMessages::BriefMessages[i - 1].tokenString;
		}
		CMessages::BriefMessages[i].message = message;
		CMessages::BriefMessages[i].flag = flag;
		CMessages::BriefMessages[i].duration = duration;
		CMessages::BriefMessages[i].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[i].tokenNumber1 = -1;
		CMessages::BriefMessages[i].tokenNumber2 = -1;
		CMessages::BriefMessages[i].tokenNumber3 = -1;
		CMessages::BriefMessages[i].tokenNumber4 = -1;
		CMessages::BriefMessages[i].tokenNumber5 = -1;
		CMessages::BriefMessages[i].tokenNumber6 = -1;
		CMessages::BriefMessages[i].tokenString = tokenString;
	} else {
		CMessages::BriefMessages[0].message = message;
		CMessages::BriefMessages[0].flag = flag;
		CMessages::BriefMessages[0].duration = duration;
		CMessages::BriefMessages[0].timeMessageAdded = CTimer::m_snTimeInMilliseconds;
		CMessages::BriefMessages[0].tokenNumber1 = -1;
		CMessages::BriefMessages[0].tokenNumber2 = -1;
		CMessages::BriefMessages[0].tokenNumber3 = -1;
		CMessages::BriefMessages[0].tokenNumber4 = -1;
		CMessages::BriefMessages[0].tokenNumber5 = -1;
		CMessages::BriefMessages[0].tokenNumber6 = -1;
		CMessages::BriefMessages[0].tokenString = tokenString;
		CMessages::AddToPreviousBriefArray(
			CMessages::BriefMessages[0].message,
			CMessages::BriefMessages[0].tokenNumber1,
			CMessages::BriefMessages[0].tokenNumber2,
			CMessages::BriefMessages[0].tokenNumber3,
			CMessages::BriefMessages[0].tokenNumber4,
			CMessages::BriefMessages[0].tokenNumber5,
			CMessages::BriefMessages[0].tokenNumber6,
			CMessages::BriefMessages[0].tokenString);
	}
}
