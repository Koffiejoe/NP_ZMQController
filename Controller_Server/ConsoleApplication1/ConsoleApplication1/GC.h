#pragma once
#include "controller.h"
class GC:public controller
{
	uint8_t rawData;
	bool bY, bX, bA, bB, bStart, bZ, bUp, bDown, bLeft, bRight, lTr, rTr;
	uint8_t joyX, joyY, cStX, cStY, lTrAnalog, rTrAnalog;
	bool getData();
	bool sendData();
	std::string getRawData();
	GC(std::string commPort = "");
};

