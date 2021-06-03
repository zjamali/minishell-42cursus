/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 14:41:59 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/03 16:05:03 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int	check_exiting_of_qoutes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			return (1);
		}
		else if (str[i] == '\"')
			return (2);
		i++;
	}
	return (0);
}

int	ft_intlen(unsigned int n)
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

char	*get_env_value(char *env_variable, t_env **env)
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

char	*ft_remove_double_quotes(char *word, int *i, t_env **env,
		char **last_env)
{
	int		j;
	char	*expand;
	char	*tmp;
	char	*tmp1;

	expand = NULL; //ft_strdup("");
	j = *i + 1; //escape first "
	while (word[j] != '"')
	{
		if (word[j] == '\\')
		{
			if (ft_strchr("$\"\\\n`", word[j + 1]))
			{
				tmp = expand;
				tmp1 = ft_substr(word, j + 1, 1);
				expand = ft_strjoin(expand, tmp1);
				j += 2;
				free(tmp);
				free(tmp1);
			}
			else
			{
				tmp = expand;
				tmp1 = ft_substr(word, j, 2);
				expand = ft_strjoin(expand, tmp1);
				j += 2;
				free(tmp);
				free(tmp1);
			}
		}
		else
		{
			if (word[j] == '$')
			{
				if (word[j + 1] == '$')
				{
					tmp1 = expand;
					tmp = ft_substr(word, j, 2);
					expand = ft_strjoin(expand, tmp);
					free(tmp1);
					free(tmp);
					j += 2;
				}
				else
				{
					tmp1 = expand;
					tmp = get_env_value(word + j, env);
					if (tmp)
					{
						if (word[j + 1] == '_' && word[j + 2] == '"')
						{
							free(tmp);
							tmp = NULL;
							tmp = ft_strdup(last_env[1]);
						}
						expand = ft_strjoin(expand, tmp);
						free(tmp1);
						free(tmp);
						if (word[j] == '$')
							j++;
						if (word[j] == '_')
							j++;
						else
						{
							while (ft_isalpha(word[j]) || ft_isalnum(word[j])
								|| word[j] == '_')
								j++;
						}
					}
					else if (word[j - 1] != '$')
					{
						if (ft_isdigit(word[j + 1])
							|| ft_strchr("!:#%@-*=/\\", word[j + 1]))
						{
							if (ft_isdigit(word[j + 1]))
							{
								if (word[j + 1] == '0')
								{
									tmp1 = expand;
									tmp = ft_strdup("minishell");
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								j += 2;
							}
							else
							{
								if (word[j + 1] == '#')
								{
									tmp1 = expand;
									tmp = ft_strdup("0");
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (word[j + 1] == '-')
								{
									tmp1 = expand;
									tmp = ft_strdup("himBH");
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (word[j + 1] == '!')
								{
									tmp1 = expand;
									tmp = ft_strdup("$!");
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (ft_strchr("\\", word[j + 1]))
								{
									tmp1 = expand;
									tmp = ft_strdup("$");
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (ft_strchr(":%=/", word[j + 1]))
								{
									tmp1 = expand;
									tmp = ft_substr(word, j, 2);
									expand = ft_strjoin(expand, tmp);
									free(tmp1);
									free(tmp);
								}
								j += 2;
							}
						}
						else if (word[j + 1] == '?')
						{
							tmp = last_env[0];
							tmp1 = expand;
							expand = ft_strjoin(expand, tmp);
							free(tmp1);
							free(tmp);
							j += 2;
						}
						else if (word[j + 1] && word[j + 1] != '\"'
							&& word[j + 1] != ' ')
						{
							if (word[j] == '$')
								j++;
							while (ft_isalpha(word[j]) || ft_isalnum(word[j])
								|| word[j] == '_')
								j++;
						}
						else //// just a 1 dollar sign
						{
							tmp1 = expand;
							tmp = ft_substr(word, j, 1);
							expand = ft_strjoin(expand, tmp);
							free(tmp1);
							free(tmp);
							j++;
						}
					}
					else
					{
						if (!word[j + 1]) //get last dollar after double dollars sign
						{
							tmp = expand;
							tmp1 = ft_substr(word, j, 1);
							expand = ft_strjoin(expand, tmp1);
							free(tmp);
							free(tmp1);
							j++;
						}
						else
						{
							while (ft_isalpha(word[j]) || word[j] == '$')
								j++;
						}
					}
				}
			}
			else
			{
				tmp = expand;
				tmp1 = ft_substr(word, j, 1);
				expand = ft_strjoin(expand, tmp1);
				free(tmp);
				free(tmp1);
				j++;
			}
		}
	}
	j++;
	*i = j;
	return (expand);
}

void	ft_expande_word(char **string, t_env **env_list, char **last_env,
		int redirection)
{
	char	*word;
	char	*expanded;
	int		i;
	int		j;
	int		k;
	char	*tmp1;
	int		back_slash;
	char	*tmp;
	int		len;

	word = *string;
	i = 0;
	k = 0;
	len = 0;
	expanded = NULL;
	back_slash = 0;
	j = 0;
	tmp1 = NULL;
	tmp = NULL;
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
	while (word[i])
	{
		if (word[i] == '\\')
		{
			j = i;
			while (word[j] == '\\')
			{
				tmp1 = expanded;
				tmp = ft_substr(word, j + 1, 1);
				expanded = ft_strjoin(expanded, tmp);
				free(tmp1);
				free(tmp);
				j += 2;
			}
			i = j;
		}
		else if (word[i] == '\'')
		{
			i++;
			j = i;
			while (word[j] != '\'')
				j++;
			tmp1 = expanded;
			tmp = ft_substr(word, i, j - i);
			expanded = ft_strjoin(expanded, tmp);
			free(tmp);
			free(tmp1);
			i = j + 1;
		}
		else if (word[i] == '"')
		{
			tmp1 = expanded;
			tmp = ft_remove_double_quotes(word, &i, env_list, last_env);
			if (tmp)
			{
				expanded = ft_strjoin(expanded, tmp);
				free(tmp);
				free(tmp1);
			}
		}
		else if (word[i] == '$')
		{
			if (word[i + 1] == '$') //// sequence of dollars sign
			{
				tmp1 = expanded;
				tmp = ft_substr(word, i, 2);
				expanded = ft_strjoin(expanded, tmp);
				free(tmp1);
				free(tmp);
				i += 2;
			}
			else
			{
				tmp1 = expanded;
				tmp = get_env_value(word + i, env_list);
				if (tmp) /// env variavle exist
				{
					if (word[i + 1] == '_' && word[i + 2] == '\0')
					{
						free(tmp);
						tmp = NULL;
						tmp = ft_strdup(last_env[1]);
					}
					tmp1 = expanded;
					expanded = ft_strjoin(expanded, tmp);
					free(tmp);
					free(tmp1);
					if (word[i] == '$')
						i++;
					if (word[i] == '_')
						i++;
					else
					{
						while (ft_isalpha(word[i]) || ft_isalnum(word[i])
							|| word[i] == '_')
							i++;
					}
				}
				else if (word[i - 1] != '$') /// not exit $$hdj vs $kfjh
				{
					if (word[i + 1] != '"')
					{
						//// Special Parameters of $
						if (ft_isdigit(word[i + 1])
							|| ft_strchr("!:#%@-*=/\\", word[i + 1]))
						{
							if (ft_isdigit(word[i + 1]))
							{
								if (word[i + 1] == '0')
								{
									tmp1 = expanded;
									tmp = ft_strdup("minishell");
									expanded = ft_strjoin(expanded, tmp);
									free(tmp1);
									free(tmp);
								}
								i += 2;
							}
							else if (ft_strchr("\\", word[i + 1]))
							{
								tmp1 = expanded;
								tmp = ft_strdup("$");
								expanded = ft_strjoin(expanded, tmp);
								free(tmp1);
								free(tmp);
								i++;
							}
							else
							{
								if (word[i + 1] == '#')
								{
									tmp1 = expanded;
									tmp = ft_strdup("0");
									expanded = ft_strjoin(expanded, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (word[i + 1] == '-')
								{
									tmp1 = expanded;
									tmp = ft_strdup("himBH");
									expanded = ft_strjoin(expanded, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (word[i + 1] == '!')
								{
									tmp1 = expanded;
									tmp = ft_strdup("$!");
									expanded = ft_strjoin(expanded, tmp);
									free(tmp1);
									free(tmp);
								}
								else if (ft_strchr(":%=/", word[i + 1]))
								{
									tmp1 = expanded;
									tmp = ft_substr(word, i, 2);
									expanded = ft_strjoin(expanded, tmp);
									free(tmp1);
									free(tmp);
								}
								i += 2;
							}
						}
						else if (word[i + 1] == '?')
						{
							tmp = last_env[0];
							tmp1 = expanded;
							expanded = ft_strjoin(expanded, tmp);
							free(tmp1);
							i += 2;
						}
						else if (word[i + 1])
						{
							if (word[i] == '$')
								i++;
							while (ft_isalpha(word[i]) || ft_isalnum(word[i])
								|| word[i] == '_')
								i++;
						}
						else //// just a 1 dollar sign
						{
							tmp1 = expanded;
							tmp = ft_substr(word, i, 1);
							expanded = ft_strjoin(expanded, tmp);
							free(tmp1);
							free(tmp);
							i++;
						}
					}
					else
						i++;
				}
				else /// env variavle not exist  no multiple sign dollars
				{
					if (!word[i + 1]) //get last dollar after double dollarssign
					{
						tmp = expanded;
						tmp1 = ft_substr(word, i, 1);
						expanded = ft_strjoin(expanded, tmp1);
						free(tmp);
						free(tmp1);
						i++;
					}
					else
					{
						while (ft_isalpha(word[i]) || word[i] == '$')
							i++;
					}
				}
			}
		}
		else /// not dollars sign no quote just join characters
		{
			tmp1 = expanded;
			tmp = ft_substr(word, i, 1);
			expanded = ft_strjoin(expanded, tmp);
			free(tmp1);
			free(tmp);
			i++;
		}
	}
	if (redirection == 1)
	{
		if (expanded != NULL && expanded[0] != '\0')
			*string = expanded;
		else
		{
			*string = ft_strdup(word);
		}
	}
	else
		*string = expanded;
	free(word);
}

t_simple_cmd	*ft_handle_cmd_expanding(t_simple_cmd **cmd)
{
	char	**splited;
	char	*to_free;
	int		i;
	t_args	*new_args;
	t_args	*tmp;

	splited = NULL;
	i = 1;
	to_free = (*cmd)->command;
	splited = ft_split((*cmd)->command, ' ');
	free(to_free);
	(*cmd)->command = splited[0];
	new_args = (t_args *)malloc(sizeof(t_args));
	tmp = new_args;
	while (splited[i])
	{
		new_args->value = splited[i];
		new_args->inside_quotes = 0;
		new_args->next = NULL;
		if (splited[i + 1])
		{
			new_args->next = (t_args *)malloc(sizeof(t_args));
			new_args = new_args->next;
		}
		i++;
	}
	if (i > 1)
	{
		new_args->next = (*cmd)->args;
		(*cmd)->args = tmp;
	}
	free(splited);
	return (*cmd);
}

t_args	*ft_handle_arg_expanding(t_args **args)
{
	char	**splited;
	char	*to_free;
	t_args	*tmp;
	t_args	*new_args;
	t_args	*tmp1;
	int		i;

	tmp1 = *args;
	to_free = tmp1->value;
	splited = ft_split(tmp1->value, ' ');
	i = 0;
	new_args = (t_args *)malloc(sizeof(t_args));
	tmp = new_args;
	while (splited[i])
	{
		tmp->value = splited[i];
		tmp->inside_quotes = 0;
		tmp->next = NULL;
		if (splited[i + 1])
		{
			tmp->next = (t_args *)malloc(sizeof(t_args));
			tmp = tmp->next;
		}
		i++;
	}
	tmp->next = tmp1->next;
	*args = new_args;
	free(splited);
	return (new_args);
}

void	ft_delete_emty_args_nodes(t_args **args)
{
	t_args	*temp;
	t_args	*prev;

	prev = NULL;
	temp = *args;
	while (*args && ((*args)->inside_quotes == 0 && !(*args)->value))
	{
		temp = *args;
		*args = (*args)->next;
		free(temp);
	}
	temp = *args;
	while (temp)
	{
		while (temp && (temp->inside_quotes != 0
				|| (temp->value && temp->inside_quotes == 0)))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return ;
		prev->next = temp->next;
		free(temp);
		temp = prev->next;
	}
}

void	ft_expande_simple_cmd(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_redirection	*redis;
	t_args			*args;
	char			*befor_expand_cmd;
	char			*after_expand_cmd;
	char			*befor_expand_arg;
	char			*after_expand_arg;
	char			*space;
	t_args			*next_args;
	int				i;
	t_args			*to_free;

	to_free = NULL;
	next_args = NULL;
	befor_expand_cmd = NULL;
	after_expand_cmd = NULL;
	befor_expand_arg = NULL;
	after_expand_arg = NULL;
	space = NULL;
	redis = NULL;
	args = (*cmd)->args;
	/// replace space by tab
	args = (*cmd)->args;
	while (args)
	{
		befor_expand_arg = ft_strdup(args->value);
		i = 0;
		/// replace space by tab
		while (args->value && args->value[i] != '\0')
		{
			if (args->value[i] == ' ')
				args->value[i] = '\t';
			i++;
		}
		///
		args->inside_quotes = check_exiting_of_qoutes(args->value);
		ft_expande_word(&args->value, env, last_env, 0);
		if (args->value)
			after_expand_arg = ft_strdup(args->value);
		if (args->inside_quotes == 0 && after_expand_arg
			&& ft_strcmp(befor_expand_arg, after_expand_arg)
			&& ft_strchr(after_expand_arg, ' ')
			&& ft_strchr(befor_expand_arg, '$'))
		{
			next_args = args->next;
			space = ft_strchr(after_expand_arg, ' ');
			if (++space)
			{
				*args = *ft_handle_arg_expanding(&args);
			}
			args = next_args;
		}
		else
			args = args->next;
		free(befor_expand_arg);
		befor_expand_arg = NULL;
		free(after_expand_arg);
		after_expand_arg = NULL;
	}
	if ((*cmd)->command)
	{
		befor_expand_cmd = ft_strdup((*cmd)->command);
		i = 0;
		while ((*cmd)->command[i] != '\0')
		{
			if ((*cmd)->command[i] == ' ')
				(*cmd)->command[i] = '\t';
			i++;
		}
		i = 0;
		(*cmd)->inside_quotes = check_exiting_of_qoutes(((*cmd)->command));
		ft_expande_word(&((*cmd)->command), env, last_env, 0);
		if ((*cmd)->command)
			after_expand_cmd = ft_strdup((*cmd)->command);
		if ((*cmd)->inside_quotes == 0 && after_expand_cmd
			&& ft_strcmp(befor_expand_cmd, after_expand_cmd)
			&& ft_strchr(after_expand_cmd, ' ')
			&& ft_strchr(befor_expand_cmd, '$'))
		{
			space = ft_strchr(after_expand_cmd, ' ');
			if (++space)
			{
				*cmd = ft_handle_cmd_expanding(cmd);
			}
			//free(space);
			//space = NULL;
		}
		free(befor_expand_cmd);
		befor_expand_cmd = NULL;
		free(after_expand_cmd);
		after_expand_cmd = NULL;
	}
	redis = (*cmd)->redirections;
	while (redis)
	{
		redis->inside_quotes = check_exiting_of_qoutes(redis->file_name);
		if (redis->inside_quotes != 0)
			ft_expande_word(&redis->file_name, env, last_env, 0);
		else
			ft_expande_word(&redis->file_name, env, last_env, 1);
		redis = redis->next;
	}
	args = (*cmd)->args;
	////// here deleting empty args
	if (args)
	{
		ft_delete_emty_args_nodes(&(*cmd)->args);
	}
	args = (*cmd)->args;
	if (!(*cmd)->command && (*cmd)->inside_quotes == 0)
	{
		if (args)
		{
			if (args->value)
			{
				(*cmd)->command = ft_strdup(args->value);
				free(args->value);
			}
			(*cmd)->inside_quotes = args->inside_quotes;
			if (args->next != NULL)
			{
				to_free = args;
				(*cmd)->args = args->next;
				free(to_free);
				to_free = NULL;
			}
			else
			{
				free((*cmd)->args);
				(*cmd)->args = NULL;
			}
		}
	}
	/// replace tab by space
	i = 0;
	if ((*cmd)->command)
	{
		while ((*cmd)->command[i] != '\0')
		{
			if ((*cmd)->command[i] == '\t')
				(*cmd)->command[i] = ' ';
			i++;
		}
	}
	i = 0;
	args = (*cmd)->args;
	while (args)
	{
		i = 0;
		if (args->value)
		{
			while (args->value[i])
			{
				if (args->value[i] == '\t')
					args->value[i] = ' ';
				i++;
			}
		}
		args = args->next;
	}
}

void	ft_expanding(t_pipe_line *pipe_line, t_env **env, char **last_env)
{
	t_simple_cmd	*current_cmd;
	t_simple_cmd	*head_cmd;

	current_cmd = NULL;
	head_cmd = NULL;
	ft_putstr_fd(RED, 1);
	head_cmd = pipe_line->child;
	while (head_cmd)
	{
		current_cmd = head_cmd;
		ft_expande_simple_cmd(&current_cmd, env, last_env);
		head_cmd = head_cmd->next;
	}
}