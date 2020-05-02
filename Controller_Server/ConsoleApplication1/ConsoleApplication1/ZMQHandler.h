#pragma once
#include <string>
#include <zmq.h>
#include "controller.h"
class ZMQHandler
{
	ZMQHandler(char* pub, char* push);
	int run();
	controller *myController = NULL;
private:
	//vars
	void* pushPtr, * subPtr, * context;

	//functions
	~ZMQHandler();
	
};

