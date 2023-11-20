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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstdlib>

#define C200 "HTTP/1.1 200 OK"
#define C204 "HTTP/1.1 204 No Content"
#define C404 "HTTP/1.1 404 Not Found"
#define C405 "HTTP/1.1 405 Method Not Allowed"


extern char** environ;

class HttpResponse
{
	private:
		std::string msg;

	public:
		HttpResponse(HttpRequest& parser);
		HttpResponse(const HttpResponse& toCopy);
		~HttpResponse();

		std::string getMsg();

		std::string postImage(std::string path, std::string body);
		std::string	getIndex(std::string code, std::string path);
		std::string getImg(std::string path);
		std::string getIco(std::string path);
		std::string getPython(std::string path);
		std::string getPhp(std::string path);
		std::string	getMessage(HttpRequest& parser);
		std::string pythonCgiHandler(std::string script, char **av);
		std::string phpCgiHandler(std::string script, char **av);
		bool	isMethodAllowed(HttpRequest& parser, Location& location);
		bool	fileFinder(const std::string& path, std::string& founDir);

		HttpResponse& operator=(const HttpResponse& toCopy);
};

#endif
