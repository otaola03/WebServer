#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

# include<iostream>
# include<fstream>
# include<vector>
# include<sstream>
# include<map>
# include "../../includes/colors.h"
#include <sys/socket.h>
#include "FileFinder.hpp"

# include "Location.hpp"
typedef std::vector<Location> locationVector;

enum RequestType {GET, POST, DELETE, UNDEFINED, METHOD_ERROR, LENGTH_ERROR, PATH_ERROR};

class HttpRequest
{
	private:
		RequestType	type;
		std::string	path;
		std::map<std::string, std::string> headers;
		std::string	body;
		Location	location;

		void	saveRequest(const std::string& toProcess);
		void	saveHeaders(const std::string& toProcess);
		void	saveBody(const std::string& toProcess);

	public:
		/* HttpRequest(const std::string& toProcess); */
		HttpRequest(int sockfd, int maxBodySize, locationVector& locations);
		HttpRequest(const HttpRequest& toCopy);
		~HttpRequest();

		bool		isValidRequest() const;
		bool		checkRequest(locationVector& locations);

		void		printRequest();
		void		printHeaders();
		void		printBody();
		int			getType();
		std::string	getPath();
		std::string	getBody();
		std::map<std::string, std::string> getHeaders();

		HttpRequest& operator=(const HttpRequest& toCopy);
};

#endif
