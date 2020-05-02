#pragma once
#include <string>
#include <zmq.h>
#include "controller.h"
class ZMQHandler
{
	ZMQHandler(char* pub, char* push);
	int recv();
	int send();
	controller *myController = NULL;
private:
	//vars
	void* pushPtr, * subPtr, * context;

	//functions
	~ZMQHandler();
	
};

