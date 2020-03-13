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

int main(char *argv){
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

        while(wait(&status) > 0);
        
        n2 = fork();

        if(n2 == 0){
            int i = 0;
            char dl[100], file[25];
            for(;i<20;i++){
                t = time(NULL);
                tm = localtime(&t);
                strftime(file, 25, "%Y-%m-%d_%X", tm);
                sprintf(dl, "https://picsum.photos/%ld", ((t%1000)+100));

                n3 = fork();

                if(n3 == 0){
                    char *arg2[] = {"wget", "-P", dirfolder, dl, NULL};
                    execv("/bin/wget", arg2);
                    exit(EXIT_SUCCESS);
                }
                sleep(5);
            }
            while(wait(&status) > 0);

            chdir("..");
            char zipname[100];
            sprintf(zipname, "%s.zip", file);

            n4 = fork();

            if(n4 == 0){
                char *arg3[] = {"zip", "-r", zipname, file, NULL};
                execv("/bin/zip", arg3);
            }
            while(wait(&status) > 0);
            
            char *arg4[] = {"/bin/rm", "rm", "-r", file, NULL};
            execv("/bin/rm", arg4);
        }

        sleep(30);
    }
}