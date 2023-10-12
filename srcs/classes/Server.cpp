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
	for (int i = 0; ports[i]; i++)
	{
		Port port(ports[i]);
		portsMap[port.getSockFd()] = port;
	}
}

Server::Server(const Server& toCopy)
{
}

Server::~Server()
{
}

Port&	Server::getPort(const int fd)
{
	portMap::iterator it = portMap.find(fd);
	if (it == portMap.end())
		throw std::Exception();
	return (it->second);
}

Server& Server::operator=(const Server& toAssign)
{
	return *this;
}

