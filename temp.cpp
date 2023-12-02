int bytesRead;
	std::string recvData;
	char buffer[1024];
	(void)maxBodySize;
	(void)locations;
	while (1)
	{
		while ((bytesRead = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
			std::cerr << "bytesRead: " << bytesRead << std::endl;
			std::cerr << "buffer size: " << std::string(buffer, bytesRead).length() << std::endl;
			std::cout << std::string(buffer, bytesRead) << std::endl;
			recvData += std::string(buffer, bytesRead);
		}
		if (recvData.find("0\r\n\r\n") != std::string::npos){
			std::cerr << "BREAKING\n";
			break;
		}
		std::cerr << "SENDING 100 CONTINUE\n";
		send (sockfd, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
	}
	while (1)
	{
		std::cerr << "AAAAAAAAAAAAAA: " << std::endl;
		exit(1);
	}
	