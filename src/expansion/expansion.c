/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 14:41:59 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/07 21:53:23 by zjamali          ###   ########.fr       */
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

void	check_word_expand_redtion(int redirection, char **expanded,
		char **word, char **string)
{
	if (redirection == 1)
	{
		if (*expanded != NULL && (*expanded)[0] != '\0')
		{
			*string = *expanded;
		}
		else
		{
			*string = ft_strdup(*word);
		}
	}
	else
	{
		*string = *expanded;
	}
	free(*word);
}

void	ft_expand_backslashes(char *str, int *i, char **expanded)
{
	int		j;
	char	*tmp1;
	char	*tmp;

	tmp = NULL;
	tmp1 = NULL;
	j = *i;
	while (str[j] == '\\')
	{
		tmp1 = *expanded;
		tmp = ft_substr(str, j + 1, 1);// get_character_after back_slash
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
		j += 2;
	}
	*i = j;
}

void	ft_remove_single_quotes(char *str, int *i, char **expanded)
{
	int		j;
	char	*tmp1;
	char	*tmp;

	(*i)++;
	j = *i;
	while (str[j] != '\'')
		j++;
	tmp1 = *expanded;
	tmp = ft_substr(str, *i, j - *i);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp);
	free(tmp1);
	*i = j + 1;
}

void	ft_expand_squence_of_dollar_sign(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 2);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	*i += 2;
}

void	ft_get_charachter(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 1);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	(*i)++;
}

void	ft_expand_underscore_in_double_qts(char *str, int i, char **env_value,
		char *last_argmnt)
{
	if (str[i + 1] == '_' && str[i + 2] == '"' && last_argmnt)
	{
		free(*env_value);
		*env_value = NULL;
		*env_value = ft_strdup(last_argmnt);
	}
}

void	ft_expand_underscore_if_exist(char *str, int i, char **env_value,
		char *last_argmnt)
{
	if (str[i + 1] == '_' && str[i + 2] == '\0' && last_argmnt)
	{
		free(*env_value);
		*env_value = NULL;
		*env_value = ft_strdup(last_argmnt);
	}
}

void	ft_replace_env_by_value(char *str, int *i, char **expanded,
		char **env_value)
{
	char	*tmp;

	tmp = *expanded;
	*expanded = ft_strjoin(*expanded, *env_value);
	free(*env_value);
	free(tmp);
	if (str[*i] == '$')
		(*i)++;
	if (str[*i] == '_')
		(*i)++;
	else
	{
		while (ft_isalpha(str[*i]) || ft_isalnum(str[*i])
			|| str[*i] == '_')
			(*i)++;
	}	
}

void	get_dollar_sign(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 1);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	(*i)++;
}

void	ft_expand_exit_status(int *i, char **expanded,
		char *exit_status)
{
	char	*tmp1;
	char	*tmp;

	tmp = exit_status;
	tmp1 = *expanded;
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	*i += 2;
}

void	ft_skip_characters_env_not_exist(char *str, int *i)
{
	if (str[*i] == '$')
		(*i)++;
	while (ft_isalpha(str[*i]) || ft_isalnum(str[*i])
		|| str[*i] == '_')
		(*i)++;
}

void	ft_digit_env(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	if (str[*i + 1] == '0')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("minishell");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	*i += 2;
}

void	ft_expand_back_slash(int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_strdup("$");
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	(*i)++;
}

void	ft_expand_rest_of_specail_params(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 2);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
}

void	ft_expand_other_special_params(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	if (str[*i + 1] == '#')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("0");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	else if (str[*i + 1] == '-')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("himBH");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	else if (ft_strchr(":%=/!", str[*i + 1]))
		ft_expand_rest_of_specail_params(str, i, expanded);
	*i += 2;
}

void	ft_expande_special_params(char *str, int *i, char **expanded,
		char *exit_status)
{
	if (str[*i + 1] != '"')
	{
		if (str[*i + 1] && (ft_isdigit(str[*i + 1])
				|| ft_strchr("!:#%@-*=/\\", str[*i + 1])))
		{
			if (ft_isdigit(str[*i + 1]))
				ft_digit_env(str, i, expanded);
			else if (ft_strchr("\\", str[*i + 1]))
				ft_expand_back_slash(i, expanded);
			else
				ft_expand_other_special_params(str, i, expanded);
		}
		else if (str[*i + 1] == '?')
			ft_expand_exit_status(i, expanded, exit_status);
		else if (str[*i + 1])
			ft_skip_characters_env_not_exist(str, i);
		else
			get_dollar_sign(str, i, expanded);
	}
	else
		(*i)++;
}

void	ft_expande_special_param_in_dble_g(char *str, int *i, char **expanded,
		char *exit_status)
{

	if (ft_isdigit(str[*i + 1])
		|| ft_strchr("!:#%@-*=/\\", str[*i + 1]))
	{
		if (ft_isdigit(str[*i + 1]))
			ft_digit_env(str, i, expanded);
		else if (ft_strchr("\\", str[*i + 1]))
			ft_expand_back_slash(i, expanded);
		else
			ft_expand_other_special_params(str, i, expanded);
	}
	else if (str[*i + 1] == '?')
		ft_expand_exit_status(i, expanded, exit_status);
	else if (str[*i + 1] && str[*i + 1] != '\"'
		&& str[*i + 1] != ' ')
		ft_skip_characters_env_not_exist(str,i);
	else //// just a 1 dollar sign
		get_dollar_sign(str, i, expanded);
	
}

int	check_end_of_string(char str,int inside_quotes)
{
	if (inside_quotes == 0)
	{
		if (!str)
			return (1);
	}
	else
	{
		if (str == '\"')
			return (1);
	}
	return (0);
}

void	ft_skip_characters_non_env_variable(char *str, int *i, char **expanded,
		int inside_quotes)
{
	char	*tmp1;
	char	*tmp;
	int		end_of_string;

	end_of_string = 0;
	tmp = NULL;
	tmp1 = NULL;
	end_of_string = check_end_of_string(str[*i + 1],inside_quotes);
	if (end_of_string) //get last dollar after double dollarssign
	{
		tmp = *expanded;
		tmp1 = ft_substr(str, *i, 1);
		*expanded = ft_strjoin(*expanded, tmp1);
		free(tmp);
		free(tmp1);
		(*i)++;
	}
	else
		while (ft_isalpha(str[*i]) || str[*i] == '$')
			(*i)++;
}

void	ft_expand_env_variable(t_expansion *expd, int *i, t_env **env_list,
		char **last_env)
{
	if (expd->word[*i + 1] == '$') //// sequence of dollars sign
		ft_expand_squence_of_dollar_sign(expd->word, i, &(expd->expanded));
	else
	{
		expd->tmp = get_env_variable_value(expd->word + *i, env_list);
		if (expd->tmp) /// env variavle exist
		{
			ft_expand_underscore_if_exist(expd->word, *i, &(expd->tmp),
				last_env[1]);
			ft_replace_env_by_value(expd->word, i, &expd->expanded,
				&(expd->tmp));
		}
		else if (i == 0 || expd->word[*i - 1] != '$') /// not exit $$hdj vs $k
			ft_expande_special_params(expd->word, i, &expd->expanded,
				last_env[0]);
		else /// env variavle not exist  no multiple sign dollars
			ft_skip_characters_non_env_variable(expd->word, i,
				&(expd->expanded),0);
	}
}
void	ft_expand_backslashes_in_dble_qts(char *str, int *i, char **expanded)
{
	int		j;
	char	*tmp1;
	char	*tmp;

	tmp = NULL;
	tmp1 = NULL;
	j = *i;
	if (ft_strchr("$\"\\\n`", str[j + 1]))
	{
		tmp = *expanded;
		tmp1 = ft_substr(str, j + 1, 1);
		*expanded = ft_strjoin(*expanded, tmp1);
		j += 2;
		free(tmp);
		free(tmp1);
	}
	else
	{
		tmp = *expanded;
		tmp1 = ft_substr(str, j, 2);
		*expanded = ft_strjoin(*expanded, tmp1);
		j += 2;
		free(tmp);
		free(tmp1);
	}
	*i = j;
}
void	ft_expand_env_variable_in_dble_qte(t_expansion *expd, int *j, t_env **env_list,
		char **last_env)
{
	
	if (expd->word[*j + 1] == '$')
		ft_expand_squence_of_dollar_sign(expd->word, j, &(expd->expanded));
	else
	{
		expd->tmp = get_env_variable_value(expd->word + *j, env_list);
		if (expd->tmp)
		{
			ft_expand_underscore_in_double_qts(expd->word, *j, &(expd->tmp), last_env[1]);
			ft_replace_env_by_value(expd->word, j, &(expd->expanded),&(expd->tmp));
		}
		else if (j == 0 || expd->word[*j - 1] != '$')
			ft_expande_special_param_in_dble_g(expd->word, j,
				&(expd->expanded), last_env[0]);
		else
			ft_skip_characters_non_env_variable(expd->word, j,
				&(expd->expanded), 1);
	}
}

char	*ft_remove_double_quotes(char *word, int *i, t_env **env_list,
		char **last_env)
{
	t_expansion expd;
	int		j;

	expd.expanded = NULL;
	j = *i + 1;
	expd.word = word;
	while (expd.word[j] != '"')
	{
		if (expd.word[j] == '\\')
			ft_expand_backslashes_in_dble_qts(expd.word, &j, &expd.expanded);
		else
		{
			if (expd.word[j] == '$')
				ft_expand_env_variable_in_dble_qte(&expd, &j, env_list, last_env);
			else
				ft_get_charachter(expd.word, &j, &expd.expanded);
		}
	}
	j++;
	*i = j;
	return (expd.expanded);
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

char	*ft_expand_double_quotes(t_expansion expd, int *i, t_env **env_list,
		char **last_env)
{
	expd.tmp1 = expd.expanded;
	expd.tmp = ft_remove_double_quotes(expd.word, i, env_list, last_env);
	if (expd.tmp)
	{
		expd.expanded = ft_strjoin(expd.expanded, expd.tmp);
		free(expd.tmp);
		free(expd.tmp1);
	}
	return (expd.expanded);
}

void	ft_expande_word(char **string, t_env **env_list, char **last_env,
		int redirection)
{
	t_expansion	expd;
	int			i;

	i = 0;
	expd.word = *string;
	expd.expanded = NULL;
	ft_replace_tilde(&expd.word);
	while (expd.word[i])
	{
		if (expd.word[i] == '\\')
			ft_expand_backslashes(expd.word, &i, &expd.expanded);
		else if (expd.word[i] == '\'')
			ft_remove_single_quotes(expd.word, &i, &expd.expanded);
		else if (expd.word[i] == '"')
			expd.expanded = ft_expand_double_quotes(expd, &i, env_list,
					last_env);
		else if (expd.word[i] == '$')
			ft_expand_env_variable(&expd, &i, env_list, last_env);
		else /// not dollars sign no quote just join characters
			ft_get_charachter(expd.word, &i, &expd.expanded);
	}
	check_word_expand_redtion(redirection, &expd.expanded, &expd.word, string);
}

void	ft_assign_value(char	**splited, int i, t_args **new_args)
{
	(*new_args)->value = splited[i];
	(*new_args)->inside_quotes = 0;
	(*new_args)->next = NULL;
	if (splited[i + 1])
	{
		(*new_args)->next = (t_args *)malloc(sizeof(t_args));
		*new_args = (*new_args)->next;
	}
}

t_simple_cmd	*ft_handle_cmd_expanding(t_simple_cmd **cmd)
{
	char	**splited;
	int		i;
	t_args	*new_args;
	t_args	*tmp;

	i = 1;
	splited = ft_split((*cmd)->command, ' ');
	free((*cmd)->command);
	(*cmd)->command = splited[0];
	new_args = (t_args *)malloc(sizeof(t_args));
	tmp = new_args;
	while (splited[i])
	{
		ft_assign_value(splited, i, &new_args);
		i++;
	}
	if (i > 1)
	{
		new_args->next = (*cmd)->args;
		(*cmd)->args = tmp;
	}
	else
		free(new_args);
	free(splited);
	return (*cmd);
}

void	ft_handle_arg_expanding(t_args **args)
{
	char	**splited;
	t_args	*tmp;
	t_args	*next;
	int		i;

	i = 0;
	splited = ft_split((*args)->value, ' ');
	tmp = *args;
	next = (*args)->next;
	free((*args)->value);
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
	tmp->next = next;
	free(splited);
}

void	ft_delete_emty_args_nodes(t_args **args)
{
	t_args	*temp;
	t_args	*prev;

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

t_args	*ft_expand_argument(char *befor_expand_arg, char	*after_expand_arg,
		t_args *args)
{
	t_args	*next_args;
	char	*space;

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
			ft_handle_arg_expanding(&args);
		args = next_args;
	}
	else
		args = args->next;
	free(after_expand_arg);
	return (args);
}

void	ft_expand_arguments(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_args			*args;
	char			*befor_expand_arg;
	char			*after_expand_arg;

	args = (*cmd)->args;
	befor_expand_arg = NULL;
	after_expand_arg = NULL;
	while (args)
	{
		befor_expand_arg = ft_strdup(args->value);
		ft_repalce_space_by_tab(cmd, 0);
		args->inside_quotes = check_exiting_of_qoutes(args->value);
		ft_expande_word(&args->value, env, last_env, 0);
		args = ft_expand_argument(befor_expand_arg, after_expand_arg, args);
		free(befor_expand_arg);
		befor_expand_arg = NULL;
		after_expand_arg = NULL;
	}
}

void	ft_expand_command(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	char	*befor_expand_cmd;
	char	*after_expand_cmd;
	char	*space;

	space = NULL;
	befor_expand_cmd = NULL;
	after_expand_cmd = NULL;
	befor_expand_cmd = ft_strdup((*cmd)->command);
	ft_repalce_space_by_tab(cmd, 1);
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
			*cmd = ft_handle_cmd_expanding(cmd);
	}
	free(befor_expand_cmd);
	free(after_expand_cmd);
}

void	ft_expande_redirection(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_redirection	*redis;

	redis = NULL;
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
}

void	ft_handle_empty_command(t_simple_cmd **cmd)
{
	t_args	*args;
	t_args	*to_free;

	args = (*cmd)->args;
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

void	ft_return_spaces_to_arguments(t_simple_cmd **cmd)
{
	t_args	*args;
	int		i;

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

void	ft_return_spaces(t_simple_cmd **cmd)
{
	int		i;

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
	ft_return_spaces_to_arguments(cmd);
}

void	ft_expande_simple_cmd(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_args			*args;
	t_args			*to_free;

	to_free = NULL;
	args = NULL;
	ft_expand_arguments(cmd, env, last_env);
	if ((*cmd)->command)
		ft_expand_command(cmd, env, last_env);
	ft_expande_redirection(cmd, env, last_env);
	args = (*cmd)->args;
	if (args)
		ft_delete_emty_args_nodes(&(*cmd)->args);
	if (!(*cmd)->command && (*cmd)->inside_quotes == 0)
		ft_handle_empty_command(cmd);
	ft_return_spaces(cmd);
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
