/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 16:58:00 by mbari             #+#    #+#             */
/*   Updated: 2021/05/25 16:55:47 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int		ft_count_args(t_args **args)
{
	int i;
	t_args *temp;

	i = 0;
	temp = *args;
	while (temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char **ft_args_to_arr(t_simple_cmd *cmd)
{
	t_args *temp;
	char **arg;
	int i;

	temp = cmd->args;
	i = ft_count_args(&temp) + 1;
	arg = (char **)malloc(sizeof(char *) * i + 1);
	i = 0;
	arg[i++] = cmd->command;
	while (temp != NULL)
	{
		arg[i] = temp->value;
		i++;
		temp = temp->next;
	}
	arg[i] = NULL;
	return (arg);
}

int		ft_exec(t_simple_cmd *cmd, t_env **head)
{
	int		pid;
	int		status;
	int		f_status;

	if (!(pid = fork()))
	{
		//child_process;
		//ft_putendl_fd(cmd->command, 1);
		if (execve(cmd->command, ft_args_to_arr(cmd), ft_list_to_arr(head)) == -1)
			exit(ft_put_err(cmd->command, ft_strjoin(": ", strerror(errno)), errno));
		// ft_putendl_fd(strerror(errno), 2);
		// ft_putnbr_fd(errno, 1);
			//ft_putstr_fd("Command not found or permission denied.\n", 2);
	}
	else if (pid == -1)
	{
		//error;
		return (ft_put_err("fork", ft_strjoin(": ", "Fork failed"), 2));
	}
	else
	{
		//parrent process;
		waitpid(pid, &status, 0);
		f_status = WEXITSTATUS(status);
		// ft_putnbr_fd(f_status, 1);
		return (f_status);
	}
	return (77);
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
		stdin = dup(STDIN_FILENO);
		stdou = dup(STDOUT_FILENO);
		stderr = dup(STDERR_FILENO);
	}
	else if (!flag)
	{
		// reset the standars if the flags == 0
		dup2(stdin, 0);
		dup2(stdou, 1);
		dup2(stderr, 2);
	}
}

void init_env(t_env **head, char **env)
{
	t_env *newnode;
	char **var;
	int i;
	newnode = NULL;
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

int ft_file_check(t_simple_cmd *cmd, t_env **head)
{
	struct stat *buf;

	buf = malloc(sizeof(struct stat));
	if (stat(cmd->command, buf) == -1)
		return (ft_put_err(cmd->command, ": No such file or directory", 127));
	else if (buf->st_mode & S_IFDIR)
		return (ft_put_err(cmd->command, ": Is a directory", 126));
	else if ((buf->st_mode & S_IXUSR) == 0)
		return (ft_put_err(cmd->command, ": Permission denied", 126));
	return (ft_exec(cmd, head));
}

int	ft_chech_path(t_simple_cmd *cmd, t_env **head)
{
	t_env *temp;
	char **path = NULL;
	char *full_path;
	struct stat *buf;
	
	if (cmd->command[0] == '/' || cmd->command[0] == '.')
		return (ft_file_check(cmd, head));
	else
	{
		temp = ft_search_in_list(head, "PATH");
		path = ft_split(temp->value, ':');
		while (*path != NULL)
		{
			buf = malloc(sizeof(struct stat));
			full_path = ft_strjoin(*path, ft_strjoin("/", cmd->command));
			stat(full_path, buf);
			if ((buf->st_mode & S_IXUSR) > 0 && (buf->st_mode & S_IFREG) > 0)
			{
				cmd->command = full_path;
				return (ft_exec(cmd, head));
			}
			free(buf);
			path++;
		}
		return(ft_put_err(cmd->command, ": command not found", 127));
	}
	return (0);
}

int		ft_is_builtins(t_simple_cmd *cmd, t_env **head)
{
	if (!(ft_strcmp(cmd->command, "echo")) || !(ft_strcmp(cmd->command, "ECHO")))
		return (ft_echo(head, cmd->args));
	else if (!(ft_strcmp(cmd->command, "cd")) || !(ft_strcmp(cmd->command, "CD")))
		return (ft_cd(cmd->args, head));
	else if (!(ft_strcmp(cmd->command, "pwd")) || !(ft_strcmp(cmd->command, "PWD")))
		return (ft_pwd(head));
	else if (!(ft_strcmp(cmd->command, "env")) || !(ft_strcmp(cmd->command, "ENV")))
		return (ft_env(head));
	else if (!(ft_strcmp(cmd->command, "export")) || !(ft_strcmp(cmd->command, "EXPORT")))
		return (ft_export(head, cmd->args));
	else if (!(ft_strcmp(cmd->command, "unset")) || !(ft_strcmp(cmd->command, "UNSET")))
		return (ft_unset(cmd->args, head));
	else if (!(ft_strcmp(cmd->command, "exit")) || !(ft_strcmp(cmd->command, "EXIT")))
		return (ft_exit(cmd->args));
	else
		return (ft_chech_path(cmd, head));
}

int ft_put_err(char *input, char *message, int ret)
{
	ft_putstr_fd("minishell: ", 2);
	if (input != NULL)
		ft_putstr_fd(input, 2);
	ft_putendl_fd(message, 2);
	return (ret);
}

int ft_pipe(t_mini *mini, t_pipe_line *cmd, t_env **head)
{
	int i;
	int command;
	cmd->simple_cmd_count--;
	//int pid[cmd->simple_cmd_count + 1];
	int status;
	//int fd[2*cmd->simple_cmd_count];
	
	i = 0;
	mini->fd = (int *)malloc(sizeof(int) * (cmd->simple_cmd_count * 2));
	mini->pid = (int *)malloc(sizeof(int) * (cmd->simple_cmd_count + 1));
	if (!mini->fd)
		return (ft_put_err("malloc", ": malloc faild", 1));
	while (i < cmd->simple_cmd_count)
	{
		if(pipe(mini->fd + i * 2) < 0)
			return(ft_put_err("pipe", ": couldn't create pipe", errno));
		i++;
	}
	command = 0;
	i = 0;
	int k = 0;
	(void)head;
	while (cmd->child)
	{
		mini->pid[k] = fork();
		if (mini->pid[k] == -1)
			return (ft_put_err("fork", ": coudn't fork properly\n", errno));
		else if (mini->pid[k] == 0)
		{
			// if not the last command
			if (cmd->child->next)
				if (dup2(mini->fd[command + 1], STDOUT_FILENO) < 0)
					return(ft_put_err("dup2", ": couldn't clone the fd", 1));
			// if not the first command
			if (command != 0)
				if (dup2(mini->fd[command - 2], STDIN_FILENO) < 0)
					return(ft_put_err("dup2", ": couldn't clone the fd1", 1));
			if (cmd->child->redirections != NULL)
			{
				if (ft_redirection(mini, cmd->child->redirections))
					return (1);
			}
			while (i < (cmd->simple_cmd_count * 2))
				close(mini->fd[i++]);
			mini->ret = ft_is_builtins(cmd->child, head);
			exit (mini->ret);
		}
		cmd->child = cmd->child->next;
		command+=2;
		do_backups(0);
		k++;
	}
	i = 0;
	while (i < (cmd->simple_cmd_count * 2))
		close(mini->fd[i++]);
	i = 0;
	while (i < cmd->simple_cmd_count + 1)
	{
		waitpid(mini->pid[i], &status, 0);
		mini->ret = WEXITSTATUS(status);
		i++;
	}
	return (mini->ret);
}

int		ft_execute(t_pipe_line *cmd, t_env **head)
{
	// char	*input;
	// int		ret;
	t_mini mini;

	/*
	input = NULL;
	// int stdin = STDIN_FILENO;
	// int stdout = STDOUT_FILENO;
	//int		fd;
	//char	*line;
	//ft_putnbr_fd(cmd->simple_cmd_count, 2);
	//ft_putendl_fd(cmd->child->next->command, 2);
	ft_init(cmd_list);
	t_simple_cmd *cmd_list = (t_simple_cmd *)malloc(sizeof(t_simple_cmd));
	//ft_delete_from_list(&head, "SHELL");
	// ft_replaceit(&head, "SHELL", "replaced");
	// t_env *temp = head;
	// while (temp != NULL)
	// {
	// 	printf("NAME: %s\nVALUE: %s\n", temp->name, temp->value);
	// 	temp = temp->next;
	// }
	// // printf("|----------------------------------------------------------------------------|\n");
	// printf("cmd : %s\narg 1: %s\narg 2: %s\n\n", cmd_list->command, cmd_list->args->value, cmd_list->args->next->value);
	// ft_delete_from_list(&head, "TMPDIR");
	// ft_delete_from_list(&head, "LANG");
	// temp = head;
	// while (temp != NULL)
	// {
	// 	printf("NAME: %s\nVALUE: %s\n", temp->name, temp->value);
	// 	temp = temp->next;
	// }
	// do_backups(1);
	// fd = open("file.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	// dup2(fd, 1);
	// ft_putstr_fd("Hello World After DUP2\n", fd);
	// ft_putstr_fd("Testing Redirections.\n", fd);
	// ft_putstr_fd("Are we out\n", 1);
	// while (1)
	// {
	// 	show_prompt();
	// 	get_next_line(&input);
	// 	exec(input, env);
	// }
	show_prompt();
	show_command(cmd_list);
	//ft_echo(cmd_list->args);
	//ft_putnbr_fd(getpid(), 1); //show the main process id
	*/
	ft_putstr_fd(BLUE,1);
	// ft_putendl_fd("------------------------------------------------------------", 1);
	// if (cmd->simple_cmd_count != 1)
	do_backups(1);
	mini.flag = 0;
	mini.red_fd[0] = 0;
	mini.red_fd[1] = 0;
	if (cmd->child->redirections != NULL)
	{
		if (ft_redirection(&mini, cmd->child->redirections))
		{
			do_backups(0);
			return (1);
		}
	}
	if (cmd->child->command == NULL && cmd->child->redirections == NULL)
		return (ft_put_err("\0", ": command not found", 127));
	if (cmd->simple_cmd_count > 1)
		mini.ret = ft_pipe(&mini, cmd, head);
	else if (cmd->child->command != NULL)
		mini.ret = ft_is_builtins(cmd->child, head);
	do_backups(0);
	//ft_putendl_fd("-------------------------execution finished------------------------------", 1);
	return (mini.ret);
}