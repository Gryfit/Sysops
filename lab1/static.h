//
// Created by gryfit on 10.03.18.
//

#ifndef LIST_STATIC_H
#define LIST_STATIC_H
void newTableStatic (int N, size_t blockSize);
//delete table
void freeTableStatic(int N);
//delete block
void delBlockStatic(int i, size_t blockSize);
//add block
void addBlockStatic( int i, size_t blockSize);
//find closest
int findStatic(int i, int N, size_t blockSize);
#endif //LIST_STATIC_H
