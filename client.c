#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char** argv) {

    int network_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (network_socket == -1) {
		perror("[CE]: error in calling socket()");
		exit(1);
	}

    struct sockaddr_in server_address;
	
	memset(&server_address, 0, sizeof(server_address));
	
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(connect(network_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
		perror("[CE]: connection error");
		close(network_socket);
		exit(1);
	}
	
	for (;;) {
		char buffer[1024] = {0};
		printf("Client: ");
		fgets(buffer, sizeof(buffer), stdin);
		send(network_socket, buffer, sizeof(buffer), 0);
		
		memset(buffer, 0, sizeof(buffer));
		
		recv(network_socket, &buffer, sizeof(buffer), 0);
		printf("Server: %s", buffer);
		
		// sleep(5);
	}
	
    close(network_socket);

    return 0;
}
