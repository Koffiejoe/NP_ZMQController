#include <zmq.h>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <string>
#include <time.h>
using namespace std;

int main(int argc, char* argv[])
{
	void* pushPtr, * subPtr, * context;
	char current;
	char up1, down1, up2, down2;
	up1 = 'z'; up2 = 'i';
	down1 = 's'; down2 = 'k';
	context = zmq_ctx_new();
	pushPtr = zmq_socket(context, ZMQ_PUSH);
	subPtr = zmq_socket(context, ZMQ_SUB);
	context = zmq_ctx_new();
	zmq_connect(pushPtr, "tcp://benternet.pxl-ea-ict.be:24041");
	zmq_connect(subPtr, "tcp://benternet.pxl-ea-ict.be:24042");

	//subscribe to channel of server 
	zmq_setsockopt(subPtr, ZMQ_SUBSCRIBE, "bart>", 5);
	string commands[4];			//the commands
	char buffer[265];
	//Decode the message received
	char* currPosPtr = buffer;
	char* nextPosPtr = buffer;
	char temp[20];

	//zmq_send(pushPtr, "bart>1>join>links>", 18, 0);
	//zmq_send(pushPtr, "bart>2>join>rechts>", 19, 0);

	while (1)
	{
		//ZMQ comm.
		for (short i = 0; i < 4; ++i)  //max commands = 4
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

		if (commands[1] == "ballup")
			zmq_send(pushPtr, "bart>1>up2>", 11, 0);
			
		if (commands[1] == "balldown")
			zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	//random movements
		/*srand((unsigned)time(0));
		int result = 1 + (rand() % 3);
		printf("%d \n", result);
		switch (result) {
		case 1:
			zmq_send(pushPtr, "bart>1>up2>", 11, 0);
			break;
		case 2:
			zmq_send(pushPtr, "bart>1>down2>", 13, 0);
			break;
		case 3:
			break;
		default:
			printf("Error");
			break;
		}
	//pattern
	zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	zmq_send(pushPtr, "bart>1>down2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);
	zmq_send(pushPtr, "bart>1>up2>", 13, 0);*/
	}
}