#ifndef CONNECTION_HPP

# define CONNECTION_HPP

# include <iostream>
# include <unistd.h>


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

		void	closeSockFd();
};

#endif
