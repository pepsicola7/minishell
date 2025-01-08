/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trans_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:25:34 by peli              #+#    #+#             */
/*   Updated: 2025/01/08 17:33:17 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char **allocate_env_array(t_env *env_lst, int *count)
{
	t_env *tmp;
	char **res;

	*count = 0;
	tmp = env_lst;
	while (tmp)
	{
		(*count)++;
		tmp = tmp->next;
	}

	res = ft_calloc((*count + 1), sizeof(char *));
	if (!res)
		return (NULL);

	return (res);
}

char *create_env_entry(t_env *env_node)
{
	char *entry;

	if (!env_node || !env_node->name || !env_node->value)
		return (NULL);

	entry = ft_calloc(sizeof(char), sizeof(char)
		* (ft_strlen(env_node->name) + ft_strlen(env_node->value) + 2));
	if (!entry)
		return (NULL);

	ft_strcat(entry, env_node->name);
	ft_strcat(entry, "=");
	ft_strcat(entry, env_node->value);

	return (entry);
}

int fill_env_array(char **res, t_env *env_lst, int count)
{
	int i = 0;

	while (i < count)
	{
		if (ft_strcmp(env_lst->name, "?") != 0)
		{
			res[i] = create_env_entry(env_lst);
			if (!res[i])
			{
				perror("Erreur d'allocation mémoire");
				while (--i >= 0)
					free(res[i]);
				free(res);
				return (-1);
			}
		}
		env_lst = env_lst->next;
		i++;
	}
	return (0);
}

char **trans_env(t_env *env_lst)
{
	char **res;
	int count;

	res = allocate_env_array(env_lst, &count);
	if (!res)
		return (NULL);

	if (fill_env_array(res, env_lst, count) == -1)
		return (NULL);

	return (res);
}

// char	**trans_env(t_env	*env_lst)
// {
// 	char **res;
// 	t_env *tmp;
// 	int count;
// 	int i;

// 	count = 0;
// 	i = 0;
// 	tmp = env_lst;
// 	while (tmp)
// 	{
// 		count++;
// 		tmp = tmp->next;
// 	}
// 	res = ft_calloc( (count + 1), sizeof(char *));
// 	if (!res)
// 		return (NULL);
// 	while (i < count)
// 	{
// 		if (!env_lst || !env_lst->name || !env_lst->value)
// 		{
// 			free (res);
// 			ft_fprintf(2, "env_lst ou ses champs sont invalides\n");
// 			return (NULL);
// 		}
// 		if (ft_strcmp(env_lst->name, "?") != 0)
// 		{
// 			res[i] = ft_calloc(sizeof(char), sizeof(char)
// 				* (ft_strlen(env_lst->name) + ft_strlen(env_lst->value) + 2));
// 			if (!res[i])
// 			{
// 				perror("Erreur d'allocation mémoire");
// 				while (--i >= 0)
// 					free(res[i]);
// 				free(res);
// 				return (NULL);
// 			}
// 			ft_strcat(res[i], env_lst->name);
// 			ft_strcat(res[i], "=");
// 			ft_strcat(res[i], env_lst->value);
// 		}
// 		env_lst = env_lst->next;
// 		i++;
// 	}
// 	return (res);
// }
