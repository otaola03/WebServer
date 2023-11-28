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

vector<struct dirent*>  getFiles(const string& directory)
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

string	listFiles(const string& directory)
{
  vector<struct dirent*> files = getFiles(directory);
  string  index;

	// index += endl << "🎱 entramos con " << directory << endl;
	for (int i = 0; i < files.size(); i++)
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

string generate_autoindex_http(const string& directory)
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

int main() {
  // Genera el autoindex para el directorio actual.
  cout << generate_autoindex_http(".");

  return 0;
}

//termina lo de autoindex recibe el path devuelve un string
