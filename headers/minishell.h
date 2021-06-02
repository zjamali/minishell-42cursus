/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:41:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/02 20:39:15 by mbari            ###   ########.fr       */
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
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <termcap.h>
#include <sys/ioctl.h>
#include "../src/libft/libft.h"

#define GREEN "\e[1;32m"
#define RESET "\e[0m"
#define RED "\e[1;91m"
#define CYAN "\e[1;36m"
#define YELLOW "\e[1;33m"
#define PURPLE "\e[1;35m"
#define BLUE "\e[1;34m"
#define YES 1
#define NO 0

/******* READLINE **********/

# define D_KEY_ESCAPE 27
# define D_KEY_SPACE 32
# define D_KEY_UP 4283163
# define D_KEY_DOWN 4348699
# define D_KEY_RIGHT 4414235
# define D_KEY_LEFT 4479771
# define D_KEY_ENTER 10
# define D_KEY_BACKSPACE 127
# define D_KEY_CTRL_C 3
# define D_KEY_CTRL_D 4

typedef struct s_char_list{
	char value;
	int len;
	struct s_char_list *next;
}t_char_list;

typedef struct s_lines_list{
	//char *value;
	char value;
	int history;
	int index;
	bool up_or_down;
	t_char_list *char_list;
	t_char_list *origin_char_list;
	struct s_lines_list *next;
	struct s_lines_list *prev;
	
}t_lines_list;
typedef struct s_cursor{
	int line_postion;
	int col_position;
}t_cursor;
typedef struct s_readline
{
	char	*term_type;
	int		term_fd;
	int		line_count;
	int		colums_count;
	long	c;
	char	*path;
	char	*line;
	t_cursor cursor;
	struct winsize *window;
	//t_lines_list *line_list;
}t_readline;

/******* LESXER **********/
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
	struct s_token *next;
}t_token;


/******* PARSER **********/
typedef enum e_redirection_type{
	RE_GREAT,
	RE_DOUBLE_GREAT,
	RE_LESS,
} t_redirection_type;

typedef struct s_redirection
{
	int index;
	t_redirection_type type;
	int inside_quotes; // 1 if file_name is a env varaible
	char *file_name;
	struct s_redirection *next;
}t_redirection;

typedef struct s_args{
	int inside_quotes; // check inside quotes
	char *value; // argument 
	struct s_args *next;
}t_args;

typedef struct s_simple_cmd
{
	int inside_quotes; // check inside quotes
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

/***** ENV LIST ******/

typedef struct s_env
{
	char *name;
	char *value;
	struct s_env *next;
}				t_env;

typedef struct s_g_vars
{
	t_command_list *cmd;
	t_lines_list *history;
	int			infork;
}				t_g_vars;

t_g_vars	g_vars;



/* ************************************************************************** */
/*                                 LEXER                                      */
/* ************************************************************************** */

t_token	*ft_lexer(char *line);
void	ft_get_word(t_token *tokens_list, char *line, int *table);
void	get_space_pipe_semi_redir(t_token *tokens_list,
							   char *line, int *j, int *index);
void	ft_destoy_token_list(t_token *tokens_list);
void	print_tokens(t_token *tokens_list);
void	add_token(t_token *token_list, t_token_type type,
		char *content, int index);
char	*ft_get_words(char *line, int *j, char *word, int *quoting);



/* ************************************************************************** */
/*                                 PARSER                                     */
/* ************************************************************************** */

t_command_list *ft_parser(t_token *tokens_list,int *status);
void ft_destroy_ast(t_command_list *cmd_list);
int ft_check_syntax(t_token *tokens_list,int *status);
void ft_print_pipeline_cmd(t_pipe_line *pipe_line);
void ft_print_cmd_list(t_command_list *cmd_list);
void ft_print_simple_cmd(t_simple_cmd *cmd);
char *ft_int_to_string(int n);
void ft_destroy_simple(t_simple_cmd *cmd);


/* ************************************************************************** */
/*                                 EXPANSION                                  */
/* ************************************************************************** */

void ft_expanding(t_pipe_line *pipe_line,t_env **env,char **last_env);


/* ************************************************************************** */
/*                                 READLINE                                   */
/* ************************************************************************** */

int	get_next_line(char **line);
int	micro_read_line(char **line,int *status);
void	ft_delete_char_list(t_char_list *char_list);
t_lines_list	*ft_destory_node(t_lines_list *node);
t_lines_list	*ft_destroy_history(t_lines_list *lines_list);

#endif