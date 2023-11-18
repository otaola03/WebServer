#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

#include "Server.hpp"
#include "Connection.hpp"
#include "Kqueue.hpp"


typedef std::vector<Server*> serverVector;
typedef std::map<int, Port*> intPortMap;
typedef std::map<int, Client*> intClientMap;

class WebServer
{
	private:
		Kqueue kq;
		serverVector	serversList;
		intPortMap			ports;

		Server*	getServerFromPort(int portFd);

	public:
		WebServer();
		WebServer(const WebServer& toCopy);
		~WebServer();

		void	serverLoop();
		bool	isAPort(int fd);
		bool	acceptNewClient(int fd);
		void	deleteClient(int fd);

		WebServer& operator=(const WebServer& toAssign);
};

#endif
