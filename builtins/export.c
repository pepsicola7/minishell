/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:11:58 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/21 17:56:05 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	my_export(t_env **env, t_parser *parser)
{
	int		i;
	int		len_equal;
	char	*n;
	char	*v;

	if (!parser->cmd || !parser->cmd[1])
		return (print_export(*env), add_node(env, ft_new_node("?", "0"), 1), 1);
	i = -1;
	len_equal = ft_strlen_c(parser->cmd[1], '=');
	if (len_equal == 0)
		return (ft_fprintf(2, "minishell: export: `=': not a valid"
				" identifier\n"), add_node(env, ft_new_node("?", "1"), 1), 1);
	n = ft_strndup(parser->cmd[1], len_equal);
	v = ft_strdup(parser->cmd[1] + len_equal + 1);
	while (parser->cmd[1][++i] != '=' && parser->cmd[1][i] != '\0')
		if (ft_isalnum(parser->cmd[1][i]) == 1 && parser->cmd[1][i] == '_')
			return (add_node(env, ft_new_node("?", "1"), 1),
				free(n), free(v), 1);
	if (add_node(env, ft_new_node(n, v), 0) == 0)
		return (add_node(env, ft_new_node("?", "0"), 1), free(n), free(v), 0);
	else
		return (add_node(env, ft_new_node("?", "1"), 1), free(n), free(v), 1);
}

char	*get_value(char *name, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	if (!name)
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(tmp->name, name) == 0)
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
