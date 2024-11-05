/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:32:20 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:37:49 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

int	is_builtin(char *cmd)
{
	return (!strcmp(cmd, "cd")
		|| !strcmp(cmd, "echo")
		|| !strcmp(cmd, "env")
		|| !strcmp(cmd, "exit")
		|| !strcmp(cmd, "export")
		|| !strcmp(cmd, "pwd")
		|| !strcmp(cmd, "unset"));
}

int (*get_builtin_function(char *cmd))(t_env *, t_parser *)
{
	if (!ft_strcmp(cmd, "echo"))
		return (&my_echo);
	else if (!ft_strcmp(cmd, "cd"))
		return (&my_cd);
	else if (!ft_strcmp(cmd, "pwd"))
		return (&my_pwd);
	else if (!ft_strcmp(cmd, "export"))
		return (&my_export);
	else if (!ft_strcmp(cmd, "unset"))
		return (&my_unset);
	else if (!ft_strcmp(cmd, "env"))
		return (&my_env);
	else if (!ft_strcmp(cmd, "exit"))
		return (&my_exit);
	return (NULL);
}
