#pragma once
#include <vector>

static std::vector<unsigned char> GetBytes(const double value) {
	double d = value;
	unsigned char buf[sizeof d];
	memcpy(buf, &d, sizeof d);
	std::vector<unsigned char> v(buf, buf + sizeof buf / sizeof buf[0]);
	return v;
}
static int ToInt32(const std::vector<unsigned char> b,const int offset) {
	int result = b[offset] & 0xff;
	result |= (b[offset + 1] & 0xff) << 8;
	result |= (b[offset + 2] & 0xff) << 16;
	result |= (b[offset + 3] & 0xff) << 24;
	return result;
}
static int ToInt32(const double value)
{
	if (value >= 0.0)
	{
		if (value < 2147483647.5)
		{
			int num = (int)value;
			double num2 = value - num;
			if ((num2 > 0.5) || ((num2 == 0.5) && ((num & 1) != 0)))
			{
				num++;
			}
			return num;
		}
	}
	else if (value >= -2147483648.5)
	{
		int num3 = (int)value;
		double num4 = value - num3;
		if ((num4 < -0.5) || ((num4 == -0.5) && ((num3 & 1) != 0)))
		{
			num3--;
		}
		return num3;
	}
	throw "The int is too big!";
}
static bool check_for_endianness()
{
	unsigned int x = 0x76543210;
	char *c = (char*)&x;
	return *c == 0x10;
}