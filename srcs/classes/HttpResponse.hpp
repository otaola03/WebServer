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
#define C404 "HTTP/1.1 404 Not Found"
#define C405 "HTTP/1.1 405 Method Not Allowed"
#define C413 "HTTP/1.1 413 Payload Too Large"
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

		std::string postImage(std::string path, std::string body, std::map<std::string, std::string> headers);
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
