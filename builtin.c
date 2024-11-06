/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:32:20 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/06 18:01:04 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "unset"));
}

int	(*get_builtin_function(char *cmd))(t_env *env, t_parser *parser)
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
