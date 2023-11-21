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

std::string HttpResponse::phpCgiHandler(std::string script, char **av)
{
	(void)av;
	std::string phpScript = "cgi-bin" + script;
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
		const int tiempoLimite = 5000000;
		clock_t inicio = clock();
		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
			output.append(buffer, bytesRead);
		}
		int status;
		while (true) {
			pid_t res = waitpid(childPid, &status, WNOHANG);
			if (res == -1) {
				std::cerr << "Error al esperar por el proceso hijo." << std::endl;
				break;
			} else if (res == 0) {
				clock_t ahora = clock();
				double tiempoTranscurrido = static_cast<double>(ahora - inicio) / CLOCKS_PER_SEC;
				if (tiempoTranscurrido > tiempoLimite) {
					std::cerr << "El programa CGI tardó demasiado tiempo en ejecutarse (más de "
							  << tiempoLimite << " segundos)." << std::endl;
					kill(childPid, SIGTERM);
					std::cerr << "CGI Error Timeout" << std::endl;
					return (getIndex(C504, "./resources/html/504.html"));
				}
			} else {
				return ("");
			}
		}
	}
	return ("");
}