#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <errno.h>
#include <memory.h>

#include "TcpNewConnectionAcceptor.h"
#include "TcpServerController.h"

TcpNewConnectionAcceptor::TcpNewConnectionAcceptor(TcpServerController* tcp_ctrlr) {

	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		printf("Error WSASTARTup");

	this->accept_fd = socket(AF_INET, SOCK_STREAM ,IPPROTO_TCP);
	
	if (this->accept_fd < 0) {
		printf("Error : Could not create Accept Fd! %d\n", this->accept_fd);
		printf("Error in socket creation: %d\n", errno);
		exit(0);
	}
	
	this->accept_new_conn_thread = (pthread_t*)calloc(1, sizeof(pthread_t));
	this->tcp_ctrlr = tcp_ctrlr;
}

TcpNewConnectionAcceptor::~TcpNewConnectionAcceptor() {

}

void TcpNewConnectionAcceptor::StartTcpNewConectionAcceptorThread() {

}