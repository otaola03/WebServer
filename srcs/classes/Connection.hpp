#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include "../../includes/colors.h"
# include <iostream>
# include <unistd.h>
#include <fcntl.h>


class Connection
{
	protected:
		int		sockfd;
		bool	open;

	public:
		Connection();
		Connection(int sockfd);
		virtual ~Connection() = 0;

		int		getSockFd();
		/* void	setOpen(bool open); */
		void	setSocketNonBlocking(int sockfd);

		void	closeSockFd();
};

#endif
