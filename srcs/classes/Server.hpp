#ifndef SERVER_HPP

# define SERVER_HPP



#include "Port.hpp"
#include "Location.hpp"
#include "Client.hpp"
#include "HttpRequest.hpp"

#include "../../includes/templates.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstdlib>

typedef std::map<int, Client*> intClientMap;
typedef std::map<int, Port*> intPortMap;


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

		intPortMap& getPortsList();
		/* void	addPortsToSet(fd_set& portsList); */
		/* void	addPortsToConnectionsList(intConnectionMap& connectionsList); */
		void	addClient(int clientFd, Client* client);

		bool	containsThisPort(int portFd);
		/* bool	containsThisClient(int clientFd); */

		void	addPortsToPortsList(intPortMap& portsList);
		/* void	addPortsToClientsList(intClientMap& clientsList); */

		/* void	addPortsToKq(int kq); */

		void	addPortsToSet(fd_set& portsList);

		Server& operator=(const Server& toAssign);
};

#endif
