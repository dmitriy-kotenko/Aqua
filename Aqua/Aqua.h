#pragma once

enum Devices : byte
{
	Light,
	Heater,
	Filter,
	Pump,
	Cooler,
	CO2,
	None
};

enum DeviceState : byte
{
	Off,
	On
};

enum ScheduleSetting : byte
{	
	Manual,
	BySchedule
};