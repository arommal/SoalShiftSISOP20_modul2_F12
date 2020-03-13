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

int main(char *argv[]){
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

    int mode;

    if(argv[1][1] == 'a'){
        mode = 1;
    }else if(argv[1][1] == 'b'){
        mode = 2;
    }else{
        mode = 0;
    }

    while(1){
        char folder[25], dirfolder[30];
        pid_t n1, n2, n3, n4;
        int status;

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        strftime(folder, 25, "%Y-%m-%d_%X", tm);
        sprintf(dirfolder, "./%s", folder);

        n1 = fork();

        if(n1 == 0){
            char *arg1[] = {"mkdir", "-p", folder, NULL};
            execv("/bin/mkdir", arg1);
        }

        wait(&status);
        
        n2 = fork();

        if(n2 == 0){
            int i = 0;

            chdir(folder);

            for(i;i<20;i++){
                char dl[100], file[25];
                time_t t2;
                time(&t2);
                struct tm *tm2 = localtime(&t2);

                strftime(file, 25, "%Y-%m-%d_%X", tm2);
                sprintf(dl, "https://picsum.photos/%ld", ((t2%1000)+100));

                n3 = fork();

                if(n3 < 0){
                    exit(1);
                }else if(n3 == 0){
                    execl("usr/bin/wget", "wget", dl, (char*)0);
                    exit(EXIT_SUCCESS);
                }else{
                    wait(NULL);
                }
                sleep(5);
            }
            while(wait(&status) > 0);

            // chdir("..");
            // char zipname[100];
            // sprintf(zipname, "%s.zip", file);

            // n4 = fork();

            // if(n4 == 0){
            //     char *arg3[] = {"zip", "-r", zipname, folder, NULL};
            //     execv("/bin/zip", arg3);
            // }
            // while(wait(&status) > 0);
            
            // char *arg4[] = {"rm", "-r", folder, NULL};
            // execv("/bin/rm", arg4);
        }
        sleep(30);
    }
}