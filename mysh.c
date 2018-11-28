/*
	MyShell program
	Ferencz Dominguez
	Nov 7 2018
	Custom shell program that runs /bin and /sbin commands
	plus custom commands	
*/

/* Prototypes */
void cmdSel(char ** command);
void myPrint(char** buffer);
void myHelp();
void clear();

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>

/* Constants */
#define BIN "/bin/"
#define SBIN "/sbin/"
#define SPACE " " 

/* Global variable */
int* status;


int main(){ 
 
 char selection[255];
 int i=0;
 char* buffer[255];
 char* token;
 
 clear();	// Clear screen

 while (1){	// Infinite loop
	
	printf("Escribe tu comando: ");	// Get command from user
	gets(selection); 
	
	if (strstr(selection,"/") || strstr(selection,"..")){ // Make sure user can't change directories
		printf("Can't use that command!\n");
		sprintf(selection, " ");
	}

	token = strtok(selection, SPACE);	// Get first command
 	buffer[i] = token;	// Store it
	
	while (token != NULL){	// Gather all extra arguments if any
		token = strtok(NULL, SPACE);
		i++;
		buffer[i] = token;
	}// https://www.tutorialspoint.com/c_standard_library/c_function_strtok.html

	i=0;	// Reset counter

	cmdSel(buffer);	// Function that decides which command to run
	
	memset(buffer, 0, sizeof(buffer)); // Reset arrays
	memset(selection, 0, sizeof(selection));	
 }

}

/*********************************************************
   Name: cmdSel()
   Input: array of strings with main command at index 0 and parameterds in extra indexes
   Functionality: Take user input and run custom command, from /bin or from /sbin
*********************************************************/
void cmdSel(char ** cmd){
	
	pid_t p; 
	pid_t mypid = getpid();
	char command[255];

	p = fork();	// Get process id - Fork and exec code from Ig Kolenko's notes and adapted by me

	if (p == -1){	// Error with fork
		printf("Error - fork failed!\n");
		exit(0);
	}

	else if (p == 0){	// Exec command as child

		if (strcmp(cmd[0], "myprint")==0){	// MyPrint command
	
			myPrint(cmd);

		}else if (strcmp(cmd[0], "myhelp")==0){	// MyHelp command
			
			myHelp();

		}else if (strcmp(cmd[0], "clear") ==0){ // Clear screen

			clear();

		}else if (strcmp(cmd[0], "mystop")==0){ // Mystop command
		
			printf("\nGracias vuelva pronto!\n\n");
			kill(p, SIGKILL); // Kill child process
			kill(mypid, SIGKILL); // Kill parent process
			
			
		}else { // Bin and sbin commands

		sprintf(command, "%s%s", BIN, cmd[0]);	// Make command into one string ex /bin/ls

		if (execvp(command, cmd) == -1){	// If not /bin go to /sbin
			
			sprintf(command, "%s%s", SBIN, cmd[0]);	// Use /sbin
			if (execvp(command, cmd) == -1){ // If not /sbin then program is not in such directories nor is custom
				printf("Command doesn't exist. Type myhelp to see instructions.\n");
			}
		}
	    }
	} else {
		waitpid(p, status, 0);	// Wait for child process to finish
	}
}

/*
   Name: myPrint
   Input: array of strings where buffer[0] is the command and the rest are arguments 
   Functionality: Print given string 
*/

void myPrint(char** buffer){

	int i = 1;	// Skip command string

	while(buffer[i] != NULL){
		printf("%s ", buffer[i]);	// Print arguments
		i++;
	}
	printf("\n\n");
	return;
}

/*
   Name: myHelp 
   Functionality: Print help information 
*/

void myHelp(){

	printf("\n");
	printf("----------------------------------- Sobre MiShell -----------------------------------------");
	printf("\n\n");
	printf("MiShell is a custom shell build on C that runs commands from /bin /sbin and any custom function inside this shell");
	printf("\n");
	printf("Commands:\n");
	printf("\t-myprint: \t myprint function will print to the shell any given string EX myprint Hola como estas?");
	printf("\n");
	printf("\t-myhelp: \t myhelp function displays this instructional menu");
	printf("\n");
	printf("\t-mystop: \t mystop sends a signal to this shell in order to exit the program");
	printf("\n");
	printf("\t-clear: \t clear shell screen");
	printf("\n");
	printf("\t-/BIN commands: Any command that is not custom and is located in /bin will be run including its arguments");
	printf("\n");
	printf("\t For mor information on /bin commands visit: https://www.tldp.org/LDP/Linux-Filesystem-Hierarchy/html/bin.html");
	printf("\n");
	printf("\t-/SBIN commands: Any command that is not custom and is located in /sbin will be run including its arguments");
	printf("\n");
	printf("\t For mor information on /sbin commands visit: http://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/sbin.html");
	printf("\n\n");

}

/*
   Name: clear()
   Functionality: Clear shell screen 
*/
void clear(){
	system("tput clear");	// Clear screen 
	printf("Bienvenido a MiShell!\n\n");	// Welcome message
}

