/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:41:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/27 16:57:16 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_h
# define MINISHELL_h

# include <stdio.h>
# include <string.h>
# include "../src/libft/libft.h"
# include "get_next_line.h"
# include "lexer.h"
# include "parser.h"
#include "expansion.h"
#include "execution.h"

#define GREEN "\e[1;32m"
#define RESET "\e[0m"
#define RED "\e[1;91m"
#define CYAN "\e[1;36m"
#define YELLOW "\e[1;33m"
#define PURPLE "\e[1;35m"
#define BLUE "\e[1;34m"

void ft_print_cmd_list(t_command_list *cmd_list);
void ft_print_simple_cmd(t_simple_cmd *cmd);
#endif