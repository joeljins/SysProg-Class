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
	cmd_buff_t buff;

	while (token != NULL)
	{
		memset(&buff, 0, sizeof(cmd_buff_t));
		build_cmd_buff(token, &buff);

		memcpy(&clist->commands[i], &buff, sizeof(cmd_buff_t));
		for (int j = 0; j < buff.argc; j++)
		{
			clist->commands[i].argv[j] = strdup(buff.argv[j]);
		}

		if (buff._cmd_buffer != NULL)
		{
			clist->commands[i]._cmd_buffer = strdup(buff._cmd_buffer);
		}
		i++;
		if (i >= CMD_MAX)
		{
			return ERR_TOO_MANY_COMMANDS;
		}
		token = strtok(NULL, PIPE_STRING);
		clist->num = i;
	}
	return OK;
}

Built_In_Cmds match_command(const char *input)
{
	if (strcmp(input, "exit") == 0)
	{
		return BI_CMD_EXIT;
	}
	if (strcmp(input, "cd") == 0)
	{
		return BI_CMD_CD;
	}
	if (strcmp(input, "dragon") == 0)
	{
		return BI_CMD_DRAGON;
	}
	return BI_NOT_BI;
}
Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd)
{
	Built_In_Cmds command = match_command(cmd->argv[0]);
	switch (command)
	{
	case BI_NOT_BI:
		return BI_NOT_BI;
	case BI_CMD_CD:
		if (cmd->argc == 1)
		{
			break;
		}
		else if (cmd->argc > 2)
		{
			perror("Too many args");
		}
		else if (chdir(cmd->argv[1]) == -1)
		{
			perror("Invalid directory");
		}
		else
		{
		}
		break;
	case BI_CMD_EXIT:
		return command;
	default:
		return command;
	}
	return command;
}
int execute_pipeline(command_list_t *clist){
	int num_commands = clist->num;
	int pipes[num_commands - 1][2];  // Array of pipes
    pid_t pids[num_commands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) {
		Built_In_Cmds command = match_command(clist->commands[i].argv[0]);
		if (command != BI_NOT_BI)
		{
			Built_In_Cmds bic = exec_built_in_cmd(&clist->commands[i]);
			if (bic == BI_CMD_EXIT)
			{
				return 0;
			}
			continue;
		}
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(0);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(0);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
	return OK;
}
int exec_local_cmd_loop()
{
	char *cmd_buff;
	int rc = 0;
	command_list_t clist;

	cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
	if (cmd_buff == NULL)
	{
		exit(-1);
	}

	while (1)
	{
		printf("%s", SH_PROMPT);
		if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
		{
			break;
		}
		cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
		memset(&clist, 0, sizeof(command_list_t));
		if (cmd_buff[0] == '\0' || cmd_buff[0] == '\n')
		{
			printf("%s", CMD_WARN_NO_CMD);
			continue;
		}
		rc = build_cmd_list(cmd_buff, &clist);
		if (rc != OK)
		{
			if (rc == ERR_CMD_OR_ARGS_TOO_BIG)
			{
				printf("Too many arguments");
			}
			continue;
		}
		execute_pipeline(&clist);

	}
	free(cmd_buff);
	return OK;
}