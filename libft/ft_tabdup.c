/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:21:04 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/21 17:49:09 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tabdup(char **tab)
{
	char	**dup;
	int		i;

	if (!tab)
		return (NULL);
	i = -1;
	while (tab && tab[++i] != NULL)
		;
	dup = ft_calloc(sizeof(char *), (i + 1));
	if (!dup)
		return (NULL);
	i = -1;
	while (tab && tab[++i] != NULL)
	{
		dup[i] = ft_strdup(tab[i]);
		if (!dup[i])
		{
			while (i > 0)
				free(dup[--i]);
			free(dup);
			return (NULL);
		}
	}
	return (dup);
}
