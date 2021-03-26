/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:38:09 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/26 16:07:54 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

void ft_destoy_token_list(t_token *tokens_list);
typedef enum e_redirection_type{
	RE_GREAT,
	RE_DOUBLE_GREAT,
	RE_LESS,
} t_redirection_type;

typedef struct s_redirection
{
	int index;
	t_redirection_type type;
	int filename_env; // 1 if file_name is a env varaible
	char *file_name;
	struct s_redirection *next;
}t_redirection;

typedef struct s_args{
	int env_variable; // 1 speacial meaning ; 0 
	char *value; // argument 
	struct s_args *next;
}t_args;

typedef struct s_simple_cmd
{
	int cmd_env; // check if command is env variable
	char *command;
	t_args *args;
	t_redirection *redirections;
	struct s_simple_cmd *next;
}t_simple_cmd;

typedef struct s_pipe_line
{
	int simple_cmd_count;
	struct s_pipe_line *next; ///  next pipeline
	t_simple_cmd *child;
}t_pipe_line;


typedef struct s_command_list
{
	//char *command_line; // pipe = 0 
	int pipe_line_count; // number of simple_cmd in one pipeline
	t_pipe_line *childs;
}t_command_list;

t_command_list *ft_parser(t_token *tokens_list);
void ft_destroy_ast(t_command_list *cmd_list);
int ft_check_syntax(t_token *tokens_list);

#endif