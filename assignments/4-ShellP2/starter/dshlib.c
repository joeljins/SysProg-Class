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
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff){
	#define QUOTE_CHAR '"' 
	int count = -1;
	int i;
    	bool quote; 
    	char *buffer = (char *)malloc(ARG_MAX * sizeof(char)); 
	if (buffer == NULL){
		return ERR_MEMORY;
	}
    	char *pointer = buffer;
    	while( *cmd_line != '\0'){
		if (count > 7){
			return ERR_TOO_MANY_COMMANDS;
		}
		i = 0;
	    	while (*cmd_line == SPACE_CHAR){
		    cmd_line++;
	    	}
	    	if (*cmd_line == QUOTE_CHAR){
		    	quote = true;
		    	*pointer = *cmd_line;
		    	pointer++;
		    	cmd_line++;
			i++;
	    	}
	    	else{
		    	quote = false;
	    	} 
	    	char condition;
	    	if (quote == true){
		    	condition = QUOTE_CHAR;
	    	}
	    	else{
		    	condition = SPACE_CHAR;
	    	}
	    	while(*cmd_line != condition && *cmd_line != '\0'){
		   	*pointer = *cmd_line; 
		   	pointer++;
		   	cmd_line++;
			i++;
			if (i > ARG_MAX){
				return ERR_CMD_OR_ARGS_TOO_BIG;
			}
	    	}
		if (quote){
			*pointer = *cmd_line;
			cmd_line++;
			pointer++;
			
		}
	    	*pointer = '\0';

		count++;
	    	cmd_buff->argv[count] = (char*)malloc(ARG_MAX * sizeof(char));	
	    	if (cmd_buff->argv[count] == NULL){
		    return ERR_MEMORY;
	    	}
	    	strncpy(cmd_buff->argv[count], buffer, ARG_MAX);
	    	// printf("%s\n", cmd_buff->argv[count]);
	    	memset(buffer, 0, ARG_MAX);
	    	pointer = buffer;
    }

    free(buffer);
    cmd_buff->argc = count;
    cmd_buff->_cmd_buffer = cmd_line;
    return OK;
}
Built_In_Cmds match_command(const char *input){
	if (strcmp(input, "exit") == 0){
		return BI_CMD_EXIT;
	}
	if (strcmp(input, "cd") == 0){
		return BI_CMD_CD;
		}	
	if (strcmp(input, "dragon") == 0){
                return BI_CMD_DRAGON;
        }
	return BI_NOT_BI;
}
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd){		
	Built_In_Cmds command = match_command(cmd->argv[0]);
	switch(command){
		case BI_CMD_EXIT:
			exit(0);
		case BI_CMD_CD:
			if (cmd->argc == 0) {
            			return BI_EXECUTED;
        		}
			printf("%s", cmd->argv[1]);
			if (chdir(cmd->argv[1]) != -1){
				char s[100];

    				printf("%s\n", getcwd(s, 100));
				return BI_EXECUTED;
			}
char s[100]; 
 
    // printing current working directory 
    printf("%s\n", getcwd(s, 100)); 
			printf("failed");
			return BI_NOT_BI;
	}
	return BI_NOT_BI;
}
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
	    	if (cmd_buff[0] == '\0' || cmd_buff[0] == '\n') {
		    	printf("%s", CMD_WARN_NO_CMD);
	    	}
		rc = build_cmd_buff(cmd_buff, &cmd);
		if (rc != OK){
			return rc;
		}
		Built_In_Cmds command = match_command(cmd.argv[0]); 
		if (command != BI_NOT_BI){
			exec_built_in_cmd(&cmd);
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
