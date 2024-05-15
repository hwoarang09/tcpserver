#include "TcpServerController.h"


int main(int argc, char** argv) {
	TcpServerController* server1 = new TcpServerController("127.0.0.1", 4000, "Default TCP Server");

	server1->Start();

	//scanf("\n");
	return 0;
}