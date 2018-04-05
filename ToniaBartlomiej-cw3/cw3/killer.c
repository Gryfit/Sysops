#include <stdlib.h>
#include <stdio.h>
#include <time.h>


void inf_loop(int i) {
    int *arr ;
	if(!(arr= malloc(1000000 * sizeof(int)))){
	puts("failed to allocate memory\n");	
	}
    for (int j=0; j<1000000; j++) {
	    arr[j]=j;
      if(j==100000) j =0;
    }
    free(arr);
}

int main() {
    inf_loop(0);
}
