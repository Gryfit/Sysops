#define _XOPEN_SOURCE 500
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>

struct date {
    int day;
    int mon;
    int year;
};


struct date MDATE;
char* COMP;


/*
              Read?              Write?         	Execute?
Owner 	S_IRUSR = 1 << 8 	S_IWUSR = 1 << 7 	S_IXUSR = 1 << 6
Group 	S_IRGRP = 1 << 5 	S_IWGRP = 1 << 4 	S_IXGRP = 1 << 3
Other 	S_IROTH = 1 << 2 	S_IWOTH = 1 << 1 	S_IXOTH = 1 << 0
*/

void permissions(mode_t mode, char * buf) {
  const char chars[] = "rwxrwxrwx";
  for (int i = 0; i < 9; i++) {
    buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-'; // w ten sposob odczytujemy poszczegolne bity odpowiadajace prawom dostepu
  }
  buf[9] = '\0';
}

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    char buf[10];
	if(S_ISDIR(sb->st_mode) || S_ISLNK(sb->st_mode)){
		return 0;
	}
    time_t stamp = sb->st_mtime; 
    struct tm T = *localtime(&stamp);
    permissions(sb->st_mode,buf);
    T.tm_mon++;
    T.tm_year+=1900;
    
    if(strcmp(COMP, "=")){
        if(MDATE.day==T.tm_mday && MDATE.mon==T.tm_mon && MDATE.year==T.tm_year) {
            printf("%s   %d %d %d \t %jd \t %s\n",
                buf,T.tm_year, T.tm_mon, T.tm_mday, sb->st_size, fpath);
        }
    }
    if(strcmp(COMP,"<")){
        if( (MDATE.day<T.tm_mday && MDATE.mon==T.tm_mon  && MDATE.year==T.tm_year) || (MDATE.mon<T.tm_mon  && MDATE.year==T.tm_year) || MDATE.year<T.tm_year) { //y<y || m<m && y=y || d<d && m=m y=y
            printf("%s   %d %d %d \t %jd \t %s\n",
                buf,T.tm_year, T.tm_mon, T.tm_mday, sb->st_size, fpath);
        }
    }
    if(strcmp(COMP,">")){
        if((MDATE.day>T.tm_mday && MDATE.mon==T.tm_mon  && MDATE.year==T.tm_year) || (MDATE.mon>T.tm_mon  && MDATE.year==T.tm_year) || MDATE.year>T.tm_year) { 
            printf("%s   %d %d %d \t %jd \t %s\n",
                buf,T.tm_year, T.tm_mon, T.tm_mday, sb->st_size, fpath);
        }
    }
    return 0; 
}


static int my_nftw(char *path, int (*fn)(const char *, const struct stat *, int, struct FTW *), int fd_limit){
    size_t len = strlen(path);
	struct stat st;
	int ok = 0;
	
	if (lstat(path, &st) == -1) {
		return -1;
	}else{
	    if (S_ISDIR(st.st_mode) != 0 && access(path, R_OK) >= 0){
	        ok = 1;
	    }
	}
    
	int r=0;
	if (ok == 1 && fd_limit > 0) {
       pid_t pid = fork(); 
        if(pid<0){
        	return -1;
		}
		if(pid!=0) return 0;
		DIR *d = opendir(path);
		if (d != NULL) {
			struct dirent *de;
			while ((de = readdir(d))) {
				if(strcmp(de->d_name,"..")!=0 && strcmp(de->d_name,".")!=0){
				    path[len]='/';
				    strcpy(path+len+1, de->d_name);
				    r=my_nftw(path, fn, fd_limit-1);
				}
			}
			closedir(d);
		}
	}
	struct FTW ftw_str;
	path[len]=0;
	r=fn(path, &st, 0, &ftw_str); // 0 i ftw_str jest dla kompatybilnosci z funkcjami fn napisanymi pod prawdziwego nftw
	return r;
}




int ls(char* path, char* mode){
    char resolved_path[PATH_MAX+1]; 
    int r;
    realpath(path, resolved_path);
    if(strcmp(mode, "lib")==0){
        r = nftw(resolved_path,display_info,20, FTW_PHYS | FTW_DEPTH);
    }
    if(strcmp(mode, "sys")==0){
    	r = my_nftw(resolved_path, display_info, 20);
    }
	return r;
}



int main(int arg, char*args[]) {
    if (arg == 1){
        printf("./main [path] [comparator] [day] [mon] [year] [mode]\n"
               "comparator: \'<\' \'=\' \'>\'\n"
               "mode: sys -> use system function, lib -> use nftw\n"
                );
        return 0;
}
    MDATE.day = atoi(args[3]);
    MDATE.mon = atoi(args[4]);
    MDATE.year = atoi(args[5]);
        
    COMP = args[2];
        
    return ls(args[1], args[6]);
}

