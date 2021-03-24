/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:38:09 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/24 18:47:17 by zjamali          ###   ########.fr       */
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
	char *file_name;
	struct s_redirection *next;
}t_redirection;

typedef struct s_args{
	char *value;
	struct s_args *next;
}t_args;

typedef struct s_simple_cmd
{
	char *command;
	char **args;
	t_redirection *redirections;
	struct s_simple_cmd *next;
}t_simple_cmd;

typedef struct s_pipe_line
{
	int simple_command_count; // number of simple_cmd in one pipeline // 0 if only simple cmd
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

#endif