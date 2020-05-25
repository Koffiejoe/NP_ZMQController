#include "NESRecv.h"

NESRecv::NESRecv(const char* sub, const char* push)
{
	//create socket for sending and receiving
	char filterString[] = "controllerService!>";
	context = zmq_ctx_new();
	pushPtr = zmq_socket(context, ZMQ_PUSH);
	subPtr = zmq_socket(context, ZMQ_SUB);

	zmq_connect(pushPtr, push);
	zmq_connect(subPtr, sub);

	//subscribe to channel of server 
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, filterString, sizeof(filterString) - 1);
	getList(); //first update the list
}

NESRecv::~NESRecv()
{
	zmq_close(pushPtr);
	zmq_close(subPtr);
	zmq_ctx_shutdown(context);
	zmq_ctx_term(context);
}

int NESRecv::getList()
{
	char prevFilterString[] = "controllerService!>";
	char currFilterString[] = "controllerService!>serv>gList>";
	char buffer[500];
	zmq_setsockopt(subPtr, ZMQ_UNSUBSCRIBE, prevFilterString, sizeof(prevFilterString) - 1);
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, currFilterString, sizeof(currFilterString) - 1);

	while (zmq_recv(subPtr, buffer, sizeof(buffer), ZMQ_NOBLOCK) != -1) {} //loop till buffer is empty

	zmq_send(pushPtr, "controllerService?>serv>gList>", 31, 0);
	zmq_recv(subPtr, buffer, sizeof(buffer), 0);
	decode(buffer);

	//decode comma separated
	int count = 0;
	contrList.clear();
	contrList.shrink_to_fit();
	contrList.push_back("");
	for (int i = 0; i < commands[2].size(); ++i)
	{
		if (commands[2].at(i) != ',')
		{
			contrList[count] += commands[2].at(i);
		}
		else
		{
			count++;
			contrList.push_back("");
		}
	}

	//set filter back to controllerService!>
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, prevFilterString, sizeof(prevFilterString) - 1);
	zmq_setsockopt(subPtr, ZMQ_UNSUBSCRIBE, currFilterString, sizeof(currFilterString) - 1);

	while (zmq_recv(subPtr, buffer, sizeof(buffer), ZMQ_NOBLOCK) != -1) {} //loop till buffer is empty
	return 0;
}

int NESRecv::updateValues()
{
	char buffer[500];
	uint8_t rawData;
	
	//check if ID is NES
	if (contrList[contrID] != "NES")
	{
		std::cout << "WARNING, contr at ID" << contrID << "is NOT a NES" << std::endl;
		return -1;
	}

	char prevFilterString[] = "controllerService!>";
	std::string currFilterString = "controllerService!>";
	currFilterString += std::to_string(contrID);
	currFilterString += '>';
	zmq_setsockopt(subPtr, ZMQ_UNSUBSCRIBE, prevFilterString, sizeof(prevFilterString) - 1);
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, currFilterString.c_str(), currFilterString.size() - 1);

	while (zmq_recv(subPtr, buffer, sizeof(buffer), ZMQ_NOBLOCK) != -1) {} //loop till buffer is empty

	zmq_recv(subPtr, buffer, sizeof(buffer), 0);
	decode(buffer);

	rawData = std::stoi(commands[1]);
	bA = rawData & 0x01;
	bB = rawData & 0x02;
	bSELECT = rawData & 0x04;
	bSTART = rawData & 0x08;
	bUP = rawData & 0x10;
	bDOWN = rawData & 0x20;
	bLEFT = rawData & 0x40;
	bRIGHT = rawData & 0x80;

	return 0;
}

int NESRecv::setUpdate()
{
	std::string updateString;
	updateString = "controllerService?>contr>";
	updateString += std::to_string(contrID);
	updateString += ">";
	updateString += "sUpdate>";
	updateString += std::to_string(updateSpeed);
	updateString += ">\0";
	return zmq_send(pushPtr, updateString.c_str(), updateString.size(), 0);
	
}

int NESRecv::getUpdate()
{
	return 0; //for now
}

int NESRecv::decode(char* buf)
{
	//Decode the message received
	char* currPosPtr = buf;
	char* nextPosPtr = buf;
	char temp[20];
	commands.clear();
	commands.shrink_to_fit();
	for (short i = 0; i < 4; ++i)  //max commands = 4
	{
		currPosPtr = strchr(currPosPtr + 1, '>');							//find where the > character is: shifts up each time
		if (currPosPtr == NULL) { break; }									//for < 3 commands

		nextPosPtr = strchr(currPosPtr + 1, '>');
		if (nextPosPtr == NULL) { break; }									//to not include the last >
		//should add break if memcpy length > sizeof buffer - 1

		memcpy(temp, currPosPtr + 1, (nextPosPtr - currPosPtr) - 1);		//copy the length of the command (excluding the start and end >)
		temp[(nextPosPtr - currPosPtr) - 1] = NULL;							//add terminating NULL
		commands.push_back("");
		commands[i] = temp;
	}
	return 0;
}