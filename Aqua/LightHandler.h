#pragma once

#include "DeviceHandler.h"
#include "SwitchTime.h"

class LightHandler : DeviceHandler
{
public:
	static void Init();
	static void Loop(DateTime& now);

private:
	static const int LedFadeSeconds;
	static const byte HoldLedSeconds = 3;

	static SwitchTime TurnOnTime;
	static SwitchTime TurnOffLunchTime;
	static SwitchTime TurnOnLunchTime;
	static SwitchTime TurnOffTime;

	static void setTime(DateTime& now);
	static void processLedStripLight(DateTime& now);
	static void processMainLight(DateTime& now);
	static int32_t getTimeSpanSeconds(const DateTime& startTime, const SwitchTime& endTime);
	static bool lunchIsSet();
};
