#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>

#define INPUT_STRING_SIZE 80
#define MAX_FILE_SIZE 1024

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"

int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

int cmd_cd(tok_t arg[]) {
	chdir (arg[0]);
}

char* concat(char *r1, char *r2) //for concating
{
    char *result = malloc(strlen(r1)+strlen(r2)+1);
    strcpy(result, r1);
    strcat(result, r2);
    return result;
}

char* split(tok_t arg[]) //for the "/"
{
	tok_t *t;
	char* pathname = getenv("PATH");
	t = getToks(pathname);
	char* split = NULL;
 	FILE* file;
 	
	int k;
	for (k = 0; k < MAXTOKS;k++)
	{
		split = concat(t[k],"/");
		split = concat(split, arg[0]);
		file = fopen(split, "r");
		if (file == NULL)	{
			continue;
			}
		else{
			break;
			}
		fclose(file);
	}
	if (split == NULL){
		return arg[0];
	}
   return split;
}

int cmd_exec(tok_t arg[]) {
   int pid;
   char* pathname = arg[0];
	
   pid = fork();

   // child process
   if(pid == 0) {
   	//fprintf(stdout, "%s\n", pathname);
   	pathname=split(arg);
   	process_redirection(arg+1);
	execv(pathname, arg);  
	exit(0);
   }
   else if(pid < 0) {
	fprintf(stderr, "Failed to exec: %s\n", arg[0]);
  	return -1;
   }
   // parent process
	wait(NULL);
   return 1;		
}

//Input/Output Redirection
int process_redirection(tok_t arg[]) {
     int index = 1; // first argument is the program itself
     int outfd, infd;
     int ret;
     while(index < MAXTOKS && arg[index]) {
	switch(arg[index][0]) {
	    case '>':
		outfd = open(arg[++index], O_WRONLY|O_CREAT, S_IRWXU|S_IRWXG|S_IROTH);
		if(outfd == -1) {
	    fprintf(stderr, "Failed to open %s\n",arg[index]);
		    return -1;
		}
		close(1);
		ret = dup2(outfd, 1);
                if(ret < 0) {
		    fprintf(stderr, "Failed to invoke dup2 for stdout, return code is %d\n", ret);
		    return -1;
	        }
		arg[index-1] = arg[index] = NULL;
                break;
	    case '<':
		infd = open(arg[++index], O_RDONLY);
		if(infd == -1) {
		    fprintf(stderr, "Failed to open %s\n",arg[index]);
		    return -1;
		}
		close(0);
		ret = dup2(infd, 0);
                if(ret < 0) {
		    fprintf(stderr, "Failed to invoke dup2 for stdin, return code is %d\n", ret);
		    return -1;
	        }
		arg[index-1] = arg[index] = NULL;
                break;
        }
	index++;
     }
     return 0;
}

int cmd_help(tok_t arg[]);


/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_cd, "cd", "changes the working directory"},
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}


int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
  char cwd[MAX_FILE_SIZE+1];
  tok_t *t;			/* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  pid_t cpid, tcpid, cpgid;

  init_shell();

  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);
  
  //getcwd(cwd, MAX_FILE_SIZE);
  lineNum=0;
  //fprintf(stdout, "%d - %s: ", cwd, lineNum);
  while ((s = freadln(stdin))){
    
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
   if(fundex >= 0) {
   	cmd_table[fundex].fun(&t[1]);
   }
    else{
	cmd_exec(t);
    }
    fprintf(stdout, "%d %s: ", ++lineNum, get_current_dir_name());
  }
  return 0;
}
