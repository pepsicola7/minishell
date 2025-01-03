/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_OLD.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 21:31:24 by peli              #+#    #+#             */
/*   Updated: 2025/01/03 16:33:55 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

// char	*ft_strdup_(char *src)
// {
// 	char	*dest;
// 	int		i;

// 	i = 0;
// 	while (src[i])
// 		i++;
// 	dest = (char *)malloc(sizeof(char) * (i + 1));
// 	if (!dest)
// 		return (NULL);
// 	i = 0;
// 	while (src[i])
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

// char	*get_next_line(int fd)
// {
// 	static char	buffer[BUFFER_SIZE];
// 	static int	buffer_read = 0;
// 	static int	buffer_pos = 0;
// 	char		*line;
// 	int			i;
	
// 	line = malloc(70000);
// 	if (fd < 0 || BUFFER_SIZE <= 0 || !(line))
// 		return (NULL);
// 	i = 0;
// 	while (1)
// 	{
// 		if (buffer_pos >= buffer_read)
// 		{
// 			buffer_read = read(fd, buffer, BUFFER_SIZE);
// 			buffer_pos = 0;
// 			if (buffer_read <= 0)
// 			{
// 				if (i == 0) // Rien n'a été lu
// 				{
// 					free(line);
// 					return (NULL);
// 				}
// 				break;
// 			}
// 		}
// 		line[i++] = buffer[buffer_pos++];
// 		if (line[i - 1] == '\n') // Check après l'ajout
// 			break;
// 	}
// 	line[i] = '\0';
// 	return (ft_strdup_(line));
// }

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

static char	*ft_nextstr(char **nextstr_ptr)
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
