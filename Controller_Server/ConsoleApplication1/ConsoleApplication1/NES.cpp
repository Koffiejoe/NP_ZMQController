#include "NES.h"
#include <stdio.h>
NES::NES(std::string commPort) : controller(commPort)
{
	type = "NES";
	controllerSerial.write("init\n");
	if (controllerSerial.readline() != "ACK\r\n")
	{
		std::cout << "Warning: controller at " << controllerSerial.getPort() << " did not ACK" << std::endl;
	}

	controllerSerial.write("gType\n");
	if (controllerSerial.readline() != "NES\r\n")
	{
		std::cout << "Warning: contr at " << controllerSerial.getPort() << " is not a NES" << std::endl;
	}
}

bool NES::getData()
{
	//Right, left, down, up, start, select, b,a 
	std::string temp;

	controllerSerial.write("gData\n");
	temp = controllerSerial.readline();

	rawData = std::stoi(temp);

	bA		= rawData & 0x01;
	bB		= rawData & 0x02;
	bSELECT = rawData & 0x04;
	bSTART	= rawData & 0x08;
	bUP		= rawData & 0x10;
	bDOWN	= rawData & 0x20;
	bLEFT	= rawData & 0x40;
	bRIGHT	= rawData & 0x80;
	return 1;
}

bool NES::sendData()
{
	//nothing to send
	return 1;
}

//automatically updates the values and sends a string ready to be transmitted over ZMQ
std::string NES::getRawData()
{
	getData();
	return(std::to_string(rawData)); //easy convert to char bc data is 8 bits wide
}