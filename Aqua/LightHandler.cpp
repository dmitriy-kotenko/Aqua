#include "LightHandler.h"
#include "Settings.h"

const int LightHandler::LedFadeSeconds = Settings::LedFadeMinutes * 60;
SwitchTime LightHandler::TurnOnTime;
SwitchTime LightHandler::TurnOffLunchTime;
SwitchTime LightHandler::TurnOnLunchTime;
SwitchTime LightHandler::TurnOffTime;

void LightHandler::Init()
{
	pinMode(Settings::LedStripPin, OUTPUT);
}

void LightHandler::Loop(DateTime& now)
{
	setTime(now);
	processLedStripLight(now);
	processMainLight(now);
}

void LightHandler::setTime(DateTime& now)
{
	if (IsWeekend(now))
	{
		TurnOnTime = Settings::WeekendTurnOnLight;
		TurnOffLunchTime = SwitchTime();
		TurnOnLunchTime = SwitchTime();
		TurnOffTime = Settings::WeekendTurnOffLight;
	}
	else
	{
		TurnOnTime = Settings::TurnOnLight;
		TurnOffLunchTime = Settings::TurnOffLightLunch;
		TurnOnLunchTime = Settings::TurnOnLightLunch;
		TurnOffTime = Settings::TurnOffLight;
	}
}

void LightHandler::processLedStripLight(DateTime& now)
{
	SwitchTime turnOnTimeArray[2] = { TurnOnTime, TurnOnLunchTime };
	SwitchTime turnOffTimeArray[2] = { TurnOffTime, TurnOffLunchTime };

	byte brightness = 0;

	for (byte i = 0; i < 2; i++)
	{
		SwitchTime switchOnTime = turnOnTimeArray[i];

		if (!switchOnTime.IsEmpty())
		{
			if (now >= switchOnTime.AddMinutes(-Settings::LedFadeMinutes) && now - TimeSpan(HoldLedSeconds) < switchOnTime)
			{
				int32_t leftSecondsToSwitchOn = max(getTimeSpanSeconds(now, switchOnTime), 0);
				brightness = 255 * (LedFadeSeconds - leftSecondsToSwitchOn) / LedFadeSeconds;
				break;
			}
		}

		SwitchTime switchOffTime = turnOffTimeArray[i];
		if (!switchOffTime.IsEmpty())
		{
			SwitchTime ledSwitchOffTime = switchOffTime.AddMinutes(Settings::LedFadeMinutes);
			if (now >= switchOffTime && now < ledSwitchOffTime)
			{
				int32_t leftSecondsToLedSwitchOff = getTimeSpanSeconds(now, ledSwitchOffTime);
				brightness = 255 * leftSecondsToLedSwitchOff / LedFadeSeconds;
				break;
			}
		}
	}

	if (lunchIsSet())
	{
		if (now >= TurnOffLunchTime && now < TurnOnLunchTime)
		{
			brightness = max(brightness, Settings::LunchLedBrightness);
		}
	}

	analogWrite(Settings::LedStripPin, brightness);
}

void LightHandler::processMainLight(DateTime& now)
{
	DeviceState state = State(Light);

	if (lunchIsSet())
	{
		if (now >= TurnOnTime && now < TurnOffLunchTime && state != On)
		{
			SwitchDevice(Light, On);
		}
		else if (now >= TurnOffLunchTime && now < TurnOnLunchTime && state != Off)
		{
			SwitchDevice(Light, Off);
		}
		else if (now >= TurnOnLunchTime && now < TurnOffTime && state != On)
		{
			SwitchDevice(Light, On);
		}
		else if ((now >= TurnOffTime || now < TurnOnTime) && state != Off)
		{
			SwitchDevice(Light, Off);
		}
	}
	else
	{
		if (now >= TurnOnTime && now < TurnOffTime && state != On)
		{
			SwitchDevice(Light, On);
		}
		else if ((now >= TurnOffTime || now < TurnOnTime) && state != Off)
		{
			SwitchDevice(Light, Off);
		}
	}
}

int32_t LightHandler::getTimeSpanSeconds(const DateTime& startTime, const SwitchTime& endTime)
{
	int32_t endTimeSeconds = (int32_t)endTime.TotalMinutes * 60;
	int32_t startTimeSeconds = (int32_t)startTime.hour() * 3600 + startTime.minute() * 60 + startTime.second();

	return endTimeSeconds - startTimeSeconds;
}

bool LightHandler::lunchIsSet()
{
	return !TurnOffLunchTime.IsEmpty() && !TurnOnLunchTime.IsEmpty();
}
