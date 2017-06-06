#pragma once

#include <RTClib.h>

struct SwitchTime {
	SwitchTime(byte hours, byte minutes);
	SwitchTime();

	bool operator>(const DateTime& other) const;
	bool operator>=(const DateTime& other) const;
	bool operator<(const DateTime& other) const;
	bool operator<=(const DateTime& other) const;
	bool operator>(const SwitchTime& other) const;
	friend bool operator>(const DateTime& dateTime, const SwitchTime& switchTime);
	friend bool operator>=(const DateTime& dateTime, const SwitchTime& switchTime);
	friend bool operator<(const DateTime& dateTime, const SwitchTime& switchTime);
	friend bool operator<=(const DateTime& dateTime, const SwitchTime& switchTime);

	SwitchTime AddMinutes(int minutes) const;
	bool IsEmpty() const;

	uint16_t TotalMinutes;

private:
	SwitchTime(uint16_t minutes);

	static uint16_t getTotalMinutes(const DateTime& dateTime);
};