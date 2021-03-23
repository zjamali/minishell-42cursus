/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:37:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/23 18:45:52 by zjamali          ###   ########.fr       */
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
			return 1;
		else
		{
			return 0;
		}
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

int ft_count_command_list(t_token *tokens_list)
{
	t_token *tmp;
	int count;
	
	count = 1;
	tmp = tokens_list;
	while(tmp->next->type != NEWLINE) /// dont count semi before newline
	{
		if (tmp->type == SEMI)
			count++;
		tmp = tmp->next;
	}
	return count;
}


int ft_count_pipe_line(t_token *begin_token,int *token_index)
{
	int count ;
	
	count = 1;
	if (begin_token->type == SEMI && begin_token->next->type != NEWLINE)
		begin_token = begin_token->next;
	while (begin_token->type != NEWLINE && begin_token->type != SEMI)
	{
		if (begin_token->type == PIPE)
			count++;
		begin_token = begin_token->next;
	}
	*token_index = begin_token->index;
	return count;
}

t_token *ft_begin_token(int token_index,t_token *begin_token)
{
	while (begin_token->index < token_index)
	{
		begin_token = begin_token->next;
	}
	return begin_token;
}

t_command_list *ft_create_ast(t_token *tokens_list)
{
	t_command_list *head;
	t_command_list *tmp;
	//int index;
	int token_index;
	t_token *begin_token;
//	
//
//	token_index = 1; // escape none token 
//	int count_cmd_list;
//	head =  malloc(sizeof(t_command_list)); /// creat ast root;
//	tmp = head;
//	count_cmd_list = ft_count_command_list(tokens_list);
//	write(1,YELLOW,ft_strlen(YELLOW));
//	ft_putstr_fd("cmd list :",1);
//	ft_putnbr_fd(count_cmd_list,1);
//	ft_putstr_fd("\n",1);
//	begin_token = tokens_list->next; // get token escape none
//	token_index = begin_token->index;
//	while (count_cmd_list > 0)
//	{
//		
//		int pipeline = ft_count_pipe_line(begin_token,&token_index);
//		write(1,PURPLE,ft_strlen(PURPLE));
//		ft_putstr_fd("Simple CMD :",1);
//		ft_putnbr_fd(pipeline,1);
//		ft_putstr_fd("\n",1);
//		
//		begin_token = ft_begin_token(token_index,begin_token);
//		count_cmd_list--;
//	}
	return head;
}

t_command_list *ft_parser(t_token *tokens_list)
{
	t_command_list *command_list;
	command_list = NULL;
	write(1,RED,ft_strlen(RED));
	if (!ft_check_syntax(tokens_list))
		command_list = ft_create_ast(tokens_list);
	return (command_list);
}