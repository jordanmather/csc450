#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1
#endif

void childOneJob(int numsc, int nums[10]);
void parentJob(int numsc, int nums[10]);

int main(int argc, char** argv)
{
    srand(time(NULL));
    int num;
    int i;
    int nums[10];
    int finalNums[10];
    int numsc = sizeof(nums)/sizeof(nums[0]);
    int stop = 0;
    int ret;
    FILE *f1, *fp;
    char* fileName = "tempFile.txt";
    int c;
    
    printf("Array:\n");
    for(i = 0; i < 10; i++)
    {
        nums[i] = rand()%2;
        printf("%d\n", nums[i]);
        
    }
    //write array to file
    if(exists(fileName) == 0)
    {
        f1 = fopen(fileName, "a+");
        for(i = 0; i < 10; i++)
        {
            fprintf(f1, "%d", nums[i]);
        }
    }
    
    
    printf("About to Fork...\n");
    int childplace = numsc/2;
   /* pid_t pid = fork();
    
    if(pid == 0)
    {
        //child process
        //childOneJob(numsc, nums);
        printf("child");
        
    }
    if(pid != 0)
    {
        //parent process
        //parentJob(numsc, nums);
        printf("parent");
        
        fclose(f1);
        ret = remove(fileName);
        
    }*/
    
    //read from file into array
    printf("print array\n");
    fp = fopen(fileName,"r");
     while(1)
   {
      c = fgetc(fp);
      if( feof(fp) )
      {
         break;
      }
      printf("%c", c); 
   }
   printf("\nend array\n");
      
      //print array
      /*
      printf("FinalArray:\n");
        for(i = 0; i < 10; i++)
        {
            printf("%d\n", finalNums[i]);
            
        }*/
    
    
    return(0);
    
   
}


int exists(const char *fname)
{
    FILE *file;
    if (file = fopen(fname, "r"))
    {
        fclose(file);
        return 1; //exists
    }
    return 0; //DNE
}

/*
void childOneJob(int numsc, int nums[10])
{
    int i = 0;
    int place = (numsc/2);
        for(i = place; i < numsc; i++)
        {
            printf("Child printing item at place %d: %d\n", i, nums[i]);
        }
}

void parentJob(int numsc, int nums[10])
{
    int i = 0;
    int place = (numsc/2);
        for(i = 0; i < place; i++)
        {
            printf("Parent printing item at place %d: %d\n", i, nums[i]);
        }
}*/