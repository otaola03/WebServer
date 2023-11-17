#ifndef KQUEUE_HPP

# define KQUEUE_HPP

# define MAX_EVENTS 25

# include <iostream>
# include <unistd.h>
#include <fcntl.h>
 #include <sys/types.h>
 #include <sys/event.h>
 #include <sys/time.h>

class Kqueue
{
	private:
		int kq;
		int	totalConnections;
		struct kevent evSet;
		struct kevent evList[MAX_EVENTS];

	public:
		Kqueue();
		Kqueue(const Kqueue& toCopy);
		~Kqueue();

		struct kevent& getEvSet(int index);
		int getKq();

		bool	addPort(int sockfd);
		int	listenNewEvents();

		bool	manageNewConnection(int fd);
		bool	enableWrite(int fd);
		bool	manageEndedConnection(int fd);

		Kqueue& operator=(const Kqueue& toCopy);
};

#endif
