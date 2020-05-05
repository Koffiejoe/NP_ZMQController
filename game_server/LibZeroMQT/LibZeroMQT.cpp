#include <stdio.h>
#include <zmq.h>

void* context; //Global context, because you only need one !

int main(int argc, char* argv[])
{
	printf("Hello World!\n");
	context = zmq_ctx_new();

	void* pusher = zmq_socket(context, ZMQ_REQ);
	zmq_connect(pusher, "tcp://benternet.pxl-ea-ict.be:24040");

	zmq_send(pusher, "Dennis Merken", 13, 0);

	//ALL MAGIC HAPPENS HERE
	printf("Magic!\n");
	char buffer[20];
	zmq_recv(pusher, buffer, 20, 0);
	printf("Received:  %s\n", buffer);
	zmq_close(pusher);
	zmq_ctx_shutdown(context); //optional for cleaning lady order (get ready you lzy fck)
	zmq_ctx_term(context); //cleaning lady goes to work
	printf("KTNXBYE!\n");
	return 0;
}