/*  tcpserver.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define MIN_PROXY_LEN 24

typedef struct struct_proxy_info {
	char proxy_string[32];
	char inet_protocol[32];
	char client_ip[32];
	char proxy_ip[32];
	char client_port[32];
	char proxy_port[32];
} PROXY_INFO;

/*
* Parse proxy format to get connection informaion
* PROXY_STRING + single space + INET_PROTOCOL + single space + CLIENT_IP + single space + PROXY_IP + single space + CLIENT_PORT + single space + PROXY_PORT + "\r\n"
* eg: PROXY TCP4 54.222.152.163 10.0.11.239 45418 5000\r\n
*/
void get_conn_info(const char *buf, int buf_size, PROXY_INFO *client_info)
{

	// FIXME: Just for quick example. You need modify to parse the string more safely
	sscanf(buf, "%s %s %s %s %s %s\r\n",
		client_info->proxy_string, client_info->inet_protocol, client_info->client_ip, client_info->proxy_ip, client_info->client_port, client_info->proxy_port);

	// printf("%s, %s, %s, %s, %s, %s\n", client_info->proxy_string, client_info->inet_protocol, client_info->client_ip, client_info->proxy_ip, client_info->client_port, client_info->proxy_port);

	return;
}


int main()
{
	int sock, connected, bytes_received , true = 1; 
	char send_data [1024] , recv_data[1024];      

	struct sockaddr_in server_addr,client_addr;   
	int sin_size;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;        
	server_addr.sin_port = htons(5000);    
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}

	printf("TCPServer Waiting for client on port 5000\n");
	fflush(stdout);

	while (1) { 
		char conn_info[1024];
		PROXY_INFO client_info;

		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		memset(conn_info, 0, sizeof(conn_info));
		sprintf(conn_info, "%s, %d", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		printf("I got a connection from (%s)\n", conn_info);

		// Parse proxy format and get client ip information
		// PROXY_STRING + single space + INET_PROTOCOL + single space + CLIENT_IP + single space + PROXY_IP + single space + CLIENT_PORT + single space + PROXY_PORT + "\r\n"
		// eg: PROXY TCP4 54.222.152.163 10.0.11.239 45418 5000\r\n
		memset(recv_data, 0, sizeof(recv_data));
		bytes_received = recv(connected,recv_data,1024,0);

		if ( bytes_received < MIN_PROXY_LEN ) {
			printf("Connection (%s): Invalid proxy length  received. Quit.\n" , conn_info);
			close(connected);
			break;
		}

		memset(&client_info, 0, sizeof(PROXY_INFO));
		get_conn_info(recv_data, bytes_received, &client_info);

		// Got the client IP for future use
		printf("Client IP: %s\n", client_info.client_ip);

		// Single connection sample
		while (1) {
			memset(recv_data, 0, sizeof(recv_data));
			bytes_received = recv(connected,recv_data,1024,0);

			// ELB noisy checking
			if ( 0 == bytes_received ) {
				printf("Connection (%s): 0 received. Quit.\n" , conn_info);
				close(connected);
				break;
			}

			// strip \r\n, FIXME 
			recv_data[bytes_received-2] = '\0';

			if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
				close(connected);
				printf("Connection (%s): Quit\n" , conn_info);
				break;
			} else {
				printf("RECIEVED DATA = %s \n" , recv_data);
				// Echo 
				send(connected, recv_data,strlen(recv_data), 0);
			}

			fflush(stdout);

			// sleep(1);
		}
	}

	close(sock);
	return 0;
} 
