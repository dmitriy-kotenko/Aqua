#pragma once

#include <Arduino.h>

static const byte HeartbeatLedPin = 13;
static const int HeartbeatIntervalInMilliseconds = 200;

class HeartbeatHandler
{
public:
	void Init();
	void Loop();

private:
	unsigned long lastLedMilliseconds = 0;
	byte lastLedState = LOW;
};