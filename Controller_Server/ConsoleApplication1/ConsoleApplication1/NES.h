#pragma once
#include "controller.h"
class NES :public controller
{
public:
	uint8_t rawData;
	bool bA, bB, bSTART, bSELECT, bUP, bDOWN, bLEFT, bRIGHT;
	bool getData();
	bool sendData();
	std::string getRawData();
	NES(std::string commPort = "");
};

