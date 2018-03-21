#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>


int generate(char* path, int records, int size){
    srand(time(NULL));
    FILE* file = fopen(path,"wt");
    if(file==NULL){
	perror("Fopen failed.\n");
	return 1;
    }
    char * tab = calloc(size,sizeof(char));
    if(tab==NULL){
	perror("Calloc failed.\n");
	return 1;
    }
    for(int i=0;i<records;i++){
	for(int i=0;i<size-1;i++){
            tab[i]=(char)(rand()%94 +33);
        }
        tab[size-1]='\n';
        int tmp =fwrite(tab,1,size,file);
        if(tmp != size){
            perror("Write failed.\n");
	    return 1;
	}
    }
    free(tab);
    fclose(file);
    return 0;
}
int copy( char* file1, char* file2, int records, int size, char* mode){
    if(strcmp(mode,"sys")==0){
        char* c = calloc(size, sizeof(char));
        if(c==NULL){
	    perror("Calloc failed.\n");
	    return 1;
        }
        int we=open(file1, O_RDONLY);
        int wy=open(file2, O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
        if(we==-1){
	    perror("open on file1 failed.\n");
	    return 1;
        }
        if(wy==-1){
	    perror("open on file2 failed.\n");
	    return 1;
        }
        for(int i=0;i<records;i++){
            if(read(we,c,size)!=size){
                perror("read from file1 failed.");
                return 1;
            }
            write(wy, c, size);
        }
        close(we);
        close(wy);
        free(c);
    }
    if(strcmp(mode,"lib")==0){
        char* c;
        FILE * we, *wy;
        if(!(c = calloc(size, sizeof(char)))){
	        perror("Calloc failed.\n");
	        return -1;
        }
        if(!(we=fopen(file1, "r"))){
	        perror("open on file1 failed.\n");
            free(c);	        
            return -1;
        }
        if(!(wy=fopen(file2, "wt"))){
	        perror("open on file2 failed.\n");
            fclose(we);
            free(c);
	        return -1;
        }
        for(int i=0;i<records;i++){
            if(!fread(c,sizeof(char),size, we)){
                perror("read from file1 failed.");
                fclose(we);
                fclose(wy);
                free(c);
                return -1;
            }
            fwrite(c, sizeof(char), size, wy);
        }
        fclose(we);
        fclose(wy);
        free(c);
    }
    return 0; 
}

int swap(FILE * file,int filesys, int size, char* mode, long i, long j){
    char * buf1;
    char * buf2;
    if(!(buf1 = calloc(size, sizeof(char)))){
        printf("calloc failed\n");
        return -1;
    }
    if(!(buf2 = calloc(size, sizeof(char)))){
        printf("calloc failed\n");
        return -1;
    }
    if(strcmp(mode,"lib")){
        fseek(file,i*size,0);
        fread(buf1, sizeof(char), size, file);
        fseek(file,j*size,0);
        fread(buf2, sizeof(char), size, file);
        fseek(file,j*size,0);
        fwrite(buf1, sizeof(char), size, file);
        fseek(file,i*size,0);
        fwrite(buf2, sizeof(char), size, file);
    }
    if(strcmp(mode,"sys")){
        lseek(filesys,i*size,SEEK_SET);
        read(filesys,buf1, size);
        lseek(filesys,j*size,SEEK_SET);
        read(filesys,buf2, size);
        lseek(filesys,j*size,SEEK_SET);
        write(filesys,buf1, size);
        lseek(filesys,i*size,SEEK_SET);
        write(filesys,buf2, size);
    }
    free(buf1);
    free(buf2);
    return 0;
}
int condition(FILE * file, int filesys, int size, char * mode, long i, long j){
    char a, b;
    if(strcmp(mode,"lib")){    
        fseek(file,i*size,0);
        fread(&a, sizeof(char), 1, file);
        fseek(file,j*size,0);
        fread(&b, sizeof(char), 1, file);
    }
    if(strcmp(mode,"sys")){
        lseek(filesys,i*size,SEEK_SET);
        read(filesys,&a, 1);
        lseek(filesys,j*size,SEEK_SET);
        read(filesys,&b, 1);
    }
    return ((int)a-(int)b);
}
int sort(char *file, int records, int size, char *mode){
    long i,j;
    if(strcmp(mode,"lib")){    
        FILE * handle;
        if(!(handle = fopen(file, "r+"))){
	        return -1;
        }
        for(i=1;i<records;i++){
            for(j=i;j>0 && condition(handle,0,size,mode,i,j)>=0;j--){
                if(swap(handle,0,size,mode,j-1,j)){
                    fclose(handle);
                    printf("swap failed\n");
                    return -1;  
                }
            }
        }    
        fclose(handle);
    }
    if(strcmp(mode,"sys")){
        int handle;
        if((handle = open(file, O_RDWR))==-1){
	        return -1;
        }
        for(i=1;i<records;i++){
            for(j=i;j>0 && condition(NULL,handle,size,mode,i,j)>=0;j--){
                if(swap(NULL,handle,size,mode,j-1,j)){
                    close(handle);
                    printf("swap failed\n");
                    return -1;  
                }
            }
        }    
        close(handle);
    }
    return 0; 
}

int main(int arg, char*args[]) {
    struct rusage usage_start, usage_end;
    clock_t start_real,end_real;
    int out=0;
    if(strcmp(args[1],"generate")==0){
        return generate(args[2], atoi(args[3]), atoi(args[4]));
    }
    if(strcmp(args[1],"copy")==0){
        start_real = clock();
        getrusage(RUSAGE_SELF, &usage_start);
//--------------Czas jest mierzony
        out = copy(args[2], args[3], atoi(args[4]), atoi(args[5]), args[6]);
//--------------Koniec mierzenia czasu
        getrusage(RUSAGE_SELF, &usage_end);
        end_real = clock();
    }
    if(strcmp(args[1],"sort")==0){
        start_real = clock();
        getrusage(RUSAGE_SELF, &usage_start);
//--------------Czas jest mierzony
        out = sort(args[2], atoi(args[3]), atoi(args[4]), args[5]);
//--------------Koniec mierzenia czasu
        getrusage(RUSAGE_SELF, &usage_end);
        end_real = clock();
    }
//WYPISIWANIE CZASU
    double real = (double)(end_real-start_real)/CLOCKS_PER_SEC;
    double user = (usage_end.ru_utime.tv_sec-usage_start.ru_utime.tv_sec)+(double)(usage_end.ru_utime.tv_usec-usage_start.ru_utime.tv_usec)/1000000;

    double sys = (usage_end.ru_stime.tv_sec-usage_start.ru_stime.tv_sec)+(double)(usage_end.ru_stime.tv_usec-usage_start.ru_stime.tv_usec)/1000000;
    printf("real time: %f s\n"
                   "user time: %f s\n"
                   "system time: %f s\n",real,user,sys);

    return out;
}
