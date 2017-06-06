/*
 Name:		Aqua.ino
 Created:	1/29/2016 10:10:49 PM
 Author:	Dmitriy Kotenko
*/

#include <avr/wdt.h>
#include <RTClib.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#include "ESP8266.h"
#include "NetworkClient.h"
#include "LightHandler.h"
#include "PumpHandler.h"
#include "CO2Handler.h"
#include "TemperatureHandler.h"

#include "Display.h"
#include "Settings.h"

NetworkClient networkClient;
RTC_DS3231 rtc;
Display display;

LightHandler lightHandler;
TemperatureHandler temperatureHandler;
PumpHandler pumpHandler;
CO2Handler co2Handler;

void syncTime()
{
	display.PrintMessage("Syncing time...");
	delay(4000);
	uint32_t unixTime = networkClient.GetCurrentTime();
	if (unixTime == 0)
	{
		display.PrintMessage("Sync time error");
		return;
	}

	rtc.adjust(DateTime(unixTime));
	display.PrintMessage("Sync time done");
}

// the setup function runs once when you press reset or power the board
void setup()
{
	rtc.begin();
	display.Init();

	DeviceHandler::Init();
	lightHandler.Init();
	temperatureHandler.Init();

	syncTime();

	wdt_enable(WDTO_8S);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	wdt_reset();

	if (networkClient.LoopGetSettings())
	{
		Settings::ApplySettings();
	}

	DateTime now = rtc.now();
	lightHandler.Loop(now);
	pumpHandler.Loop(now);
	co2Handler.Loop(now);
	temperatureHandler.Loop();

	display.Loop(&now);
	networkClient.LoopPostData();
}