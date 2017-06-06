#pragma once

#include "SwitchTime.h"
#include "Aqua.h"
#include "TypeUtilities.h"
#include "DeviceHandler.h"

namespace {
	class Settings
	{
	public:
		static const byte MessageTimeoutSeconds = 3;
		static const byte TempSensorPin = 5;
		static const byte LedStripPin = 9;

		static const byte NumberOfTriggeredDevices = 6;

		static const byte LedFadeMinutes = 15;
		static const byte LunchLedBrightness = 7;
		static const SwitchTime TurnOnLight;
		static const SwitchTime TurnOffLightLunch;
		static const SwitchTime TurnOnLightLunch;
		static const SwitchTime TurnOffLight;
		static const SwitchTime WeekendTurnOnLight;
		static const SwitchTime WeekendTurnOffLight;

		static const SwitchTime TurnOnPump;
		static const SwitchTime TurnOffPump;
		
		static const SwitchTime TurnOnCO2;
		static const SwitchTime TurnOffCO2;
		static const SwitchTime WeekendTurnOnCO2;
		static const SwitchTime WeekendTurnOffCO2;
		static const byte TurnOnCO2LeadMinutes = 30;
		static const byte TurnOffCO2LeadMinutes = 15;

		static const float DesiredTemperature;
		static const float SwitchDelta;
		static const byte MinTempBound = 10;
		static const byte MaxTempBound = 75;
		static const int ReadTempIntervalInSeconds = 2;

		static void SetSettings(char* settings)
		{
			char* settingByDevice = strtok(settings, "@");
			while (settingByDevice != NULL)
			{
				Devices device = (Devices)TypeUtilities::CharToByte(settingByDevice[0]);
				DeviceHandler::DeviceSettings[device][3] = TypeUtilities::CharToByte(settingByDevice[2]);
				DeviceHandler::DeviceSettings[device][4] = TypeUtilities::CharToByte(settingByDevice[4]);

				settingByDevice = strtok(NULL, "@");
			}
		}

		static void SetDefaultSettings()
		{
			for (byte i = 0; i < NumberOfTriggeredDevices; i++)
			{
				Devices device = (Devices)i;
				DeviceHandler::DeviceSettings[device][3] = BySchedule;
			}
		}

		static void ApplySettings()
		{
			for (byte i = 0; i < NumberOfTriggeredDevices; i++)
			{
				Devices device = (Devices)i;
				ScheduleSetting scheduleSetting = (ScheduleSetting)DeviceHandler::DeviceSettings[device][3];

				if (scheduleSetting == Manual)
				{
					DeviceState targetState = (DeviceState)DeviceHandler::DeviceSettings[device][4];
					DeviceHandler::SwitchDevice(device, targetState, true);
				}
				else
				{
					if (device == Filter)
					{
						DeviceHandler::SwitchDevice(device, On);
					}
				}
			}
		}
	};

	const SwitchTime Settings::TurnOnLight = SwitchTime(9, 45);
	const SwitchTime Settings::TurnOffLightLunch = SwitchTime(14, 30);
	const SwitchTime Settings::TurnOnLightLunch = SwitchTime(15, 45);
	const SwitchTime Settings::TurnOffLight = SwitchTime(21, 15);

	const SwitchTime Settings::WeekendTurnOnLight = SwitchTime(11, 30);
	const SwitchTime Settings::WeekendTurnOffLight = SwitchTime(21, 45);

	const SwitchTime Settings::TurnOnPump = SwitchTime(2, 0);
	const SwitchTime Settings::TurnOffPump = SwitchTime(6, 0);

	const SwitchTime Settings::TurnOnCO2 = TurnOnLight.AddMinutes(-TurnOnCO2LeadMinutes);
	const SwitchTime Settings::TurnOffCO2 = TurnOffLight.AddMinutes(-TurnOffCO2LeadMinutes);
	const SwitchTime Settings::WeekendTurnOnCO2 = WeekendTurnOnLight.AddMinutes(-TurnOnCO2LeadMinutes);
	const SwitchTime Settings::WeekendTurnOffCO2 = WeekendTurnOffLight.AddMinutes(-TurnOffCO2LeadMinutes);

	const float Settings::DesiredTemperature = 25;
	const float Settings::SwitchDelta = 0.4;
}