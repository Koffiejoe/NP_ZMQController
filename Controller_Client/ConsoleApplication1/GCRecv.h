#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <zmq.h>

//using namespace std;

class GCRecv
{
public:
	//funcitons
	GCRecv(const char* pub, const char* push);
	~GCRecv();

	int getList();
	int updateValues();
	int setUpdate();
	int setRumble(bool value); //true = on
	int contrID = 1;
	int updateSpeed = 200; //in ms
	bool bY, bX, bA, bB, bStart, bZ, bUp, bDown, bLeft, bRight, lTr, rTr;
private:
	//vars
	void* pushPtr, * subPtr, * context;
	std::vector<std::string> contrList;
	std::vector<std::string> commands;			//the commands decoded
	//functions
	int decode(char* buf);

};

