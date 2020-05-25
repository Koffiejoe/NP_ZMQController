#include "GCRecv.h"

GCRecv::GCRecv(const char* sub, const char* push)
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

GCRecv::~GCRecv()
{
	zmq_close(pushPtr);
	zmq_close(subPtr);
	zmq_ctx_shutdown(context);
	zmq_ctx_term(context);
}

int GCRecv::getList()
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

int GCRecv::updateValues()
{
	char buffer[500];
	uint64_t rawData;

	//check if ID is NES
	if (contrList[contrID] != "GC")
	{
		std::cout << "WARNING, contr at ID" << contrID << "is NOT a GC" << std::endl;
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


	rawData = strtoull(commands[1].c_str(), NULL, 16);

	//stuff all data in vars (yes could be done better but this is copy paste and readable)
	bStart = rawData & 0x1000000000000000;
	bY = rawData & 0x0800000000000000;
	bX = rawData & 0x0400000000000000;
	bB = rawData & 0x0200000000000000;
	bA = rawData & 0x0100000000000000;
	lTr = rawData & 0x0040000000000000;
	rTr = rawData & 0x0020000000000000;
	bZ = rawData & 0x0010000000000000;
	bUp = rawData & 0x0008000000000000;
	bDown = rawData & 0x0004000000000000;
	bRight = rawData & 0x0002000000000000;
	bLeft = rawData & 0x0001000000000000;

	return 0;
}

int GCRecv::setUpdate()
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

int GCRecv::decode(char* buf)
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

int GCRecv::setRumble(bool value)
{
	std::string updateString;
	updateString = "controllerService?>contr>";
	updateString += std::to_string(contrID);
	updateString += ">";
	updateString += "sRumble>";
	updateString += std::to_string(value);
	updateString += ">\0";
	return zmq_send(pushPtr, updateString.c_str(), updateString.size(), 0);
}