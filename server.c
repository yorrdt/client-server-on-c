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
        perror("[ServerError]: error in calling socket()");
        exit(1);
    }

    struct sockaddr_in server_address;
	
	memset(&server_address, 0, sizeof(server_address));
	
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    char server_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(server_address.sin_addr), server_ip, INET_ADDRSTRLEN);

    printf("[ServerMessage]: server %s:%d\n", server_ip, ntohs(server_address.sin_port));

    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("[ServerError]: bind() call failed");
		close(server_socket);
        exit(1);
    }

    if(listen(server_socket, 5) == -1) {
		perror("[ServerError]: listen() call failed");
		close(server_socket);
        exit(1);
	}


	while (1) {

		struct sockaddr_in client_struct;
		int client_struct_len = sizeof(client_struct);

		int client_socket = accept(server_socket, (struct sockaddr*)&client_struct, &client_struct_len);
		
		if (client_socket < 0) {
			perror("[ServerError]: accept() call failed");
			close(server_socket);
			exit(1);
		}


		char client_ip[INET_ADDRSTRLEN];
	    inet_ntop(AF_INET, &(client_struct.sin_addr), client_ip, INET_ADDRSTRLEN);

		printf("[ServerMessage]: connected client (%s:%d)\n", client_ip, ntohs(client_struct.sin_port));
		

		shutdown(client_socket, SHUT_RDWR);

		close(client_socket);
	}
	
    return 0;
}
