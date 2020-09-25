#include<iostream>
#include "util.h"
using namespace std;
#define LIMIT 256 // max number of tokens for a command
#define MAXLINE 1024

int main() {
    char line[MAXLINE]; // buffer for the user input
    char *tokens[LIMIT]; // array for the different tokens in the command
    int numTokens;
    
    welcomeScreen();
    char currentDirectory[100];
    
    // We set shell=<pathname>/simple-c-shell as an environment variable for
    // the child
    setenv("shell", getcwd(currentDirectory, 1024), 1);

    // Main loop, where the user input will be read and the prompt
    // will be printed
    number = 0;
    while (1) {
        // We print the shell prompt if necessary
       shellPrompt();
        // We empty the line buffer
        memset (line, '\0', MAXLINE);

        // We wait for user input
        fgets(line, MAXLINE, stdin);
        for(int k = 0; k < MAXLINE; k++){
           history[number][k] = line[k];
        }
        number++;
        
        // If nothing is written, the loop is executed again
        if ((tokens[0] = strtok(line, " \n\t")) == NULL) continue;
        
        // We read all the tokens of the input and pass it to our
        // commandHandler as the argument
        numTokens = 1;
        
        while ((tokens[numTokens] = strtok(NULL, " \n\t")) != NULL){
           numTokens++; 
         }
        commandHandler(tokens);
        //printf("\n");
    }
    
    exit(0);
}
