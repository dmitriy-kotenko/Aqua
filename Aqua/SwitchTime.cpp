#include "SwitchTime.h"

SwitchTime::SwitchTime(byte hours, byte minutes) : SwitchTime(hours * 60 + minutes)
{
};

SwitchTime::SwitchTime(uint16_t minutes)
{
	TotalMinutes = minutes;
}

SwitchTime::SwitchTime() : SwitchTime(0)
{
}

bool SwitchTime::operator>(const DateTime& other) const
{
	return TotalMinutes > getTotalMinutes(other);
}

bool SwitchTime::operator>=(const DateTime& other) const
{
	return TotalMinutes >= getTotalMinutes(other);
}

bool SwitchTime::operator<(const DateTime& other) const
{
	return TotalMinutes < getTotalMinutes(other);
}

bool SwitchTime::operator<=(const DateTime& other) const
{
	return TotalMinutes <= getTotalMinutes(other);
}

bool SwitchTime::operator>(const SwitchTime& other) const
{
	return TotalMinutes > other.TotalMinutes;
}

bool operator>(const DateTime& dateTime, const SwitchTime& switchTime)
{
	return !(switchTime >= dateTime);
}

bool operator>=(const DateTime& dateTime, const SwitchTime& switchTime)
{
	return !(switchTime > dateTime);
}

bool operator<(const DateTime& dateTime, const SwitchTime& switchTime)
{
	return !(switchTime <= dateTime);
}

bool operator<=(const DateTime& dateTime, const SwitchTime& switchTime)
{
	return !(switchTime < dateTime);
}

SwitchTime SwitchTime::AddMinutes(int minutes) const
{
	return SwitchTime(TotalMinutes + minutes);
}

bool SwitchTime::IsEmpty() const
{
	return TotalMinutes == 0;
}

uint16_t SwitchTime::getTotalMinutes(const DateTime& dateTime)
{
	return dateTime.hour() * 60 + dateTime.minute();
}
