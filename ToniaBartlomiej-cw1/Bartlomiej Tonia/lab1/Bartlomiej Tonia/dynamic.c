//
// Created by gryfit on 10.03.18.
//
#include <stdlib.h>
#include <time.h>

char** newTableCalloc(int size, size_t base_size) {
    char** new_array = calloc(size,sizeof(char*));
    for(int i=0;i<size;i++){
 
    int number_of_characters = base_size;
    char* new_array2 = calloc(number_of_characters, sizeof(char));

    for(int i=0;i<number_of_characters;i++){
        new_array2[i] = 'a';
    }
	        new_array[i] =    new_array2;
    }


    return new_array;
}


//delete table
void freeTableCalloc(char ** T, int N){
    for(int i=0;i<N;i++){
        free(T[i]);
    }
    free(T);
}
//delete block
void delBlockCalloc(char** T, int i){
    char * tmp = T[i];
    T[i]=NULL;
    free(tmp);
}
//add block
void addBlockCalloc(char** T, int i, size_t blockSize){
    char* Block = calloc(blockSize, sizeof(char));
    T[i] = Block;
    for(int j =0; j<blockSize;j++){
        T[i][j]= (char)(rand()%94 +33);
    }
}
//find closest
int findCalloc(char **T, int i, int N, size_t blockSize){
    int s =0;
    for(int j=0;j<blockSize;j++){
        s+=(int) T[i][j];
    }
    int diff = (int)(blockSize)*120;
    int min_index=i;
    for(int j=0;j<N;j++){
        if(j!=i) {
            int curr = 0;
            for (int k = 0; k < blockSize; k++) {
                curr += (int) T[j][k];
            }
            if(abs(s-curr)<abs(s-diff)){
                diff = abs(s-diff);
                min_index = j;
            }
        }
    }
    return min_index;
}


