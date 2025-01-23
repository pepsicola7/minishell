/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utile.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:34:50 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 10:32:04 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

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

static int	is_valid_path(char *path, char *cmd)
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

char	*clean_up_and_exit(char **sp_path, int res, char *path)
{
	int	i;

	i = 0;
	while (sp_path[i])
		free(sp_path[i++]);
	free(sp_path);
	if (res > 0)
		exit(res);
	return (path);
}

char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;
	int		res;

	i = 0;
	res = 0;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	sp_path = ft_split(pathname, ':');
	if (!pathname || !sp_path)
		return (NULL);
	while (sp_path[i])
	{
		path = join_path_and_cmd(sp_path[i], cmd);
		res = is_valid_path(path, cmd);
		if (res != 0)
			return (clean_up_and_exit(sp_path, res, path));
		free(path);
		i++;
	}
	return (clean_up_and_exit(sp_path, 0, NULL));
}
