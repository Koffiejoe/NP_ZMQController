#pragma once
#include <string>
#include <string.h>
#include <vector>
#include <zmq.h>
#include "controller.h"

//defines for printing return code
#define ERR_INV_CONTR_NUM "controllerService!>err>INV_CONTR_NUM>"
#define ERR_INV_UPDATE_SPEED "controllerService!>err>INV_UPD_SPEED>"
#define ERR_INV_3RD_COMM  "controllerService!>err>INV_3RD_COM>"
#define ERR_INV_1ST_COMM "controllerService!>err>INV_1ST_COM>"
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

