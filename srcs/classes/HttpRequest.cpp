#include "HttpRequest.hpp" 

// HttpRequest::HttpRequest()
// {
// }

bool	HttpRequest::isChunked()
{
	return chunked;
}

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


/* HttpRequest::HttpRequest(const std::string& toProcess) */
/* { */
/* 	saveRequest(toProcess); */
/* 	saveHeaders(toProcess); */
/* 	if (type == POST) */
/* 		saveBody(toProcess); */
/* } */


static bool	isValidType(RequestType& type, Location& location)
{
	if (type == GET && location.isGET())
		return true;
	else if (type == POST && location.isPOST())
		return true;
	else if (type == DELETE && location.isDELETE())
		return true;
	return false;
}

static bool	isASlashLocation(const std::string& requestedPath, const std::string& rootDir, const std::string& path)
{
	std::string filePath;
	std::string fileName;
	if (path == "/")
		fileName = "index.html";
	else
		fileName = requestedPath;
	if (FileFinder::fileFinder(fileName, filePath, rootDir))
		return true;
	return false;
}

static bool	isValidPath(const std::string& path, std::string locationPath)
{
	size_t pos = path.find('/', 1);
	std::string temp = path.substr(0, pos);
	if (locationPath == temp)
		return true;
	return false;
}

bool	HttpRequest::checkRequest(locationVector& locations)
{
	std::string file;
	for (locationVector::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (it->getPath() == "/"){
			if (isValidType(type, *it))
			{
				location = *it;
				return (true);
			}
			else
				return (type = METHOD_ERROR, false);
		}
		else if (isValidPath(path, it->getPath()))
		{
			if (isValidType(type, *it))
			{
				location = *it;
				path = path.substr(it->getPath().length());
				if (path.empty())
					path = "/";
				return (true);
			}
			else
				return (type = METHOD_ERROR, false);
		}
	}
	if (isASlashLocation(this->path.substr(1), location.getRoot(), path))
		return true;
	type = PATH_ERROR;
	return false;
}

static bool	checkNumBytes(int numbytes, RequestType& type, int sockfd)
{
	if (numbytes <= 0)
	{
		if (numbytes == 0)
		{
			std::cout << "selectserver: socket "<< sockfd << " hung up\n";
			type = UNDEFINED;
			return false;
		}
		if (numbytes == -1)
		{
 			perror("recv");
			// type = UNDEFINED;
			// return false;
		}
		if (numbytes == EWOULDBLOCK)
		{
			type = UNDEFINED;
			return false;
		}
	}
	return true;
}

void HttpRequest::refererCheck(std::map<std::string, std::string> headers, locationVector& locations)
{
	std::string referer = headers["Referer"];
	size_t lastSlashPos = referer.rfind('/');

	if (lastSlashPos != std::string::npos)
		referer = referer.substr(lastSlashPos + 1);
	for (locationVector::iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (isValidPath("/" + referer, it->getPath()))
		{
			if (isValidType(type, *it))
				location = *it;
			else
				type = METHOD_ERROR;
			return;
		}
	}
}

// HttpRequest::HttpRequest(int sockfd, int maxBodySize, locationVector& locations)
// {
// 	char buf[1025];
// 	std::string recvData;
// 	int numbytes = 1024;
// 	int bytesRecived = 0;

// 	while (numbytes == 1024)
// 	{
// 		numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0);
// 		if (!checkNumBytes(numbytes, type, sockfd))
// 			return ;

// 		std::string readData(buf, numbytes);
// 		bytesRecived += numbytes;
// 		recvData += readData;;

// 		if (readData.find("HTTP/1.1"))
// 		{
// 			saveRequest(recvData);
// 			if (!checkRequest(locations))
// 				return ;
// 		}

// 		if (bytesRecived > maxBodySize)
// 		{
// 			type = LENGTH_ERROR;
// 			std::cout << RED << "Error: MAXBODY_SIZE\n" << WHITE;
// 			return ;
// 		}
// 	}
// 	saveHeaders(recvData);
// 	if (type == DELETE)
// 		refererCheck(headers, locations);
// 	if (type == POST)
// 		saveBody(recvData);
// }

int	HttpRequest::headCheck(const std::string& toProcess, locationVector& locations)
{
	std::string request = toProcess.substr(0, toProcess.find('\n'));

	std::vector<std::string> temps;
	std::istringstream iss(request);
	for (std::string temp; iss >> temp;) {
		temps.push_back(temp);
	}
	if (temps.size() != 3){
		std::cerr << "Error: Bad request\n";
		return 1;
	}
	if (temps[2].find("HTTP/") == std::string::npos){
		std::cerr << "Error: Bad Request\n";
		return 1;
	}
	else{
		size_t pos = temps[2].find("HTTP/");
		if (pos == std::string::npos){
			std::cerr << "Error: HTTP version\n";
			return 2;
		}
		if (temps[2].length() < 8){
			std::cerr << "Error: HTTP version\n";
			return 2;
		}
		std::string version = temps[2].substr(pos + 5, 3);
		if (version != "1.1"){
			std::cerr << "Error: HTTP version\n";
			return 2;
		}
	}
	if (temps[0] == "GET")
		type = GET;
	else if (temps[0] == "POST")
		type = POST;
	else if (temps[0] == "DELETE")
		type = DELETE;
	else{
		type = UNDEFINED;
		return 0;
	}
	path = temps[1];
	checkRequest(locations);
	return 0;
}

int	HttpRequest::recvData(int sockfd, int maxBodySize, locationVector& locations)
{
	int bytes = 1023;
	char buf[bytes + 1];
	std::string recvData;
	int numbytes = bytes;
	static int bytesRecived;

	while (numbytes == bytes)
	{
		numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0);
		if (!checkNumBytes(numbytes, type, sockfd))
			return UNDEFINED;
		recvData += std::string(buf, numbytes);
		/* std::cout << recvData << "\n"; */

		if (recvData.find("\r\n\r\n") != std::string::npos && type == UNRECIVED)
		{
			int error_int = headCheck(recvData, locations);
			if (error_int == 1)
			{
				type = BAD_REQUEST;
				return BAD_REQUEST;
			}
			else if (error_int == 2)
			{
				type = HTTP_VERSION_ERROR;
				return HTTP_VERSION_ERROR;
			}
			saveHeaders(recvData);
			bytesRecived = 0;
			if (headers["Transfer-Encoding"] != " chunked")
				bytesRecived = recvData.length() - (recvData.find("\r\n\r\n") + 4);
		}
		else
			bytesRecived += numbytes;

		if (headers["Transfer-Encoding"] == " chunked")
		{
			if (recvData.find("0\r\n\r\n") != std::string::npos){
				body += recvData.substr(0, recvData.find("0\r\n\r\n") - 2);
				saveBody(body);
				return (chunked = false, 0);
			}
			if (bytesRecived > maxBodySize){
				type = LENGTH_ERROR;
				std::cout << RED << "Error: MAXBODY_SIZE\n" << WHITE;
				chunked = false;
				return -1;
			}
			if (recvData.find("\r\n\r\n") != std::string::npos)
				body += recvData;
			else{
				// std::cout << "----------------------------\n" << recvData << "\n";
				size_t pos = recvData.find("\r\n") + 2;
				if (pos != std::string::npos && pos != 1)
					recvData = recvData.substr(pos);
				// std::cout << "+++++++++++++++++++++++++++++\n" << recvData << "\n\n\n";
				body += recvData;
			}
			// send(sockfd, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
			chunked = true;
			return CHUNKED;
		}
	}
	if (bytesRecived > maxBodySize){
		type = LENGTH_ERROR;
		std::cout << RED << "Error: MAXBODY_SIZE\n" << WHITE;
		return -1;
	}
	if (type == DELETE)
		refererCheck(headers, locations);
	if (type == POST)
		saveBody(recvData);
	/* std::cout << body << "\n"; */
	return 0;
}


HttpRequest::HttpRequest(int sockfd, int maxBodySize, locationVector& locations): type(UNRECIVED), chunked(false)
{
	recvData(sockfd, maxBodySize, locations);
	/* exit(-1); */

/* 	int bytes = 1023; */
/* 	char buf[bytes + 1]; */
/* 	std::string recvData; */
/* 	int numbytes = bytes; */
/* 	int bytesRecived = 0; */

/* 	while (recvData.find("\r\n\r\n") == std::string::npos && numbytes == bytes) */
/* 	{ */
/* 		numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0); */
/* 		if (!checkNumBytes(numbytes, type, sockfd)) */
/* 			return ; */
/* 		recvData += std::string(buf, numbytes); */
/* 	} */
/* 	int error_int = headCheck(recvData, locations); */
/* 	if (error_int == 1){ */
/* 		type = BAD_REQUEST; */
/* 		return ; */
/* 	} */
/* 	else if (error_int == 2){ */
/* 		type = HTTP_VERSION_ERROR; */
/* 		return ; */
/* 	} */
/* 	while (numbytes == bytes) */
/* 	{ */
/* 		numbytes = recv(sockfd, buf, sizeof(buf) - 1, 0); */
/* 		if (!checkNumBytes(numbytes, type, sockfd)) */
/* 			return ; */
/* 		recvData += std::string(buf, numbytes); */
/* 		bytesRecived += numbytes; */
/* 		if (bytesRecived > maxBodySize){ */
/* 			type = LENGTH_ERROR; */
/* 			std::cout << RED << "Error: MAXBODY_SIZE\n" << WHITE; */
/* 			return ; */
/* 		} */
/* 	} */
/* 	saveHeaders(recvData); */
/* 	if (type == DELETE) */
/* 		refererCheck(headers, locations); */
/* 	if (type == POST) */
	/* saveBody(recvData); */
}

HttpRequest::HttpRequest(const HttpRequest& toCopy)
{
	(void)toCopy;
}

HttpRequest::~HttpRequest()
{
}

Location	HttpRequest::getLocation() const {return (this->location);}

bool	HttpRequest::isValidRequest() const {return (type == GET || type == POST || type == DELETE);}

bool	HttpRequest::isUnfinishedRequest() const {return (type == METHOD_ERROR || type == LENGTH_ERROR || type == PATH_ERROR);}

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
