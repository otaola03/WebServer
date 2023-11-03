#include "Connection.hpp"

Connection::Connection() : open(false) {}

Connection::Connection(int sockfd) : sockfd(sockfd), open(false) {}

Connection::~Connection() {}

int	Connection::getSockFd() {return(sockfd);}

struct kevent&	Connection::getEvSet() {return(evSet);}

/* void	Connection::setOpen(bool _open) {open = _open;} */

void	Connection::closeSockFd()
{
	if (open)
		close(sockfd);
}

void	Connection::setSocketNonBlocking(int sockfd)
{
	int flags;

	if (-1 == (flags = fcntl(sockfd, F_GETFL, 0)))
		flags = 0;
	if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK))
	{
		perror("fcntl");
		exit(-1);
	}
}

void	Connection::ev_set()
{
	EV_SET(&evSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
}
