
/* bool dirFinder(const std::string& path, const std::string& temp, std::string& founDir) { */
/*     DIR* dir = opendir(temp.c_str()); */

/*     if (dir != NULL) { */
/*         struct dirent* ent; */
/*         while ((ent = readdir(dir)) != NULL) { */
/*             if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) { */
/*                 std::string subtemp = temp + "/" + ent->d_name; */
/*                 if (dirFinder(path, subtemp, founDir)) { */
/*                     closedir(dir); */
/*                     return true; */
/*                 } */
/*             } else if (ent->d_type == DT_REG && std::string(ent->d_name) == path) { */
/*                 founDir = temp + "/" + ent->d_name; */
/*                 closedir(dir); */
/*                 return true; */
/*             } */
/*         } */
/*         closedir(dir); */
/*     } */
/*     return false; */
/* } */

/* bool Server::fileFinder(const std::string& path, std::string& founDir) */
/* { */
/* 	std::string base = "./resources"; */
/* 	return dirFinder(path, base, founDir); */
/* } */
