#pragma once

#include "Arduino.h"
#include "Display.h"
#include "NetworkClient.h"
#include "Aqua.h"

class DeviceHandler
{
public:
	static byte DeviceSettings[][5];

	static void Init();
	static void SwitchDevice(Devices device, DeviceState state, bool forceSwitch = false);

protected:
	static DeviceState State(Devices device);
	static bool IsWeekend(DateTime& now);

private:
#if _DEBUG
	static char* toString(Devices device);
	static char* toString(DeviceState state);
#endif
};
