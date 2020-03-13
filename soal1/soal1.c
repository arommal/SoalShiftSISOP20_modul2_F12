#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int flag=0;
  if ((strcmp(argv[1],"*") == 0 || (atoi(argv[1])>=0 && atoi(argv[1])<=59 && argv[1][0] >= '0' && argv[1][0] <= '5' )) &&
      (strcmp(argv[2],"*") == 0 || (atoi(argv[2])>=0 && atoi(argv[2])<=59 && argv[2][0] >= '0' && argv[3][0] <= '5' )) &&
      (strcmp(argv[3],"*") == 0 || (atoi(argv[3])>=0 && atoi(argv[3])<=23 && argv[3][0] >= '0' && argv[2][0] <= '2' ))
      ) flag = 1;

  if (flag==1){
    pid_t pid, sid;        // Variabel untuk menyimpan PID

    pid = fork();     // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
    * (nilai variabel pid < 0) */
    if (pid < 0) {
      exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
    * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) {
      exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0) {
      exit(EXIT_FAILURE);
    }

    // if ((chdir("/")) < 0) {
    //   exit(EXIT_FAILURE);
    // }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while (1) {

        time_t now;
        struct tm *tm_struct;
        now = time(NULL);
        tm_struct=localtime(&now);
        int hour = tm_struct->tm_hour;
        int minute = tm_struct->tm_min;
        int second = tm_struct->tm_sec;
        char sh[3], sm[3], ss[3];
        sprintf(sh,"%d",hour); sprintf(sm,"%d",minute); sprintf(ss,"%d",second);
      if (strcmp(argv[3],"*") == 0|| strcmp(argv[3],sh)== 0) {
            printf("jam ");
            if (strcmp(argv[2],"*")  == 0|| strcmp(argv[2],sm)== 0) {
                printf("menit ");
                if (strcmp(argv[1],"*")  == 0|| strcmp(argv[1],ss) == 0){
                    if (fork()==0){
                      char *arg[] = {"bash", argv[4], NULL};
                      execv("/bin/sh", arg);
                        // char *arg[]={"bash", path, NULL};
                        // execv("/bin/bash", arg);
                        //  char *arg[]={"touch", "kekw.txt", NULL};
                        // execv("/usr/bin/touch", arg);
                    }
                }
            }
        }
      // Tulis program kalian di sini
      // printf ("\n");
      sleep(1);
      }
    }

  else printf("Error, argumen yang diberikan tidak sesuai\n");
  
}