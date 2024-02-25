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
    int len;
    struct sockaddr_in address;
    int result;
    int multiplier = 2;
    int retry_time = 0;
    double wait_interval=0;

    char buffer[BUFFER_SIZE]; // Buffer for the string

    // Example string to send
    const char *message = "Hello, server!";

    // Create a socket for the client.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Name the socket, as agreed with the server.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9734;
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr *)&address, len);
    printf("%d\n", result);

    while (result == -1 && wait_interval < MAX_WAIT_INTERVAL){
        wait_interval = BASE * pow((double)multiplier, (double)(retry_time++));
        printf("%f\n", wait_interval);
        usleep((int)wait_interval*1000);
    
        result = connect(sockfd, (struct sockaddr *)&address, len);
    } 

    if (result == -1)
    {
        perror("oops: client");
        exit(0);
    }

    // We can now read/write via sockfd.

    // Send the message to the server
    write(sockfd, message, strlen(message));

    // Read the response from the server
    ssize_t bytes_read = read(sockfd, buffer, BUFFER_SIZE);
    if (bytes_read > 0)
    {
        // Ensure the received data is null-terminated
        buffer[bytes_read] = '\0';
        printf("Received from server: %s\n", buffer);
    }

    close(sockfd);
    return 1;
}

