#include <zmq.h>
#include <Windows.h>
#include <conio.h>

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
	while (1)
	{
		if (_kbhit())
		{
			current = _getch();
			if (current == up1)
				zmq_send(pushPtr, "bart>1>up1>", 11, 0);
			if (current == up2)
				zmq_send(pushPtr, "bart>1>up2>", 11, 0);
			if (current == down1)
				zmq_send(pushPtr, "bart>1>down1>", 13, 0);
			if (current == down2)
				zmq_send(pushPtr, "bart>1>down2>", 13, 0);

		}
	}
}