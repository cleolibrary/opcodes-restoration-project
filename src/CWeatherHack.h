#pragma once

#include "vcclasses.h"

class CWeatherHack : public CWeather
{
public:
	static short Stored_OldWeatherType;
	static short Stored_NewWeatherType;
	static float Stored_InterpolationValue;
	static float Stored_Rain;
};
