#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>
#include <dirent.h>
using namespace std;
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define LIMIT 256 // max number of tokens for a command
#define MAXLINE 1024
#define min(a, b) a<b?a:b

static char* currentDirectory;
char history[1024][1024];
int number;

void history_(char* args[]){

    if(args[1]){
       int i = 0;
       while(i < (min(atoi(args[1]), number - 1))) {
            printf("%s", history[number - 2 - i]);
            i++;
       }
    }
    else{
        int i = 0;
        while (i < number - 1) {
           printf("%s", history[number - 2 - i]);
           i++;
        }
    }
}

void welcomeScreen() {
    printf(ANSI_COLOR_RED "\n\t============================================\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "\t             NEW SHELL:\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED "\t============================================\n" ANSI_COLOR_RESET);
    printf("\n\n");
}

void shellPrompt() {
    // We print the prompt in the form "<user>@<host> <cwd> >"
    char hostn[1204] = "";
    gethostname(hostn, sizeof(hostn));
    
    printf(ANSI_COLOR_GREEN "%s:" ANSI_COLOR_RESET, hostn);
    
    printf(ANSI_COLOR_BLUE    "~%s>> "    ANSI_COLOR_RESET , getcwd(currentDirectory, 1024));
 
}

int changeDirectory(char *args[]) {
    // If we write no path (only 'cd'), then go to the home directory
    if (args[1] == NULL) {
        chdir(getenv("HOME"));
        return 1;
    }
    // Else we change the directory to the one specified by the
    // argument, if possible
    else {
        if (chdir(args[1]) == -1) {
            printf(" %s: no such directory\n", args[1]);
            return -1;
        }
    }
    return 0;
}

void ls_(char* args[]){
     int count, i;
     struct dirent **files;
     char pathname[1000];
     getcwd(pathname, 1024);
     count = scandir(pathname, &files, NULL, alphasort);

     /* If no files found, make a non-selectable menu item */
     if (count <= 0) {
          perror("No files in this directory\n");
          
     }
        for (i = 0; i < count; ++i)
            printf("%s\n", files[i]->d_name);
}

void mkdir_(char* args[]){
     
     if(args[1]==NULL){
       printf("mkdir: missing operand");
       return;
     }
     int i=2;
     char name[100];
     strcpy(name, args[1]);
     while(args[i]!=NULL){
        strcat(name, " ");
        strcat(name, args[i]);
        i++;
     }
     
     char path[1024];
     getcwd(path, 1024);
     strcat(path, "/");
     strcat(path, name);
       
     if(mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1)
        printf("Error");
     
}

int removeFile(char *file) {
    int ret = remove(file);
    if (ret == 0) {
        printf("File deleted successfully\n");
    } else {
        printf("Error: unable to delete the file\n");
    }
}

int removeDirectory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;
    if (d) {
        struct dirent *p;
        r = 0;
        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;
            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf = (char*)malloc(len);
            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = removeDirectory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r) {
        r = rmdir(path);
        printf("Directory Deleted Successfully\n");
    }
    return r;
}

int rmdir_(char *args[]){
    int r = rmdir(args[1]);
    printf("Directory Deleted Successfully\n");
    return r;
}

int commandHandler(char *args[]) {
    int i=0;
    
    if(strcmp(args[0], "exit") == 0) exit(0);
    
    else if (strcmp(args[0], "clear") == 0) system("clear");
    
    else if (strcmp(args[0], "cd") == 0) changeDirectory(args);
    
    else if (strcmp(args[0], "ls") == 0) ls_(args);
    
    else if (strcmp(args[0], "history") == 0) history_(args);
    
    else if (strcmp(args[0], "mkdir") == 0) mkdir_(args);
    
    else if (strcmp(args[0], "rmdir") == 0) rmdir_(args);
    
    else return (strcmp(args[0], "rm") == 0) rm_(args);
   
}
