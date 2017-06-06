#include "TemperatureHandler.h"
#include "Display.h"
#include "Settings.h"

TemperatureHandler::TemperatureHandler()
	: DeviceHandler(), oneWire(Settings::TempSensorPin), sensors(&oneWire)
{
}

void TemperatureHandler::Init()
{
	sensors.begin();
}

void TemperatureHandler::Loop()
{
	unsigned long mil = millis();
	if (mil - lastFetchMilliseconds <= Settings::ReadTempIntervalInSeconds * 1000 && lastFetchMilliseconds != 0)
	{
		return;
	}
	lastFetchMilliseconds = mil;

	float temperature = getTemperature();

	if (temperature < Settings::MinTempBound || temperature > Settings::MaxTempBound)
	{
		temperature = 0;
		SwitchDevice(Heater, Off);
		SwitchDevice(Cooler, Off);
	}
	else
	{
		DeviceState heaterState = State(Heater);
		if (temperature > Settings::DesiredTemperature && heaterState == On)
		{
			SwitchDevice(Heater, Off);
		}
		else if (temperature < Settings::DesiredTemperature - Settings::SwitchDelta && heaterState == Off)
		{
			SwitchDevice(Heater, On);
		}

		DeviceState coolerState = State(Cooler);
		if (temperature > Settings::DesiredTemperature + Settings::SwitchDelta && coolerState == Off)
		{
			SwitchDevice(Cooler, On);
		}
		else if (temperature < Settings::DesiredTemperature && coolerState == On)
		{
			SwitchDevice(Cooler, Off);
		}
	}

	Display::PrintTemperature(temperature);
	NetworkClient::Temperature = temperature;
}

float TemperatureHandler::getTemperature()
{
	if (!sensors.requestTemperaturesByAddress(deviceAddress))
	{
		return DEVICE_DISCONNECTED_C;
	}

	return sensors.getTempC(deviceAddress);
}
