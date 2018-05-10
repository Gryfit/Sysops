#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int
main(int argc, char *argv[])
{
   int         i;
   struct stat   buf;
   char      *tekst;

   for (i = 1; i < argc; i++) {
      printf("%s: ", argv[i]);
      if (lstat(argv[i], &buf) < 0) {
         printf("lstat error");
         continue;
      }
      switch (buf.st_mode & S_IFMT) {
          case S_IFREG:
            tekst = "zwykly plik";
            break;
         case S_IFDIR:
            tekst = "katalog";
            break;
        case S_IFLINK:
            tekst = "link symboliczny";
            break;
        default:
            tekst = "**** cos innego !!! ****";
            break;
      }
      printf("%s\n", tekst);
   }
   exit(0);
}
