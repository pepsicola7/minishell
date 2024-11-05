/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:29:14 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_cd(t_env *env, t_parser *parser)
{
	(void)env;
	if (!parser->cmd[1])
	{
		printf("cd: missing argument\n");
		return (1);
	}
	if (chdir(parser->cmd[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
