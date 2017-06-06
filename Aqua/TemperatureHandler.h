#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DeviceHandler.h"

class TemperatureHandler : DeviceHandler
{
public:
	TemperatureHandler();
	void Init();
	void Loop();

private:
	unsigned long lastFetchMilliseconds = 0;

	OneWire oneWire;
	DallasTemperature sensors;

	DeviceAddress deviceAddress = { 40, 255, 172, 0, 178, 21, 1, 228 };

	float getTemperature();
};