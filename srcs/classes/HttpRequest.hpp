#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

# include<iostream>
# include<fstream>
# include<vector>
# include<sstream>
# include<map>
# include "../../includes/colors.h"

enum RequestType {GET, POST, DELETE, UNDEFINED};

class HttpRequest
{
	private:
		RequestType	type;
		std::string	path;
		std::map<std::string, std::string> headers;
		std::string	body;

		void	saveRequest(const std::string& toProcess);
		void	saveHeaders(const std::string& toProcess);
		void	saveBody(const std::string& toProcess);

	public:
		HttpRequest(const std::string& toProcess);
		HttpRequest(const HttpRequest& toCopy);
		~HttpRequest();

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
