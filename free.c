/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:35:56 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/21 17:54:34 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

void	free_cmds(t_parser *cmds)
{
	t_parser	*tmp;
	int			i;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		if (tmp->cmd)
		{
			i = 0;
			while (tmp->cmd[i])
			{
				free(tmp->cmd[i]);
				i++;
			}
			free(tmp->cmd);
		}
		free_tokens(tmp->redirections);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	free_tokens(t_lexer *tokens)
{
	t_lexer	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_array(char	**str)
{
	int	i;

	i = 0;
	if (!str)
		return ;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return ;
}

void	free_exe(t_exe *exe)
{
	ft_free_tab(exe->env);
	free(exe->pid);
	free(exe);
}
