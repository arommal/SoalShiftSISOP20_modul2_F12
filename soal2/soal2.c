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
            for(;i<2;i++){
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
            while(wait(&status) > 0);

            n4 = fork();

            if(n4 == 0){
                char zipname[100];
                sprintf(zipname, "%s.zip", dirfolder);
                chdir("..");
                char *arg3[] = {"zip", "-r", zipname, dirfolder, NULL};
                execv("/usr/bin/zip", arg3);
            }else{
                while(wait(&status) > 0);
                
                char *arg4[] = {"rm", "-r", folder, NULL};
                execv("/bin/rm", arg4);
            }
            
        }
        sleep(30);
        
    }

}