/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/06 19:05:02 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

static void	check_exit_code(struct s_parser *parser)
{
	int	i;

	i = 0;
	while (parser->cmd[1] && parser->cmd[1][i])
	{
		if (i == 0 && !ft_isdigit(parser->cmd[1][i])
			&& parser->cmd[1][i] != '-' && parser->cmd[1][i] != '+')
		{
			printf("minishell: exit: %s: numeric argument required\n"
				, parser->cmd[1]);
			exit(2);
		}
		else if (i != 0 && !ft_isdigit(parser->cmd[1][i]))
		{
			printf("minishell: exit: %s: numeric argument required\n"
				, parser->cmd[1]);
			exit(2);
		}
		i++;
	}
}

int	my_exit(t_env *env, struct s_parser *parser)
{
	int		exit_code;
	char	*str;

	
	printf("exit\n");
	check_exit_code(parser);
	printf("%d %d %d %d %d\n", g_signum, g_signum, g_signum, g_signum, g_signum);
	if (parser->cmd[1] && !parser->cmd[2])
	{
		printf("minishell:qwdqwdqwd exit: too many arguments\n");
		g_signum = ft_atoi(parser->cmd[1]) % 256;
		if (g_signum < 0)
			g_signum = 256 + g_signum;
	}
	else if (parser->cmd[1] && parser->cmd[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else
		exit_code = 1;
	str = ft_itoa(exit_code);
	add_node(env, ft_new_node("?", str), 1);
	free(str);
	print_env(env);
	exit(exit_code);
	return (exit_code);
}
