#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void readInput(int max, char* userinput);

int main(int argc, char** argv)
{
    int max = 20;
    char* userinput = (char*)malloc(max);
    uint16_t port = 3000;
    int error;
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_family = AF_INET;
    server->sin_port = htons( port );
    //memset(server->sin_zero, '\0', sizeof server->sin_zero);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    error = connect(sockfd, (struct sockaddr *)server, sizeof(struct sockaddr));
    if(error < 0)
    {
        puts("Problem with connecting...\n");
    }
    else
    {
        puts("We are Connected");
        //Receive a reply from the server
        
        char* server_reply = malloc(2000 * sizeof(char));
        while(1)
        {
            error = recv(sockfd, server_reply, sizeof(server_reply), 0);
            if(error < 0)
            {
                puts("recv failed");
            }
            else
            {
                printf("message: ");
                puts(server_reply);
                
                readInput(max, userinput);
                send(sockfd, userinput , strlen(userinput) , 0);
            }
        }
    }
    free(server);
    return 0;
}

void readInput(int max, char* userinput)
{
    while (1) 
    { /* skip leading whitespace */
        int c = getchar();
        if (c == EOF) break; /* end of file */
        if (!isspace(c)) 
        {
             ungetc(c, stdin);
             break;
        }
    }
    
    int i = 0;
    while (1) 
    {
        int c = getchar();
        if (c == '\n') /* at end, add terminating zero */
        {
            userinput[i] = 0;
            break;
        }
        userinput[i] = c;
        if (i == max - 1) /* buffer full */
        { 
            max = max + max;
            userinput = (char*)realloc(userinput, max); /* get a new and larger buffer */
        }
        i++;
    }
}