#include "PumpHandler.h"
#include "LightHandler.h"
#include "Settings.h"

void PumpHandler::Loop(DateTime& now)
{
	DeviceState state = State(Pump);

	bool shouldTurnOn = Settings::TurnOnPump > Settings::TurnOffPump
		? now >= Settings::TurnOnPump || now < Settings::TurnOffPump
		: now >= Settings::TurnOnPump && now < Settings::TurnOffPump;

	bool shouldTurnOff = Settings::TurnOnPump > Settings::TurnOffPump
		? now >= Settings::TurnOffPump && now < Settings::TurnOnPump
		: now >= Settings::TurnOffPump || now < Settings::TurnOnPump;

	if (shouldTurnOn && state != On)
	{
		SwitchDevice(Pump, On);
	}
	else if (shouldTurnOff && state != Off)
	{
		SwitchDevice(Pump, Off);
	}
}