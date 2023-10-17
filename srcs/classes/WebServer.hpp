#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

#include "Server.hpp"

typedef std::vector<Server*> serverVector;
typedef std::map<int, Port*> intPortMap;

class WebServer
{
	private:
		fd_set	portsList;
		fd_set	socketList;
		serverVector	serversList;

	public:
		WebServer();
		WebServer(const WebServer& toCopy);
		~WebServer();

		void	serverLoop();
		Server&	getServer(const int fd);
		Port&	getPort(const int fd);

		WebServer& operator=(const WebServer& toAssign);
};

#endif
