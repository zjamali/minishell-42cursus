#include "libft/libft.h"
#include "../headers/get_next_line.h"
#include <fcntl.h>
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

t_env init_env(char **env)
{
	t_env listenv;

	return (listenv);
}

int		main(int ac, char **av, char **env)
{
	char	*input;
	int		fd;

	input = NULL;
	t_env env = init_env(env);
	(void)ac;
	(void)av;
	(void)env;
	/*do_backups(1);
	fd = open("file.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(fd, 1);
	ft_putstr_fd("Hello World After DUP2\n", fd);
	ft_putstr_fd("Testing Redirections.\n", fd);
	ft_putstr_fd("Are we out\n", 1);*/
	while (1)
	{
		ft_putstr_fd("MBARI $> ", 1);
		get_next_line(&input);
		exec(input, env);
	}
}
