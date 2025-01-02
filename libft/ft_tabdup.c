/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peiqi <peiqi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:21:04 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/24 00:19:52 by peiqi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	**ft_tabdup(char **tab)
// {
// 	char	**dup;
// 	int		i;
// 	int		len;

// 	if (!tab)
// 		return (NULL);
// 	i = -1;
// 	while (tab && tab[++i] != NULL)
// 		;
// 	dup = ft_calloc(sizeof(char *), (i + 1));
// 	i = -1;
// 	while (tab && tab[++i] != NULL)
// 	{
// 		len = ft_strlen(tab[i]);
// 		dup[i] = ft_strdup(tab[i]);
// 	}
// 	return (dup);
// }
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
		if (!dup[i]) // Vérification pour éviter des fuites en cas d'échec
		{
			while (i > 0)
				free(dup[--i]);
			free(dup);
			return (NULL);
		}
	}
	return (dup);
}
