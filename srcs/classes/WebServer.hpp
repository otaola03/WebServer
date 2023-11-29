#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

#include "Server.hpp"
#include "Connection.hpp"
#include "Config.hpp"
#include "Kqueue.hpp"
#include "HttpResponse.hpp"


typedef std::vector<Server*> serverVector;
typedef std::map<int, Port*> intPortMap;
typedef std::map<int, std::string> intStrMap;
typedef std::map<int, Server*> intServerMap;
typedef std::map<int, HttpRequest*> intRequestMap;

class WebServer
{
	private:
		Kqueue kq;
		serverVector	serversList;
		intPortMap			ports;
		intStrMap			clientsData;
		intServerMap			clientsServers;
		intRequestMap		clientsRequests;

		static bool	isWebServerActivated;
		Server*	getServerFromPort(int portFd);

		WebServer();


	public:
		WebServer(const WebServer& toCopy);
		WebServer(const Config& config);
		~WebServer();

		void	serverLoop();
		bool	isAPort(int fd);
		bool	acceptNewClient(int fd);
		void	deleteClient(int fd);
		static void	signalHandler(int signal);

		WebServer& operator=(const WebServer& toAssign);
};

#endif
