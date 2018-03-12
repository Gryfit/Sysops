//
// Created by gryfit on 10.03.18.
//
#include <stdlib.h>
#include <time.h>

char** newTableCalloc (int N, size_t blockSize){
    srand (time(NULL));
    char ** T = calloc(N, sizeof(char*));
    for(int i=0;i<N;i++){
        T[i] = calloc(blockSize/ sizeof(char), sizeof(char));
        for(int j =0; j<blockSize/ sizeof(char);j++){
            T[i][j]= (char)(rand()%128);
        }
    }
    return T;
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
    char* Block = calloc(blockSize / sizeof(char), sizeof(char));
    T[i] = Block;
    for(int j =0; j<blockSize/ sizeof(char);j++){
        T[i][j]= (char)(rand()%128);
    }
}
//find closest
int findCalloc(char **T, int i, int N, size_t blockSize){
    int s =0;
    for(int j=0;j<blockSize/ sizeof(char);j++){
        s+=(int) T[i][j];
    }
    int diff = (int)(blockSize/ sizeof(char))*120;
    int min_index=i;
    for(int j=0;j<N;j++){
        if(j!=i) {
            int curr = 0;
            for (int k = 0; k < blockSize/ sizeof(char); k++) {
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

