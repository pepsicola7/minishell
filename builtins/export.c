/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2024/12/20 15:48:26 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_export(t_env *env, t_parser *parser)
{
	int		i;
	int		len_equal;
	char	*name;
	char	*value;

	if (!parser->cmd || !parser->cmd[1])
		return (print_export(env), add_node(env, ft_new_node("?", "0"), 1), 1);
	i = -1;
	len_equal = ft_strlen_c(parser->cmd[1], '=');
	name = ft_strndup(parser->cmd[1], len_equal);
	value = ft_strdup(parser->cmd[1] + len_equal + 1);
	while (parser->cmd[1][++i] != '=' && parser->cmd[1][i] != '\0')
		if (ft_isalnum(parser->cmd[1][i]) == 1 && parser->cmd[1][i] == '_')
			return (add_node(env, ft_new_node("?", "1"), 1), 1);
	add_node(env, ft_new_node(name, value), 0);
	return (add_node(env, ft_new_node("?", "0"), 1), 0);
}

char	*get_value(char *name, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	if (!name)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) != 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static void	index_export(t_env *env)
{
	t_env	*first_node;
	t_env	*to_index;
	int		index;

	first_node = env;
	to_index = env;
	while (to_index)
	{
		index = 0;
		while (env)
		{
			if (to_index != env && ft_strcmp(env->name, to_index->name) < 0)
				++index;
			env = env->next;
		}
		to_index->index = index;
		to_index = to_index->next;
		env = first_node;
	}
}

void	print_export(t_env *env)
{
	t_env	*first_node;
	int		max;
	int		i;

	i = 0;
	first_node = env;
	index_export(env);
	max = max_index(env);
	while (i <= max)
	{
		if (env->index == i)
		{
			if (ft_strcmp(env->name, "?") != 0)
			{
				if (printf("declare -x %s", env->name) && env->value)
					printf("=\"%s\"", env->value);
				printf("\n");
			}
			i++;
		}
		if (env->next == NULL)
			env = first_node;
		else
			env = env->next;
	}
}
