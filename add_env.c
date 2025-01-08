/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:19 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/08 15:55:42 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

t_env	*ft_new_node(char *name, char *value)
{
	t_env	*new;

	if (!name)
		return (0);
	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->index = 0;
	new->name = ft_strdup(name);
	if (value && ft_strlen(value) > 0)
		new->value = ft_strdup(value);
	else if (value && ft_strlen(value) == 0)
		new->value = ft_strdup("");
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

static t_env	*last_node(t_env *lst)
{
	if (lst)
	{
		while (lst->next != NULL)
			lst = lst->next;
	}
	return (lst);
}

int	env_edit(t_env *lst, t_env *newnode)
{
	t_env	*tmp;

	newnode->index = max_index(lst) + 1;
	tmp = lst;
	while (tmp)
	{
		if (newnode && tmp->name && newnode->name && ft_strcmp(tmp->name,
				newnode->name) == 0)
		{
			if (!tmp->value || (newnode->value && ft_strcmp(tmp->value,
						newnode->value) != 0))
			{
				free(tmp->value);
				tmp->value = ft_strdup(newnode->value);
			}
			free(newnode->name);
			free(newnode->value);
			free(newnode);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	add_node(t_env *env, t_env *nw, int ret)
{
	int		i;
	t_env	*last;

	i = -1;
	while (nw && nw->name[++i] && !ret)
	{
		if ((i == 0 && ft_isdigit(nw->name[i])) || (!ft_isalnum(nw->name[i])
				&& nw->name[i] != '_'))
		{
			ft_fprintf(2, "minishell: export: `%s=%s': not a valid identifier\n",
				nw->name, nw->value);
			free(nw);
			return (1);
		}
	}
	i = env_edit(env, nw);
	if (nw && !i && env)
	{
		last = last_node(env);
		last->next = nw;
	}
	else if (nw && !i)
		env = nw;
	return (0);
}

int	max_index(t_env *env)
{
	int	max;

	max = 0;
	while (env)
	{
		if (env->index > max)
			max = env->index;
		env = env->next;
	}
	return (max);
}
