/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 14:41:59 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/03 15:27:53 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int check_exiting_of_qoutes(char *str)
{
	if (str)
	{
		if (str[0] == '\'')
		{
			return (1);
		}
		else if (str[0] == '\"')
			return 2;
		else
			return (0);
	}
	return (0);
}

int ft_intlen(unsigned int n)
{
	int i;

	i = 1;
	while (n / 10 != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char *ft_int_to_string(int n)
{
	int i;
	int len;
	char *str;

	i = 0;
	len = 0;
	str = NULL;
	if (n == 0)
		return ft_strdup("0");
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
		return str;
	}
}

char *get_env_value(char *env_variable, t_env **env, int inside_dq)
{
	t_env *tmp;
	int j;
	char *str;

	str = env_variable + 1;
	j = 0;
	if (inside_dq == 1)
	{
		j++;
		if (env_variable[j] == '_')
			str = ft_strdup("_");
		else
		{
			while (ft_isalpha(env_variable[j]))
				j++;
			str = ft_substr(env_variable, 1, j - 1);
		}
		//ft_putstr_fd(str,1);
	}
	if (inside_dq == 0)
	{
		j++;
		if (env_variable[j] == '_')
			str = ft_strdup("_");
		else
		{
			while (ft_isalpha(env_variable[j]))
				j++;
			str = ft_substr(env_variable, 1, j - 1);
		}
	}
	tmp = ft_search_in_list(env, str);
	if (str)
		free(str);
	if (tmp)
		return ft_strdup(tmp->value);
	else
		return NULL;
}

char *ft_remove_double_quotes(char *word, int *i, t_env **env,int status)
{
	int j;
	char *expand;
	char *tmp;
	char *tmp1;

	expand = NULL; //ft_strdup("");
	j = *i + 1;	   // escape first "

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
				expand = ft_substr(word, j, 2);
				j += 2;
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
					tmp = get_env_value(word + j, env, 1);
					if (tmp)
					{
						expand = ft_strjoin(expand, tmp);
						free(tmp1);
						free(tmp);
						if (word[j] == '$')
							j++;
						if (word[j] == '_')
							j++;
						else
						{
							while (ft_isalpha(word[j]))
								j++;
							//ft_putstr_fd(&word[j],1);
						}
					}
					else if (word[j - 1] != '$')
					{
						//if (word[j + 1] != '"')
						//{
							//// Special Parameters of $
							if (ft_isdigit(word[j + 1]) || ft_strchr("!:#%@-*=", word[j + 1]))
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
									else if (ft_strchr(":%=", word[j + 1]))
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
								char *status_string = ft_int_to_string(status);
								tmp1 = expand;
								expand = ft_strjoin(expand, status_string);
								free(tmp1);
								free(tmp);
								j += 2;
							}
							else if (word[j + 1] && word[j + 1] != '\"' && word[j + 1] != ' ')
							{
								while (ft_isalpha(word[j]) || (word[j] == '$'))
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
						//}
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
							while (ft_isalpha(word[j]))
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
	return expand;
}

void ft_expande_word(char **string, t_env **env_list, int status, int redirection)
{
	char *word;
	char *expanded;
	int i;
	int j;
	int k;
	char *tmp1;
	int back_slash;
	char *tmp;

	word = *string;
	i = 0;
	k = 0;
	expanded = NULL;
	back_slash = 0;
	j = 0;
	tmp1 = NULL;
	tmp = NULL;

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
			tmp = ft_remove_double_quotes(word, &i, env_list,status);
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
				tmp = get_env_value(word + i, env_list, 0);

				if (tmp) /// env variavle exist
				{
					expanded = ft_strjoin(expanded, tmp);
					free(tmp1);
					free(tmp);

					if (word[i] == '$')
						i++;
					if (word[i] == '_')
						i++;
					else
					{
						while (ft_isalpha(word[i]))
							i++;
						//ft_putstr_fd(&word[i],1);
					}
					//i+= ft_strlen(word + i);
				}
				else if (word[i - 1] != '$') /// not exit $$hdj vs $kfjh
				{
					if (word[i + 1] != '"')
					{
						//// Special Parameters of $
						if (ft_isdigit(word[i + 1]) || ft_strchr("!:#%@-*=", word[i + 1]))
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
								else if (ft_strchr(":%=", word[i + 1]))
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
							char *status_string = ft_int_to_string(status);
							tmp1 = expanded;
							expanded = ft_strjoin(expanded, status_string);
							free(tmp1);
							free(tmp);
							i += 2;
						}
						else if (word[i + 1])
						{
							while (ft_isalpha(word[i]) || (word[i] == '$'))
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
					if (!word[i + 1]) //get last dollar after double dollars sign
					{
						//ft_putstr_fd(word + i,1);
						tmp = expanded;
						tmp1 = ft_substr(word, i, 1);
						expanded = ft_strjoin(expanded, tmp1);
						free(tmp);
						free(tmp1);
						i++;
					}
					else
					{
						while (ft_isalpha(word[i]))
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

t_simple_cmd *ft_handle_expanding(t_simple_cmd **cmd)
{
	char **splited;
	char *to_free;
	t_args *new_args;
	t_args *tmp;
	int i;

	
	i = 1;
	to_free = (*cmd)->command;
	splited = ft_split((*cmd)->command,' ');
	
	(*cmd)->command = splited[0];

	new_args = (t_args*)malloc(sizeof(t_args));
	tmp = new_args;
	while(splited[i])
	{
		new_args->value = splited[i];
		new_args->inside_quotes = 0;
		new_args->next = NULL;
		ft_putstr_fd(splited[i],1);
		if (splited[i + 1])
		{
			new_args->next = (t_args*)malloc(sizeof(t_args));
			new_args = new_args->next;
		}
		i++;
	}
	
	new_args->next = (*cmd)->args;

	(*cmd)->args = tmp;
	return (*cmd);
}

void ft_expande_simple_cmd(t_simple_cmd **cmd, t_env **env, int status)
{
	t_args *args;
	t_redirection *redis;
	char *befor_expand_cmd;
	char *after_expand_cmd;
	char *space;
	
	

	redis = NULL;
	if ((*cmd)->command)
	{
		befor_expand_cmd = ft_strdup((*cmd)->command);
		(*cmd)->inside_quotes = check_exiting_of_qoutes(((*cmd)->command));
		ft_expande_word(&((*cmd)->command), env, status, 0);
		after_expand_cmd = ft_strdup((*cmd)->command);
		if ((*cmd)->inside_quotes == 0 && after_expand_cmd && ft_strcmp(befor_expand_cmd,after_expand_cmd) && ft_strchr(after_expand_cmd,' '))
		{
			space = ft_strchr(after_expand_cmd,' ');
			if (++space)
			{
				//ft_putstr_fd(space,1);
				*cmd = ft_handle_expanding(cmd);
			}
		}
	}
	args = (*cmd)->args;
	while (args)
	{
		args->inside_quotes = check_exiting_of_qoutes(args->value);
		ft_expande_word(&args->value, env, status, 0);
		args = args->next;
	}
	redis = (*cmd)->redirections;
	while (redis)
	{
		redis->inside_quotes = check_exiting_of_qoutes(redis->file_name);
		ft_expande_word(&redis->file_name, env, status, 1);
		redis = redis->next;
	}
}

void ft_expanding(t_pipe_line *pipe_line, t_env **env, int status)
{
	t_simple_cmd *current_cmd;
	t_simple_cmd *head_cmd;

	//head_cmd = (*pipe_line)->child;
	current_cmd = NULL;
	head_cmd = NULL;
	ft_putstr_fd(PURPLE, 1);
	head_cmd = pipe_line->child;
	while (head_cmd)
	{
		current_cmd = head_cmd;
		ft_expande_simple_cmd(&current_cmd, env, status);
		head_cmd = head_cmd->next;
	}
}