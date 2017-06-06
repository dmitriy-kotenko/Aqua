#pragma once

#include "DeviceHandler.h"

class PumpHandler : DeviceHandler
{
public:
	static void Loop(DateTime& now);
};
