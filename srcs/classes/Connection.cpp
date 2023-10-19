#include "Connection.hpp"

Connection::Connection() : open(false) {}

Connection::Connection(int sockfd) : sockfd(sockfd), open(false) {}

Connection::~Connection() {}

int	Connection::getSockFd() {return(sockfd);}

/* void	Connection::setOpen(bool _open) {open = _open;} */

void	Connection::closeSockFd()
{
	if (open)
		close(sockfd);
}
