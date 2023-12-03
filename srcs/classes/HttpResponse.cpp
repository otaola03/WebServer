#include "HttpResponse.hpp"

bool isDirectory2(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

vector<string>  getFiles(const string& directory)
{
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        cerr << "No se puede abrir el directorio " << directory << endl;//iug
    }
    vector<string> files;
    while (struct dirent* file = readdir(dir)) {
        files.push_back(file->d_name);
    }
    closedir(dir);
    return(files);
}

string	listFiles(const string& path, const string& root)
{
    vector<string> const files = getFiles(root);
    string  index;

	for (int i = 0; i < (int)files.size(); i++)
	{
		string file = files[i];
		
		if (file[0] && file[0] != '.')
		{
			if (!isDirectory2(root + "/" + file))
			{
				index += "<li><a href=\"" + path + "/" + file + "\">";
				index += path + "/" + file;
				index += "</a></li>\n";
			}
			else
        {
            index += listFiles(path + "/" + file, root + "/" + file);
        }
		}
	}
    return (index);
}

string generate_autoindex_http(const string& path, const string& root)
{
    string  index;

    index += "HTTP/1.1 200 OK\n\n";
    index += "<html>\n";
    index += "<head>\n";
    index += "<title>Autoindex</title>\n";
    index += "</head>\n";
    index += "<body>\n";
    index += "<ul>\n";

    index += listFiles(path, root);

    index += "</ul>\n";
    index += "</body>\n";
    index += "</html>\n";
    return (index);
}

HttpResponse::HttpResponse(HttpRequest& parser, std::map<int, std::string> errors)
{
	msg = getMessage(parser, errors);
}

HttpResponse::HttpResponse(const HttpResponse& toCopy)
{
	(void)toCopy;
}

HttpResponse::~HttpResponse()
{
}

std::string HttpResponse::getMsg() {return msg;}

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

std::string HttpResponse::getPhp(std::string path, std::string args)
{
	std::string php = phpCgiHandler(path, args);
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

std::string HttpResponse::postImage(std::string path, std::string body, std::map<std::string, std::string> headers, std::string destination, std::string root){

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
	else if (path.find(".php") != std::string::npos){
		std::string findPath;
		std::string args = body_content;
		FileFinder::fileFinder(path.substr(1), findPath, root);
		std::cerr << "FINDPATH: " << findPath << std::endl;
		std::cerr << "PATH: " << path << std::endl;
		std::cerr << "ROOT: " << root << std::endl;
		std::string php = phpCgiHandler(findPath, args);
		std::string html_name = path;
		std::ifstream file(html_name.c_str());
		msg.append(php);
		file.close();
		return msg;
	}
	else{
		if (headers["Content-Type"] == "plain/text")
			fileName = "archivo.txt";
		else
			fileName = "archivo";
	}
	std::string msg = "HTTP/1.1 201 Created\nLocation: " + destination;
	DIR *dir;
	struct dirent *entry;
	mkdir(destination.c_str(), 0777);
	dir = opendir(destination.c_str());
	if (dir){
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_name == fileName){
				dir = opendir(destination.c_str());
				fileName = + "copy_" + fileName;
			}
		}
		std::ofstream imageFile(destination + "/" + fileName, std::ios::binary);
		imageFile.write(body_content.c_str(), body_content.length());
		imageFile.close();
		closedir(dir);
	}
	// msg.append("\nContent-Type: text/html");
	// msg.append("\nContent-Length: ");
	// std::string html_name = "./resources/html/post.html";
	// std::ifstream file(html_name.c_str());
	// if (file.is_open()) {
	// 	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// 	msg.append(std::to_string(content.length()));
	// 	msg.append("\n\n");
	// 	msg.append(content);
	// 	file.close();
	// } else {
	// 	std::cerr << "Fatal" << std::endl;
	// }
	return msg;
}

std::string HttpResponse::redirector(std::string page){
	std::string rtn;

	// std::cerr << "REDIRECTOR: {" << page << "}" << std::endl;
	rtn += C302;
	rtn += "\r\n";
	rtn += "Location: http://" + page;
	return rtn;
}

void jose(std::string& varPath, std::string& root){
	
	size_t lastSlashPos = varPath.rfind('/');
	if (lastSlashPos == std::string::npos)
		return ;
	root = root + "/" + varPath.substr(0, lastSlashPos);
	varPath = varPath.substr(lastSlashPos + 1);
}

std::string HttpResponse::returner(HttpRequest& parser, std::map<int, std::string> errors, std::string varPath, std::string args){
	std::string founDir;
	std::string root = parser.getLocation().getRoot();

	jose(varPath, root);
	if (FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".html") != std::string::npos)
			return (getIndex(C200, founDir));
	else if ((FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".png") != std::string::npos) ||
			(FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".jpg") != std::string::npos) ||
			(FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".jpeg") != std::string::npos) ||
			(FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".gif") != std::string::npos))
		return (getImg(founDir));
	else if (FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".php") != std::string::npos)
	{
		if (parser.getLocation().hasCGI() == false)
			return (getIndex(C403, "./resources/html/403.html"));
		else
			return (getPhp(founDir, args));
	}
	else if (FileFinder::fileFinder(varPath, founDir, root) && varPath.find(".ico") != std::string::npos)
		return (getIco(founDir));
	if (errors[404].empty() == false)
		return (getIndex(C404, errors[404]));
	return (getIndex(C404, "./resources/html/404.html"));
}

std::string HttpResponse::getMessage(HttpRequest& parser, std::map<int, std::string> errors)
{
	std::cout << std::endl;
	std::string args;
	std::string getPath;
	Location	location = parser.getLocation();
	// std::cerr << "TYPE: " << parser.getType() << std::endl;
	// std::cerr << "PATH: " << parser.getPath() << std::endl;
	// parser.printHeaders();
	// parser.printBody();

	if (parser.getPath().find("?") != std::string::npos){
		args = parser.getPath().substr(parser.getPath().find("?") + 1);
		getPath = parser.getPath().substr(0, parser.getPath().find("?"));
	}
	else
		getPath = parser.getPath();
	if (parser.getType() == PATH_ERROR){
		if (errors[404].empty() == false)
			return (getIndex(C404, errors[404]));
		return (getIndex(C404, "./resources/html/404.html"));
	}
	else if (parser.getType() == METHOD_ERROR){
		if (errors[405].empty() == false)
			return (getIndex(C405, errors[405]));
		return (getIndex(C405, "./resources/html/405.html"));
	}
	else if (parser.getType() == BAD_REQUEST || parser.getType() == UNDEFINED){
		if (errors[400].empty() == false)
			return (getIndex(C400, errors[400]));
		return (getIndex(C400, "./resources/html/400.html"));
	}
	else if (parser.getType() == HTTP_VERSION_ERROR){
		if (errors[505].empty() == false)
			return (getIndex(C505, errors[505]));
		return (getIndex(C505, "./resources/html/505.html"));
	}
	else if (parser.getType() == LENGTH_ERROR){
		if (errors[413].empty() == false)
			return (getIndex(C413, errors[413]));
		return (getIndex(C413, "./resources/html/413.html"));
	}
	if (parser.isChunked() == true)
		return ("HTTP/1.1 100 Continue\r\n\r\n");

	std::string founDir;
	std::string root = location.getRoot();
	std::string redir = location.getRedirection();
	if (parser.getType() == GET){
		if (redir.empty() == false)
			return (redirector(redir));
		if (location.hasAutoindex() && (getPath.empty() == true || getPath == "/"))
			return (generate_autoindex_http(location.getPath(), root));
		if (getPath.empty() == true){
			if (FileFinder::fileFinder(location.getIndex(), founDir, root))
				return (returner(parser, errors, location.getIndex(), args));
		}
		if (getPath == "/"){
			if (FileFinder::fileFinder(location.getIndex(), founDir, root))
				return (returner(parser, errors, location.getIndex(), args));
		}
		return (returner(parser, errors, getPath.substr(1), args));
	}

	else if (parser.getType() == POST){
		return (postImage(getPath, parser.getBody(), parser.getHeaders(), location.getRoot() + "/" + location.getDestination(), location.getRoot()));
	}
	else if (parser.getType() == DELETE){
		if (FileFinder::fileFinder(getPath.substr(1), founDir, root)){
			std::remove(founDir.c_str());
			if (FileFinder::fileFinder(location.getIndex(), founDir, root))
				return (getIndex(C200, founDir));
		}
	}
	return "";
}

std::string HttpResponse::phpCgiHandler(std::string script, std::string args)
{
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
		const char* phpArgs[] = {"php", phpScript.c_str(), args.c_str(), nullptr};
		if (execve("/usr/bin/php", const_cast<char**>(phpArgs), nullptr) == -1) {
			perror("Error al ejecutar execve");
			exit(EXIT_FAILURE);
		}
		std::cerr << "CGI Error" << std::endl;
	} else {
		close(pipefd[1]);
		std::string output;
		char buffer[128];
		ssize_t bytesRead;
		int status;
		clock_t startTime = std::clock();
		while (waitpid(childPid, &status, WNOHANG) == 0) {
			clock_t endTime = std::clock();
			if (static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC > 5.0) {
				kill(childPid, SIGTERM);
			}
		}
		if (WIFEXITED(status)) {
			while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
				output.append(buffer, bytesRead);
			}
			std::string msg = "HTTP/1.1 200 OK";
			msg.append("\nContent-Type: text/html");
			msg.append("\nContent-Length: ");
			msg.append(std::to_string(output.length()));
			msg.append("\n\n");
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

