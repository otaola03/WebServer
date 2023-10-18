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

void	Client::recvData()
{
	int numbytes;
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
	{
 		perror("recv");
 		exit(1);
	}
}

Client& Client::operator=(const Client& toAssign)
{
	(void)toAssign;
	return *this;
}

