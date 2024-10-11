#include "webserv.hpp"

void handler(int signal)
{
	(void)signal;
	throw std::runtime_error("\nServer stopped!");
}

int WebServ::server_core(void)
{
	signal(SIGINT, handler);
	try
	{
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		bzero(&address, addrlen);
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY: any ip address
		int port = this->_servers[0]->getPort()[0];
		address.sin_port = htons(port);
		cout << "Server opened to any address on port: " << port << endl;

		// domain IPv4, service TCP, default protocol, port, interface, bklog
		// AF_INET,     SOCK_STREAM, 0,                8080, INADDR_ANY, 10
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == -1)
			throw std::runtime_error("Error: Failed to create socket");
		int enable = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			throw std::runtime_error("Error: Failed to set setsockopt to SO_REUSEADDR");
		if (bind(sock, (struct sockaddr *)&address, addrlen) < 0)
			throw std::runtime_error("Error: Failed to bind socket or socket already open");
		int bklog = 10;
		if (listen(sock, bklog) < 0)
			throw std::runtime_error("Error: Failed to listen to socket");

		int i = 1;
		while (true)
		{
			cout << "Server waiting for request..." << endl;
			int new_sock;
			if ((new_sock = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
				throw std::runtime_error("Error: Failed to accept connection");
			char buffer[30000];
			bzero(&buffer, sizeof(buffer));
			read(new_sock, buffer, 30000);
			string msg = "Hi!";
			string resp = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";
			// msg = string(buffer); // Uncomment to send what we recieved instead of msg
			std::stringstream ss;
			ss << msg.length();
			resp = resp + ss.str() + "\n\n" + msg;
			write(new_sock, resp.c_str(), resp.length());
			close(new_sock);
			cout << buffer << endl;
			cout << "Request received!" << " Requests: " << i << endl;
			i++;
		}
		cout << RED "Reached uncommon place" END << endl;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << " 💀" << '\n';
	}
	close(sock);
	return 0;
}
