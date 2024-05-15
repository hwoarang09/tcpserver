#include "TcpNewConnectionAcceptor.h"
#include "TcpClientServiceManager.h"

TcpClientServiceManager::TcpClientServiceManager(TcpServerController* tcp_ctrlr) {
	this->tcp_ctrlr = tcp_ctrlr;
	
}

TcpClientServiceManager::~TcpClientServiceManager() {

}

void TcpClientServiceManager::StartTcpClientServiceManagerThread() {

}