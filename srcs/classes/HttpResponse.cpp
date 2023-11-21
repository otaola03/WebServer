#include "HttpResponse.hpp"

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
	msg.append("\nContent-Type: image/png");
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

std::string HttpResponse::getPython(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: script/python");
	msg.append("\nContent-Length: ");
	std::string html_name = path;
	std::ifstream file(html_name.c_str());
	msg.append(std::to_string(pythonCgiHandler(path, environ).length()));
	msg.append("\n\n");
	msg.append(pythonCgiHandler(path, environ));
	file.close();
	std::cout << "MSG = " << msg << std::endl;
	return msg;
}

std::string HttpResponse::getPhp(std::string path)
{
	std::string msg = "HTTP/1.1 200 OK";
	msg.append("\nContent-Type: text/html");
	msg.append("\nContent-Length: ");
	std::string html_name = path;
	std::ifstream file(html_name.c_str());
	msg.append(std::to_string(phpCgiHandler(path, environ).length()));
	msg.append("\r\n\r\n");
	msg.append(phpCgiHandler(path, environ));
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

std::string HttpResponse::postImage(std::string path, std::string body){

	(void)path;
	std::cerr << "ELLA TIENE GATO PERO NORMAL\n";
	std::string body_content = body;
	// std::cout << "BODY = [" << body[0] << "]\n";
	if (body[1] == '-')
		body_content = body.substr(body.find("\r\n\r\n") + 4);
	/* std::cout << body_content << "\n"; */
	std::string msg = "HTTP/1.1 201 Created\nLocation: /resources/bin/";
	DIR *dir;
	int i = 0;
	struct dirent *entry;
	std::cout << "PARAGUAYO\n";
	dir = opendir("./resources/bin");
	if (dir){
		std::cout << "DNREOOOOOOO\n";
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == std::to_string(i) + ".png"){
				i++;
			}
		}
		std::ofstream imageFile("./resources/bin/" + std::to_string(i) + ".png", std::ios::binary);
		imageFile.write(body_content.c_str(), body_content.length());
		imageFile.close();
		closedir(dir);

    	/* // Crear un vector de bytes a partir de la cadena binaria */
    	/* std::vector<unsigned char> imageBytes(body.begin(), body.end()); */
		/* /1* std::cout << BLUE << "====================================\n" << WHITE; *1/ */
		/* /1* std::cout << body << "\n"; *1/ */
		/* /1* std::cout << BLUE << "====================================\n" << WHITE; *1/ */

    	/* // Escribir los datos en un archivo binario */
    	/* std::ofstream outputFile("./resources/bin/imagen_recibida.png", std::ios::binary); */
    	/* if (outputFile.is_open()) { */
    	/*     outputFile.write(reinterpret_cast<const char*>(imageBytes.data()), imageBytes.size()); */
    	/*     outputFile.close(); */
    	/*     std::cout << "Imagen creada y guardada con éxito." << std::endl; */
    	/* } else { */
    	/*     std::cerr << "Error al abrir el archivo para escritura." << std::endl; */
    	/* } */

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
				return (getIndex(C200, founDir));
			else if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".png") != std::string::npos)
				return (getImg(founDir));
			else if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".py") != std::string::npos)
				return (getPython(founDir));
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
			return (postImage(parser.getPath(), parser.getBody()));
		// }
	}
	else if (parser.getType() == DELETE){
		if (fileFinder(parser.getPath().substr(1), founDir) && parser.getPath().find(".png") != std::string::npos){
			std::remove(founDir.c_str());
			return (getIndex(C204, "./resources/html/index.html"));
		}
	}
	return "";
}

std::string HttpResponse::pythonCgiHandler(std::string script, char **av)
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

std::string HttpResponse::phpCgiHandler(std::string script, char **av)
{
	(void)av;
	std::string phpScript = script;
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
		const char* phpArgs[] = {"php", phpScript.c_str(), nullptr};
		execve("/usr/bin/php", const_cast<char**>(phpArgs), nullptr);
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

HttpResponse& HttpResponse::operator=(const HttpResponse& toAssign)
{
	(void)toAssign;
	return *this;
}
