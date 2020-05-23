#pragma once
#include <iostream>
#include <stdint.h>
#include <string>
#include <chrono>
#include "serial.h"

class controller
{
public:
	//Vars
	serial::Serial controllerSerial;
	void* rawData;
	std::chrono::steady_clock::time_point lastUpdate;	//used to set the update speed
	int updateSpeed = 500;								//ms
	//functions
	virtual bool getData() = 0;
	virtual bool sendData() = 0;
	virtual std::string getRawData() = 0;
	std::string getPort();
	controller(std::string commPort = "");
	~controller();
};

