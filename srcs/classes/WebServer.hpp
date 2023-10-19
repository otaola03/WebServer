#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

#include "Server.hpp"
#include "Connection.hpp"

typedef std::vector<Server*> serverVector;
typedef std::map<int, Port*> intPortMap;
typedef std::map<int, Connection*> intConnectionMap;

class WebServer
{
	private:
		fd_set	portsList;
		fd_set	socketList;
		serverVector	serversList;
		intConnectionMap	connectionsList;

		Server*	getServerFromPort(int portFd);
		Server*	getServerFromClient(int clientFd);

	public:
		WebServer();
		WebServer(const WebServer& toCopy);
		~WebServer();

		void	serverLoop();

		WebServer& operator=(const WebServer& toAssign);
};

#endif
