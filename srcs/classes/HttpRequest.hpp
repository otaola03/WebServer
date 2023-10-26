#ifndef HTTPREQUEST_HPP

# define HTTPREQUEST_HPP

# include<iostream>
# include<vector>
# include<sstream>
# include<map>

enum RequestType {GET, POST, DELETE, UNDEFINED};

class HttpRequest
{
	private:
		std::map<std::string, std::string> headers;
		RequestType	type;
		std::string	path;
		std::string	body;

		HttpRequest();
		void	saveRequest(const std::string& toProcess);
		void	saveHeaders(const std::string& toProcess);
		void	saveBody(char* toProcess);

	public:
		HttpRequest(char* toProcess);
		HttpRequest(const HttpRequest& toCopy);
		~HttpRequest();

		void		printRequest();
		void		printHeaders();
		void		printBody();
		int			getType();
		std::string	getPath();
		std::string	getBody();

		HttpRequest& operator=(const HttpRequest& toCopy);
};

#endif
