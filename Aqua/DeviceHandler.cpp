#include "DeviceHandler.h"
#include "Settings.h"

// Device, Pin, CurrentState, ScheduleSetting, TargetState
byte DeviceHandler::DeviceSettings[][5] =
{
	{ Light, 4, Off, BySchedule, On },
	{ Heater, 7, Off, BySchedule, On },
	{ Filter, 8, On, BySchedule, On },
	{ Pump, 12, Off, BySchedule, On },
	{ Cooler, 11, Off, BySchedule, On },
	{ CO2, 10, Off, BySchedule, On }
};

void DeviceHandler::Init()
{
	for (byte i = 0; i < Settings::NumberOfTriggeredDevices; i++)
	{
		Devices device = (Devices)i;
		byte devicePin = DeviceSettings[device][1];

		pinMode(devicePin, OUTPUT);
		SwitchDevice(device, State(device));
	}
}

void DeviceHandler::SwitchDevice(Devices device, DeviceState state, bool forceSwitch)
{
	if (!forceSwitch && DeviceSettings[device][3] == Manual)
	{
		return;
	}

	byte signal;
	if (device == Light)
	{
		signal = state == On ? LOW : HIGH;
	}
	else
	{
		signal = state == On ? HIGH : LOW;
	}

	digitalWrite(DeviceSettings[device][1], signal);
	Display::SetDeviceState(device, state);

	if (DeviceSettings[device][2] == state)
	{
		return;
	}
	DeviceSettings[device][2] = state;

	NetworkClient::PostDeviceState(device, state);
}

DeviceState DeviceHandler::State(Devices device)
{
	return (DeviceState)DeviceSettings[device][2];
}

bool DeviceHandler::IsWeekend(DateTime& now)
{
	byte dayOfWeek = now.dayOfTheWeek();
	return dayOfWeek == 0 || dayOfWeek == 6;
}

#if _DEBUG

char* DeviceHandler::toString(Devices device)
{
	switch (device)
	{
	case None:		return "None";
	case Light:		return "Light";
	case Heater:	return "Heater";
	case Filter:	return "Filter";
	case Pump:		return "Pump";
	case Cooler:	return "Cooler";
	default:		return "[Unknown Device]";
	}
}

char* DeviceHandler::toString(DeviceState state)
{
	switch (state)
	{
	case On:		return "On";
	case Off:		return "Off";
	default:		return "[Unknown State]";
	}
}

#endif