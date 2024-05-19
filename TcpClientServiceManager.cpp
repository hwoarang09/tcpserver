#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <assert.h>
#include <errno.h>
#include <WinSock2.h>
#include <windows.h>

#include "TcpClientServiceManager.h"
#include "TcpServerController.h"
#include "TcpClient.h"
#include "network_utils.h"
#pragma comment(lib, "Ws2_32.lib")


#define TCP_CLIENT_RECV_BUFFER_SIZE 1024
char client_recv_buffer[TCP_CLIENT_RECV_BUFFER_SIZE];

TcpClientServiceManager::TcpClientServiceManager(TcpServerController* tcp_ctrlr) {
	this->tcp_ctrlr = tcp_ctrlr;
	this->max_fd = 0;
	FD_ZERO(&this->active_fd_set);
	FD_ZERO(&this->backup_fd_set);
	this->client_svc_mgr_thread = (pthread_t*)calloc(1, sizeof(pthread_t));
	
}

TcpClientServiceManager::~TcpClientServiceManager() {

}

void TcpClientServiceManager::StartTcpClientServiceManagerThreadInternal() {
	int rcv_bytes;
	TcpClient* tcp_client, * next_tcp_client;
	struct sockaddr_in client_addr;
	std::list<TcpClient*>::iterator it;

	int addr_len = sizeof(client_addr);

	this->max_fd = this->GetMaxFd();
	FD_ZERO(&this->backup_fd_set);
	this->CopyClientFDtoFDSet(& this->backup_fd_set);

	while (true) {
		memcpy(&this->active_fd_set, &this->backup_fd_set, sizeof(fd_set));
		select(this->max_fd + 1, &this->active_fd_set, 0, 0, 0);

		
		for (it = this->tcp_client_db.begin();
			it != this->tcp_client_db.end();
			) {
		
			tcp_client = *it;
			++it;

			if (FD_ISSET(tcp_client->comm_fd, &this->active_fd_set)) {
				rcv_bytes = recvfrom(tcp_client->comm_fd,
					client_recv_buffer,
					TCP_CLIENT_RECV_BUFFER_SIZE,
					0,
					(struct sockaddr*)&client_addr, &addr_len);

				if (this->tcp_ctrlr->client_msg_recvd) {
					this->tcp_ctrlr->client_msg_recvd(this->tcp_ctrlr, tcp_client,
						client_recv_buffer, rcv_bytes);
				}
			}
		}
		
	}
}

void* tcp_client_svc_manager_thread_fn(void *arg) {
	TcpClientServiceManager* svc_mgr = (TcpClientServiceManager*)arg;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	svc_mgr->StartTcpClientServiceManagerThreadInternal();
	return NULL;
}

void TcpClientServiceManager::StartTcpClientServiceManagerThread() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(this->client_svc_mgr_thread, &attr,
		tcp_client_svc_manager_thread_fn, (void*)this);
	printf("Service started : TcpClientServiceManagerThread\n");
}

void TcpClientServiceManager::StopTcpClientServiceManagerThread() {
	pthread_cancel(*this->client_svc_mgr_thread);
	//TerminateThread(*this->client_svc_mgr_thread, 0);
	pthread_join(*this->client_svc_mgr_thread, NULL);
	free(this->client_svc_mgr_thread);
	this->client_svc_mgr_thread = NULL;
}


void TcpClientServiceManager::ClientFDStartListen(TcpClient* tcp_client) {

}

void
TcpClientServiceManager::CopyClientFDtoFDSet(fd_set* fdset) {

	TcpClient* tcp_client;
	std::list<TcpClient*>::iterator it;

	for (it = this->tcp_client_db.begin();
		it != this->tcp_client_db.end();
		++it) {

		tcp_client = *it;
		FD_SET(tcp_client->comm_fd, fdset);
	}
}

int
TcpClientServiceManager::GetMaxFd() {

	int max_fd_lcl = 0;

	TcpClient* tcp_client;
	std::list<TcpClient*>::iterator it;

	for (it = this->tcp_client_db.begin(); it != this->tcp_client_db.end(); ++it) {

		tcp_client = *it;
		if (tcp_client->comm_fd > max_fd_lcl) {
			max_fd_lcl = tcp_client->comm_fd;
		}
	}
	return max_fd_lcl;
}