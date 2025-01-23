/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_OLD.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:31:24 by peli              #+#    #+#             */
/*   Updated: 2025/01/21 17:50:32 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

static char	*read_buffer(int fd, int *bytes_read)
{
	char	*buf;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	*bytes_read = read(fd, buf, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free(buf);
		return (NULL);
	}
	buf[*bytes_read] = '\0';
	return (buf);
}

static char	*concat_text(char *text, char *buf)
{
	char	*temp;

	if (text)
	{
		temp = ft_strdup(text);
		free(text);
		text = ft_strjoin(temp, buf);
	}
	else
	{
		text = ft_strdup(buf);
		free(buf);
	}
	return (text);
}

static int	ft_end(char *buf, char **text, char **nextstr)
{
	int	i;

	i = 0;
	while (buf[i] != '\0' && buf[i] != '\n')
		i++;
	if (buf[i] == '\0')
		*text = concat_text(*text, buf);
	else if (buf[i] == '\n')
	{
		*text = ft_strjoin(*text, ft_substr(buf, 0, ++i));
		if (i != ft_strlen(buf))
			*nextstr = ft_substr(buf, i, ft_strlen(buf));
		return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	char		*buf;
	char		*text;
	int			j;
	static char	*nextstr;

	text = NULL;
	while (1)
	{
		if (nextstr)
		{
			text = ft_nextstr(&nextstr);
			if (nextstr)
				return (text);
		}
		buf = read_buffer(fd, &j);
		if (!buf)
			return (NULL);
		if (j == 0)
			break ;
		if (!ft_end(buf, &text, &nextstr))
			break ;
	}
	free (buf);
	return (text);
}
