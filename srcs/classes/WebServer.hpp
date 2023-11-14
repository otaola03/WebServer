#ifndef WEBSERVER_HPP

# define WEBSERVER_HPP

#include "Server.hpp"
#include "Config.hpp"

typedef std::vector<Server*> serverVector;
typedef std::map<int, Port*> intPortMap;

class WebServer
{
	private:
		fd_set			portsList;
		fd_set			socketList;
		serverVector	serversList;

		WebServer();

	public:
		WebServer(const WebServer& toCopy);
		WebServer(const Config& config);
		~WebServer();

		void	serverLoop();
		Server&	getServer(const int fd);
		Port&	getPort(const int fd);

		WebServer& operator=(const WebServer& toAssign);
};

#endif
