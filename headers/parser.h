/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:38:09 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/16 15:33:09 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum e_redirection_type{
	STD_IN,
	STD_OUT,
	SRD_ERROR,
} t_redirection_type;


typedef struct s_redirection
{
	int index;
	t_redirection_type type;
	int io_number;
	char *file_name;
	struct s_redirection *next;
}t_redirection;
/*
typedef struct s_simple_cmd
{
	char *command;
	char **args;
	t_redirection *redirections;
	struct  s_simple_cmd *next;
}t_simple_cmd;
*/
/*
typedef struct s_pipe_line
{
	int simple_command_count; // number of simple_cmd in one pipeline
	t_simple_cmd *simple_commands;
	struct s_pipe_line *next;
}t_pipe_line;
*/
/*
typedef struct s_command_list
{
	char *command_line;
	int pipe_line_count; // number of simple_cmd in one pipeline
	t_pipe_line *pipe_lines; // list of pipelines 
}t_command_list;
*/
/*
typedef enum e_node_type{
	CMD_LIST,
	PIPE_LINE,
	SIMPLE_CMD,
}t_node_type;
*/

typedef struct s_node
{
	/* NODE TYPE */
	enum e_node_type{
	CMD_LIST,
	PIPE_LINE,
	SIMPLE_CMD,
	}t_node_type;
	
	 /* NODE  DATA */
	union u_node_data
	{
		struct s_simple_cmd
		{
			char *command;
			char **args;
			t_redirection *redirections;
			struct s_node *sibling;
		}t_simple_cmd;
		
		struct s_pipe_line
		{
			int simple_command_count; // number of simple_cmd in one pipeline
			struct s_node *sibling;
			struct s_node *child;
		}t_pipe_line;
		
		struct s_command_list
		{
			char *command_line;
			int pipe_line_count; // number of simple_cmd in one pipeline
			struct s_node *child; // list of pipelines
		}t_command_list;
		
		
		

		//struct s_node *sibling;
		//struct s_node *child;
	}t_node_data;
}t_node;




void ft_parser(t_token *tokens_list);

#endif