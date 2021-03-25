/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:37:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/24 19:49:26 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

///#include "../headers/parser.h"
#include "../headers/minishell.h"

void ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",1);
	ft_putstr_fd(token->value,1);
	ft_putstr_fd("'\n",1);
}
void ft_print_simple_cmd(t_simple_cmd *cmd, int len)
{
	t_redirection *tmp;

	tmp = cmd->redirections;
	int i = 0;
	ft_putstr_fd(YELLOW,1);
	
	ft_putstr_fd("cmd ->",1);
	ft_putstr_fd(cmd->command,1);
	ft_putstr_fd("\n",1);
	
	ft_putstr_fd("args ->",1);
	while (len > 0)
	{
		ft_putstr_fd("[",1);
		ft_putstr_fd(cmd->args[i],1);
		ft_putstr_fd("] ",1);
		i++;
		len--;
	}
	ft_putstr_fd("\n",1);

	ft_putstr_fd("redirs ->",1);
	while (tmp != NULL)
	{
		ft_putstr_fd("(",1);
		ft_putnbr_fd(tmp->type,1);
		ft_putstr_fd(" ",1);
		ft_putstr_fd(tmp->file_name,1);
		ft_putstr_fd(") ",1);
		i++;
		tmp= tmp->next;
		len--;
	}
	ft_putstr_fd("\n",1);
}
void ft_destoy_token_list(t_token *tokens_list)
{
	t_token *tmp;

	tmp = NULL;
	while (tokens_list->type != NEWLINE)
	{
		tmp = tokens_list;
		tokens_list = tokens_list->next;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
	if (tokens_list->type == NEWLINE)
	{
		free(tokens_list->value);
		tokens_list->value = NULL;
		free(tokens_list);
		tokens_list = NULL;
		write(1,CYAN,ft_strlen(CYAN));
		ft_putstr_fd("token_list freed\n",1);
	}
}

int ft_check_closing_quotes(char *word)
{
	int i ;
	int quote;
	int back_slash;

	back_slash = 0;
	i = 0;
	quote = 0;
	while (word[i])
	{
		
		while (word[i] == 92) // get all backslashes and count them
		{
			i++;
			back_slash++;
		}
		if (quote == 0 && word[i] == 34/* && word[i - 1] != 92*/)
		{
			if (word[i - 1] != 92)
				quote += 2;
			else
			{
				if (back_slash % 2 == 0)
					quote += 2;
			}
			i++;
		}
		else if (quote == 0 && word[i] == '\''/*  && word[i - 1] != 92*/)
		{
			if (word[i - 1] != 92)
			{
				quote += 1;
			}
			else
			{
				if (back_slash % 2 == 0)
					quote += 1 ;
			}
			i++;
		}
		else if (quote == 2 && word[i] == 34  /*&& word[i - 1] != 92*/)
		{
			if (word[i - 1] != 92)
				quote -= 2;
			else
			{
				if (back_slash % 2 == 0)
					quote -= 2;
			}
			i++;
		}
		else if (quote == 1 && word[i] == '\'')
		{
			i++;
			quote -= 1;
		}
		else if (quote == 2 && word[i] == '`') // check ' case 
		{
			if (word[i - 1] != 92)
				quote -= 2;
			else
			{
				if (back_slash % 2 == 0)
					quote -= 2;
			}
			i++;				
		}
		else
			i++;
	}
	if (quote != 0)
		return 1;
	else
		return 0;
}

int ft_check_backslash(char *word)
{
	int i;
	int cont;
	
	i = 0;
	cont = 0;
	while (word[i])
		i++;
	if (word[i - 1] == '\\')
	{
		i--;
		while(word[i] == '\\')
		{
			cont++;
			i--;
		}
		if ((cont % 2) != 0)
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

int ft_check_syntax(t_token *tokens_list)
{
	int result;
	t_token *tmp;
	
	result = 0;
	tmp = tokens_list;
	while(tmp->type != NEWLINE)
	{
		/*** check the first token must be word or redierction ***/
		if (tmp->type == NONE)
		{
			if (tmp->next->type == PIPE || tmp->next->type == SEMI)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; //// for dont get segfault  in tmp = tmp->next;
			}
			else if (tmp->next->type == NEWLINE)
			{
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; //// for dont get segfault  in tmp = tmp->next;
			}
		}
		/*** if token is redirection ***/
		else if (tmp->type == GREAT || tmp->type == DOUBLE_GREAT || tmp->type == LESS)
		{
			if (tmp->next->type != WORD)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; /// for egfault
			}
		}
		else if (tmp->type == PIPE)
		{
			if (tmp->next->type == PIPE || tmp->next->type == SEMI)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; /// for egfault
			}
			else if (tmp->next->type == NEWLINE)
			{
				ft_print_systax_error(tmp);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; // for segfqult
			}	
		}
		else if (tmp->type == WORD && tmp->next->type == NEWLINE)
		{
			if (ft_check_closing_quotes(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break;
			}

			if (ft_check_backslash(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break;
			}
		}
		else if (tmp->type == WORD) ///// check multiline
		{
			if (ft_check_closing_quotes(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break;
			}
		}
		else if (tmp->type == SEMI) ///// check multiline
		{
			if (tmp->next->type == PIPE || tmp->next->type == SEMI)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; //// for dont get segfault  in tmp = tmp->next;
			}
			else if (tmp->next->type == NEWLINE)
			{
				ft_destoy_token_list(tokens_list);
				result = 1;
				break; //// for dont get segfault  in tmp = tmp->next;
			}
		}
		tmp = tmp->next;
	}
	return (result);
}



t_token *ft_begin_token(int token_index,t_token *begin_token)
{
	while (begin_token->index < token_index)
	{
		begin_token = begin_token->next;
	}
	return begin_token;
}


/*****************************/

t_command_list *init_cmd_list(void)
{
	t_command_list *cmd_list;

	cmd_list = malloc(sizeof(t_command_list));
	cmd_list->childs = NULL;
	cmd_list->pipe_line_count = 0;
	return cmd_list;
}

t_redirection *ft_create_redirection(t_token **tokens,int index)
{
	t_redirection *redirection ;


	redirection = malloc(sizeof(t_redirection));
	redirection->index = index;
	if ((*tokens)->type == GREAT)
		redirection->type = RE_GREAT;
	else if ((*tokens)->type == DOUBLE_GREAT)
		redirection->type = RE_DOUBLE_GREAT;
	else if ((*tokens)->type == LESS)
		redirection->type = RE_LESS;
	(*tokens) = (*tokens)->next;
	redirection->file_name = (*tokens)->value;
	(*tokens) = (*tokens)->next;
	return redirection;
}

t_redirection *ft_insert_redirection(t_redirection *redirection,t_token **tokens,int index)
{
	t_redirection *tmp;

	tmp = NULL;
	if (redirection == NULL)
		redirection = ft_create_redirection(tokens,index);
	else
	{
		tmp = redirection;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redirection(tokens,index);
	}
	return (redirection);
}

int ft_get_number_of_arguments(t_token *tokens)
{
	int len;

	len = 0;
	while(tokens->type != PIPE && tokens->type != SEMI && tokens->type != NEWLINE)
	{
		if (tokens->type != LESS && tokens->type != GREAT && tokens->type != DOUBLE_GREAT && tokens->next->type == WORD)
			len++;
		tokens = tokens->next;
	}
	if (len == 1)
		return (0); /// no args
	return (len - 1);  // -1 for cmd
}

t_simple_cmd *ft_create_simple_cmd(t_token **tokens)
{
	t_simple_cmd *cmd;
	int i;
	int r;
	int len;
	int j = 0;

	i = 0;
	r = 0;
	len = 0;
	cmd = malloc(sizeof(t_simple_cmd));
	len = ft_get_number_of_arguments(*tokens);
	if (len > 0)
		cmd->args = malloc(sizeof(char*) * (len));
	while ((*tokens)->type != PIPE && (*tokens)->type != SEMI && (*tokens)->type != NEWLINE)
	{
		if ((*tokens)->type == GREAT || (*tokens)->type == DOUBLE_GREAT || (*tokens)->type == LESS)
		{
			cmd->redirections = ft_insert_redirection(cmd->redirections,tokens,r);
			r++;
		}
		else if ((*tokens)->type == WORD)
		{
			if (i == 0 && (*tokens)->type == WORD)
			{
				cmd->command = ft_strdup((*tokens)->value);
					i++;
			}
			else
			{
				if (len > 0)
				{
					cmd->args[j] = ft_strdup((*tokens)->value);
					j++;
				}
			}
			(*tokens) = (*tokens)->next;
		}
		else 
			(*tokens) = (*tokens)->next;
	}
	ft_print_simple_cmd(cmd,len);
	return cmd;
}

void ft_insert_simple_cmd(t_simple_cmd *head,t_simple_cmd *current_cmd)
{
	t_simple_cmd *tmp;
	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_cmd;
}

t_pipe_line *ft_create_pieline(t_token **tokens)
{
	t_pipe_line *pipe_line;
	t_simple_cmd *head;
	t_simple_cmd *current_cmd;
	pipe_line = malloc(sizeof(t_pipe_line));
	pipe_line->simple_command_count = 0;
	while ((*tokens)->type != NEWLINE)
	{
		if ((*tokens)->type == PIPE ||  (*tokens)->type == SEMI)
			break;
		
		if (pipe_line->simple_command_count == 0)
			head =  ft_create_simple_cmd(tokens);
		else
		{
			current_cmd = ft_create_simple_cmd(tokens);
			ft_insert_simple_cmd(head,current_cmd);
		}
	}
	pipe_line->child = head;
	return pipe_line;
}

t_command_list *ft_create_ast(t_token *tokens_list)
{
	t_command_list *head;
	t_pipe_line *current_pipeline;
	int pipe_count;
	//t_simple_cmd *current_cmd_list;

	pipe_count = 0;

	head = init_cmd_list(); // create first pipeline

	current_pipeline = NULL;
	while (tokens_list->type != NEWLINE)
	{
		//if (head->pipe_line_count == 0)
		//{
			head->childs = ft_create_pieline(&tokens_list);
			head->pipe_line_count++;
		//}
		//else
		//{
		//	current_pipeline = head->childs;
		//	while (current_pipeline->next != NULL)
		//		current_pipeline = current_pipeline->next;
		//	current_pipeline->next = ft_create_pieline(&tokens_list);
		//	head->pipe_line_count++;
		//}
	}
	return head;
}

/***************************************/


t_command_list *ft_parser(t_token *tokens_list)
{
	t_command_list *command_list;
	command_list = NULL;
	write(1,RED,ft_strlen(RED));
	if (!ft_check_syntax(tokens_list))
		command_list = ft_create_ast(tokens_list);
	return (command_list);
}