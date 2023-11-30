#ifndef HTTRESPONSE_HPP

# define HTTRESPONSE_HPP

#include "HttpRequest.hpp"
#include "Location.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <stdio.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sys/wait.h>


#define C200 "HTTP/1.1 200 OK"
#define C204 "HTTP/1.1 204 No Content"
#define C302 "HTTP/1.1 302 Found"
#define C307 "HTTP/1.1 307 Temporary Redirect"
#define C400 "HTTP/1.1 400 Bad Request"
#define C401 "HTTP/1.1 401 Unauthorized"
#define C403 "HTTP/1.1 403 Forbidden"
#define C404 "HTTP/1.1 404 Not Found"
#define C405 "HTTP/1.1 405 Method Not Allowed"
#define C411 "HTTP/1.1 411 Length Required"
#define C500 "HTTP/1.1 500 Internal Server Error"
#define C501 "HTTP/1.1 501 Not Implemented"
#define C505 "HTTP/1.1 505 HTTP Version Not Supported"
#define C413 "HTTP/1.1 413 Content Too Large"
#define C504 "HTTP/1.1 504 Gateway Timeout"


extern char** environ;

class HttpResponse
{
	private:
		std::string msg;

	public:
		HttpResponse(HttpRequest& parser, std::map<int, std::string> errors);
		HttpResponse(const HttpResponse& toCopy);
		~HttpResponse();

		std::string getMsg();
		std::string redirector(std::string page);

		std::string postImage(std::string path, std::string body, std::map<std::string, std::string> headers, std::string destination);
		std::string	getIndex(std::string code, std::string path);
		std::string getImg(std::string path);
		std::string getIco(std::string path);
		std::string getPython(std::string path);
		std::string getPhp(std::string path);
		std::string	getMessage(HttpRequest& parser, std::map<int, std::string> errors);
		std::string pythonCgiHandler(std::string script, char **av);
		std::string phpCgiHandler(std::string script, char **av);
		std::string returner(HttpRequest& parser, std::map<int, std::string> errors, std::string varPath);
		bool	isMethodAllowed(HttpRequest& parser, Location& location);

		HttpResponse& operator=(const HttpResponse& toCopy);
};

#endif
