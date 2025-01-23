/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/21 17:55:06 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

static void	get_target_dir(t_env **env, t_parser *parser, char **dir)
{
	if (parser->cmd[1] == NULL || ft_strncmp(parser->cmd[1], "~", 1) == 0)
	{
		*dir = get_value("HOME", *env);
		if (!*dir)
		{
			add_node(env, ft_new_node("?", "1"), 1);
			ft_fprintf(2, "minishell: cd: HOME not set\n");
		}
		else if (parser->cmd[1] && parser->cmd[1][1])
			*dir = ft_strjoin(*dir, parser->cmd[1] + 1);
	}
	else if (ft_strcmp(parser->cmd[1], "-") == 0)
	{
		*dir = get_value("OLDPWD", *env);
		if (!*dir)
		{
			add_node(env, ft_new_node("?", "1"), 1);
			ft_fprintf(2, "minishell: cd: OLDPWD not set\n");
		}
		else
			printf("%s\n", *dir);
	}
	else
		*dir = parser->cmd[1];
}

static int	check_cd_error(t_env **env, t_parser *parser)
{
	if (parser->cmd[1] && parser->cmd[2])
	{
		ft_fprintf(2, "minishell: cd: too many arguments\n");
		add_node(env, ft_new_node("?", "1"), 1);
		return (1);
	}
	return (0);
}

int	my_cd(t_env **env, t_parser *parser)
{
	char	*temp;

	temp = NULL;
	if (check_cd_error(env, parser))
		return (1);
	get_target_dir(env, parser, &temp);
	if (!temp)
		return (1);
	if (chdir(temp) != 0)
	{
		ft_fprintf(2, "minishell: cd: %s: %s\n", temp, strerror(errno));
		add_node(env, ft_new_node("?", "1"), 1);
		return (1);
	}
	add_node(env, ft_new_node("OLDPWD", get_value("PWD", *env)), 0);
	temp = getcwd(NULL, 0);
	add_node(env, ft_new_node("PWD", temp), 0);
	free(temp);
	add_node(env, ft_new_node("?", "0"), 1);
	return (0);
}
