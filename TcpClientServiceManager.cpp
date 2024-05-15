#include "TcpNewConnectionAcceptor.h"
#include "TcpClientServiceManager.h"
#include "TcpClient.h"

TcpClientServiceManager::TcpClientServiceManager(TcpServerController* tcp_ctrlr) {
	this->tcp_ctrlr = tcp_ctrlr;
	
}

TcpClientServiceManager::~TcpClientServiceManager() {

}

void TcpClientServiceManager::StartTcpClientServiceManagerThread() {

}

void TcpClientServiceManager::ClientFDStartListen(TcpClient* tcp_client) {

}