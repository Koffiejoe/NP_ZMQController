#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <zmq.h>

//using namespace std;

class NESRecv
{
public:
	//funcitons
	NESRecv(const char* pub, const char* push);
	~NESRecv();

	int getList();
	int updateValues();
	int setUpdate();
	int getUpdate();
	int contrID = 1;
	int updateSpeed = 200; //in ms
	bool bA, bB, bSTART, bSELECT, bUP, bDOWN, bLEFT, bRIGHT;
private:
	//vars
	void* pushPtr, * subPtr, * context;
	std::vector<std::string> contrList;
	std::vector<std::string> commands;			//the commands decoded
	//functions
	int decode(char* buf);
	
};

