#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void readInput(int max, char* userinput);

int sockfdID;
int* sockfd;

int main(int argc, char** argv)
{
    //shared memory
    sockfdID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    sockfd = shmat(sockfdID, NULL, 0);
    
    
    //set up shared memory so we can listen for new messages from both the user and server
    int MAX_SIZE = 2000 * sizeof(char);
    char* serverReply = malloc(MAX_SIZE);
    char* clientReply = malloc(MAX_SIZE);
    
    int pid;
    int max = 20;
    //char* userInput = (char*)malloc(max);
    uint16_t port = 4000;
    int error;
    struct sockaddr_in* server = malloc(sizeof(struct sockaddr_in));
    server->sin_addr.s_addr = inet_addr("127.0.0.1");
    server->sin_family = AF_INET;
    server->sin_port = htons( port );
    //memset(server->sin_zero, '\0', sizeof server->sin_zero);
    
    sockfd[0] = socket(AF_INET, SOCK_STREAM, 0);
    error = connect(sockfd[0], (struct sockaddr *)server, sizeof(struct sockaddr));
    if(error < 0)
    {
        puts("Problem with connecting...\n");
    }
    else
    {
        puts("We are Connected");
        
        //fork the child for receiving server messages
        pid = fork();
        if(pid == 0)
        {
            puts("Child 1");
            while(1)
            {
                //child 1 should monitor for new messages received
                error = recv(sockfd[0], serverReply, MAX_SIZE, 0);
                if(error < 0)
                {
                    puts("recv failed");
                }
                else
                {
                    puts(serverReply);
                }
            }
        }
        else
        {
            //parent should monitor for new messages sent
            int max = 20;
            while(1)
            {
                readInput(max, clientReply);
               send(sockfd[0] , clientReply , strlen(clientReply) , 0);
            }
            
        }
        
            
    }
    free(server);
    return 0;
}



void readInput(int max, char* userinput)
{
    while (1) 
    { // skip leading whitespace 
        int c = getchar();
        if (c == EOF) break; // end of file 
        if (!isspace(c)) 
        {
             ungetc(c, stdin);//push the first valid char back onto the stack so our next loop can grab it
             break;
        }
    }
    
    int i = 0;
    while (1) 
    {
        int c = getchar();
        if (c == '\n') // at end, add terminating zero 
        {
            userinput[i] = 0;
            break;
        }
        userinput[i] = c;
        if (i == max - 1) // buffer full 
        { 
            max = max + max;
            userinput = (char*)realloc(userinput, max); // get a new and larger buffer 
        }
        i++;
    }
}