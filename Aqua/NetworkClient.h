#pragma once

#include "ESP8266.h"
#include "Aqua.h"

#define HostName "aquaserver.azurewebsites.net"
#define Port 80

class NetworkClient
{
public:
	static void LoopPostData();
	static bool LoopGetSettings();

	static float Temperature;
	static void PostDeviceState(Devices device, DeviceState state);
	static uint32_t GetCurrentTime();
private:
	static const uint32_t ConnectionSpeed = 115200;
	static const byte PostDataIntervalInSeconds = 60;
	static const byte GetSettingsIntervalInSeconds = 10;
	
	static const byte MaxNumberOfGetSettingsAttempts = 3;
	static byte GetSettingsAttempts;

	static ESP8266 wifi;

	static unsigned long lastPostDataMilliseconds;
	static unsigned long lastGetSettingsMilliseconds;
	static void postData();
	static bool getSettings(char *result, byte resultLength);

	static bool get(char *result, char *actionName, byte bodyLength);
	static void post(char *actionName, const char *content);
	static bool sendRequest(const char *request);
};