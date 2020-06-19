#pragma once
#include <iostream>
#include <stdint.h>
#include <string>
#include <chrono>
#include <vector>
#include "serial.h"

class controller
{
public:
	//Vars
	serial::Serial controllerSerial;
	void* rawData;
	std::chrono::steady_clock::time_point lastUpdate;	//used to set the update speed
	int updateSpeed = 500;								//ms
	bool rumble = false;
	std::string type;

	//used to only update when something changed
	std::string prevRespons;
	bool onlyNewUpdate = false;
	
	//specific topic with specific presses
	std::vector<std::string> customTopicTrigger; //data on which to trigger
	std::vector<std::string> custonTopicRespons; //data to respond with

	//functions
	virtual bool getData() = 0;
	virtual bool sendData() = 0;
	virtual std::string getRawData() = 0;
	std::string getPort();
	controller(std::string commPort = "");
	~controller();
};

