//Sam Emison
//10/05/2025
//Simple C++ Web Server
//Listens on port 8080 and responds with a "Hello, World!" web page

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>


int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    //Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    //Bind to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    address.sin_port = htons(8080);       // Port 8080

     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

     //Starts listening for incoming connections
     if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

      std::cout << "Server running at http://localhost:8080\n";
      std::cout << "Waiting for connections...\n";

      while (true) {
	//Accept a connection
	client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

	//Read the request
	 char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);
        std::cout << "\n--- Incoming Request ---\n" << buffer << "\n------------------------\n";

	//Prepare a HTTP response for Hello World
	std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
            "<!DOCTYPE html>"
            "<html><head><title>Simple Server</title></head>"
            "<body><h1>Hello, World!</h1><p>This is a simple C++ web server.</p></body></html>";

	//Send the response
	 send(client_fd, response.c_str(), response.size(), 0);

	 //Close the conection
	  close(client_fd);
    }
      //Close the socket  
    close(server_fd);
    return 0;
}
