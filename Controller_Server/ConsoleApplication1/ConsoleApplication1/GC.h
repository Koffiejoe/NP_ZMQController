#pragma once
#include "controller.h"
class GC:public controller
{
public:
	uint64_t rawData;
	std::string recvString;
	bool bY, bX, bA, bB, bStart, bZ, bUp, bDown, bLeft, bRight, lTr, rTr;
	int joyX, joyY, cStX, cStY, lTrAnalog, rTrAnalog;
	bool getData();
	bool sendData();
	std::string getRawData();
	GC(std::string commPort = "");
};

