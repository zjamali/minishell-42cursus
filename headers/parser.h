/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:38:09 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/23 18:27:31 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

void ft_destoy_token_list(t_token *tokens_list);
typedef enum e_redirection_type{
	STD_IN,
	STD_OUT,
	STD_ERROR,
	OTHER,
} t_redirection_type;

typedef struct s_redirection
{
	int index;
	t_redirection_type type;
	int io_number;
	char *file_name;
	struct s_redirection *next;
}t_redirection;


typedef struct s_simple_cmd
{
	char *command;
	char **args;
	t_redirection *redirections;
	struct s_simple_cmd *next;
}t_simple_cmd;

typedef struct s_pipe_line
{
	int simple_command_count; // number of simple_cmd in one pipeline
	struct s_pipe_line *next; ///  next pipeline
	t_simple_cmd *bottom;
}t_pipe_line;


typedef struct s_command_list
{
	//char *command_line; // pipe = 0 
	int pipe_line_count; // number of simple_cmd in one pipeline
	struct s_command_list *next;
	t_pipe_line *bottom;
}t_command_list;


t_command_list *ft_parser(t_token *tokens_list);

#endif