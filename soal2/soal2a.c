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

int main(){
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
        char a[25];
        pid_t child_id;
        int status;

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);

        strftime(a, 25, "%Y-%m-%d_%X", tm);

        child_id = fork();

        if(child_id == 0){
            char *argv[] = {"mkdir", "-p", a, NULL};
            execv("/bin/mkdir", argv);
        }
        wait(&status)
        
        child_id = fork();
        

        sleep(30);
    }
}