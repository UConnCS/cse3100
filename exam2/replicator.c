#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void write_message(char* cmd, int copies, char* fname) {

   fprintf(stderr, "Writing command output %d times to file %s\n", copies, fname);

   int fdes = open(fname, O_WRONLY | O_TRUNC | O_CREAT, 0644);
   dup2(fdes, 1);

      for(int i=0; i<copies; i++) {
             if( fork() == 0) {

         execlp(cmd, cmd, NULL);
         exit(1); // should not be here

      }
   }
    while(wait(NULL) > 0);

   close(fdes);
   return;
}

int main(int argc, char *argv[]) {

   char* cmd = argv[1];

   for(int j=2; j<argc; j++) {
      write_message(cmd, j-1, argv[j]);
   }

   for(int i=2; i<argc; i++) {
      fprintf(stderr, "File %s:\n", argv[i]);

      FILE* f_out = fopen(argv[i], "r");

      char ch;
      while((ch=fgetc(f_out))!=EOF) putc(ch, stderr);

      fclose(f_out);

   }


   return 0;
}