#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "tictactoeFork.h"

#define SHMSZ 10

int main(int argc, char** argv)
{
    char c;
    int shmid, i;
    key_t key;
    char *board, *s;
    time_t t;
    
    /*
    we'll name our shared memory segement
    "5678".
    */
    key = 5678;
    
    /*
    create the segment
    */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        return -1;
    }
    
    /*
    Now we attach the segment to our data source
    */
    if((board = shmat(shmid, NULL, 0)) == (char *) -1)
    {
        perror("shmat");
        return -1;
    }
    
    //initialize RNG
    srand((unsigned) time(&t));
    //initialize who goes first
    // 'p' is the parent
    // 'c' is the child
    // 'w' means the game was won
    board[0] = 'p';
    //initailize the board with all 'a's.
    for(i = 1; i < 10; i++)
    {
        board[i] = 'a';
    }
    
    int pid = fork();
    
    if(pid != 0)
    {
        while(1)
        {
            //parent
            if(board[0] == 'p')
            {
                //critical section
               //parent's turn (uses 'x')
                int turn = validTurn(board);
                board[turn] = 'x';
                
                int won = checkWin('x', board);
                if(won == 1) //winner
                {
                 printf("Parent: I won!\n");
                 printf("%c  %c  %c\n", board[1], board[2], board[3]);
                 printf("%c  %c  %c\n", board[4], board[5], board[6]);
                 printf("%c  %c  %c\n", board[7], board[8], board[9]);
                 board[0] = 'w';
                 break;
                }
                else if(won == 2) //tie
                {
                 printf("Parent: Tie!\n");
                 printf("%c  %c  %c\n", board[1], board[2], board[3]);
                 printf("%c  %c  %c\n", board[4], board[5], board[6]);
                 printf("%c  %c  %c\n", board[7], board[8], board[9]);
                 board[0] = 'w';
                 break;
                }
                else
                {
                    board[0] = 'c';
                }
            }
            else if(board[0] == 'w')
            {
                exit(0); //board was won, end.
            }
            waitWhileEqual('c', board);
            
        }
    }
    else
    {
        while(1)
        {
            //child
            if(board[0] == 'c')
            {
                //critical section
               //child's turn (uses 'o')
            
                int turn = validTurn(board);
                board[turn] = 'o';
                
                int won = checkWin('o', board);
                if(won == 1) //winner
                {
                 printf("Child: I won!\n");
                 printf("%c  %c  %c\n", board[1], board[2], board[3]);
                 printf("%c  %c  %c\n", board[4], board[5], board[6]);
                 printf("%c  %c  %c\n", board[7], board[8], board[9]);
                 board[0] = 'w';
                 break;
                }
                else if(won == 2) //tie
                {
                 printf("Child: Tie!\n");
                 printf("%c  %c  %c\n", board[1], board[2], board[3]);
                 printf("%c  %c  %c\n", board[4], board[5], board[6]);
                 printf("%c  %c  %c\n", board[7], board[8], board[9]);
                 board[0] = 'w';
                 break;
                }
                else
                {
                    board[0] = 'p';
                }
            }
            else if(board[0] == 'w')
            {
                exit(0); //board was won, end.
            }
            waitWhileEqual('p', board);
            
        }
    }
    
    printf("The mem address is: %p\n", board);
    return 0;
}

void waitWhileEqual(char val, char* board)
{
    while(board[0] == val)
    {
        //wait
        
    }
}

int checkWin(char c, char* board)
{
    if((((board[1] == board[2]) && (board[1] == board[3])) && (board[1] == c)) || //check win conditions.  Board spaces are numbered
       (((board[4] == board[5]) && (board[4] == board[6])) && (board[4] == c)) ||      // from 1-9, left to right, top to bottom
       (((board[7] == board[8]) && (board[7] == board[9])) && (board[7] == c)) ||      //any three in a row means win
       (((board[1] == board[4]) && (board[1] == board[7])) && (board[1] == c)) ||
       (((board[2] == board[5]) && (board[2] == board[8])) && (board[2] == c)) ||
       (((board[3] == board[6]) && (board[3] == board[9])) && (board[3] == c)) ||
       (((board[1] == board[5]) && (board[1] == board[9])) && (board[1] == c)) ||
       (((board[7] == board[5]) && (board[7] == board[3])) && (board[7] == c)))    
    {
        return 1; //winner
    }
    else if( board[1] != 'a' && 
             board[2] != 'a' && 
             board[3] != 'a' &&           //if NO spots are empty
             board[4] != 'a' &&
             board[5] != 'a' &&
             board[6] != 'a' && 
             board[7] != 'a' && 
             board[8] != 'a' && 
             board[9] != 'a')
    {
        return 2; //tie
    }
    return 0; //not a winning board yet
}

int validTurn(char* board)
{
    //get random move
    int turn = 0;
    while(turn == 0) //placed in a while loop to make sure i don't choose 0
    {
       turn =  rand() % 10; //mod to make sure it's under 10
    }
    if(board[turn] != 'a')
    {
        turn = validTurn(board); //recall function to hopefully get a good move
    }
    return turn; //this should have a valid spot to move to
    
}