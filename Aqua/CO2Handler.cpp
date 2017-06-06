#include "CO2Handler.h"
#include "LightHandler.h"
#include "Settings.h"

SwitchTime CO2Handler::TurnOnTime;
SwitchTime CO2Handler::TurnOffTime;

void CO2Handler::Loop(DateTime& now)
{
	setTime(now);
	DeviceState state = State(CO2);

	bool shouldTurnOn = now >= TurnOnTime && now < TurnOffTime;
	bool shouldTurnOff = now < TurnOnTime || now >= TurnOffTime;

	if (shouldTurnOn && state != On)
	{
		SwitchDevice(CO2, On);
	}
	else if (shouldTurnOff && state != Off)
	{
		SwitchDevice(CO2, Off);
	}
}

void CO2Handler::setTime(DateTime& now)
{
	if (IsWeekend(now))
	{
		TurnOnTime = Settings::WeekendTurnOnCO2;
		TurnOffTime = Settings::WeekendTurnOffCO2;
	}
	else
	{
		TurnOnTime = Settings::TurnOnCO2;
		TurnOffTime = Settings::TurnOffCO2;
	}
}
