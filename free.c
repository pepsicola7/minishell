/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:35:56 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:00:13 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

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
		free(tmp->hd_file_name);
		free_tokens(tmp->redirections);
		free(tmp);
	}
}
