#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char** argv) {

    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket == -1) {
        perror("[SE]: error in calling socket()");
        exit(1);
    }

    struct sockaddr_in server_address;
	
	memset(&server_address, 0, sizeof(server_address));
	
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("[SE]: bind() call failed");
		close(server_socket);
        exit(1);
    }

    if(listen(server_socket, 5) == -1) {
		perror("[SE]: listen() call failed");
		close(server_socket);
        exit(1);
	}
	
	int client_accept = accept(server_socket, 0, 0);
		
	if (client_accept < 0) {
		perror("[SE]: accept() call failed");
		close(server_socket);
		exit(1);
	}
	
	for (;;) {
		char buffer[1024] = {0};
		recv(client_accept, &buffer, sizeof(buffer), 0);
		printf("Client: %s", buffer);
		
		memset(buffer, 0, sizeof(buffer));
		
		printf("Server: ");
		fgets(buffer, sizeof(buffer), stdin);
		send(client_accept, buffer, sizeof(buffer), 0);

		// sleep(5);
	}
	
	shutdown(client_accept, SHUT_RDWR);

	close(client_accept);

    return 0;
}
