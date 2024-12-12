/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_utile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2024/12/12 17:14:52 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;

	i = 0;
	sp_path = ft_split(pathname, ':');
	while (sp_path[i])
	{
		path = ft_strjoin(sp_path[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, X_OK) == 0)
			break;
		free(path);
		i++;
	}
	// free_array(sp_path);
	return (path);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	char	*exc_pathname;

	if (exe->fd[1] != STDOUT_FILENO) // passer les redirs;
	{
		dup2(exe->fd[1], STDOUT_FILENO);
		close(exe->fd[1]);
	}
	if (cmds->cmd)
	{
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]); // check cmd[0] faut parcourir dans la commande?
		if (!exc_pathname)
		{
			perror("command not found\n");
			free(exc_pathname);
			exit(EXIT_FAILURE);
		}
		// printf("Avant executer fd[0] is : %d\n", exe->fd[0]);
		// printf("Avant executer fd[1] is : %d\n", exe->fd[1]);
		// printf("Avant executer STDIN is : %d\n", STDIN_FILENO);
		// printf("Avant executer STDOUT is : %d\n", STDOUT_FILENO);
		// fflush(stdout);
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
		{
			perror("Erreur d'exécution de la commande");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}

void	exc_solo_cmd(t_exe *exe, t_parser *cmds)
{
	if (!cmds->prev && !cmds->next)
	{
		if (handle_redir(exe, cmds) == -1)
		{	
			perror("Erreur d'exécution de la redirection");
			exit(EXIT_FAILURE);
		}
		close(exe->pipefd[0]);
		close(exe->pipefd[1]);
		if (exe->fd[1] != STDOUT_FILENO) // passer les redirs;
		{
			dup2(exe->fd[1], STDOUT_FILENO);
			close (exe->fd[1]);
		}
		exec_commande(exe, cmds);
		perror("Erreur d'exécution de la seule commande");
		exit(1);
	}
	return ;
}
