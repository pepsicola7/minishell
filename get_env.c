/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:13:11 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/06 18:13:21 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

t_env	*create_env_node(char *name, char *value, int index)
{
	t_env	*node;

	node = ft_calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	node->index = index;
	node->name = ft_strdup(name);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*tmp;

	if (!*env)
		*env = new_node;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	split_env_var(char *env_var, char **name, char **value)
{
	int	i;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	*name = ft_strndup(env_var, i);
	if (env_var[i] == '=')
		*value = ft_strdup(env_var + i + 1);
	else
		*value = ft_strdup("");
}

t_env	*get_env(char **initial_env)
{
	t_env	*env;
	t_env	*new_node;
	char	*name;
	char	*value;
	int		i;

	env = NULL;
	i = -1;
	while (initial_env[++i])
	{
		name = NULL;
		value = NULL;
		split_env_var(initial_env[i], &name, &value);
		new_node = create_env_node(name, value, i);
		if (!new_node)
		{
			free(name);
			free(value);
			return (NULL);
		}
		add_env_node(&env, new_node);
		free(name);
		free(value);
	}
	return (env);
}
