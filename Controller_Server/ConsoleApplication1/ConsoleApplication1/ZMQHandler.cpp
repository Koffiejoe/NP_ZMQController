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

}
int ZMQHandler::send()
{
	if (myController == NULL)
	{
		return -1;
	}
}