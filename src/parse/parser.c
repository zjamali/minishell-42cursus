/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:37:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/15 14:57:01 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

///#include "../headers/parser.h"
#include "../../headers/minishell.h"

void ft_destroy_redirection(t_redirection *redis)
{
	t_redirection *current_redi;
	t_redirection *parent_redi;
	
	parent_redi = redis;
	while (parent_redi)
	{
		current_redi = parent_redi;
		parent_redi = parent_redi->next;
		
		free(current_redi->file_name);
		free(current_redi);
	}
}


void ft_destroy_simple(t_simple_cmd *cmd)
{
	int i;
	t_args *head;
	t_args *current_arg;

	i = 0;
	head = NULL;
	if (cmd->command)
		free(cmd->command);
	head= cmd->args ;
	while (head != 0)
	{
		current_arg = head;
		head = head->next;
		
		free(current_arg->value);
		free(current_arg);
		current_arg = NULL;
		i++;
	}
	current_arg = NULL;
	if (cmd->redirections)
		ft_destroy_redirection(cmd->redirections);
	cmd->redirections = NULL;
	free(cmd);
}

void ft_destroy_pipe_line(t_pipe_line *pipe_line)
{
	t_simple_cmd *current_cmd;
	t_simple_cmd *parent_cmd;
	
	parent_cmd = pipe_line->child;
	while (parent_cmd)
	{
		current_cmd = parent_cmd;
		parent_cmd = parent_cmd->next;
		ft_destroy_simple(current_cmd);
		current_cmd = NULL;
	}
	free(pipe_line);
}

void ft_destroy_ast(t_command_list *cmd_list)
{
	t_pipe_line *current_pipe;
	t_pipe_line *parent_pipe;

	ft_putstr_fd(RED,1);
	ft_putstr_fd("destroy ast\n",1);
	parent_pipe = cmd_list->childs;
	while(parent_pipe)
	{
		current_pipe = parent_pipe;
		parent_pipe = parent_pipe->next;
		ft_destroy_pipe_line(current_pipe);
		current_pipe = NULL;
	}
	free(cmd_list);
}

void ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",1);
	ft_putstr_fd(token->value,1);
	ft_putstr_fd("'\n",1);
}


void ft_print_simple_cmd(t_simple_cmd *cmd)
{
	t_redirection *tmp;
	t_args *tmp1;

	tmp1 = cmd->args;
	tmp = cmd->redirections;
	ft_putstr_fd(YELLOW,1);
	ft_putstr_fd("cmd -> ",1);
	ft_putstr_fd("{",1);
	if (cmd->command)
		write(1,cmd->command,ft_strlen(cmd->command));
	ft_putstr_fd("}",1);
	ft_putstr_fd("\t",1);
	ft_putstr_fd("env_cmd -> ",1);
	ft_putnbr_fd(cmd->cmd_env,1);
	ft_putstr_fd("\n",1); 
	ft_putstr_fd("args -> ",1);
	while (tmp1)
	{
		ft_putstr_fd("[",1);
		ft_putstr_fd("{",1);
		ft_putstr_fd(tmp1->value,1);
		ft_putstr_fd("}",1);
		ft_putstr_fd("\t",1);
		ft_putstr_fd("env_arg -> ",1);
		ft_putnbr_fd(tmp1->env_variable,1);
		ft_putstr_fd("] ",1);
		tmp1 = tmp1->next;
	}
	ft_putstr_fd("\n",1);

	ft_putstr_fd("redirs ->",1);
	while (tmp != NULL)
	{
		ft_putstr_fd("(",1);
		ft_putnbr_fd(tmp->type,1);
		ft_putstr_fd(" ",1);
		ft_putstr_fd("{",1);
		ft_putstr_fd(tmp->file_name,1);
		ft_putstr_fd("}",1);
		ft_putstr_fd("\t",1);
		ft_putstr_fd("redis -> ",1);
		ft_putnbr_fd(tmp->filename_env,1);
		ft_putstr_fd(") ",1);
		tmp= tmp->next;
	}
	ft_putstr_fd("\n",1);
}

void ft_print_pipeline_cmd(t_pipe_line *pipe_line)
{
	t_simple_cmd *tmp;
	int i;

	i  = 0;
	ft_putstr_fd("simple_cmd :",1);
	ft_putnbr_fd(pipe_line->simple_cmd_count,1);
	ft_putstr_fd("\n",1);
	tmp = pipe_line->child;
	while (tmp != NULL)
	{
		ft_putstr_fd(PURPLE,1);
		ft_putstr_fd("pipe : ",1);
		ft_putnbr_fd(i,1);
		ft_putstr_fd("\n",1);
		ft_print_simple_cmd(tmp);
		tmp = tmp->next;
		i++;
	}
}

void ft_print_cmd_list(t_command_list *cmd_list)
{
	t_pipe_line *tmp ;

	tmp = cmd_list->childs;
	int i = 0;
	ft_putstr_fd("\n",1);
	ft_putstr_fd("pipeline_count :",1);
	ft_putnbr_fd(cmd_list->pipe_line_count,1);
	while (tmp)
	{
		ft_putstr_fd("\n",1);
		ft_putstr_fd(BLUE,1);
		ft_putstr_fd("pipeline  : ",1);
		ft_putnbr_fd(i,1);
		ft_putstr_fd("\n",1);
		ft_print_pipeline_cmd(tmp);
		ft_putstr_fd("\n",1);
		tmp = tmp->next;
		i++;
	}
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
			back_slash++;
			i++;
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
			back_slash = 0;
		}
		else if (quote == 0 && word[i] == '\''/*  && word[i - 1] != 92*/)
		{
			if (word[i - 1] != 92)
			{
				quote += 1;
			}
			else
			{
				ft_putnbr_fd(back_slash,1);
				if (back_slash % 2 == 0)
				{
					quote += 1 ;
				}
			}
			i++;
			back_slash = 0;
		}
		else if (quote == 2 && word[i] == 34  /*&& back_slash % 2 == 0 && word[i - 1] != 92*/)
		{
			if (word[i - 1] != 92)
				quote -= 2;
			else
			{
				if (back_slash % 2 == 0)
					quote -= 2;
			}
			back_slash = 0;
			i++;
		}
		else if (quote == 1 && word[i] == '\'')
		{
			i++;
			quote -= 1;
			back_slash = 0;
		}
		else if (quote == 2 && word[i] == '`') // check ' case 
		{
			if (word[i - 1] != 92)
				quote -= 2;
			else
			{
				if (back_slash % 2 == 0)
				{
					back_slash = 0;
					quote -= 2;
				}
			}
			i++;				
		}
		else
		{
			i++;
			back_slash = 0;
		}
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
			if (ft_check_backslash(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				result = 1;
				break;
			}
			if (ft_check_closing_quotes(tmp->value))
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
			//else if (tmp->next->type == NEWLINE)
			//{
			//	ft_destoy_token_list(tokens_list);
			//	result = 1;
			//	break; //// for dont get segfault  in tmp = tmp->next;
			//}
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

	if (!(cmd_list = malloc(sizeof(t_command_list))))
		return NULL;
	cmd_list->childs = NULL;
	cmd_list->pipe_line_count = 0;
	return cmd_list;
}

t_redirection *ft_create_redirection(t_token **tokens,int index)
{
	t_redirection *redirection ;
	if (!(redirection = malloc(sizeof(t_redirection))))
		return NULL;
	redirection->index = index;
	redirection->next = NULL;
	if ((*tokens)->type == GREAT)
		redirection->type = RE_GREAT;
	else if ((*tokens)->type == DOUBLE_GREAT)
		redirection->type = RE_DOUBLE_GREAT;
	else if ((*tokens)->type == LESS)
		redirection->type = RE_LESS;
	(*tokens) = (*tokens)->next;
	redirection->file_name = ft_strdup((*tokens)->value);
	(*tokens) = (*tokens)->next;
	return redirection;
}

t_redirection *ft_insert_redirection(t_redirection *redirection,t_token **tokens,int index)
{
	t_redirection *tmp;

	tmp = NULL;
	if (redirection == NULL)
	{
		
		redirection = ft_create_redirection(tokens,index);
	}
	else
	{
		tmp = redirection;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redirection(tokens,index);
	}
	
	return (redirection);
}

void ft_insert_arg(t_args *head,t_args *current_args)
{
	t_args *tmp;
	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_args;
}

t_args *ft_create_arg(char *value)
{
	t_args *arg;
	
	arg = malloc(sizeof(t_args));
	arg->value = ft_strdup(value);
	arg->next = NULL;
	arg->env_variable = 0;
	return arg;
}

t_simple_cmd *ft_create_simple_cmd(t_token **tokens)
{
	t_simple_cmd *cmd;
	int i;
	int r;
	int j;
	t_args *tmp;
	

	j = 0;
	i = 0;
	r = 0;
	tmp = NULL;
	if (!(cmd = malloc(sizeof(t_simple_cmd))))
		return NULL;
	cmd->args = NULL;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	while ((*tokens)->type != PIPE && (*tokens)->type != SEMI && (*tokens)->type != NEWLINE)
	{
		if ((*tokens)->type == GREAT || (*tokens)->type == DOUBLE_GREAT || (*tokens)->type == LESS)
		{
			cmd->redirections = ft_insert_redirection(cmd->redirections,tokens,r);
			r++;
		}
		else if ((*tokens)->type == WORD)
		{
			if (cmd->command == NULL)
			{
				cmd->command = strdup((*tokens)->value);
			}
			else
			{
				if (cmd->args == NULL)
				{
					cmd->args = ft_create_arg((*tokens)->value);
				}
				else
				{
					tmp = ft_create_arg((*tokens)->value);
					ft_insert_arg(cmd->args,tmp);
				}
			}
			(*tokens) = (*tokens)->next;
		}
		else
			(*tokens) = (*tokens)->next;
	}
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

	if (!(pipe_line = malloc(sizeof(t_pipe_line))))
		return NULL;
	pipe_line->next = NULL;
	pipe_line->child = NULL;
	pipe_line->simple_cmd_count = 0;
	current_cmd = NULL;
	head = NULL;
	while ((*tokens)->type != NEWLINE)
	{
		if (head == NULL)
		{
			head =  ft_create_simple_cmd(tokens);
			pipe_line->simple_cmd_count+=1;
		}
		else
		{
			if ((*tokens)->type == SEMI || (*tokens)->type == NEWLINE)
				break;
			(*tokens) = (*tokens)->next;
			current_cmd = ft_create_simple_cmd(tokens);
			ft_insert_simple_cmd(head,current_cmd);
			pipe_line->simple_cmd_count+=1;
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
	pipe_count = 0;
	head = init_cmd_list(); // create first pipeline
	current_pipeline = NULL;
	while (tokens_list->type != NEWLINE)
	{
		if (head->childs == NULL)
		{
			head->childs = ft_create_pieline(&tokens_list);
			head->pipe_line_count += 1;
		}
		else if (tokens_list->type == SEMI)
		{
			tokens_list = tokens_list->next; /// escape semi
			current_pipeline = head->childs;
			while (current_pipeline->next != NULL)
				current_pipeline = current_pipeline->next;
			if (tokens_list->type != NEWLINE && tokens_list->type != SEMI)
			{
				current_pipeline->next = ft_create_pieline(&tokens_list);
				head->pipe_line_count += 1;
			}
		}
		else
				tokens_list = tokens_list->next;
	}
	return (head);
}

/***************************************/

t_command_list *ft_parser(t_token *tokens_list)
{
	t_command_list *command_list;
	command_list = NULL;
	write(1,RED,ft_strlen(RED));
	if (!ft_check_syntax(tokens_list))
	{
		command_list = ft_create_ast(tokens_list);
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}


//// ""