#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

int server_sockfd;

void sigint_handler(int sig) {
    printf("Terminating server...\n");
    close(server_sockfd); // Close the server socket
    exit(0); // Exit the program
}

int main(int argc, char *argv[]) {

    int respond = 1;

    for(int i=1 ; i<argc ; i++) {
        if(strcmp(argv[i], "--no-resp") == 0) {
            respond=0;
            break;
        }
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int client_len = sizeof(client_address);
    int server_len;

    // Set up signal handler for SIGINT
    signal(SIGINT, sigint_handler);

    /* Create a UDP socket for the server */
    server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));

    /* Name the socket */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);

    /* Bind the socket to the address */
    if (bind(server_sockfd, (const struct sockaddr *)&server_address, server_len) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP server waiting for messages...\n");

    while (1) {

        int n = recvfrom(server_sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        if (respond) {
            // Echo the message back to the client
            sendto(server_sockfd, buffer, n, 0, (const struct sockaddr *)&client_address, client_len);
            printf("Message sent back to client.\n");
        } else {
            printf("Server is configured not to respond.\n");
        }
    }

    return 0;
}
