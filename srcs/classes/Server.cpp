#include "Server.hpp"

Server::Server()
{
}

Server::Server(\
	const std::string& name, \
	const std::string& root, \
	const intVector& ports, \
	const intCharMap& errorPages, \
	const locationVector& locations\
) : name(name), root(root), errorPages(errorPages), locations(locations)
{
	for (int i = 0; i != (int)ports.size(); i++)
	{
		Port *port = new Port(ports[i]);
		fdMax = port->getSockFd();
		port->activatePort();
		fdPortsList[port->getSockFd()] = port;
	}
	std::cout << "fdMax: " << fdMax << "\n";
}

Server::Server(const Server& toCopy)
{
	(void)toCopy;
}

Server::~Server()
{
}

Port&	Server::getPort(const int fd)
{
	intPortMap::iterator it = fdPortsList.find(fd);
	if (it == fdPortsList.end())
		throw std::exception();
	return *(it->second);
}

void	Server::addPortsToSet(fd_set& portsFdSet)
{
	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it)
		FD_SET(it->first, &portsFdSet);
}

Server& Server::operator=(const Server& toAssign)
{
	(void)toAssign;
	return *this;
}

