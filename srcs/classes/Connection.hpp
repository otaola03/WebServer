#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../../includes/colors.h"
# include <iostream>
# include <unistd.h>
#include <fcntl.h>
 #include <sys/types.h>
 #include <sys/event.h>
 #include <sys/time.h>


class Connection
{
	protected:
		int		sockfd;
		bool	open;

		struct kevent	evSet;

	public:
		Connection();
		Connection(int sockfd);
		virtual ~Connection() = 0;

		int		getSockFd();
		struct kevent&	getEvSet();
		/* void	setOpen(bool open); */
		void	setSocketNonBlocking(int sockfd);

		void	ev_set();
		void	closeSockFd();
		void	closeSockFd(int kq);

		void	add_event(int kq, int event);
		void	delete_event(int kq, int event);
		void	enable_event(int kq, int event);
		void	disable_event(int kq, int event);
};

#endif
