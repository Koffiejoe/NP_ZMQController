#pragma once
#include <string>
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
private:
	//vars
	void* pushPtr, * subPtr, * context;

	//functions
	
	
};

