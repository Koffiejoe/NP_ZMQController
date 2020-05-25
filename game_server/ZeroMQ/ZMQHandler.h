#pragma once
#include <string>
#include <zmq.h>

class ZMQHandler
{
public:
	ZMQHandler(const char* pub, const char* push);
	~ZMQHandler();
	int recv();
	int send();
private:
	//vars
	void* pushPtr, * subPtr, * context;

	//functions


};

