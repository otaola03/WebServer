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
		/* int kq; */
		Kqueue kq;
		/* fd_set	portsList; */
		/* fd_set	socketList; */
		serverVector	serversList;
		/* intConnectionMap	connectionsList; */
		intPortMap			ports;
		intClientMap		clients;


		Server*	getServerFromPort(int portFd);
		Server*	getServerFromClient(int clientFd);

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
