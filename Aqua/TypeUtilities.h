#pragma once

class TypeUtilities
{
public:
	static byte CharToByte(char chr);
	static char* FloatToString(char * outstr, float value, int places, int minwidth = 0);
};