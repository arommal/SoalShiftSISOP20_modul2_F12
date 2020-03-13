#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() 
{ 
    int pid, pid1, pid2, pid3, pid4, status; 
  
    pid = fork(); 
  
    if (pid == 0) { 
  
        // First 
        // printf("1\n");
        char *argv[] = {"mkdir", "indomie", "sedaap", NULL};  
        execv("/bin/mkdir", argv);
    } 
  
    else { 
        while ((wait(&status))>0);
        pid1 = fork(); 
        if (pid1 == 0) { 
            // // second
            // printf("2\n");
            char *argv[] = {"unzip", "jpg.zip", NULL}; 
            execv("/usr/bin/unzip", argv);
        } 
        else { 
            while ((wait(&status))>0);
            // sleep(1);
            pid2 = fork(); 
            if (pid2 == 0) { 
                // third
                // printf("3\n");

                char *argv[] = {"find", "/home/insanity/Modul2/jpg/.","-type","d", "-exec", "mv", "{}", "/home/insanity/Modul2/indomie",";" ,NULL};
                execv("/usr/bin/find", argv);
            } 
            else { 
              while ((wait(&status))>0);
              pid3 = fork();
              if (pid3 == 0){
                // printf("4\n");
                char *argv[] = {"find","/home/insanity/Modul2/jpg/.", "-type", "f","-exec","mv","{}","/home/insanity/Modul2/sedaap",";", NULL}; 
                execv("/usr/bin/find", argv);
              }
              else{
                    while ((wait(&status))>0);
                    pid4 = fork();
                    if (pid4 == 0){
                        //fifth
                        // printf("5\n");
                        char *argv[] = {"find", "/home/insanity/Modul2/indomie/.","-mindepth","1","-maxdepth","1","-type","d", "-exec", "touch", "{}/coba1.txt ",";", NULL}; 
                        execv("/usr/bin/find", argv);
                    }
                    else{
                        while ((wait(&status))>0);
                        sleep(3);
                        // printf("6\n");
                        char *argv[] = {"find", "/home/insanity/Modul2/indomie/.","-mindepth","1","-maxdepth","1","-type","d", "-exec", "touch", "{}/coba2.txt ",";", NULL}; 
                        execv("/usr/bin/find", argv);
                    }
                }
            } 
        } 
    } 
  
    return 0; 
} 