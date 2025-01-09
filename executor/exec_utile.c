/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:50 by peli              #+#    #+#             */
/*   Updated: 2025/01/09 18:54:43 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char	*ft_strcpy_(char *dest, const char *src)
{
	int	i;

	i = 0;
	if (!dest || !src)
		return (NULL);
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*join_path_and_cmd(const char *dir, const char *cmd)
{
	char	*path_with_slash;
	char	*full_path;

	path_with_slash = ft_strjoin(dir, "/");
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	return (full_path);
}

char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	if (pathname == NULL)
		return (NULL);
	sp_path = ft_split(pathname, ':');
	if (!sp_path)
		return (NULL);
	path = NULL;
	i = 0;
	while (sp_path[i])
	{
		path = join_path_and_cmd(sp_path[i], cmd);
		if (path && access(path, X_OK) == 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	i = 0;
	while (sp_path[i])
		free(sp_path[i++]);
	free(sp_path);
	return (path);
}
