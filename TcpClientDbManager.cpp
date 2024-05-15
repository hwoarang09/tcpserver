#include "TcpNewConnectionAcceptor.h"
#include "TcpClientDbManager.h"
#include "TcpClient.h"

TcpClientDbManager::TcpClientDbManager(TcpServerController* tcp_ctrlr) {
	this->tcp_ctrlr = tcp_ctrlr;
}

TcpClientDbManager::~TcpClientDbManager() {

}
void TcpClientDbManager::StartTcpClientDbManagerInit() {

}

void TcpClientDbManager::AddClienttoDB(TcpClient* tcp_client) {
	this->tcp_client_db.push_back(tcp_client);
}