/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_utile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2025/01/03 18:57:11 by tbartocc         ###   ########.fr       */
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
	return (path);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	char	*exc_pathname;

	if (cmds->cmd)
	{
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]); // check cmd[0] faut parcourir dans la commande?
		if (!exc_pathname)
		{
			perror("command not found\n");
			free(exc_pathname);
			exit(EXIT_FAILURE);
		}
		setup_signals(1);
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
		{
			printf("%s: command not found\n", cmds->cmd[0]);
			// exit(EXIT_FAILURE);
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
			// perror("Erreur d'exécution de la redirection");
			exit(EXIT_FAILURE);
		}
		close(exe->pipefd[0]);
		close(exe->pipefd[1]);
		if (exe->fd[1] != STDOUT_FILENO) // passer les redirs;
		{
			dup2(exe->fd[1], STDOUT_FILENO);
			close(exe->fd[1]);
		}
		if (exe->fd[0] != STDIN_FILENO)
		{
			dup2(exe->fd[0], STDIN_FILENO);
			close(exe->fd[0]);
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
	
	(void)exe;
	redirection = cmds->redirections;
	while (redirection) // && redirection->type != PIPE
	{
		if (redirection->type == REDIR_IN || redirection->type == HEREDOC) // < or <<
		{
			old_fd = open(redirection->value, O_RDONLY);
			if (redirection->type == HEREDOC)
				unlink(redirection->value);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			close(old_fd);
		}
		if (redirection->type == REDIR_OUT) // >
		{
			old_fd = open(redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			close(old_fd);
		}
		if (redirection->type == APPEND) // >> test le cas special apres
		{
			old_fd = open(redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			close(old_fd);
		}
		redirection = redirection->next;
	}
	return (0);
}
