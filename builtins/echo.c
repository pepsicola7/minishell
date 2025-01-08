/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/08 18:56:27 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	pass_args(t_parser *parser, int *i)
{
	int	j;

	while (parser->cmd[*i])
	{
		if (parser->cmd[*i][0] != '-' || parser->cmd[*i][1] != 'n')
			break ;
		j = 2;
		while (parser->cmd[*i][j] == 'n')
			j++;
		if (parser->cmd[*i][j] != '\0' && parser->cmd[*i][j] != 'n')
			break ;
		(*i)++;
	}
	if ((*i) == 1)
		return (1);
	return (0);
}

int	my_echo(t_env **env, t_parser *parser)
{
	int	i;
	int	n;

	(void)env;
	i = 1;
	n = pass_args(parser, &i);
	while (parser->cmd[i])
	{
		printf("%s", parser->cmd[i]);
		if (parser->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (n)
		printf("\n");
	return (0);
}
