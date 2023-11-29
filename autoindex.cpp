#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
using namespace std;
#include <iostream>
#include <vector>

#include <sys/stat.h>

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
using namespace std;

bool isDirectory(const std::string& path) {
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

	for (int i = 0; i < files.size(); i++)
	{
		string file = files[i];
		
		if (file[0] && file[0] != '.')
		{
			if (!isDirectory(root + "/" + file))
			{
				index += "<li><a href=\"" + root + "/" + file + "\">";
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

// int main() {
//   cout << generate_autoindex_http("directorio focal", "./resources");

//   return 0;
// }
