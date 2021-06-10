/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:03:35 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:25:40 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

static int	ft_intlen(unsigned int n)
{
	int	i;

	i = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_int_to_string(int n)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	str = NULL;
	if (n == 0)
		return (ft_strdup("0"));
	else
	{
		len = ft_intlen(n);
		str = (char *)malloc(sizeof(len + 1));
		str[len] = '\0';
		len--;
		while (n / 10 != 0)
		{
			str[len] = (n % 10) + '0';
			n = n / 10;
			len--;
		}
		str[len] = (n % 10) + '0';
		return (str);
	}
}

char	*get_env_variable_value(char *env_variable, t_env **env)
{
	t_env	*tmp;
	int		j;
	char	*str;

	str = env_variable + 1;
	j = 1;
	while (!ft_isalnum(env_variable[0]) && (ft_isalpha(env_variable[j])
			|| env_variable[j] == '_' || ft_isalnum(env_variable[j])))
	{
		j++;
	}
	str = ft_substr(env_variable, 1, j - 1);
	tmp = ft_search_in_list(env, str);
	if (str)
		free(str);
	if (tmp)
		return (ft_strdup(tmp->value));
	else
		return (NULL);
}

void	ft_replace_tilde(char **string)
{
	char	*tmp;
	int		len;
	char	*tmp1;
	char	*word;

	len = 0;
	word = *string;
	if (word[0] == '~' && (word[1] == '\0' || word[1] == '/'))
	{
		tmp = word;
		if (word[1] == '\0')
			word = ft_strdup("$HOME");
		else if (word[1] == '/')
		{
			len = ft_strlen(word);
			tmp1 = ft_substr(word, 1, len - 1);
			free(word);
			tmp = ft_strdup("$HOME");
			word = ft_strjoin(tmp, tmp1);
			free(tmp1);
		}
		free(tmp);
		tmp = NULL;
	}
	*string = word;
}

void	ft_repalce_space_by_tab(t_simple_cmd **cmd, int args_or_cmd)
{
	t_args	*args;
	int		i;

	args = NULL;
	i = 0;
	if (args_or_cmd)
	{
		while ((*cmd)->command[i] != '\0')
		{
			if ((*cmd)->command[i] == ' ')
				(*cmd)->command[i] = '\t';
			i++;
		}
	}
	else
	{
		args = (*cmd)->args;
		while (args->value && args->value[i] != '\0')
		{
			if (args->value[i] == ' ')
				args->value[i] = '\t';
			i++;
		}
	}
}
