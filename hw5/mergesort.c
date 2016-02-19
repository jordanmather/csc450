#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void mergesort(int a[], int low, int high);

int main(int argc, char** argv)
{
    srand(time(NULL));
    int num;
    int i;
    int nums[10];
    int numsc = sizeof(nums)/sizeof(nums[0]);
    int stop = 0;
    
    printf("Array:\n");
    for(i = 0; i < 10; i++)
    {
        nums[i] = rand()%2;
        printf("%d\n", nums[i]);
        
    }

    mergesort(nums, 0, numsc-1);
    printf("sorted\n");
    
    for(i = 0; i < 10; i++)
    {
        printf("%d\n", nums[i]);
        
    }
    
   
}

void merge(int a[], int low, int mid, int high)
{
    int aCount = sizeof(a)/sizeof(a[0]);   //get the size of a
    int b[aCount];                               //make a second temp array of same size
    int i = low, j = mid + 1, k = 0;            
  
    while (i <= mid && j <= high) { //most of the sorting is done here
        if (a[i] <= a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }
    while (i <= mid) //the bottom half didn't finish sorting
        b[k++] = a[i++];
  
    while (j <= high) //the top half didn't finish sorting
        b[k++] = a[j++];
  
    k--;
    while (k >= 0) { //set our array equal to the temp array
        a[low + k] = b[k];
        k--;
    }
}
  
void mergesort(int a[], int low, int high)
{
    if (low < high) { //if there is only one item, do nothing
        int m = (high + low)/2;     //find the middle
        mergesort(a, low, m);       //split in half, sending each to a differnt call of mergesort
        mergesort(a, m + 1, high);  //this could be the fork() point
        merge(a, low, m, high); 
    }
}



