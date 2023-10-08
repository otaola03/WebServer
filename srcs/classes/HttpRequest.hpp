#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

# include<iostream>
# include<vector>
# include<sstream>

enum RequestType {GET, POST, DELETE, UNDEFINED};

class HttpRequest
{
	private:
		std::vector< std::pair<std::string, std::string> >	headers;
		RequestType	type;
		std::string	source;

		HttpRequest();
		void	saveRequest(const std::string& toProcess);
		void	saveHeaders(const std::string& toProcess);

	public:
		HttpRequest(const std::string& toProcess);
		HttpRequest(const HttpRequest& toCopy);
		~HttpRequest();

		void	printRequest();

		HttpRequest& operator=(const HttpRequest& toCopy);
};

#endif
