#include "HeartbeatHandler.h"

void HeartbeatHandler::Init()
{
	lastLedMilliseconds = millis();
	pinMode(HeartbeatLedPin, OUTPUT);
}

void HeartbeatHandler::Loop()
{
	unsigned long mil = millis();
	if (mil - lastLedMilliseconds > HeartbeatIntervalInMilliseconds)
	{
		digitalWrite(HeartbeatLedPin, lastLedState);

		lastLedMilliseconds = mil;
		lastLedState = !lastLedState;
	}
}