/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:41:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 21:11:54 by zjamali          ###   ########.fr       */
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
# include <termios.h>
# include <term.h>
# include <curses.h>
# include <termcap.h>
# include <sys/ioctl.h>
# include "../src/libft/libft.h"

# define GREEN "\e[1;32m"
# define RESET "\e[0m"
# define RED "\e[1;91m"
# define CYAN "\e[1;36m"
# define YELLOW "\e[1;33m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"
# define YES 1
# define NO 0

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
	char				value;
	int					len;
	struct s_char_list	*next;
}				t_char_list;

typedef struct s_lines_list{
	char				value;
	int					history;
	int					index;
	bool				up_or_down;
	t_char_list			*char_list;
	t_char_list			*origin_char_list;
	struct s_lines_list	*next;
	struct s_lines_list	*prev;
}				t_lines_list;
typedef struct s_cursor{
	int	line_postion;
	int	col_position;
}				t_cursor;
typedef struct s_readline
{
	char		*term_type;
	int			term_fd;
	int			line_count;
	int			colums_count;
	long		c;
	char		*path;
	char		*line;
	t_cursor	cursor;
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
}			t_token_type;

typedef struct s_token
{
	int				index;
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}				t_token;

/******* PARSER **********/
typedef enum e_redirection_type{
	RE_GREAT,
	RE_DOUBLE_GREAT,
	RE_LESS,
}			t_redirection_type;

typedef struct s_redirection
{
	int						index;
	t_redirection_type		type;
	int						inside_quotes;
	char					*file_name;
	struct s_redirection	*next;
}				t_redirection;

typedef struct s_args{
	int				inside_quotes;
	char			*value;
	struct s_args	*next;
}				t_args;

typedef struct s_simple_cmd
{
	int					inside_quotes;
	char				*command;
	t_args				*args;
	t_redirection		*redirections;
	struct s_simple_cmd	*next;
}				t_simple_cmd;

typedef struct s_pipe_line
{
	int					simple_cmd_count;
	struct s_pipe_line	*next;
	t_simple_cmd		*child;
}				t_pipe_line;

typedef struct s_command_list
{
	int			pipe_line_count;
	t_pipe_line	*childs;
}				t_command_list;

/***** ENV LIST ******/

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_g_vars
{
	t_command_list	*cmd;
	t_lines_list	*history;
	int				infork;
}				t_g_vars;

t_g_vars	g_vars;

typedef struct s_expansion
{
	char	*word;
	char	*expanded;
	char	*tmp1;
	char	*tmp;
}				t_expansion;

/* ************************************************************************** */
/*                                 LEXER                                      */
/* ************************************************************************** */

t_token			*ft_lexer(char *line);
void			ft_get_word(t_token *tokens_list, char *line, int *table);
void			get_space_pipe_semi_redir(t_token *tokens_list,
					char *line, int *j, int *index);
void			ft_destoy_token_list(t_token *tokens_list);
void			print_tokens(t_token *tokens_list);
void			add_token(t_token *token_list, t_token_type type,
					char *content, int index);
char			*ft_get_words(char *line, int *j, char *word, int *quoting);

/* ************************************************************************** */
/*                                 PARSER                                     */
/* ************************************************************************** */

t_command_list	*ft_parser(t_token *tokens_list, int *status);
void			ft_destroy_ast(t_command_list *cmd_list);
int				ft_check_syntax(t_token *tokens_list, int *status);
void			ft_print_pipeline_cmd(t_pipe_line *pipe_line);
void			ft_print_cmd_list(t_command_list *cmd_list);
void			ft_print_simple_cmd(t_simple_cmd *cmd);
char			*ft_int_to_string(int n);
void			ft_destroy_simple(t_simple_cmd *cmd);
t_command_list	*ft_create_ast(t_token *tokens_list);
t_command_list	*init_cmd_list(void);
t_simple_cmd	*ft_create_simple_cmd(t_token **tokens);
void			ft_insert_simple_cmd(t_simple_cmd *head,
					t_simple_cmd *current_cmd);
t_redirection	*ft_insert_redirection(t_redirection *redirection,
					t_token **tokens, int index);
t_args			*ft_create_arg(char *value);
void			ft_insert_arg(t_args *head, t_args *current_args);
int				check_tokn_next_semi(t_token *tokens_list, t_token *token,
					int *status);
int				check_word_token(t_token *tokens_list, t_token *token,
					int *status);
int				check_last_word_token(t_token *tokens_list, t_token *token,
					int *status);
int				check_first_token(t_token *tokens_list, t_token *first_token,
					int *status);
int				check_redirection(t_token *tokens_list, t_token *token,
					int *status);
int				check_tokn_nxt_pipe(t_token *tokens_list, t_token *token,
					int *status);
void			ft_print_systax_error(t_token *token);
int				ft_check_backslash(char *word);
int				ft_check_backslash(char *word);
int				ft_check_closing_quotes(char *word);
int				count_bachslashes(char *word, int *index, int back_slash);
int				ft_get_first_double_quotes(char *word, int *index,
					int *back_slash);
int				ft_get_first_single_quotes(char *word, int *index,
					int *back_slash);
void			ft_get_close_double_quotes(char *word, int *index,
					int *back_slash, int *quote);
void			ft_get_close_single_quotes(int *index, int *back_slash,
					int *quote);

/* ************************************************************************** */
/*                                 EXPANSION                                  */
/* ************************************************************************** */

void			ft_expanding(t_pipe_line *pipe_line, t_env **env,
					char **last_env);
void			ft_repalce_space_by_tab(t_simple_cmd **cmd, int args_or_cmd);
void			ft_replace_tilde(char **string);
char			*get_env_variable_value(char *env_variable, t_env **env);
char			*ft_int_to_string(int n);

void			ft_expande_word(char **string, t_env **env_list,
					char **last_env, int redirection);
void			ft_handle_empty_command(t_simple_cmd **cmd);
void			ft_expand_command(t_simple_cmd **cmd, t_env **env,
					char **last_env);
void			ft_expande_redirection(t_simple_cmd **cmd, t_env **env,
					char **last_env);
void			check_word_expand_redtion(int redirection, char **expanded,
					char **word, char **string);
int				check_end_of_string(char str, int inside_quotes);
void			ft_delete_emty_args_nodes(t_args **args);
void			ft_return_spaces(t_simple_cmd **cmd);
void			ft_assign_value_to_arg(char	**splited, int i,
					t_args **new_args);
void			ft_expand_arguments(t_simple_cmd **cmd, t_env **env,
					char **last_env);
int				check_exiting_of_qoutes(char *str);
void			ft_expand_squence_of_dollar_sign(char *str, int *i,
					char **expanded);

void			ft_replace_env_by_value(char *str, int *i, char **expanded,
					char **env_value);
void			ft_expande_special_params(char *str, int *i, char **expanded,
					char *exit_status);
void			ft_skip_characters_non_env_variable(char *str, int *i,
					char **expanded, int inside_quotes);

void			ft_expand_other_special_params(char *str, int *i,
					char **expanded);

void			ft_expande_special_param_in_dble_g(char *str, int *i,
					char **expanded, char *exit_status);
void			ft_get_charachter(char *str, int *i, char **expanded);
void			ft_expand_exit_status(int *i, char **expanded,
					char *exit_status);
void			ft_skip_characters_env_not_exist(char *str, int *i);
void			get_dollar_sign(char *str, int *i, char **expanded);
void			ft_digit_env(char *str, int *i, char **expanded);
void			ft_expand_back_slash(int *i, char **expanded);
void			ft_expand_backslashes(char *str, int *i, char **expanded);
char			*ft_expand_double_quotes(t_expansion expd, int *i,
					t_env **env_list, char **last_env);
void			ft_remove_single_quotes(char *str, int *i, char **expanded);
void			ft_expand_env_variable_in_dble_qte(t_expansion *expd, int *j,
					t_env **env_list, char **last_env);
void			ft_expand_env_variable(t_expansion *expd, int *i,
					t_env **env_list, char **last_env);

/* ************************************************************************** */
/*                                 READLINE                                   */
/* ************************************************************************** */

int				get_next_line(char **line);
int				micro_read_line(char **line, int *status);
void			ft_delete_char_list(t_char_list *char_list);
t_lines_list	*ft_destory_node(t_lines_list *node);
t_lines_list	*ft_destroy_history(t_lines_list *lines_list);
void			ft_print_char_list(t_char_list *chars_list);

char			*get_last_argument_or_command(t_pipe_line *current_pipe_line);
t_simple_cmd	*ft_delete_emty_simple_cmds(t_pipe_line **pipe_line);
t_lines_list	*push_curr_node_to_list(t_lines_list **list,
					t_lines_list *current, int history);
char			*ft_return_line(t_readline readline);
t_lines_list	*ft_down_in_history(t_readline *readline,
					t_lines_list **history);
t_lines_list	*ft_up_in_history(t_readline *readline, t_lines_list **history);
t_lines_list	*ft_create_line_node(void);
char			*create_line_from_chars_list(t_char_list *char_list);
t_lines_list	*ft_delete_node_from_list(t_lines_list *current);
t_char_list		*ft_copy_char_list(t_char_list *char_list);
t_lines_list	*ft_handle_history_buttons(long character, t_readline readline,
					t_lines_list *current);
t_lines_list	*ft_init_history(long *character, int *newline_break);
void			ft_move_cursor_and_clear(t_cursor cursor);
void			ft_print_char_list(t_char_list *chars_list);
char			*ft_get_input(t_readline readline, int *status,
					struct termios old_term);
int				add_curr_node_to_history(t_readline *readline,
					t_lines_list *current, t_lines_list **history);
void			ft_init_readline(t_readline *readline);
void			set_terminal(struct termios *termios, struct termios *old,
					int fd);
void			reset_terminal(struct termios old_termios, int fd);
int				ft_copy_char_value(int len, t_char_list **origin,
					t_char_list **tmp);
void			ft_get_cursor_position(int *x, int *y);
int				get_char_list_lenght(t_char_list *char_list);
void			ft_add_to_char_list(t_readline *readline, char c,
					t_char_list **chars_list);
void			show_prompt(void);
void			ft_minishell(char **env);

#endif