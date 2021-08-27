/*
Kaden Carter, Alejandro Figueroa, Jarrett Lumsden
Operating Systems Project
Dr. Sun
COSC 4302
7/30/2021

This project was done using C as the programming language. It is a shell that is ran through the linux OS 
and executes linux commands. It also includes a help command that helps the user.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "minishell.h"

//Protoypes
char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
int parsePath(char **);
void printPrompt();
void readCommand(char *);
void helpCommand();


int main(){

    /* Shell initialization */
    //initialization for the struct that is in "minishell.h".
    struct command_t command;
    
    //initialization of the commandLine variable which stores the user input as a string.
    char *commandLine = (char *)malloc(LINE_LEN);
    
    //initialization of the pathv variable which contains the paths for the commands.
    char **pathv = (char **)malloc(sizeof(char)*MAX_PATHS);

    while(1) {

        //invokes the parsePath method which reads the PATH variable for this environment, 
        //then builds an array, dirs[], of the directories in PATH
        parsePath(pathv);

        //prints the prompt to the user from the printPrompt method.
        printPrompt();

        //reads the command from the user.
        readCommand(commandLine);

        //parses the command from the user.
        parseCommand(commandLine, &command);

        //uses lookupPath to get the path name for the command.
        command.name = lookupPath(command.argv, pathv);

        //checks if the user inputs the "help" command and invokes helpCommand.
        if(strcmp(command.argv[0], "help") == 0){
            helpCommand();
            command.name = "";
        }

        //checks if the user inputs the "quit" or "exit" command, prints a closing message and breaks.
        if (strcmp(command.argv[0], "quit") == 0 || strcmp(command.argv[0], "exit") == 0) {
            printf("Closing program.\n");
            break;
        }

        //child process that executes a command.
        if(fork() == 0) {
            execv(command.name, command.argv);
            exit(0);
        }
        
        //parent process waits for child to execute.
        else {
            wait(NULL);
        }
         //displays a warning to the user if the command is not found.
        if(command.name == NULL) {
            printf("Command not found.\n");
            continue;
        }
    }
    //when the user exits the shell this will be printed to let the user know that the shell has terminated successfully.
    printf("Terminating successfully.\n");
    return 0;
}

char *lookupPath(char **argv, char **directories) {
/* This function was created to search the directories using the arguement of dir.
We then created a new string variable that will have the full path name in it, 
then return said string. */

    //We create some variables to assist with the work.
    int i; //Variable of the for-loop.
    int sizeOfPath = (sizeof(*directories) / sizeof(char)); 
    char *pathOfFile[CHAR_MAX]; 
    strcpy(pathOfFile, ""); 

    //We check to see if the name of the file is already within the absolute path.
    if (*argv[0] == '.' || *argv[0] == '/') {
        return *argv;
    }

    /*We then use this for-loop to look at the path directories. 
    While using the access() keyword. */
    for (i = 0; i < sizeOfPath - 1; i++) {
        directories++;

        //We concate our strings.
        strcat(pathOfFile, *directories);
        strcat(pathOfFile, "/");
        strcat(pathOfFile, argv[0]);

        //If the results is 0, then there is an executable file.
        if (access(pathOfFile, F_OK) == 0) {
            return &pathOfFile;
        }

        //We then clear the char array.
        memset(pathOfFile, 0, CHAR_MAX);
		char pathOfFile[CHAR_MAX] = "";
    }
    
    //If the file name is not found, then return this.
    return NULL;
}

int parseCommand(char *commandLine, struct command_t *command) {
    /* This function was created to parse the commands within 
    the shell and set them up for later use. */

    //We create some variables to assist with the work.
    int argc;
    char **controlPointer;

    //We links the variables and begin to allocate space for them.
    controlPointer = &commandLine;
    argc = 0;
    command->argv[argc] = (char *) malloc(MAX_ARG_LEN);

    //We work through the string and parse it so that it can go into "argv".
    while((command -> argv[argc] = strsep(controlPointer, WHITESPACE)) != NULL) {
        command -> argv[++argc] = (char *) malloc(MAX_ARG_LEN);
    }

    //Placing the first element within the command name.
    command -> argc = argc -1;
    command -> name = (char *) malloc(sizeof(command -> argv[0]));
    strcpy(command -> name, command -> argv[0]);

    return 1;
}

int parsePath(char **directories) {
    /*This function will read the path of the variables that are within the envirnoment.
    Then it will build an array called directories[], for the directories in the path.*/

    //We create some variables to assist with the work.
	char *pathEnvirnomentVariable;
	char **thePath;
	const char delimiter[2] = ":";
	
    //We wipe the directories[] and setting it to null.
	while(*directories++) {
		*directories = NULL; 
	} 
	
    //Getting the envirnoment path and copying it to the PATH.
	pathEnvirnomentVariable = (char *)getenv("PATH"); 
	thePath = (char *) malloc(strlen(pathEnvirnomentVariable) + 1);
	strcpy(thePath, pathEnvirnomentVariable);
	
    //Creating the token and making it the size of the "pathEnvirnomentVariable".
    char *token = (char *) malloc(strlen(pathEnvirnomentVariable));
   
    //We get the first token to use.
	token = strtok(thePath, delimiter);
    
    //We then get the other tokens that we will need.
    while(token != NULL) {
	    token = strtok(NULL, delimiter);
	    *directories = token;
	    directories++;
    }

   return 1;
}

void printPrompt() {
    /*This function will build the prompt with the information listed below.*/

    //We create some variables to assist with the work.
    char promptString[CHAR_MAX];
    strcpy(promptString, "");
    char hostName[CHAR_MAX];
    char *currentWorkingDirectory = (char *)malloc(MAX_ARG_LEN);

    //We will retrive the host name for display.
    gethostname(hostName, sizeof(hostName));
    getcwd(currentWorkingDirectory, MAX_ARG_LEN);

    //We then put everything in order to be displayed to the user.
	strcat(promptString, getenv("USER"));
	strcat(promptString, "@");
	strcat(promptString, hostName);
	strcat(promptString, ":");
	strcat(promptString, currentWorkingDirectory);
	printf ("%s", promptString);
	free(currentWorkingDirectory);
	
    //We then wipe the screen.
	fflush(stdout);
}

void readCommand(char* commandLine) {
    /*This funtion was created to read the commands inputted by the user.*/

    //We get the input from the command line from the user.
    fgets(commandLine, LINE_LEN, stdin);
    commandLine[strlen(commandLine) -1] = '\0';
}

void helpCommand() {
    /*This method is for the help command and can be edited by using printf to add more commands to the list.*/

    //This is what will be outputted for the user.
    printf("Here you will find a list of the commands within this shell.\n");
    printf("Type 'help' to bring up a list of commands that can be used.\n");
    printf("Type 'quit' or 'exit' to close the shell.\n");
}
