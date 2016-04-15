#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void broadcast(char* msg, int* clients, int numClients);

int main(int argc, char** argv)
{
    uint16_t port = 3000;
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        puts("Bad Socket!!!!");
    }
    
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_port = htons( port );
    memset(server->sin_zero, '\0', sizeof server->sin_zero);

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
    socklen_t serverSize = sizeof(server);
    
    int bindfd = bind(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr));
    if(bindfd < 0)
    {
        puts("Problem with binding...\n");
    }
    int listenfd;
    int clientfd;
    int MAX_CLIENTS = 1000;
    int* clients = malloc(MAX_CLIENTS * sizeof(int));
    int numberOfConnectedClients = 0;
    
    char* message = "hello";
    while(1)
    {
        listenfd = listen(sockfd, 100);
    
        printf("Listening....\n");
        clientfd = accept(sockfd ,  (struct sockaddr *)server , &serverSize);
        printf("Client Connected.... %d\n", clientfd);
        
        //add this client to our array of clients
        clients[numberOfConnectedClients++] = clientfd;
        
        pid_t pid = fork();
        if(pid == 0)
        {
            printf("Entered Child\n");
            char* server_reply = malloc(2000 * sizeof(char));
            char* message = "Welcome";
            broadcast(message, clients, numberOfConnectedClients);
            while(1)
            {
                int error = recv(clientfd, server_reply, sizeof(server_reply), 0);
                if(error < 0)
                {
                    puts("recv failed");
                }
                else
                {
                    puts("Reply received");
                    puts(server_reply);
                    //send(clientfd, message , strlen(message) , 0);
                    broadcast(server_reply, clients, numberOfConnectedClients);
                }
                    
            }
            free(server_reply);
            break;
        }
    }
    //write(clientfd, buffer, sizeof(buffer));
}

void broadcast(char* msg, int* clients, int numClients)
{
    int i;
    for(i = 0; i < numClients; i++)
    {
        printf("Trying to send to client with FD: %d\n", clients[i]);
        //printf("Trying to send to client with FD: %d\n", *(clients + (i * sizeof(int))));
        
        send(clients[i] , msg , strlen(msg) , 0);
        //puts("sent");
    }
}