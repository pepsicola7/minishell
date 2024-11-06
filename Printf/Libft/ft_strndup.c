/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:19:02 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/06 16:07:12 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strncpy(char *dest, const char *src, int n)
{
	int	i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
}

char	*ft_strndup(const char *src, int n)
{
	char	*str;
	int		len;

	if (src == NULL)
		return (NULL);
	len = ft_strlen(src);
	if (n > len)
		n = len;
	str = malloc(sizeof(char) * (n + 1));
	if (!str)
		return (NULL);
	ft_strncpy(str, src, n);
	str[n] = '\0';
	return (str);
}
