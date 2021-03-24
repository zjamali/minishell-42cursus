/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 15:45:53 by mbari             #+#    #+#             */
/*   Updated: 2021/03/24 16:10:37 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H
#include "../src/libft/libft.h"

typedef struct s_env
{
	char *name;
	char *values;
	struct s_env *nex;
}				t_env;

#endif