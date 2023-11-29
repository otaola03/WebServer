#ifndef SERVER_HPP

# define SERVER_HPP



#include "Port.hpp"
#include "Location.hpp"
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

typedef std::map<int, Port*> intPortMap;

typedef std::map<int, Port*> intPortMap;

extern char** environ;

class Server
{
	private:
		std::string 	name;
		std::string 	root;
		intPortMap		fdPortsList;	//map(fd, Port*)
		intCharMap		errorPages;
		locationVector	locations;
		int 			maxBodySize;

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

		locationVector& getLocations();
		int getMaxBodySize();

		intPortMap& getPortsList();

		bool	containsThisPort(int portFd);

		void	addPortsToPortsList(intPortMap& portsList);

		Server& operator=(const Server& toAssign);
		std::ostream& operator<<(std::ostream& os) const;
		std::map <int, std::string> getErrorPages();
};

#endif
