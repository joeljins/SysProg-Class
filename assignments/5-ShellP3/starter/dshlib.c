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

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
#define QUOTE_CHAR '"'
	int count = 0;
	bool quote = false;
	char *buffer = (char *)malloc(strlen(cmd_line) + 1 * sizeof(char));
	if (buffer == NULL)
	{
		return ERR_MEMORY;
	}
	strcpy(buffer, cmd_line);
	char *pointer = buffer;
	while (*pointer)
	{
		while (*pointer == SPACE_CHAR && quote == false)
		{
			pointer++;
		}

		if (*pointer == '\0')
		{
			break;
		}

		if (*pointer == QUOTE_CHAR)
		{
			quote = true;
			pointer++;
		}

		cmd_buff->argv[count] = pointer;
		count++;

		char condition = quote ? QUOTE_CHAR : SPACE_CHAR;

		while (*pointer && *pointer != condition)
		{
			if (*pointer == '\0')
			{
				break;
			}
			pointer++;
		}
		*pointer = '\0';
		pointer++;
		quote = false;

		if (count >= CMD_ARGV_MAX - 1)
		{
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
	char *token = strtok(cmd_line, PIPE_STRING);
	int i = 0;
	command_buff_t buff;

	while (token != NULL)
	{
		memset(&buff, 0, sizeof(command_buff_t));
		build_cmd_buff(token, &buff);

		memcpy(&clist->commands[i], &buff, sizeof(command_buff_t));
		for (int j = 0; j < buff.argc; j++) {
			clist->commands[i].argv[j] = strdup(buff.argv[j]);
		}
		
		if (buff._cmd_buffer != NULL) {
			clist->commands[i]._cmd_buffer = strdup(buff._cmd_buffer);
		}
		i++;
		if (i >= CMD_MAX)
		{
			return ERR_TOO_MANY_COMMANDS;
		}
		token = strtok(NULL, PIPE_STRING);
	}
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
		case BI_NOT_BI:
			return BI_NOT_BI;
		case BI_CMD_CD:
			if (cmd->argc == 1) {
            			break;
        		}
			else if (cmd->argc > 2){
				perror("Too many args");
			}
			else if (chdir(cmd->argv[1]) == -1){
				perror("Invalid directory");
			}
			else{
			}
			break;
		case BI_CMD_EXIT:
			return command;
		default:
			return command;
	}
	return command;
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
			break;
	    	}
	    	cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
	    	memset(&cmd, 0, sizeof(cmd_buff_t));
	    	if (cmd_buff[0] == '\0' || cmd_buff[0] == '\n') {
		    	printf("%s", CMD_WARN_NO_CMD);
			continue;
	    	}
		rc = build_cmd_buff(cmd_buff, &cmd);
		if (rc != OK){
			if (rc == ERR_CMD_OR_ARGS_TOO_BIG){
				printf("Too many arguments");
			}
			continue;
		}
		Built_In_Cmds command = match_command(cmd.argv[0]); 
		if (command != BI_NOT_BI){
			Built_In_Cmds bic = exec_built_in_cmd(&cmd);
			if (bic == BI_CMD_EXIT){
				return 0;
			}
			continue;
		}

		pid_t pid = fork();
		if (pid == 0){
			char **args = cmd.argv;
			execvp(args[0], args);
			perror("execvp");
			exit(ERR_EXEC_CMD);
		}
		else if (pid > 0){
			int status;
			waitpid(pid, &status, 0);

		}
		else{
			perror("fork failed");
		}
	}
	free(cmd_buff);
	return OK;
}

