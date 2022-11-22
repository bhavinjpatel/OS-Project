#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define parameters 2 
int main(int argc, char** argv, char** env) {
	char* path = getenv("PATH");
	char* tok = strtok(path, ":");
	char* wd = NULL;
	while(tok != NULL){
		if(!strcmp(tok, "/usr/bin")){
			wd = malloc(sizeof(char)*20);
			strcpy(wd, tok);
			break;
		}
		tok = strtok(NULL, ":");
	}
	char* dir = malloc(sizeof(char)*(strlen(wd)+1));
	int check_stop = 1;
	while (check_stop == 1) {
		strcpy(dir, wd);
		char in_line[200];
		in_line[0] = '\0';
		printf("sh> ");
		fgets(in_line, 200, stdin);
		in_line[strlen(in_line) - 1] = '\0';
		int ind = 0;
		char** comm;
		comm = malloc(sizeof(char*)*(parameters+2));
		for(ind = 0; ind < (parameters + 2); ind++){
			comm[ind] = NULL;
		}
		ind = 0;
		char* token = strtok(in_line, " ");
		while(token != NULL){
			comm[ind] = malloc(sizeof(char)*(strlen(token)+1));
			strcpy(comm[ind++], token);
			token = strtok(NULL, " ");
		}
		if(!strcmp(comm[0], "exit")){
			check_stop = 0;
		}
		else{
			strncat(dir, "/", strlen("/") + 1);
			strncat(dir, comm[0], strlen(comm[0]) + 1);
			strcpy(comm[0], dir);
			pid_t pid = fork();
			if (pid == 0){
				if(execv(*comm, comm) == -1){
					printf("Invalid command: %s\n", comm[0]);
					exit(1);
				}
			}
			else{
				wait(NULL);
				printf("\n");
			}
		}
	}
	return 0;
}