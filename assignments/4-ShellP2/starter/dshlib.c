#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
	char *cmd_buff;
	int rc = 0;
	cmd_buff_t cmd;

	cmd_buff = (char*)malloc(SH_CMD_MAX * sizeof(char));
	if (cmd_buff == NULL){
		exit(-1);
	}
	while(1){
	    printf("%s", SH_PROMPT);

	    if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
		    free(cmd_buff);
		exit(rc);
	    }
	    
	    cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
	    memset(&cmd, 0, sizeof(cmd_buff_t));
	    cmd->_cmd_buffer = cmd_buff;
	    while (*cmd_buff == SPACE_CHAR){
		    cmd_buff++;
	    }
	    while( *cmd_buff != '\0'){

		    bool quote = false;
		    if (*cmd_buff != '"'){
			    quote = true;
			    *(cmd->argv)='"';
			    (cmd->argv)++;
		    }
		    else{
			    quote = false;
			    *(cmd->argv)=*cmd_buff;
			     (cmd->argv)++;
			     cmd_buff++;
		    } 
		    if (quote){
			    char condition = '"';
		    }
		    else{
			    char condition = SPACE_CHAR;
		    }
		    while(*cmd_buff != condition){
			   *(cmd->argv) = *cmd_buff; 
			   cmd->argv++;
			   cmd_buff++;
		    }
		    if (!quote){
			    *(cmd->argv) = '"';
			    cmd->argv++;
		    }
		    *(cmd->argv) = ',';
                     cmd->argv++;

	    }
	    if (cmd_buff[0] == '\0' || cmd_buff[0] == '\n') {
		    printf("%s", CMD_WARN_NO_CMD);
	    }
	    else if (strcmp(cmd_buff, EXIT_CMD) == 0){
	        exit(rc);
	       }
	    else if (strcmp(cmd_buff, "dragon") == 0) {
		     // printf("Accepted command");
		     int fd = open("binary.txt", O_RDONLY);
		     if (fd == -1){
			     printf("Error opening file");
			     exit(rc);
		     }
		     char buffer[9];
		     ssize_t bytes_read;
		     while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
			     buffer[8] = '\0';
			     int decimal = strtol(buffer, NULL, 2);
			     char character = (char)decimal;
			     printf("%c", character); 
		     }
		     printf("\n");
	    
	    }
	    else if (strncmp(cmd_buff, "cd", 2) == 0){
		   while (*cmd_buff == SPACE_CHAR){
			   cmd_buff++;
			   if (*cmd_buff == NULL){
				   break;
			   }
		   }
		   char dir[20];
		   if (*cmd_buff != NULL){
		   	strcpy(cmd_buff, dir);
			chdir(dir);
		   }
		   
	    }
	    else{
		    printf("Hello");
	    }

	}
	free(cmd_buff);
	exit(rc);
    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

	return OK;
}
