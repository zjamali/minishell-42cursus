/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:41:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/13 13:27:36 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/stat.h>
# include <errno.h>
# include "../src/libft/libft.h"
#define GREEN "\e[1;32m"
#define RESET "\e[0m"
#define RED "\e[1;91m"
#define CYAN "\e[1;36m"
#define YELLOW "\e[1;33m"
#define PURPLE "\e[1;35m"
#define BLUE "\e[1;34m"
#define YES 1
#define NO 0


typedef enum e_token_type{
	NONE,
	WORD,
	SEMI,
	GREAT,
	LESS,
	DOUBLE_GREAT,
	PIPE,
	NEWLINE,
} t_token_type;

typedef struct s_token
{
	int index;
	t_token_type type;
	char *value;
	char *fields;
	struct s_token *next;
}t_token;


typedef struct s_env
{
	char *name;
	char *value;
	struct s_env *next;
}				t_env;


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

int get_next_line(char **line);

t_token	*ft_lexer(char *line);
void ft_destoy_token_list(t_token *tokens_list);
t_command_list *ft_parser(t_token *tokens_list);
void ft_destroy_ast(t_command_list *cmd_list);
int ft_check_syntax(t_token *tokens_list);

void ft_expanding(t_pipe_line *pipe_line,t_env **env);
void ft_print_pipeline_cmd(t_pipe_line *pipe_line);
void ft_print_cmd_list(t_command_list *cmd_list);
void ft_print_simple_cmd(t_simple_cmd *cmd);
#endif