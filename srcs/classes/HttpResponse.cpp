#include "HttpResponse.hpp"

volatile sig_atomic_t terminationSignalReceived = 0;

void handleTerminationSignal(int signo) {
	(void)signo;
	std::cout << "Termination signal received" << std::endl;
	terminationSignalReceived = 1;
}

HttpResponse::HttpResponse(HttpRequest& parser)
{
	msg = getMessage(parser);
}

HttpResponse::HttpResponse(const HttpResponse& toCopy)
{
	(void)toCopy;
}

HttpResponse::~HttpResponse()
{
}

std::string HttpResponse::getMsg() {return msg;}

static bool dirFinder(const std::string& path, const std::string& temp, std::string& founDir) {
    DIR* dir = opendir(temp.c_str());

    if (dir != NULL) {
        struct dirent* ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                std::string subtemp = temp + "/" + ent->d_name;
                if (dirFinder(path, subtemp, founDir)) {
                    closedir(dir);
                    return true;
                }
            } else if (ent->d_type == DT_REG && std::string(ent->d_name) == path) {
                founDir = temp + "/" + ent->d_name;
                closedir(dir);
                return true;
            }
        }
        closedir(dir);
    }
    return false;
}

bool HttpResponse::fileFinder(const std::string& path, std::string& founDir)
{
	std::string base = "./resources";
	return dirFinder(path, base, founDir);
}


std::string HttpResponse::getIco(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: image/x-icon");
	msg.append("\nContent-Length: ");
	std::string html_name = path;
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

std::string HttpResponse::getImg(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: image/" + path.substr(path.find(".") + 1));
	msg.append("\nContent-Length: ");
	std::string html_name = path;
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

std::string HttpResponse::getPhp(std::string path)
{
	std::string php = phpCgiHandler(path, environ);
	std::string html_name = path;
	std::ifstream file(html_name.c_str());
	msg.append(php);
	file.close();
	return msg;
}

std::string HttpResponse::getIndex(std::string code, std::string path){
	std::string msg = code;
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = path;
	std::ifstream file(html_name.c_str());
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		msg.append(std::to_string(content.length()));
		msg.append("\n\n");
		msg.append(content);
		file.close();
	} else {
		std::cerr << "Index Fatal" << std::endl;
	}
	return msg;
}

std::string HttpResponse::postImage(std::string path, std::string body, std::map<std::string, std::string> headers){

	(void)path;
	std::string body_content = body;
	std::string contentType;
	std::string fileName;
	if (headers["Content-Type"] == "multipart/form-data"){
		size_t i = body.find("\r\n");
		std::string boundary = body.substr(i + 2, body.find("\r\n", i + 2) - i - 2);
		contentType = body.substr(body.find("Content-Type: ") + 14, body.find("\r\n", body.find("Content-Type: ") + 14) - body.find("Content-Type: ") - 14);
		fileName = body.substr(body.find("filename=\"") + 10, body.find("\"", body.find("filename=\"") + 10) - body.find("filename=\"") - 10);
		body_content = body.substr(body.find("\r\n\r\n") + 4);
	}
	else{
		if (headers["Content-Type"] == "plain/text")
			fileName = "archivo.txt";
		else
			fileName = "archivo";
	}
	std::string msg = "HTTP/1.1 201 Created\nLocation: /resources/bin/";
	DIR *dir;
	struct dirent *entry;
	dir = opendir("./resources/bin");
	if (dir){
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == fileName){
				fileName = + "copy_" + fileName;
			}
		}
		std::ofstream imageFile("./resources/bin/" + fileName, std::ios::binary);
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

std::string HttpResponse::getMessage(HttpRequest& parser)
{
	std::string founDir;
	if (parser.getType() == GET){
		// if (this->locations[0].isGET() == false)
		// 	return (getIndex(C405, "./resources/html/405.html"));
		// else{
			if (parser.getPath() == "/")
				return (getIndex(C200, "./resources/html/index.html"));
			else if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".html") != std::string::npos)
				return (getIndex(C204, founDir));
			else if ((fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".png") != std::string::npos) ||
					(fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".jpg") != std::string::npos) ||
					(fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".jpeg") != std::string::npos) ||
					(fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".gif") != std::string::npos))
				return (getImg(founDir));
			else if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".php") != std::string::npos)
				return (getPhp(founDir));
			else if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".ico") != std::string::npos)
				return (getIco(founDir));
			else
				return (getIndex(C404, "./resources/html/404.html"));
		// }
	}
	else if (parser.getType() == POST){
		// if (this->locations[0].isPOST() == false)
		// 	return (getIndex(C405, "405.html"));
		// else{
			return (postImage(parser.getPath(), parser.getBody(), parser.getHeaders()));
		// }
	}
	else if (parser.getType() == DELETE){
		if (fileFinder(parser.getPath().substr(1), founDir)){
			std::remove(founDir.c_str());
			return (getIndex(C204, "./resources/html/index.html"));
		}
	}
	return "";
}

std::string HttpResponse::phpCgiHandler(std::string script, char **av)
{
	(void)av;
	signal(SIGTERM, handleTerminationSignal);
	std::string phpScript = script;
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		std::cerr << "PIPE Error" << std::endl;
	}
	pid_t childPid = fork();
	if (childPid < 0) {
		std::cerr << "PID Error" << std::endl;
	} else if (childPid == 0) {
		signal(SIGTERM, SIG_DFL);
		while (!terminationSignalReceived) {
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			const char* phpArgs[] = {"php", phpScript.c_str(), nullptr};
			execve("/usr/bin/php", const_cast<char**>(phpArgs), nullptr);
			std::cerr << "CGI Error" << std::endl;
		}
		exit(0);
	} else {
		close(pipefd[1]);
		std::string output;
		char buffer[128];
		ssize_t bytesRead;
		int status;
		usleep(300000);
		if (waitpid(childPid, &status, WNOHANG) == 0) {
			sleep(4);
			if (waitpid(childPid, &status, WNOHANG) == 0) {
				kill(childPid, SIGTERM);
				usleep(100);
				waitpid(childPid, &status, 0);
		}
		}
		if (WIFEXITED(status)) {
			while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
				output.append(buffer, bytesRead);
			}
			std::string msg = "HTTP/1.1 200 OK";
			msg.append("\nContent-Type: text/html");
			msg.append("\nContent-Length: ");
			msg.append(output);
			return (msg);
		} else {
			return (getIndex(C504, "./resources/html/504.html"));
		}

	}
	return ("");
}

HttpResponse& HttpResponse::operator=(const HttpResponse& toAssign)
{
	(void)toAssign;
	return *this;
}

