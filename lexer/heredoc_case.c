/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_case.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 14:26:13 by peli              #+#    #+#             */
/*   Updated: 2025/01/02 15:43:39 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

static char	*compose_heredoc_name(int hd_index);
static void	write_to_heredoc(int fd, char *delimiter);

char	*create_heredoc(char *delimiter)
{
	static int	hd_index;
	char		*name;
	int			fd;

	hd_index = (hd_index + 1) % INT_MAX;
	name = compose_heredoc_name(hd_index);
	if (!name)
		return (NULL);
	if (access(name, F_OK) == 0)
		unlink(name);
	fd = open(name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		free(name);
		return (NULL);
	}
	write_to_heredoc(fd, delimiter);
	close(fd);
	return (name);
}

static char	*compose_heredoc_name(int hd_index)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_itoa(hd_index);
	tmp2 = ft_strjoin(".heredoc_", tmp1);
	free(tmp1);
	tmp1 = ft_strjoin(tmp2, ".tmp");
	free(tmp2);
	return (tmp1);
}

static void	write_to_heredoc(int fd, char *delimiter)
{
	// Consider replacing GNL with readline here
	char	*line;

	ft_putstr_fd("> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		line[ft_strlen(line) - 1] = 0;
		if (!ft_strcmp(line, delimiter))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	if (!line)
	{
		ft_putstr_fd("\nminishell: warning: here-document ", STDERR_FILENO);
		ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
		ft_putstr_fd(delimiter, STDERR_FILENO);
		ft_putstr_fd("')\n", STDERR_FILENO);
	}
}