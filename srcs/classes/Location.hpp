#ifndef LOCATION_HPP

# define LOCATION_HPP

#include <iostream>
#include <vector>

#include <sys/stat.h>

#include <string>
#include <dirent.h>
#include <fstream>
using namespace std;


using namespace std;
class Location
{
	private://mejor const
		std::string path;
		std::string	root;
		std::string	index;
		std::string	allowed_methods;
		bool	GET;
		bool	POST;
		bool	DELETE;
		bool	autoindex;
		std::string	redirection;
		std::string	destination;
		std::string	cgi_destinaation;

		// Location();
		bool	isAllowed(const std::string	&method);

		void		check();


	public:
		Location();

		Location(\
				const std::string	path, \
				const std::string	root, \
				const bool			GET, \
				const bool			POST, \
				const bool			DELETE, \
				const bool			autoindex, \
				std::string			redirection, \
				std::string			destination \
		);
		Location(\
			const std::string	&path, \
			const std::string	&root, \
			const std::string	&index, \
			const std::string	&allowed_methods, \
			const std::string	&autoindex, \
			const std::string	&redirection, \
			const std::string	&destination, \
			const std::string	&cgi_destinaation \
		);
		/* Location(const Location& toCopy); */
		~Location();
		bool		isGET() const;
		bool		isPOST() const;
		bool		isDELETE() const;

		Location& operator=(const Location& toCopy);
		// Getters
		std::string getPath() const {
			return path;
		}

		std::string getRoot() const {
			return root;
		}

		std::string getIndex() const {
			return index;
		}

		bool getAutoindex() const {
			return autoindex;
		}

		std::string getRedirection() const {
			return redirection;
		}

		std::string getDestination() const {
			return destination;
		}

		std::string getCGIDestination() const {
			return cgi_destinaation;
		}
};

#endif
