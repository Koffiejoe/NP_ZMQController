#pragma once
#include <iostream>
#include <stdint.h>
#include <string>
#include "serial.h"

class controller
{
public:
	//Vars
	int ID;
	serial::Serial controllerSerial;
	void* rawData;
	//functions
	virtual bool getData() = 0;
	virtual bool sendData() = 0;
	virtual std::string getRawData() = 0;
	std::string getPort();
	controller(std::string commPort = "");
	~controller();
};

