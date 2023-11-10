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

void	Connection::closeSockFd(int kq)
{
	/* struct kevent	auxEvSet; */

	EV_SET(&evSet, sockfd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);
	close(sockfd);
}

void	Connection::setSocketNonBlocking(int sockfd)
{
	int flags;

	if (-1 == (flags = fcntl(sockfd, F_GETFL, 0)))
		flags = 0;
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK))
	{
		perror("fcntl");
		exit(-1);
	}
}

void	Connection::ev_set()
{
	EV_SET(&evSet, sockfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
}

void	Connection::add_event(int kq, int event)
{
	EV_SET(&evSet, sockfd, event, EV_ADD, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);
}

void	Connection::delete_event(int kq, int event)
{
	EV_SET(&evSet, sockfd, event, EV_DELETE, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);
}

void	Connection::enable_event(int kq, int event)
{
	EV_SET(&evSet, sockfd, event, EV_ENABLE, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);
}

void	Connection::disable_event(int kq, int event)
{
	EV_SET(&evSet, sockfd, event, EV_DISABLE, 0, 0, NULL);
	kevent(kq, &evSet, 1, NULL, 0, NULL);
}
