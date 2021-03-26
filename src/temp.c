/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 16:58:00 by mbari             #+#    #+#             */
/*   Updated: 2021/03/26 19:56:39 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/execution.h"

void	exec(char *input, char **env)
{
	char	**cmd;
	int		pid;
	int		status;
	int		f_status;

	cmd = ft_split(input, ' ');
	if (!(pid = fork()))
	{
		//child_process;
		if (execve(cmd[0], cmd, env) == -1)
			ft_putstr_fd("Command not found or permission denied.\n", 2);
	}
	else if (pid == -1)
	{
		//error;
		ft_putstr_fd("Fork failed.\n", 2);
	}
	else
	{
		//parrent process;
		waitpid(pid, &status, 0);
		f_status = WEXITSTATUS(status);
		ft_putstr_fd("Exit Status is : ", 1);
		ft_putnbr_fd(status, 1);
		ft_putendl_fd("", 1);
	}
}
void	do_backups(int flag)
{
	//this function for backup all 
	static int	stdin;
	static int	stdou;
	static int	stderr;

	if (flag)
	{
		//do a backup for all the standars if flag == 1
		stdin = dup2(0, 166);
		stdou = dup2(1, 167);
		stdou = dup2(2, 168);
	}
	else
	{
		if (flag == 3)
		{
			// reset the standars if the flags == 3
			dup2(stdin, 0);
			dup2(stdou, 0);
			dup2(stderr, 0);
		}
	}
}

void init_env(t_env **head, char **env)
{
	t_env *newnode;
	char **var;
	int i;

	i = 0;
	while (env[i])
	{
		var = ft_split(env[i], '=');
		newnode = ft_create_node(var[0], var[1]);
		ft_add_to_list(head, newnode);
		free(var);
		i++;
	}
}

void show_prompt()
{
	write(1,GREEN,ft_strlen(GREEN));
	ft_putstr_fd("MINISHELL $> ", 1);
	write(1,RESET,ft_strlen(RESET));
}

void show_command(t_simple_cmd *cmd)
{
	ft_putstr_fd(cmd->command, 1);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd(cmd->args->value, 1);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd(cmd->args->next->value, 1);
	ft_putchar_fd(' ', 1);
	ft_putstr_fd(cmd->args->next->next->value, 1);
	ft_putchar_fd('\n', 1);
}

void ft_init(t_simple_cmd *command)
{
	
	command->args = (t_args *)malloc(sizeof(t_args));
	command->args->next = (t_args *)malloc(sizeof(t_args));
	command->args->next->next = (t_args *)malloc(sizeof(t_args));
	command->command = ft_strdup("echo");
	command->args->value = ft_strdup("-n");
	command->args->next->value = ft_strdup("-n");
	command->args->next->next->value = ft_strdup("how are u ?");
	command->args->next->next->next = NULL;
}

void ft_is_builtins(t_simple_cmd *cmd)
{
	if (!(ft_strcmp(cmd->command, "echo")))
		ft_echo(cmd->args);

}

int		main(int ac, char **av, char **env)
{
	char	*input;
	int		fd;
	char	*line;
	t_env *head;
	t_simple_cmd *cmd_list = (t_simple_cmd *)malloc(sizeof(t_simple_cmd));

	ft_init(cmd_list);
	input = NULL;
	head = NULL;
	init_env(&head, env);
	//ft_delete_from_list(&head, "SHELL");
	//ft_replaceit(&head, "SHELL", "replaced");
	t_env *temp = head;
	// while (temp != NULL)
	// {
	// 	printf("NAME: %s\nVALUE: %s\n", temp->name, temp->value);
	// 	temp = temp->next;
	// }
	// printf("|----------------------------------------------------------------------------|\n");
	printf("cmd : %s\narg 1: %s\narg 2: %s\n\n", cmd_list->command, cmd_list->args->value, cmd_list->args->next->value);
	// ft_delete_from_list(&head, "TMPDIR");
	// ft_delete_from_list(&head, "LANG");
	// temp = head;
	// while (temp != NULL)
	// {
	// 	printf("NAME: %s\nVALUE: %s\n", temp->name, temp->value);
	// 	temp = temp->next;
	// }
	/*do_backups(1);
	fd = open("file.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(fd, 1);
	ft_putstr_fd("Hello World After DUP2\n", fd);
	ft_putstr_fd("Testing Redirections.\n", fd);
	ft_putstr_fd("Are we out\n", 1);*/
	// while (1)
	// {
	// 	show_prompt();
	// 	get_next_line(&input);
	// 	exec(input, env);
	// }
	show_prompt();
	show_command(cmd_list);
	ft_echo(cmd_list->args);
}
