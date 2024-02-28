#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_SIZE 256

int main()
{
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    /* Create an unnamed socket for the server. */
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Name the socket. */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = 9734;
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

    /* Create a connection queue and wait for clients. */
    listen(server_sockfd, 5); 

    signal(SIGCHLD, SIG_IGN);

    while(1) {
        char buffer[BUFFER_SIZE]; 

        printf("server waiting\n");

        /* Accept a connection. */
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, 
            (struct sockaddr *)&client_address, &client_len);

        /* We can now read/write to client on client_sockfd. */
        ssize_t bytes_read = read(client_sockfd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            if(fork() == 0){
                printf("Received: %s\n", buffer);
                
                // Echo back the modified string to the client
                // usleep(5000*1000);
                // write(client_sockfd, buffer, bytes_read); 
                close(client_sockfd);
        
            } else {
                close(client_sockfd);
                exit(0);
            }
        }

    }   
    return 0;
}

