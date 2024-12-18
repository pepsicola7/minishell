/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_utile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2024/12/18 14:55:15 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return(cmd);
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

	// if (exe->fd[1] != STDOUT_FILENO) // passer les redirs;
	// {
	// 	printf("i should be there twice\n");
	// 	fflush(stdout);
	// 	if (dup2(exe->fd[1], STDOUT_FILENO) == -1)
	// 	{
	// 		perror("1dup2 failed");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	// dup2(exe->fd[1], STDOUT_FILENO);
	// 	// close(exe->fd[1]);
		if (exe->hd_pipe[0] != -1) // there has a heredoc;
		{
			close(exe->hd_pipe[1]);
			// close(exe->hd_pipe[0]);
		}
	// }
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

int	handle_redir_solo(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	
	redirection = cmds->redirections;
	while (redirection) // && redirection->type != PIPE
	{
		if (redirection->type == REDIR_IN) // <
		{
			// printf("the value is : %s\n", redirection->value);
			// fflush(stdout);
			old_fd = open(redirection->value, O_RDONLY);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			close(old_fd);
		}
		if (redirection->type == REDIR_OUT) // >
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			// printf("the fd of redir_out%d\n", old_fd);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			// if (dup2(old_fd, exe->fd[1]) == -1)
			// {
			// 	perror("Erreur dup2 pour REDIR_OUT");
			// 	return (-1);
			// }
			// printf("avant fd[1] est : %d\n", exe->fd[1]);
			// fflush(stdout);
			// exe->fd[1] = old_fd;  // Pour refléter que la redirection est active
			close(old_fd);
		}
		if (redirection->type == APPEND) // >> test le cas special apres
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			// if (dup2(old_fd, exe->fd[1]) == -1)
			// {
			// 	perror("Erreur dup2 pour APPEND");
			// 	return (-1);
			// }
			// exe->fd[1] = old_fd;
			close(old_fd);
		}
		if (redirection->type == HEREDOC) // <<
		{
			if (redir_heredoc(exe, cmds) == -1)
			{
				perror("Erreur dup2 pour HERE_DOC");
				return (-1);
			}
		}
		redirection = redirection->next;
	}
	// printf("number of the cmds : %d\n", exe->nmb_cmd);
	return (0);
}
