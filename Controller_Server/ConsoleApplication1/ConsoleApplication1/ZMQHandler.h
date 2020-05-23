#pragma once
#include <string>
#include <string.h>
#include <vector>
#include <zmq.h>
#include "controller.h"

using namespace std;

class ZMQHandler
{
public:
	//funcitons
	ZMQHandler(const char* pub, const char* push);
	~ZMQHandler();
	int recv();
	int send();
	vector<controller*> controllerList;
private:
	//vars
	void* pushPtr, * subPtr, * context;
	//functions
	int sendRespons(std::string* commands);
	
};

