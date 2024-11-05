/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/04 14:30:14 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_export(t_env *env, t_parser *parser)
{
	t_env	*tmp;

	(void)parser;
	tmp = env;
	while (tmp)
	{
		printf("declare -x %s=%s\n", tmp->name, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
