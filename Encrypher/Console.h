#pragma once
#include "stdafx.h"
#include <iostream>
#include <string> 
#include <windows.h>
class Console {
public:
	static WORD GetColor() {
		CONSOLE_SCREEN_BUFFER_INFO info;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
			return NULL;
		return info.wAttributes;
	}
	static void SetColor(const WORD color) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
};
