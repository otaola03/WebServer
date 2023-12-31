#include "Kqueue.hpp"

Kqueue::Kqueue(): totalConnections(0)
{
	kq = kqueue();
}

Kqueue::Kqueue(const Kqueue& toCopy)
{
	(void)toCopy;
}

Kqueue::~Kqueue()
{
}

struct kevent& Kqueue::getEvSet(int index) {return evList[index];}

int	Kqueue::getKq() {return kq;}

static bool manage_event(int kq, int fd, struct kevent& evSet, int type, int option)
{
	EV_SET(&evSet, fd, type, option, 0, 0, NULL);
	if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
		return false;
	return true;
}

bool	Kqueue::addPort(int sockfd)
{
	if (!manage_event(kq, sockfd, evSet, EVFILT_READ, EV_ADD | EV_ENABLE))
		return false;
	return true;
}

int	Kqueue::listenNewEvents()
{
	/* std::cout << "Listening...\n"; */
	return (kevent(kq, NULL, 0, evList, 25, NULL));
}

bool	Kqueue::manageNewConnection(int fd)
{
	if (!manage_event(kq, fd, evSet, EVFILT_READ, EV_ADD | EV_ENABLE))
		return false;

	if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_ADD | EV_DISABLE))
		return false;
	totalConnections++;
	return true;
}

bool	Kqueue::enableWrite(int fd)
{
	if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_ENABLE))
		return false;
	return true;
}

bool	Kqueue::disableRead(int fd)
{
	if (!manage_event(kq, fd, evSet, EVFILT_READ, EV_DISABLE))
		return false;
	return true;
}

bool	Kqueue::manageEndedConnection(int fd)
{
	if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_DISABLE))
		return false;

	if (!manage_event(kq, fd, evSet, EVFILT_READ, EV_DELETE))
		return false;

	if (!manage_event(kq, fd, evSet, EVFILT_WRITE, EV_DELETE))
		return false;
	return true;
}

Kqueue& Kqueue::operator=(const Kqueue& toAssign)
{
	(void)toAssign;
	return *this;
}

