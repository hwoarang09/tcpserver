#ifndef __TcpNewConnectionAcceptor__
#define __TcpNewConnectionAcceptor__

#include <pthread.h>

class TcpServerController;
class TcpNewConnectionAcceptor {
	private:
		int accept_fd;
		pthread_t* accept_new_conn_thread;
	public:
		TcpServerController* tcp_ctrlr;

		TcpNewConnectionAcceptor(TcpServerController*);
		~TcpNewConnectionAcceptor();

		void StartTcpNewConectionAcceptorThread();


};
#endif
