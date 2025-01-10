/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:50 by peli              #+#    #+#             */
/*   Updated: 2025/01/10 16:35:41 by peli             ###   ########.fr       */
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

char	*check_path(t_exe *exe, t_parser *cmds)
{
	struct stat	path_stat;
	char		*res;

	res = find_path(exe->pathname, cmds->cmd[0]);
	if (stat(res, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		if (ft_strchr(cmds->cmd[0], '/') == NULL)
		{
			ft_fprintf(2, "%s: command not found\n", cmds->cmd[0]);
			exit(127);
		}
		ft_fprintf(2, "minishell: Is a directory\n");
		exit(126);
	}
	return (res);
}

static int is_valid_path(char *path, char* cmd)
{
	if (cmd && (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, "./", 2) == 0
		|| ft_strncmp(cmd, "../", 3) == 0))
	{
		if (access(cmd, F_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd,
				strerror(errno));
			return (127);
		}
		if (access(cmd, X_OK) != 0)
		{
			ft_fprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd,
				strerror(errno));
			return (126);
		}
		return (-1);
	}
	else
		if (access(path, F_OK) == 0)
			return (-1);
	return (0);
}
char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;
	int		res;

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
		res = is_valid_path(path, cmd);
		if (res != 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	i = 0;
	while (sp_path[i])
		free(sp_path[i++]);
	free(sp_path);
	if (res > 0)
		exit(res);
	return (path);
}

