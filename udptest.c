/* Xeon 2014 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {

	int udpSocket, ret, addrlen, pktcnt = 0;
	struct sockaddr_in udpClient, udpServer;
	char buf[BUFSIZE], msg;

	if(argc < 3) {
		fprintf(stderr, "Usage: %s <ip address> <port> <payload>\n", argv[0]);
		fprintf(stderr, "Example: %s 127.0.0.1 8080 AAAAAAAAAAAA\n", argv[0]);
		return 1;
	}

	/* Create socket */
	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if(udpSocket == -1) {
		fprintf(stderr, "Could not create socket!\n");
		return -1;
	} else {
		printf("Socket created.\n");
	}

	/* Client address */
	/* Use INADDR_ANY to use all local addresses */
	udpClient.sin_family = AF_INET;
	udpClient.sin_addr.s_addr = INADDR_ANY;
	udpClient.sin_port = 0;

	ret = bind(udpSocket, (struct sockaddr*)&udpClient, sizeof(udpClient));

	if(ret == 0) {
		fprintf(stderr, "Bind completed.\n");
	} else {
		fprintf(stderr, "Could not bind to address!\n");
		close(udpSocket);
		return -1;
	}

	/* Server address */
	udpServer.sin_family = AF_INET;
	udpServer.sin_addr.s_addr = inet_addr(argv[1]);
	udpServer.sin_port = htons(atoi(argv[2]));

	while(1) {
		/* Set up message */
		if(argv[3]) {
			strcpy(buf, argv[3]);
		} else {
			sprintf(buf, "This is a UDP test string! (packet %d)\n", ++pktcnt);
		}

		ret = sendto(udpSocket, buf, strlen(buf)+1, 0, (struct sockaddr*)&udpServer, sizeof(udpServer));
		if(ret == -1) {
			fprintf(stderr, "Could not send message!\n");
			return -1;
		} else {
			printf("Message sent. (packet %d)\n", pktcnt);
		}
		sleep(1);
	}
	/* Cleanup */
	close(udpSocket);
}
