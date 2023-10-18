#ifndef PORT_HPP

#define PORT_HPP

#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <sstream>
 
#include "Connection.hpp"

#define BACKLOG 5

class Port : public Connection
{
	private:
		int	port;

	public:
		Port(const int port);
		Port(const Port& toCopy);
		~Port();


		void	activatePort();
		int		acceptConnection();

		Port& operator=(const Port& toAssign);
};


#endif
