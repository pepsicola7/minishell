/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_OLD_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:31:24 by peli              #+#    #+#             */
/*   Updated: 2025/01/20 18:37:00 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nextelse(int a, char **nextstr_ptr)
{
	char	*t1;
	char	*nextstr;

	t1 = NULL;
	nextstr = *nextstr_ptr;
	if (a != ft_strlen(nextstr))
	{
		t1 = ft_strdup(nextstr);
		free(nextstr);
		*nextstr_ptr = ft_substr(t1, a, ft_strlen(t1) - a);
		free(t1);
		return (0);
	}
	return (1);
}

char	*ft_nextstr(char **nextstr_ptr)
{
	int		a;
	char	*text;
	char	*nextstr;

	a = 0;
	text = NULL;
	nextstr = *nextstr_ptr;
	while ((*nextstr_ptr)[a] != '\n' && (*nextstr_ptr)[a] != '\0')
		a++;
	if (a == ft_strlen(nextstr))
		text = ft_strdup(nextstr);
	else
	{
		text = ft_substr(nextstr, 0, ++a);
		if (nextelse(a, nextstr_ptr) == 0)
			return (text);
	}
	free (nextstr);
	*nextstr_ptr = NULL;
	return (text);
}
