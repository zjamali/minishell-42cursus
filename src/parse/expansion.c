/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 14:41:59 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/30 17:12:04 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

char *ft_remove_double_quotes(char *word,int *i,t_env **env)
{
	int j ;
	char *expand;
	char *tmp;
	char *tmp1;

	expand = NULL;//ft_strdup("");
	j = *i + 1; // escape first "

	while (word[j] != '"')
	{
		if (word[j] == '\\')
		{		
			tmp = expand;
			tmp1 = ft_substr(word,j+1,1);
			expand = ft_strjoin(expand,tmp1);
			j+=2;
			free(tmp);
			free(tmp1);
		}
		else
		{
			if (word[j] == '$')
			{
				tmp = expand;
				tmp1 = ft_substr(word,j,1);
				expand = ft_strjoin(expand,tmp1);
				free(tmp);
				free(tmp1);
				j++;
				(*env)++;
			}
			else
			{
				tmp = expand;
				tmp1 = ft_substr(word,j,1);
				expand = ft_strjoin(expand,tmp1);
				free(tmp);
				free(tmp1);
				j++;
			}
		}
	}
	j++; // last "
	*i = j;
	return expand;
}

char *get_env_value(char *env_variable,t_env **env)
{
	t_env *tmp;
	
	
	tmp = ft_search_in_list(env,env_variable + 1);
	if(tmp)
		return ft_strdup(tmp->value);
	else
	{
		return NULL;
	}
}

void ft_remove_quote(char **string,t_env **env_list)
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
				tmp = ft_substr(word,j+1,1);
				expanded = ft_strjoin(expanded,tmp);
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
			tmp = ft_substr(word,i,j - i);
			expanded = ft_strjoin(expanded,tmp);
			free(tmp);
			free(tmp1);
			i = j + 1;
		}
		else if (word[i] == '"')
		{
			tmp1 = expanded;
			tmp = ft_remove_double_quotes(word,&i,env_list);
			if (tmp)
			{
				expanded = ft_strjoin(expanded,tmp);
				free(tmp);
				free(tmp1);
			}
		}
		else if (word[i] == '$')
		{
			tmp1 = expanded;
			tmp = get_env_value(word + i,env_list);
			if (tmp)
			{
				expanded = ft_strjoin(expanded,tmp);
				free(tmp1);
				free(tmp);
			}
			i+= ft_strlen(word + i);
		}
		else
		{
			tmp1 = expanded;
			tmp = ft_substr(word,i,1);
			expanded = ft_strjoin(expanded,tmp);
			free(tmp1);
			free(tmp);
			i++;
		}
	}
	free(word);
	*string = expanded;
}

void ft_expande_simple_cmd(t_simple_cmd **cmd,t_env **env)
{
	t_args *args;
	t_redirection *redis;
	char *tmp;
	
	tmp = NULL;
	redis = NULL;
	if ((*cmd)->command)
		ft_remove_quote(&((*cmd)->command),env);
	args = (*cmd)->args;
	while (args)
	{
		ft_remove_quote(&args->value,env);
		args = args->next;
	}
	redis = (*cmd)->redirections;
	while (redis)
	{
		ft_remove_quote(&redis->file_name,env);
		redis = redis->next;
	}
}

void ft_expanding(t_pipe_line *pipe_line,t_env **env)
{
	t_simple_cmd *current_cmd;
	t_simple_cmd *head_cmd;

	//head_cmd = (*pipe_line)->child;
	current_cmd = NULL;
	head_cmd = NULL;
	ft_putstr_fd(PURPLE,1);
	head_cmd = pipe_line->child;
	while (head_cmd)
	{
		current_cmd = head_cmd;
		ft_expande_simple_cmd(&current_cmd,env);
		head_cmd = head_cmd->next;
	}
}

//// ?^H""
///// >""