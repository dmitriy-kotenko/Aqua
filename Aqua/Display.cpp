#include "Display.h"
#include <Arduino.h>
#include "Settings.h"

LiquidCrystal_I2C Display::lcd = LiquidCrystal_I2C(0x27, screenWidth, 2);

unsigned long Display::lastUpdateMilliseconds = 0;
unsigned long Display::lastMessageMilliseconds = 0;
DateTime* Display::currentDateTime = NULL;

void Display::Init()
{
	lastUpdateMilliseconds = millis();

	lcd.begin();
}

void Display::Loop(DateTime* now)
{
	unsigned long mil = millis();

	if (mil - lastMessageMilliseconds <= Settings::MessageTimeoutSeconds * 1000 && lastMessageMilliseconds != 0)
	{
		return;
	}

	if (mil - lastUpdateMilliseconds <= IntervalInMilliseconds && lastUpdateMilliseconds != 0)
	{
		return;
	}
	lastUpdateMilliseconds = mil;

	currentDateTime = now;
	printDateTime();
}

void Display::PrintMessage(const char *message)
{
	lcd.setCursor(0, 0);
	lcd.print(message);
	fillBySpaces(screenWidth - strlen(message));
	lastMessageMilliseconds = millis();
}

void Display::PrintTemperature(float temperature)
{
	lcd.setCursor(0, 1);
	lcd.print(temperature, 1);	// 4 symbols
	lcd.print(char(223));		// 1 symbol
	lcd.print('C');				// 1 symbol
}

void Display::SetDeviceState(Devices device, DeviceState state)
{
	lcd.setCursor(getDevicePosition(device) - 1, 1);

	char deviceChar = state == Off ? ' ' : getDeviceChar(device);
	lcd.print(deviceChar);
}

void Display::printDateTime()
{
	lcd.setCursor(0, 0);

	if (currentDateTime)
	{
		padLeftAndPrint(currentDateTime->hour());
		lcd.print(':');
		padLeftAndPrint(currentDateTime->minute(), '0');
		lcd.print(':');
		padLeftAndPrint(currentDateTime->second(), '0');

#if _DEBUG
		lcd.print(" * ");
#else
		lcd.print("   ");
#endif

		padLeftAndPrint(currentDateTime->day());
		lcd.print('.');
		padLeftAndPrint(currentDateTime->month(), '0');
	}
}

void Display::padLeftAndPrint(byte value, char padChar)
{
	if (value < 10)
	{
		lcd.print(padChar);
	}

	if (value > 59)
	{
		lcd.print(F("--"));
	}
	else
	{
		lcd.print(value);
	}
}

void Display::fillBySpaces(int count)
{
	for (byte i = 0; i < count; i++)
	{
		lcd.print(' ');
	}
}

char Display::getDeviceChar(Devices device)
{
	switch (device)
	{
	case Light:			return 'L';
	case Heater:		return 'H';
	case Filter:		return 'F';
	case Pump:			return 'P';
	case Cooler:		return 'C';
	case CO2:			return 'D';
	default:			return ' ';
	}
}

byte Display::getDevicePosition(Devices device)
{
	switch (device)
	{
	case Pump:			return 11;
	case Cooler:		return 12;
	case Heater:		return 13;
	case CO2:			return 14;
	case Light:			return 15;
	case Filter:		return 16;
	default:			return 42;
	}
}
