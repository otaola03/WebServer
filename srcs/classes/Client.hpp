#ifndef CLIENT_HPP

#define CLIENT_HPP

#include "Connection.hpp"

class Client : public Connection
{
	private:
	public:
		Client(int sockfd);
		Client(const Client& toCopy);
		~Client();

		void	recvData(void* buf, );
		void	sendData(const void* buf);

		Client& operator=(const Client& toCopy);
};

#endif
