#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define parameters 2        //the word parameter is set as the value 2
int main(int argc, char** argv, char** env) {                 
	char* path = getenv("PATH");   //searches for the environment string pointed to by name and returns the associated value to the string
	char* tok = strtok(path, ":");  //breaks string in the variable path into a series of tokens using the delimiter ":" and assigns it to tok variable
	char* wd = NULL;         //initialising a char pointer wd as NULL
	while(tok != NULL){        //the code segment inside the while loop is executed until tok variable is unequal to NULL
		if(!strcmp(tok, "/usr/bin")){   //compare tok variable with "/usr/bin" and if it is unequal then execute the following code segment
			wd = malloc(sizeof(char)*20);    //allocate memory space equivalent to 20 char variables
			strcpy(wd, tok);     //copies the content in tok variable to wd variable and returns the pointer at wd variable    
			break;               //break out of the loop
		}
		tok = strtok(NULL, ":");  //breaks string in the NULL into a series of tokens using the delimiter ":" and assigns it to tok variable    
	}
	char* dir = malloc(sizeof(char)*(strlen(wd)+1));    //allocate memory space of size equivalent to length of the wd char variable
	int check_stop = 1;                      //declare an int variable called check_stop and initialise it to 1                       
	while (check_stop == 1) {           //execute the code segment inside the while loop until check_stop variable is equal to 1
		strcpy(dir, wd);               //copies the content in wd variable to dir variable and returns the pointer at dir variable  
		char in_line[200];      //declare a chare array of size 200
		in_line[0] = '\0';  //set the zeroth index of the char array "in_line" as the end of line charachter '\0'
		printf("sh> ");     //display "sh> " to the user (terminal)
		fgets(in_line, 200, stdin);    //reads a line from the specified stdin and stores it into the string pointed to by in_line ,It stops when either (200-1) characters are read,the newline character is read, or the end-of-file is reached, whichever comes first.
		in_line[strlen(in_line) - 1] = '\0'; //end of line charachter is assigned to the strlen(in_line) - 1 th index 
		int ind = 0;  //int variable is declared and assigned to 0
		char** comm;  //a pointer that points to a char pointer comm declared
		comm = malloc(sizeof(char*)*(parameters+2));  //allocate memory of char pointer size of (parameter+2)
		for(ind = 0; ind < (parameters + 2); ind++){ //iterate the code segment inside the for loop Parameter+2 times
			comm[ind] = NULL;     //assign the ind index of comm array with NULL pointer
		}
		ind = 0;    //set the ind variable with zero
		char* token = strtok(in_line, " ");         //breaks string in the variable in_line into a series of tokens using the delimiter " " and assigns to the char pointer variable token
		while(token != NULL){             //the code segment inside the while loop executed untill the token variable is equal to NULL
			comm[ind] = malloc(sizeof(char)*(strlen(token)+1));    //allocate memory space of size equivalent to length of the token variable + 1
			strcpy(comm[ind++], token);   //copies the content in token variable to comm array at the ind index and returns the pointer at dir variable  
			token = strtok(NULL, " ");   //breaks the in_line into a series of tokens using the delimiter " " and assigns to token
		}
		if(!strcmp(comm[0], "exit")){    //compare the zeroth index of comm array variable with "exit" and if it is unequal then execute the following code segment
			check_stop = 0;                //check_stop is set to zero
		}   
		else{                          //compare the zeroth index of comm array variable with "exit" and if it is equal then execute the following code segment
			strncat(dir, "/", strlen("/") + 1);   //  appends not more than strlen("/") + 1 characters from the string pointed to by dir to the end of the string pointed to by "/" plus a terminating Null-character
			strncat(dir, comm[0], strlen(comm[0]) + 1); //  appends not more than strlen(comm[0]) + 1 characters from the string pointed to by dir to the end of the string pointed to by comm[0] plus a terminating Null-character
			strcpy(comm[0], dir);//function copies the string pointed by dir to the comm[0].
			pid_t pid = fork();//Fork system call is used for create a new child process, which runs concurrently with the parent process upto now
			if (pid == 0){// if child process
				if(execv(*comm, comm) == -1){//if replacing the current process image comm with a new process image specified by pointer *comm is not successfull
					printf("Invalid command: %s\n", comm[0]);//outputs invalid command
					exit(1);//return exit status of failiure
				}
			}
			else{//if parent process
				wait(NULL);//blocks the calling process until  its child process exits
				printf("\n");//print new line
			}
		}
	}
	return 0;//exit from the program
}