#ifndef SERVER_HPP

# define SERVER_HPP

#include "Port.hpp"
#include "Location.hpp"
#include "Client.hpp"

#include "../../includes/templates.h"

#include <algorithm>
#include <exception>


class Server
{
	private:
		std::string 	name;
		std::string 	root;
		intPortMap		fdPortsList;	//map(fd, Port*)
		intClientMap	fdClientsList;	//map(fd, Client*)
		intCharMap		errorPages;
		locationVector	locations;

		Server();

	public:
		int fdMax;

	public:
		Server(\
				const std::string& name, \
				const std::string& root, \
				const intVector& ports, \
				const intCharMap& errorPages, \
				const locationVector& locations\
		);
		Server(const std::string& name, const std::vector<int> ports);
		Server(const Server& toCopy);
		~Server();

		void	addPortsToSet(fd_set& portsList);
		void	addClient(int clientFd, Client* client);
		bool	containsThisPort(int portFd);
		bool	containsThisClient(int clientFd);

		Server& operator=(const Server& toAssign);
};

#endif
