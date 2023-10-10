#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"

class Server
{
	private:
	public:
		Server();
		Server(const Server& toCopy);
		~Server();

		Server& operator=(const Server& toAssign);
};

#endif
