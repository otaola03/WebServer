#include "Server.hpp"

Server::Server()
{
}

Server::Server(\
	const std::string& name, \
	const std::string& root, \
	const intVector& ports, \
	const intCharMap& errorPages, \
	const locationVector& locations\
) : name(name), root(root), errorPages(errorPages), locations(locations)
{
	for (int i = 0; i != (int)ports.size(); i++)
	{
		Port *port = new Port(ports[i]);
		fdMax = port->getSockFd();
		port->activatePort();
		fdPortsList[port->getSockFd()] = port;
	}
	std::cout << "fdMax: " << fdMax << "\n";
}

Server::Server(const Server& toCopy)
{
	(void)toCopy;
}

Server::~Server()
{
}

Port&	Server::getPort(const int fd)
{
	intPortMap::iterator it = fdPortsList.find(fd);
	if (it == fdPortsList.end())
		throw std::exception();
	return *(it->second);
}

void	Server::addPortsToSet(fd_set& portsFdSet)
{
	for (intPortMap::iterator it = fdPortsList.begin(); it != fdPortsList.end(); ++it)
		FD_SET(it->first, &portsFdSet);
}

Server& Server::operator=(const Server& toAssign)
{
	(void)toAssign;
	return *this;
}

bool Server::fileFinder(const std::string& path)
{
	DIR *dir;
	struct dirent *entry;
	dir = opendir("./resources/images");
	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == path){
				closedir(dir);
				return (true);
			}
		}
		closedir(dir);
	}
	dir = opendir("./resources/html");
	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == path){
				closedir(dir);
				return (true);
			}
		}
		closedir(dir);
	}
	dir = opendir("./cgi-bin");
	if (dir) {
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == path){
				closedir(dir);
				return (true);
			}
		}
		closedir(dir);
	}
	return (false);
}

std::string Server::getImg(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: image/png");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/images/" + path;
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string Server::getPython(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: script/python");
	msg.append("\nContent-Length: ");
	std::string html_name = "./cgi-bin/" + path;
	std::ifstream file(html_name.c_str());
	msg.append(std::to_string(pythonCgiHandler(path, environ).length()));
	msg.append("\n\n");
	msg.append(pythonCgiHandler(path, environ));
	file.close();
	std::cout << "MSG = " << msg << std::endl;
	return msg;
}

std::string Server::getIndex(std::string code, std::string path){
	std::string msg = code;
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/html/" + path;
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string Server::postImage(std::string path, std::string body){

	(void)path;
	std::string body_content = body;
	std::cout << "BODY = [" << body[0] << "]\n";
	if (body[1] == '-')
		body_content = body.substr(body.find("\r\n\r\n") + 4);
	std::string msg = "HTTP/1.1 200 OK";
	DIR *dir;
	int i = 0;
	struct dirent *entry;
	dir = opendir("./bin");
	if (dir){
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == std::to_string(i) + ".png"){
				i++;
			}
		}
		std::ofstream imageFile("./bin/" + std::to_string(i) + ".png", std::ios::binary);
		imageFile.write(body_content.c_str(), body_content.length());
		imageFile.close();
		closedir(dir);
	}
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = "./resources/html/post.html";
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Fatal" << std::endl;
	}
	return msg;
}

std::string Server::getMessage(HttpRequest& parser)
{
	if (parser.getType() == GET){
		// if (this->locations[0].isGET() == false)
		// 	return (getIndex(C405, "405.html"));
		// else{
			if (parser.getPath() == "/")
				return (getIndex(C200, "index.html"));
			else if (fileFinder(parser.getPath().substr(1)) && parser.getPath().find(".html") != std::string::npos)
				return (getIndex(C200, parser.getPath()));
			else if (fileFinder(parser.getPath().substr(1)) && parser.getPath().find(".png") != std::string::npos)
				return (getImg(parser.getPath()));
			else if (fileFinder(parser.getPath().substr(1)) && parser.getPath().find(".py") != std::string::npos)
				return (getPython(parser.getPath()));
			else
				return (getIndex(C404, "404.html"));
		// }
	}
	else if (parser.getType() == POST){
		// if (this->locations[0].isPOST() == false)
		// 	return (getIndex(C405, "405.html"));
			std::ofstream file ("tempFile");
			file << parser.getBody();
			// std::cerr << "BODY = [" << parser.getBody() << "]\n";
			return (postImage(parser.getPath(), parser.getBody()));
		// else{
		// }
	}
	return "";
}

std::string Server::pythonCgiHandler(std::string script, char **av)
{
	(void)av;
	std::string pythonScript = "cgi-bin" + script;
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		std::cerr << "PIPE Error" << std::endl;
	}
	pid_t childPid = fork();
	if (childPid < 0) {
		std::cerr << "PID Error" << std::endl;
	} else if (childPid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		const char* pythonArgs[] = {"python3", pythonScript.c_str(), nullptr};
		execve("/usr/bin/python3", const_cast<char**>(pythonArgs), nullptr);
		std::cerr << "CGI Error" << std::endl;
	} else {
		close(pipefd[1]);
		std::string output;
		char buffer[128];
		ssize_t bytesRead;
		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
			output.append(buffer, bytesRead);
		}
		int status;
		waitpid(childPid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
			return (output);
		}
	}
	return ("");
}