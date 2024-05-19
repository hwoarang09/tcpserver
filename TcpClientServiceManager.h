#ifndef __TcpClientServiceManager__
#define __TcpClientServiceManager__


#include <pthread.h>
#include <list>
#include <WinSock2.h>

#define MAX_CLIENT_SUPPORTED 127

class TcpServerController;
class TcpClient;

class TcpClientServiceManager {
private:
	fd_set active_fd_set;
	fd_set backup_fd_set;
	int max_fd;
	pthread_t *client_svc_mgr_thread;
	std::list<TcpClient*>tcp_client_db;

public:
	TcpServerController* tcp_ctrlr;
	TcpClientServiceManager(TcpServerController*);
	~TcpClientServiceManager();
	void StartTcpClientServiceManagerThread();
	void ClientFDStartListen(TcpClient* tcp_client);
	void StartTcpClientServiceManagerThreadInternal();
	int GetMaxFd();
	void CopyClientFDtoFDSet(fd_set* fdset);
	void StopTcpClientServiceManagerThread();
};
#endif

