#pragma once

#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "Aqua.h"

class Display
{
public:
	static void Init();
	static void Loop(DateTime* now);

	static void PrintMessage(const char *message);
	static void PrintTemperature(float temperature);
	static void SetDeviceState(Devices device, DeviceState state);
private:
	static LiquidCrystal_I2C lcd;
	static const byte screenWidth = 16;

	static const int IntervalInMilliseconds = 1000;
	static unsigned long lastUpdateMilliseconds;
	static unsigned long lastMessageMilliseconds;

	static DateTime* currentDateTime;

	static void printDateTime();

	static void padLeftAndPrint(byte value, char padChar = ' ');
	static void fillBySpaces(int count);
	static char getDeviceChar(Devices device);
	static byte getDevicePosition(Devices device);
};
