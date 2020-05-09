#pragma once
#include <string>
#include <string.h>
#include <vector>
#include <zmq.h>
#include "controller.h"

class ZMQHandler
{
public:
	ZMQHandler(const char* pub, const char* push);
	~ZMQHandler();
	int recv();
	int send();
	controller *myController = NULL;
	int updateSpeed = 500;				//in ms
private:
	//vars
	void* pushPtr, * subPtr, * context;

	//functions
	
	
};

