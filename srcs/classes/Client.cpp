#include "Client.hpp"

Client::Client(int sockfd) : Connection(sockfd)
{
}

Client::Client(const Client& toCopy)
{
	(void)toCopy;
}

Client::~Client()
{
	std::cout << "Client " << sockfd << "closed" << "\n";
	closeSockFd();
}

std::string	Client::recvData()
{
	int numbytes;
	char buf[1024];
	std::string recvData;

	if ((numbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0)
	{
		if (numbytes == 0)
			std::cout << RED << "selectserver: socket "<< sockfd << " hung up\n" << WHITE;
		if (numbytes == -1)
		{
 			perror("recv");
 			exit(1);
		}
	}
	buf[numbytes] = '\0';
	recvData += buf;
	return recvData;
}

Client& Client::operator=(const Client& toAssign)
{
	(void)toAssign;
	return *this;
}

