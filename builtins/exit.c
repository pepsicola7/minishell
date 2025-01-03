/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/12/23 16:40:51 by tbartocc         ###   ########.fr       */
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
	// (void)env;
	exit_code = 0;
	check_exit_code(parser);
	if (parser->cmd[1] && !parser->cmd[2])
	{
		exit_code = ft_atoi(parser->cmd[1]) % 256;
		if (exit_code < 0)
			exit_code = 256 + exit_code;
	}
	else if (parser->cmd[1] && parser->cmd[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	else
		exit_code = 1;
	str = ft_itoa(exit_code);
	printf("code : %d\n", exit_code);
	add_node(env, ft_new_node("?", str), 1);
	free(str);
	print_env(env);
	exit(exit_code);
	printf("TEST\n");
	return (exit_code);
}
