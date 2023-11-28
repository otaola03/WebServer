#include "HttpResponse.hpp"

static bool isDirectory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false;
    return S_ISDIR(info.st_mode);
}

static vector<struct dirent*>  getFiles(const string& directory)
{
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        cerr << "No se puede abrir el directorio " << directory << endl;//iug
    }
    vector<struct dirent*> files;
    while (struct dirent* file = readdir(dir)) {
        files.push_back(file);
    }
    closedir(dir);
    return(files);
}

static string	listFiles(const string& directory)
{
  vector<struct dirent*> files = getFiles(directory);
  string  index;

	// index += endl << "🎱 entramos con " << directory << endl;
	for (int i = 0; i < (int)files.size(); i++)
	{
		struct dirent* file = files[i];
		// index += endl << "🎗 " << i << " " << file->d_name << endl;
		
		if (file->d_name[0] && file->d_name[0] != '.')
		{
			if (!isDirectory(directory + "/" + file->d_name))
			{
				index += "<li><a href=\"" + directory + "/" + file->d_name + "\">";
				index += directory + "/" + file->d_name;
				index += "</a></li>\n";
			}
			else
				listFiles(directory + "/" + file->d_name);
		}
	}
  return (index);
}

static string generate_autoindex_http(const string& directory)
{
  string  index;

  index += "HTTP/1.1 200 OK\n\n";
  index += "<html>\n";
  index += "<head>\n";
  index += "<title>Autoindex</title>\n";
  index += "</head>\n";
  index += "<body>\n";
  index += "<ul>\n";

	index += listFiles(directory);

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

std::string HttpResponse::redirector(std::string page){
	std::string rtn;

	// std::cerr << "REDIRECTOR: {" << page << "}" << std::endl;
	rtn += C302;
	rtn += "\r\n";
	rtn += "Location: http://" + page;
	return rtn;
}

std::string HttpResponse::getMessage(HttpRequest& parser, std::map<int, std::string> errors)
{
	Location	location = parser.getLocation();
	if (parser.getType() == PATH_ERROR)
		return (getIndex(C404, errors[404]));
	else if (parser.getType() == METHOD_ERROR || parser.getType() == UNDEFINED)
		return (getIndex(C405, "./resources/html/405.html"));
	else if (parser.getType() == LENGTH_ERROR)
		return (getIndex(C413, "./resources/html/413.html"));

	std::string founDir;
	std::string root = location.getRoot();
	std::string redir = location.getRedirection();
	if (parser.getType() == GET){
		if (redir.empty() == false)
			return (redirector(redir));
		if (location.hasAutoindex())
			return (generate_autoindex_http(root));
		if (parser.getPath() == "/")
			return (getIndex(C200, "./resources/html/index.html"));
		else if (FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".html") != std::string::npos)
			return (getIndex(C200, founDir));
		else if ((FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".png") != std::string::npos) ||
				(FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".jpg") != std::string::npos) ||
				(FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".jpeg") != std::string::npos) ||
				(FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".gif") != std::string::npos))
			return (getImg(founDir));
		else if (FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".php") != std::string::npos)
			return (getPhp(founDir));
		else if (FileFinder::fileFinder(parser.getPath().substr(1), founDir, root) && parser.getPath().find(".ico") != std::string::npos)
			return (getIco(founDir));
		else
			return (getIndex(C404, "./resources/html/404.html"));
	}
	else if (parser.getType() == POST){
		return (postImage(parser.getPath(), parser.getBody(), parser.getHeaders()));
	}
	else if (parser.getType() == DELETE){
		if (FileFinder::fileFinder(parser.getPath().substr(1), founDir, root)){
			std::remove(founDir.c_str());
			return (getIndex(C204, "./resources/html/index.html"));
		}
	}
	return "";
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

