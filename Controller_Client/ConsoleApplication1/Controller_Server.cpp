#include <iostream>
#include <stdlib.h>
#include <zmq.h>
#include "zmq.hpp"
#include <Windows.h>
#include <vector>

#include "NESRecv.h"
#include "GCRecv.h"
int main(int argc, char* argv[])
{
	using namespace std;

	int count = 0;
	GCRecv GCHandler("tcp://benternet.pxl-ea-ict.be:24042", "tcp://benternet.pxl-ea-ict.be:24041");
	NESRecv NESHandler("tcp://benternet.pxl-ea-ict.be:24042", "tcp://benternet.pxl-ea-ict.be:24041");

	//Setting contrID
	GCHandler.contrID = 0;
	NESHandler.contrID = 1;
	NESHandler.setUpdate();
	GCHandler.setUpdate();

	//first demo of then NES controller
	cout << "DEMO" << endl;
	cout << "First demo of the NES controller" << endl;

	Sleep(1000);

	//showing values
	for (int i = 0; i < 20; i++)
	{
		NESHandler.updateValues();
		cout << "A button: " << NESHandler.bA << " " << "B button: " << NESHandler.bB << "\r";
	}
	//values with increased speed
	cout << "increasing speed from " << NESHandler.updateSpeed << "to 20ms" << endl;
	NESHandler.updateSpeed = 20;
	NESHandler.setUpdate();
	for (int i = 0; i < 200; i++)
	{
		NESHandler.updateValues();
		cout << "A button: " << NESHandler.bA << " " << "B button: " << NESHandler.bB << "\r";
	}
	cout << endl << endl;

	//demo of GCcontroller
	cout << "DEMO" << endl;
	cout << "Demo of the GC controller" << endl;
	Sleep(1000);

	//showing values
	for (int i = 0; i < 20; i++)
	{
		GCHandler.updateValues();
		cout << "Y button: " << GCHandler.bY << " " << "START button: " << GCHandler.bStart << "\r";
	}
	//values with increased speed
	cout << "increasing speed from " << GCHandler.updateSpeed << "to 20ms" << endl;
	GCHandler.updateSpeed = 20;
	GCHandler.setUpdate();
	for (int i = 0; i < 200; i++)
	{
		GCHandler.updateValues();
		cout << "Y button: " << GCHandler.bY << " " << "START button: " << GCHandler.bStart << "\r";
	}

	//reset updatespeed
	NESHandler.updateSpeed = 200;
	GCHandler.updateSpeed = 200;
	GCHandler.setUpdate();
	NESHandler.setUpdate();

	cout << "Intermittend setting rumble on GC controller" << endl;
	bool rumble = FALSE;
	for (int i = 0; i < 20; i++)
	{
		GCHandler.setRumble(rumble);
		rumble = !rumble;
		cout << "Rumble : " << rumble << "\r"; 
		Sleep(1500);
	}

}