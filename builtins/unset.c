/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:31:04 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_unset(t_env *env, t_parser *parser)
{
	t_env	*prev;
	t_env	*current;

	if (!parser->cmd[1])
		return (printf("unset: missing argument\n"), 1);
	current = env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, parser->cmd[1]) == 0)
		{
			if (prev)
				prev->next = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
