#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include<signal.h>
#define MAX_WORD 10
#define MAX_CHAR 100


void remove_end(char line[]){   //function to remove end of line to be easier for parsing
int i=0;
while(line[i] != '\n')
{i++;}
line[i]='\0';

}

int read_line(char line[]){  // function to read line from command
fgets(line, MAX_CHAR, stdin);
remove_end(line);
if(strcmp(line, "exit")==0)  // to exit the shell
{exit(0);}
if(line[strlen(line)-1] == '&' ) {  // to check if it is a background process
line[strlen(line)-1] = '\0';
return 1;
}else {return 0;}
}



int parse_line(char* args[] , char line[]){  //function to parse commands and to deal with arguments
int i=0;
args[i]=strtok(line," ");
if(args[i] == NULL){
return 1;
}
while(args[i] != NULL){
i++;
args[i]=strtok(NULL," ");
}
return 1;

}
void sigchld_handler (int sig) {  // signalhandler function to send signal for the parent and append termainatig the child to the log
  int status;
    int pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {}
  FILE *fptr;
fptr = fopen("log.txt", "a");
    fprintf(fptr, "%s", "Child process was terminated\n");
    fclose(fptr);

}

int main(){
FILE *fptr;
fptr = fopen("log.txt", "w");
    fprintf(fptr, "%s", "");
signal(SIGCHLD, sigchld_handler);
 char* args[MAX_WORD];
 char line[MAX_CHAR];
  int status;

 while(1){
  printf("My Shell -> ");
 int b= read_line(line); //read and parse
 parse_line(args,line);

 pid_t child_pid = fork();  //fork a child
 if (child_pid  < 0) {
          printf("ERROR: forking child process failed\n");
          exit(1);
     }
     else if (child_pid  == 0) {
          if (execvp(args[0],args) < 0) {   // execute command and return error for invalid command
               printf("ERROR: invalid execution\n");
               exit(1);
          }
     }
     else {
      if(b != 1){  // if it's not a background process wait
          waitpid(child_pid,&status,0);}
     }

 }
 return 0;
}

