/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/11/29 20:21:18 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

// int	redir_pipe(t_exe *exe, t_parser *cmds)
// {
	
// }

void	redir_heredoc(t_exe *exe, t_parser *cmds)
{
	char	*line;
	t_lexer	*redirection;

	redirection = cmds->redirections;
	if (pipe(exe->hd_pipe) == -1)
	{
		perror("Erreur lors de la création du pipe");
		exit(EXIT_FAILURE);
	}
	close(exe->hd_pipe[0]);
	while (1)
	{
		line = readline(">");
		if (ft_strcmp(line, redirection->value) == 0)
			break;
		write(exe->hd_pipe[1], line, ft_strlen(line));
		write(exe->hd_pipe[1], "/n", 1);
	}
	dup2(exe->hd_pipe[1], exe->fd[1]);
	close(exe->hd_pipe[1]);
	return;
}

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	
	redirection = cmds->redirections;
	while (redirection) // && redirection->type != PIPE
	{
		if (redirection->type == REDIR_IN) // <
		{
			old_fd = open(redirection->value, O_RDONLY);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			dup2(old_fd, exe->fd[0]);
			close(old_fd);
		}
		if (redirection->type == REDIR_OUT) // >
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			dup2(old_fd, exe->fd[1]);
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
			dup2(old_fd, exe->fd[1]);
			close(old_fd);
		}
		if (redirection->type == HEREDOC) // <<
			redir_heredoc(exe, cmds);
		redirection = redirection->next;
	}
	printf("number of the cmds : %d\n", exe->nmb_cmd);
	printf("pipefd[1] est : %d\n", exe->pipefd[1]);
	fflush(stdout);
	if (exe->nmb_cmd > 1) // gerer pipesfd[1]ici
	{
		printf("dhiwehdwhe\n");
		fflush(stdout);
		if (exe->fd[1] != STDOUT_FILENO) // if there have a output deja;
		{
			printf("Pipe avec les redirections\n");
			fflush(stdout);
			//exe->fd[1] = STDOUT_FILENO; il faut initial ou?
			close(exe->pipefd[1]);
		}
		else if (exe->fd[1] == STDOUT_FILENO)
		{
			printf("Pipe sans les redirections\n");
			fflush(stdout);
			exe->fd[1] = exe->pipefd[1];
			dup2(exe->fd[1], STDOUT_FILENO);
			printf("fd[1] est : %d\n", exe->fd[1]);
			fflush(stdout);
			close(exe->pipefd[1]);
		}
		// if (exe->fd[0] != STDIN_FILENO) // if there have a input;si'il existe des choses dans le pipe precedent, fd[0] == pipefd[1](ancien avant);
		// 	close(exe->pipefd[0]);
		// if (exe->fd[0] == STDIN_FILENO)
		// {
		// 	dup2(exe->pipefd[0], );
		// 	close(exe->pipefd[0]);
		// }
	}
	// redir_pipe(exe, cmds);
	return (0);
}


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
	// while (i > 0)
	// {
	// free(sp_path[i]);
	// 	i--;
	// }
	// free(sp_path); // check free;
	return (path);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	char	*exc_pathname;

	if (handle_redir(exe, cmds) == -1)
	{	
		perror("Erreur d'exécution de la redirection");
		exit(EXIT_FAILURE);
	}
	// if (exe->nmb_cmd != 0)
	// {
	// 	if (exe->nmb_cmd > 1) // if this is not the last commande
	// 	{
	// 		exe->nmb_cmd -= 1;
	// 		printf("nb command = %d\n", exe->nmb_cmd);
	// 	}
	// }
	if (cmds->cmd)
	{
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]); // check cmd[0] faut parcourir dans la commande?
		// printf("path complet is : %s\n", exc_pathname); 
		// ajouter la condition pour check the pathname;
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
		{
			perror("Erreur d'exécution de la commande");
			exit(EXIT_FAILURE);
		}
		
	}
	exit(EXIT_SUCCESS);
}

int	pipeline(t_exe *exe, t_parser *cmds)
{
	int	i;

	i = exe->index_pid;
	while (cmds)
	{
		exe->pid[i] = fork(); // sauvgarder le pid pour waitpit() a la fin;
		if (cmds == NULL)
			return (0);
		if (exe->pid[i] == -1)
		{
			free(exe->pid);
			perror("Erreur du fork");
			return (-1);
		}
		if (exe->pid[i] == 0) // processus enfant
		{
			// printf ("pid child is : %d", exe->pid[i]);
			printf("Pour la commande : %s le pipefd[0] = %d\n", cmds->cmd[0], exe->pipefd[0]);
			fflush(stdout);
			// if (exe->pipefd[0] == -1) //the first commande
			if (exe->pipefd[0] != -1) //not the first cmd, envoyer la sortie a l'entree
			{
				dup2(exe->pipefd[0], STDIN_FILENO); // ??? pas sure
				// close(exe->pipefd[0]);
			}
			close(exe->pipefd[0]);
			exec_commande(exe, cmds);
			perror("Erreur d'exécution de la commande");
			exit(1);
		}
		cmds = cmds->next;
		exe->index_pid++; 
		exe->nmb_cmd -= 1;
		// ici a la fin check si'il est bien imprimente;
		// exe->pipefd[0] = 42;
		// printf("Index après incrémentation : %d\n", exe->index_pid);
	}
	while (i >= 0)
	{
		if (waitpid(exe->pid[i], NULL, 0) == -1) // nEed to modifier pid as a tableau
		{
			perror("Error de exec parent");
			return (-1);
		}
		i--;
	}
	// gerer exit() et dree pid avec la taille exe->num_cmd;
	return (0);
}
