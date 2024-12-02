/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:40:18 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/26 15:25:48 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strcpy(char *dest, const char *src)
{
	while (*src)
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';
}

char	*ft_strdup(char *src)
{
	char	*str;

	if (src == NULL)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(src) + 1));
	ft_strcpy(str, src);
	return (str);
}
