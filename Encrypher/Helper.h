#pragma once
class _HThreads {
public:
	static bool checkCorrectThreadNumber(const unsigned int ThreadNum) {
		if (ThreadNum == 64 || ThreadNum == 32 || ThreadNum == 16 ||
			ThreadNum == 8 || ThreadNum == 4 || ThreadNum == 2 ||
			ThreadNum == 1)
			return true;
		return false;
	}
};
