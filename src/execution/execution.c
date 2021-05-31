/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 16:58:00 by mbari             #+#    #+#             */
/*   Updated: 2021/05/31 20:34:46 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int	ft_count_args(t_args **args)
{
	int		i;
	t_args	*temp;

	i = 0;
	temp = *args;
	while (temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**ft_args_to_arr(t_simple_cmd *cmd)
{
	t_args	*temp;
	char	**arg;
	int		i;

	temp = cmd->args;
	i = ft_count_args(&temp) + 1;
	arg = (char **)malloc(sizeof(char *) * i + 1);
	i = 0;
	arg[i++] = ft_strdup(cmd->command);
	while (temp != NULL)
	{
		if (temp->value == NULL && temp->inside_quotes == 2)
			temp->value = ft_strdup("");
		arg[i] = ft_strdup(temp->value);
		i++;
		temp = temp->next;
	}
	arg[i] = NULL;
	return (arg);
}

void	ft_free_list(char **list)
{
	int		i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

void	ft_execve(t_simple_cmd *cmd, char **args, char **list)
{
	char	*error;

	if (execve(cmd->command, args, list) == -1)
	{
		error = ft_strjoin(": ", strerror(errno));
		ft_put_err(cmd->command, error, errno);
		free(error);
		exit(errno);
	}
}

int	ft_exec(t_simple_cmd *cmd, t_env **head)
{
	int		pid;
	int		status;
	int		f_status;

	pid = fork();
	if (!pid)
	{
		ft_execve(cmd, ft_args_to_arr(cmd), ft_list_to_arr(head));
	}
	else if (pid == -1)
		return (ft_put_err("fork: ", "Fork failed", errno));
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			f_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			f_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit: 3\n", 8);
		}
		return (f_status);
	}
	return (77);
}

void	do_backups(int flag)
{
	static int	stdin;
	static int	stdou;
	static int	stderr;

	if (flag)
	{
		stdin = dup(STDIN_FILENO);
		stdou = dup(STDOUT_FILENO);
		stderr = dup(STDERR_FILENO);
	}
	else if (!flag)
	{
		dup2(stdin, 0);
		dup2(stdou, 1);
		dup2(stderr, 2);
	}
}

void	init_env(t_env **head, char **env)
{
	t_env	*newnode;
	char	**var;
	int		i;

	newnode = NULL;
	i = 0;
	while (env[i])
	{
		var = ft_split(env[i], '=');
		newnode = ft_create_node(var[0], var[1]);
		ft_add_to_list(head, newnode);
		free(var[0]);
		free(var[1]);
		free(var);
		i++;
	}
}

int	ft_file_check(t_simple_cmd *cmd, t_env **head)
{
	struct stat		*buf;

	buf = malloc(sizeof(struct stat));
	if (ft_strchr(cmd->command, '/') || cmd->command[0] == '.')
		cmd->command = ft_strjoin("/", cmd->command);
	if (stat(cmd->command, buf) == -1)
	{
		free(buf);
		return (ft_put_err(cmd->command, ": No such file or directory", 127));
	}
	else if (buf->st_mode & S_IFDIR)
	{
		free(buf);
		return (ft_put_err(cmd->command, ": Is a directory", 126));
	}
	else if ((buf->st_mode & S_IXUSR) == 0)
	{
		free(buf);
		return (ft_put_err(cmd->command, ": Permission denied", 126));
	}
	free(buf);
	return (ft_exec(cmd, head));
}

char	*ft_join_path(char *path, char *cmd)
{
	char	*path_tmp;
	char	*full_path;

	path_tmp = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_tmp, cmd);
	free(path_tmp);
	return (full_path);
}
/**********************************/
void ft_exec_file(t_simple_cmd *cmd , t_env **head, char *path, int *status)
{
	struct stat		*buf;
	char			*full_path;
	
	buf = malloc(sizeof(struct stat));
	full_path = ft_join_path(path, cmd->command);
	stat(full_path, buf);
	if (((buf->st_mode & S_IXUSR) > 0 && (buf->st_mode & S_IFREG) > 0) && *status == -77)
	{
		free(cmd->command);
		cmd->command = ft_strdup(full_path);
		*status = ft_exec(cmd, head);
	}
	free(buf);
	free(full_path);
}

int	ft_find_file(t_simple_cmd *cmd, t_env **head, int *status)
{
	t_env			*temp;
	char			**path;
	int				i;
	
	i = 0;
	temp = ft_search_in_list(head, "PATH");
	if (temp == NULL)
		return (ft_file_check(cmd, head));
	path = ft_split(temp->value, ':');
	while (path[i] != NULL)
	{
		ft_exec_file(cmd, head, path[i], status);
		free(path[i]);
		i++;
	}
	free(path);
}

int	ft_chech_path(t_simple_cmd *cmd, t_env **head)
{
	t_env			*temp;
	char			**path;
	char			*full_path;
	struct stat		*buf;
	int				status;
	int 			i;
	
	i = 0;
	status = -77;
	path = NULL;
	if (ft_strchr(cmd->command, '/') || cmd->command[0] == '.')
		return (ft_file_check(cmd, head));
	else
	{
		ft_find_file(cmd, head, &status);
		if (status != -77)
			return (status);
		return(ft_put_err(cmd->command, ": command not found", 127));
	}
	return (0);
}
/************************************/

int	ft_is_builtins(t_simple_cmd *cmd, t_env **head)
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
	int status;
	
	i = 0;
	cmd->simple_cmd_count--;
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
			if (cmd->child->next)
				if (dup2(mini->fd[command + 1], STDOUT_FILENO) < 0)
					return(ft_put_err("dup2", ": couldn't clone the fd", 1));
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
	free(mini->fd);
	free(mini->pid);
	return (mini->ret);
}

void signal_handler(int sig)
{
    char child_str[] = "this is ctrl + c in the child prosess\n";
    if (sig == SIGINT) {
        write(STDOUT_FILENO, "\n", 1);
    }
}

int		ft_execute(t_pipe_line *cmd, t_env **head)
{
	t_mini mini;

	ft_putstr_fd(BLUE,1);
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
	return (mini.ret);
}