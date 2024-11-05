/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:28:26 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_echo(t_env *env, t_parser *parser)
{
	int	i;
	int	no_newline;

	(void)env;
	i = 1;
	no_newline = 0;
	if (parser->cmd[1] && ft_strcmp(parser->cmd[1], "-n") == 0)
	{
		no_newline = 1;
		i++;
	}
	while (parser->cmd[i])
	{
		printf("%s", parser->cmd[i]);
		if (parser->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
