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
#include <wait.h>
#include <dirent.h>

void generateKillerProgram(char mode[]){
    FILE *program = fopen("killerprogram.sh", "w");
    pid_t n1, n2;
    int status;

    if(strcmp(mode, "-a") == 0){
        fprintf(program, "#!/bin/bash\nkillall -9 soal2\nrm \"$0\"");
    }else if(strcmp(mode, "-b") == 0){
        fprintf(program, "#!/bin/bash\nkill %d\nrm \"$0\"", getpid());
    }

    fclose(program);

    n1 = fork();

    if(n1 == 0){
        char *arg1[] = {"chmod", "+x", "killerprogram.sh", NULL};
        execv("/bin/chmod", arg1);
    }
}

int main(int argc, char **argv){
    pid_t pid, sid;

    pid = fork();

    if(pid < 0){
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if(sid < 0){
        exit(EXIT_FAILURE);
    }
   
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if(argc != 2){                                      // pre-req
        exit(EXIT_FAILURE);
    }

    generateKillerProgram(argv[1]);

    while(1){

        // generateKillerProgram(argv[1]);

        char folder[25], dirfolder[30];
        pid_t n1, n2, n3, n4;
        int status;

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        strftime(folder, 25, "%Y-%m-%d_%X", tm);

        n1 = fork();

        if(n1 < 0){
            exit(EXIT_FAILURE);
        }
        
        if(n1 == 0){
            char *arg1[] = {"mkdir", "-p", folder, NULL};
            execv("/bin/mkdir", arg1);
        }

        while(wait(&status) > 0);
        
        n2 = fork();

        if(n2 == 0){
            int i = 0;
            char dl[100], file[25];
            for(;i<20;i++){
                time_t t2;
                time(&t2);
                struct tm *tm2 = localtime(&t2);
                strftime(file, 25, "%Y-%m-%d_%X", tm2);
                sprintf(dl, "https://picsum.photos/%ld", ((t2%1000)+100));

                n3 = fork();

                if(n3 == 0){
                    char *arg2[] = {"wget", "-O", file, dl, NULL};
                    chdir(folder);
                    execl("/usr/bin/wget", "wget", "-O", file, dl, NULL);
                }
                sleep(5);
            }
            
            char zipname[100];
            sprintf(zipname, "%s.zip", folder);
            char *arg3[] = {"zip", "-rm", zipname, folder, NULL};
            execv("/usr/bin/zip", arg3);
  
        }
        sleep(30);
    }
}