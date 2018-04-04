#include <stdio.h>
#include <stdlib.h>
#define SIZE 10000000
int Fibonnaci(long a, long b){
	long c = a+b;
	printf("%d %d %d ",a,b,c);
	while(c){
		a=b;
		b=c;
		c=b+a;
		printf("%d ",c);
	}
	printf("\n");
	return 0;
}
int Fibonnaci2(long * T){
	T[0]=1;
	T[1]=1;
	for(int i=2;i<SIZE;i++){
		T[i]=T[i-1]+T[i-2];
	}
	return 0;
}
int main(int arg, char*args[]) {
	long* T = calloc(SIZE,sizeof(long));
	return Fibonnaci2(T);
}
