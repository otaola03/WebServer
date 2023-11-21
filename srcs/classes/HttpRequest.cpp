#include "HttpRequest.hpp"

// HttpRequest::HttpRequest()
// {
// }

static	RequestType whatTypeIs(const std::string& type)
{
	if (type == "GET")
		return GET;
	else if (type == "POST")
		return POST;
	else if (type == "DELETE")
		return DELETE;
	return UNDEFINED;
}

std::map<std::string, std::string> HttpRequest::getHeaders()
{
	return headers;
}

void	HttpRequest::saveRequest(const std::string& toProcess)
{
	std::string request = toProcess.substr(0, toProcess.find('\n'));

	size_t p = request.find(' ');
	type = whatTypeIs(request.substr(0, p));

	size_t p2 = request.find(' ', p + 1);
	path = request.substr(p + 1, p2 - p);
	path = path.erase(path.size() - 1);
}

/* void	HttpRequest::saveBody(const char* toProcess) */
/* { */
/* 	std::string contentLength; */
/* 	contentLength = std::strstr(toProcess, "content-length: ") ? std::strstr(toProcess, "content-length: ") : std::strstr(toProcess, "Content-Length: "); */
/* 	if (contentLength.empty()) */
/* 		return ; */
/* 	int length = std::atoi(contentLength.c_str() + 16); */
/* 	const char* body_t = std::strstr(toProcess, "\r\n\r\n"); */
/* 	body = std::string(body_t, length); */
/* 	body = body.substr(body.find("\r\n\r\n") + 4); */
/* 	size_t lastLine = body.find_last_of("\n"); */
/* 	body = body.substr(0, lastLine); */
/* } */

void	HttpRequest::saveBody(const std::string& toProcess)
{
	/* std::string auxBody = toProcess.substr(toProcess.find("\r\n\r\n") + 4); */

	/* body = auxBody.substr(auxBody.find("\r\n\r\n")); */
	body = toProcess.substr(toProcess.find("\r\n\r\n") + 4);
}

void	HttpRequest::printBody()
{
	if (body.empty()){
		std::cerr << "No body to print\n";
		return ;
	}
	for (std::string::iterator it = body.begin(); it != body.end(); ++it){
		if (*it == '\n')
			std::cout << "\\n";
		else if (*it == '\r')
			std::cout << "\\r";
		else
			std::cout << *it;
	}
}

std::string	HttpRequest::getBody()
{
	return body;
}

void	HttpRequest::printHeaders()
{
	std::map<std::string, std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); ++it) {
		std::cout << "| " << it->first << ":" << it->second << std::endl;
	}
}

static void	splitRequest(std::vector<std::string>& lines, const std::string& toProcess)
{
	std::istringstream stream(toProcess);
	std::string	line;
	size_t posicion = toProcess.find("\r\n\r\n");

	if (posicion != std::string::npos) {
		std::string subcadena = toProcess.substr(0, posicion);
		std::istringstream ss(subcadena);
		std::string linea;
		while (std::getline(ss, linea, '\n')) {
			lines.push_back(linea);
		}
	} else {
		lines.push_back(toProcess);
	}
}

void	HttpRequest::saveHeaders(const std::string& toProccess)
{
	std::vector<std::string> lines;
	splitRequest(lines, toProccess);

	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		size_t p = it->find(':');
		if (p == std::string::npos)
			continue;
		if (it->find("multipart/form-data") != std::string::npos)
			headers[it->substr(0, p)] = "multipart/form-data";
		else
			headers[it->substr(0, p)] = it->substr(p + 1, it->size() - p - 2);
	}
}


HttpRequest::HttpRequest(const std::string& toProcess)
{
	saveRequest(toProcess);
	saveHeaders(toProcess);
	if (type == POST)
		saveBody(toProcess);
	printRequest();
	printHeaders();
}

HttpRequest::HttpRequest(const HttpRequest& toCopy)
{
	(void)toCopy;
}

HttpRequest::~HttpRequest()
{
}

void	HttpRequest::printRequest()
{
	std::cout << "Request: " << path << std::endl;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& toAssign)
{
	(void)toAssign;
	return *this;
}

int		HttpRequest::getType()
{
	return type;
}

std::string	HttpRequest::getPath()
{
	return path;
}
