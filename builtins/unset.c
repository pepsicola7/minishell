/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/21 17:56:20 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

static int	remove_elem(t_env **env, t_env **last, t_env **tmp)
{
	if (*last == NULL)
		*env = (*tmp)->next;
	else
		(*last)->next = (*tmp)->next;
	free((*tmp)->name);
	free((*tmp)->value);
	free(*tmp);
	*tmp = *env;
	if (*last == NULL)
		return (1);
	return (0);
}

int	my_unset(t_env **env, t_parser *parser)
{
	t_env	*last;
	t_env	*tmp;
	int		i;

	tmp = *env;
	last = NULL;
	i = 1;
	while (tmp && parser->cmd[i])
	{
		if (ft_strcmp(parser->cmd[i], tmp->name) == 0
			&& ft_strcmp(parser->cmd[i], "?") != 0)
		{
			if (remove_elem(env, &last, &tmp))
				continue ;
			if (parser->cmd[i++ + 1] == NULL)
			{
				add_node(env, ft_new_node("?", "0"), 1);
				return (0);
			}
		}
		last = tmp;
		tmp = tmp->next;
	}
	add_node(env, ft_new_node("?", "0"), 1);
	return (0);
}
