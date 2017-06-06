#pragma once

#include "DeviceHandler.h"
#include "SwitchTime.h"

class CO2Handler : DeviceHandler
{
public:
	static void Loop(DateTime& now);
private:
	static SwitchTime TurnOnTime;
	static SwitchTime TurnOffTime;

	static void setTime(DateTime& now);
};
