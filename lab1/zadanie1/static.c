//
// Created by gryfit on 10.03.18.
//
#include <stdlib.h>
#include <time.h>

#define SIZE 100000
char T[SIZE][SIZE];
void newTableStatic (int N, size_t blockSize){
    srand (time(NULL));
    for(int i=0;i<N;i++){
        for(int j =0; j<blockSize/ sizeof(char);j++){
            T[i][j]= (char)(rand()%128);
        }
    }
}

//delete table
void freeTableStatic(int N){
    //Legacy
}
//delete block
void delBlockStatic(int i, size_t blockSize){
    for(int j =0; j<blockSize/ sizeof(char);j++){
        T[i][j]= (char) 0;
    }
}
//add block
void addBlockStatic(int i, size_t blockSize){
    for(int j =0; j<blockSize/ sizeof(char);j++){
        T[i][j]= (char)(rand()%128);
    }
}
//find closest
int findStatic(int i, int N, size_t blockSize){
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

