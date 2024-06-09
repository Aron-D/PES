#include "clientSocket.h"

#define DEFAULT_PORT 8080

/**
 * Connects to given IP address and sends given message.
 * Afterwards, listens for an answer and returns received message.
 */
string ClientSocket::communicate(string ip, string message) {
	static int socketCounter = 0; //verbinding teller
	int readOk = 0; //foutcontrole op lezen en schrijven
		
	printf("Verbinding %d, IP: %s\n", socketCounter, ip);
	//lees bestand met waardes om deze te versturen

	printf("Ik ga sturen: %s\n", message);
		
	//socket met server RPI
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return "ERR_SOCK_CREATION";
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(DEFAULT_PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form

	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) //serverIp bepaalt welke server benadert wordt
		<= 0) {
		printf(
		"\nInvalid address/ Address not supported \n");
		return "ERR_INV_ADDR";
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
		printf("\nConnection failed\n");
		return "ERR_CONN_FAILED";
	}
		
	send(client_fd, message, strlen(message), 0);
	valread = read(client_fd, buffer, 1024 - 1); // subtract 1 for the null
	// terminator at the end
	printf("%s\n", buffer);
	socketCounter++;
	
	// closing the connected socket
	close(client_fd);
	return buffer;
}

ClientSocket::ClientSocket() {}
