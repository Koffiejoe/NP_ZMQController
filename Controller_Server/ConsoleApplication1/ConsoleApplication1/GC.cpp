#include "GC.h"
#include "NES.h"
#include <stdio.h>
#include <stdlib.h>
GC::GC(std::string commPort) : controller(commPort)
{
	type = "GC";
	controllerSerial.write("t");
	if (controllerSerial.readline() != "GC_Contr\r\n")
	{
		std::cout << "Warning: contr at " << controllerSerial.getPort() << " is not a GC_Contr" << std::endl;
	}
}

bool GC::getData()
{
	controllerSerial.write("g");
	recvString = controllerSerial.readline();
	recvString.pop_back();	//remove \n
	recvString += '\0';		//add NULL for decoder
	rawData = strtoull(recvString.c_str(), NULL, 16);
	
	//stuff all data in vars (yes could be done better but this is copy paste and readable)
	bStart	= rawData & 0x1000000000000000;
	bY		= rawData & 0x0800000000000000;
	bX		= rawData & 0x0400000000000000;
	bB		= rawData & 0x0200000000000000;
	bA		= rawData & 0x0100000000000000;
	lTr		= rawData & 0x0040000000000000;
	rTr		= rawData & 0x0020000000000000;
	bZ		= rawData & 0x0010000000000000;
	bUp		= rawData & 0x0008000000000000;
	bDown	= rawData & 0x0004000000000000;
	bRight	= rawData & 0x0002000000000000;
	bLeft	= rawData & 0x0001000000000000;
	joyX	= (rawData & 0x0000FF0000000000) >> 40; //>>40
	joyY	= (rawData & 0x000000FF00000000) >> 32; //>>32
	cStX	= (rawData & 0x00000000FF000000) >> 24;	//>>24
	cStY	= (rawData & 0x0000000000FF0000) >> 16;  //>>16
	rTrAnalog = (rawData & 0x000000000000FF00) >> 8; //>>8
	lTrAnalog = (rawData & 0x00000000000000FF);

	return 1;
}

bool GC::sendData()
{
	if (rumble)
	{
		controllerSerial.write("r");	//enable rumble
	}
	else
	{
		controllerSerial.write("n");	//disable rumble
	}
	return 1;
}

//automatically updates the values and sends a string ready to be transmitted over ZMQ
std::string GC::getRawData()
{
	getData();
	return(recvString); //easy convert to char bc data is 8 bits wide
}