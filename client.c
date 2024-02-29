#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> // For string functions

#define BUFFER_SIZE 256 // Maximum buffer size for the string
#define MAX_WAIT_INTERVAL 8000
#define BASE 500

int main()
{
    int sockfd;
    struct sockaddr_in server_address;
    socklen_t server_len;
    char buffer[BUFFER_SIZE]; // Buffer for the string

    // Example string to send
    const char *message = "Hello, server!";

    // Create a UDP socket for the client.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));

    // Name the socket, as agreed with the server.
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);

    int retry_time = 0;
    double wait_interval = 0;
    ssize_t bytes_sent, bytes_read;
    int multiplier = 2;

    // Send the message to the server
    bytes_sent = sendto(sockfd, message, strlen(message), 0,
                        (const struct sockaddr *)&server_address, server_len);

    if (bytes_sent < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    // Try to receive a response with exponential backoff
    while (wait_interval < MAX_WAIT_INTERVAL) {
        bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_DONTWAIT,
                              (struct sockaddr *)&server_address, &server_len);

        if (bytes_read > 0) {
            // Ensure the received data is null-terminated
            buffer[bytes_read] = '\0';
            printf("Received from server: %s\n", buffer);
            break; // Exit the loop if we receive data
        } else {
            wait_interval = BASE * pow(multiplier, retry_time++);
            // if (wait_interval > MAX_WAIT_INTERVAL) wait_interval = MAX_WAIT_INTERVAL;
            usleep((int)wait_interval * 1000); // Wait before retrying
            printf("%d\n", (int)wait_interval);
        }
    }

    if (bytes_read <= 0) {
        printf("Server did not respond.\n");
    }

    close(sockfd);
    return 0;
}
