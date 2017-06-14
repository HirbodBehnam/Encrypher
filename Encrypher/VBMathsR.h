#include "CSharp.h"
#include <windows.h>
#include <iostream>
#pragma once
class VBMathsR
{
public:
	void Randomize(double Number) {
		long num2;
		long rndSeed = m_rndSeed;
		if (check_for_endianness()) {
			num2 = ToInt32(GetBytes(Number), 4);
		}
		else {
			num2 = ToInt32(GetBytes(Number), 0);
		}
		num2 = ((num2 & 0xffff) ^ (num2 >> 0x10)) << 8;
		rndSeed = (rndSeed & -16776961) | num2;
		m_rndSeed = rndSeed;
	}
	float Rnd() { 
		m_rndSeed = (unsigned long long)(((m_rndSeed * 0x43fd43fd) + 0xc39ec3) & 0xffffff);
		return (((float)m_rndSeed) / 1.677722E+07f);
	}
	float Rnd(float Number) {
		if (Number != 0.0)
		{
			if (Number == -1.0)
			{
				m_rndSeed = 8388799;
			}
			m_rndSeed = (unsigned long long)(((m_rndSeed * 0x43fd43fd) + 0xc39ec3) & 0xffffff);
		}
		return (((float)m_rndSeed) / 1.677722E+07f);
	}
	void setSeed(long seed) {
		m_rndSeed = seed;
	}
	long getSeed() {
		return m_rndSeed;
	}
private:
	long m_rndSeed = 327680;
};

