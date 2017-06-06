#include "NetworkClient.h"
#include "Settings.h"

unsigned long NetworkClient::lastPostDataMilliseconds = 0;
unsigned long NetworkClient::lastGetSettingsMilliseconds = 0;
float NetworkClient::Temperature = 0;
byte NetworkClient::GetSettingsAttempts = 0;

ESP8266 NetworkClient::wifi = ESP8266(Serial1, ConnectionSpeed);

void NetworkClient::LoopPostData()
{
	unsigned long mil = millis();
	if (mil - lastPostDataMilliseconds <= (uint64_t)PostDataIntervalInSeconds * 1000 && lastPostDataMilliseconds != 0)
	{
		return;
	}
	lastPostDataMilliseconds = mil;

	postData();
}

bool NetworkClient::LoopGetSettings()
{
	unsigned long mil = millis();
	if (mil - lastGetSettingsMilliseconds <= (uint64_t)GetSettingsIntervalInSeconds * 1000 && lastGetSettingsMilliseconds != 0)
	{
		return false;
	}
	lastGetSettingsMilliseconds = mil;

	const byte SettingsLength = 36;
	char settings[SettingsLength];

	if (getSettings(settings, SettingsLength))
	{
		GetSettingsAttempts = 0;
		Settings::SetSettings(settings);
		return true;
	}

	GetSettingsAttempts++;

	if (GetSettingsAttempts == MaxNumberOfGetSettingsAttempts)
	{
		Settings::SetDefaultSettings();
		return true;
	}

	return false;
}

void NetworkClient::PostDeviceState(Devices device, DeviceState state)
{
	String content = String("{ \"Device\":") + (byte)device + F(", \"State\":") + (byte)state + F("}");
	post("deviceState", content.c_str());
}

uint32_t NetworkClient::GetCurrentTime()
{
	const byte BodyLength = 10;
	char result[BodyLength + 1];
	if (!get(result, "currentTime", BodyLength))
	{
		return 0;
	}

	return atol(result);
}

void NetworkClient::postData()
{
	char buffer[7];
	String content = TypeUtilities::FloatToString(buffer, Temperature, 2);
	content += String("\r\n") + millis();

	for (byte device = 0; device < Settings::NumberOfTriggeredDevices; device++)
	{
		byte deviceState = DeviceHandler::DeviceSettings[device][2];
		content += String("\r\n") + device + ":" + deviceState;
	}

	post("data", content.c_str());
}

bool NetworkClient::getSettings(char *result, byte resultLength)
{
	return get(result, "settings?separator=@", resultLength);
}

bool NetworkClient::get(char *result, char *actionName, byte bodyLength)
{
	String request = String("GET") + " /api/arduino/" + actionName +
		F(" HTTP/1.1\r\nHost: ") + HostName + F("\r\nConnection: close\r\n\r\n");

	if (!sendRequest(request.c_str()))
	{
		return false;
	}

	return wifi.recvBody(result, bodyLength) != 0;
}

void NetworkClient::post(char *actionName, const char *content)
{
	String request = String("POST /api/arduino/") +
		actionName + F(" HTTP/1.1\r\nHost: ") + HostName +
		F("\r\nConnection: close\r\nContent-type: application/json\r\nContent-length: ") + strlen(content) +
		F("\r\n\r\n") + content;

	sendRequest(request.c_str());
}

bool NetworkClient::sendRequest(const char *request)
{
	delay(75);

	if (!wifi.createTCP(HostName, Port))
	{
		return false;
	}
	return wifi.send((const byte*)request, strlen(request));
}