#include "ZMQHandler.h"
ZMQHandler::ZMQHandler(char* sub, char* push)
{
	context = zmq_ctx_new();
	pushPtr = zmq_socket(context, ZMQ_PUSH);
	subPtr = zmq_socket(context, ZMQ_SUB);

	zmq_connect(pushPtr, push);
	zmq_connect(subPtr, sub);

	//subscribe to channel of server 
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, "controllerService?>", 20);
}

ZMQHandler::~ZMQHandler()
{
	zmq_close(pushPtr);
	zmq_close(subPtr);
	zmq_ctx_shutdown(context); //optional for cleaning lady order (get ready you l*zy f*ck)
	zmq_ctx_term(context); //cleaning lady goes to work
}

int ZMQHandler::recv()
{
	char buffer[500];					//entire incoming string
	char* commandPtr, command[15];		//command part
	char *argumentsPtr, arguments[100];	//arguments part

	if (myController == NULL)
	{
		return -1;
	}
	zmq_recv(subPtr, buffer, sizeof(buffer), 0);
	//Decode the message received
	std::string commands[3];			//the commands
	char* currPosPtr = buffer;
	char* nextPosPtr = buffer;
	char temp[20];

	for (short i = 0; i < 3; ++i)  //max commands = 3
	{
		currPosPtr = strchr(currPosPtr + 1, '>');							//find where the > character is: shifts up each time
		if (currPosPtr == NULL) { break; }									//for < 3 commands

		nextPosPtr = strchr(currPosPtr + 1, '>');
		if (nextPosPtr == NULL) { break; }									//to not include the last >
		//should add break if memcpy length > sizeof buffer - 1

		memcpy(temp, currPosPtr + 1, (nextPosPtr - currPosPtr) - 1);		//copy the length of the command (excluding the start and end >)
		temp[(nextPosPtr - currPosPtr) - 1] = NULL;							//add terminating NULL
		commands[i] = temp;
	}

}

int ZMQHandler::send()
{
	if (myController == NULL)
	{
		return -1;
	}
}