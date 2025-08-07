#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#define MAX_TOKENS 100

// Removes newline if it exists in the buffer
void cleaner(char * buffer){
    int i = 0;
    while(buffer[i] != '\n' && buffer[i] != '\0'){
        i += 1; 
    }
    if (buffer[i] == '\n') buffer[i] = '\0';
}

// Signal Handler
void handle_sigchld(int sig){
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void){

    // When a child finishes, call handle_sigchild()
    signal(SIGCHLD, handle_sigchld);
    // Main loop
    while(1){  
        char buffer[1024]; // Buffer to store commands
        char *tokens[MAX_TOKENS]; // Array of command and arguments
        int token_count = 0;
        char curr_dir[1024]; // Buffer to store the current directory
        int background = 0; // Background process flag

        // Get the current directory
        if(getcwd(curr_dir, 1024) == NULL){
            perror("getcwd");
        }
        printf("%s> ", curr_dir);

        // Stores the command into a buffer
        if(fgets(buffer, sizeof(buffer), stdin) == NULL){
            break;
        }
        cleaner(buffer);

        // First token
        char *token = strtok(buffer, " ");
        
        // Putting all the tokens in an array
        while (token != NULL && token_count < MAX_TOKENS){
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, " ");
        }

        // for (int i = 0; i < token_count; i++){
        //     printf("[%d] %s ",i, tokens[i]);
        // }

        // Make the last element of the array NULL
        tokens[token_count] = NULL;
        token_count += 1;

        if (token_count == 0) continue;
        
        // Check for background process
        if (token_count > 0 && strcmp(tokens[token_count - 2], "&") == 0){
            background = 1;
            tokens[token_count - 2] = NULL; // Remove the "&" so that it doesn't get parsed in exexvp()
        }


        // Check for exit
        if (strcmp(tokens[0], "exit") == 0){
            exit(0);
        }
        // check for cd
        else if (strcmp(tokens[0], "cd") == 0){
            if (tokens[1] == NULL){
                fprintf(stderr, "cd: missing arguments");
            }
            else if (chdir(tokens[1]) < 0){
                perror("cd");
            }
            continue;
        }

        // Create a child process
        int id = fork();

        // Execute the command in the child process
        if (id == 0){
            if(execvp(tokens[0], tokens) == -1){
                perror("execvp");
                exit(1);
            }
        }

        // Wait for the child process in the parent process
        if (id != 0){
            // User used "&"
            if(background != 1){
                waitpid(id, NULL, 0);
            }
            else{
                while (waitpid(-1, NULL, WNOHANG) > 0);
            }
        }

        // for (int i = 0; i < token_count; i++){
        //     printf("[%d] %s ",i, tokens[i]);
        // }
        
    }
}