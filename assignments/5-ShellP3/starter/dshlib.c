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
	int count = 0;
    	bool quote = false; 
    	char *buffer = (char *)malloc(strlen(cmd_line)+1 * sizeof(char)); 
	if (buffer == NULL){
		return ERR_MEMORY;
	}
	strcpy(buffer, cmd_line);
    	char *pointer = buffer;
    	while (*pointer){
	    	while (*pointer == SPACE_CHAR && quote == false){
		    pointer++;
	    	}

		if (*pointer == '\0'){
			break;
		}

	    	if (*pointer == QUOTE_CHAR){
		    	quote = true;
		    	pointer++;
	    	}

		cmd_buff->argv[count] = pointer;
		count++;

		char condition = quote ? QUOTE_CHAR : SPACE_CHAR;

	    	while(*pointer && *pointer != condition){
			if (*pointer == '\0'){
				break;
			}
		   	pointer++;
	    	}
	    	*pointer = '\0';
		pointer++;
		quote = false;

		if (count >= CMD_ARGV_MAX - 1) {
            		free(buffer);
            		return ERR_CMD_OR_ARGS_TOO_BIG;
        }
    }
	cmd_buff->argc = count;
	cmd_buff->argv[count] = NULL;
    cmd_buff->_cmd_buffer = cmd_line;
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	char* token = strtok(cmd_line, PIPE_STRING);
	int i = 0;

    command_t current;
    command_buff_t buff;

	while (token != NULL){
		memset(&current, 0, sizeof(command_t));
        memset(&buff, 0, sizeof(command_buff_t));
        build_cmd_buff(token, &buff);

        current.exe = buff->argv[0];
        int j = 1;
        while (*(buff->argv[j]) != NULL){
            current.eve
        }

        while (*token != NULL){
            token++;
        }
		memcpy(&clist->commands[i], &current, sizeof(command_t));
		clist->num = i+1;
		if (token == NULL){
			break;
		}
		token = strtok(NULL, PIPE_STRING);
		i++;
		if (i>CMD_MAX){
			return ERR_TOO_MANY_COMMANDS;
		}

}
return OK;
}

int exec_local_cmd_loop()
{
   
    return OK;
}
