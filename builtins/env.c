/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/03 19:12:53 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_env(t_env *env, t_parser *parser)
{
	(void)parser;
	while (env)
	{
		if (ft_strcmp(env->name, "?"))
			printf("%s=%s\n", env->name, env->value);
		env = env->next;
	}
	return (0);
}
