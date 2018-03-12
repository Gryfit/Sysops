//
// Created by gryfit on 10.03.18.
//

#include <glob.h>

#ifndef LIST_DYNAMIC_H
#define LIST_DYNAMIC_H
char** newTableCalloc (int N, size_t blockSize);
//delete table
void freeTableCalloc(char ** T, int N);
//delete block
void delBlockCalloc(char** T, int i);
//add block
void addBlockCalloc(char** T, int i, size_t blockSize);
//find closest
int findCalloc(char **T, int i, int N, size_t blockSize);

#endif //LIST_DYNAMIC_H
