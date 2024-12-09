/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/12/09 15:37:07 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	redir_heredoc(t_exe *exe, t_parser *cmds)
{
	char	*line;
	t_lexer	*redirection;

	redirection = cmds->redirections;
	if (pipe(exe->hd_pipe) == -1)
	{
		perror("Erreur lors de la création du pipe");
		exit(EXIT_FAILURE);
	}
	// if (dup2(exe->hd_pipe[0], exe->fd[0]) == -1)
	// {
	// 	perror("Erreur dup2 pour REDIR_IN");
	// 	return (-1);
	// }
	while (1)
	{
		line = readline(">");
		if (ft_strcmp(line, redirection->value) == 0)
			break;
		write(exe->hd_pipe[1], line, ft_strlen(line));
		write(exe->hd_pipe[1], "\n", 1);
	}
	if (dup2(exe->hd_pipe[0], exe->fd[0]) == -1)
		return (-1);
	exe->fd[1] = exe->hd_pipe[1];
	// ici est juste pour changer la valeur de fd, pour que'il note le changement de la redirection, pcq dup2 change pas la valuer de fd;
	dup2(exe->hd_pipe[1], exe->fd[1]);
	// printf("heredoc fd[1] is :%d\n", exe->fd[1]);
	// printf("heredoc fd[0] is :%d\n", exe->fd[0]);
	// fflush(stdout);
	close(exe->hd_pipe[1]);
	close(exe->hd_pipe[0]);
	// close(exe->hd_pipe[1]);
	return(0);
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
			if (dup2(old_fd, exe->fd[0]) == -1)
			{
				perror("Erreur dup2 pour REDIR_IN");
				return (-1);
			}
			close(old_fd);
		}
		if (redirection->type == REDIR_OUT) // >
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			// printf("the fd of redir_out%d\n", old_fd);
			// fflush(stdout);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			if (dup2(old_fd, exe->fd[1]) == -1)
			{
				perror("Erreur dup2 pour REDIR_OUT");
				return (-1);
			}
			// printf("avant fd[1] est : %d\n", exe->fd[1]);
			exe->fd[1] = old_fd;  // Pour refléter que la redirection est active
			// close(old_fd);
		}
		if (redirection->type == APPEND) // >> test le cas special apres
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			if (dup2(old_fd, exe->fd[1]) == -1)
			{
				perror("Erreur dup2 pour APPEND");
				return (-1);
			}
			exe->fd[1] = old_fd;
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
	// printf("APRES fd[1] est : %d\n", exe->fd[1]);
	// fflush(stdout);
	return (0);
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
			dup2(exe->fd[1], STDOUT_FILENO);
		close (exe->fd[1]);
		exec_commande(exe, cmds);
		perror("Erreur d'exécution de la seule commande");
		exit(1);
	}
	return ;
}

void	pipex(t_exe *exe)
{
	if (exe->nmb_cmd > 1) // gerer pipesfd[1]ici
	{
		// if (exe->fd[1] != STDOUT_FILENO) // if there have a output deja;
		// {
		// 	printf("Pipe avec les redirections\n");
		// 	fflush(stdout);
		// 	close(exe->pipefd[1]);
		// }
		if (exe->fd[1] == STDOUT_FILENO) // sans redirections
		{
			// printf("Pipe sans les redirections\n");
			// printf("exe->pipefd[1] est :%d\n", exe->pipefd[1]);
			// printf("Avant dup2, pipefd[1] = %d\n", exe->pipefd[1]);
			if (dup2(exe->pipefd[1], STDOUT_FILENO) == -1) {
				perror("dup2 failed");
				exit(EXIT_FAILURE);
			}
			// printf("Après dup2\n");
			// fflush(stdout);
			// exe->fd[1] = exe->pipefd[1];
		}
		close(exe->pipefd[1]);
		close(exe->pipefd[0]);
	}
	return ;
}

int	pipeline(t_exe *exe, t_parser *cmds)
{
	int	i;
	int prev_pipefd = -1;

	i = exe->index_pid;
	while (cmds)
	{
		if (cmds->next)
		{
			if (pipe(exe->pipefd) == -1)
			{
				perror("Erreur lors de la création du pipe");
				exit(EXIT_FAILURE);
			}
		}
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
			printf("Pour la commande : %s le pipefd[1] = %d\n", cmds->cmd[0], exe->pipefd[1]);
			printf("prev est %d\n", prev_pipefd);
			fflush(stdout);
			exc_solo_cmd(exe, cmds);
			if (cmds->prev) //not the first cmd, envoyer la sortie a l'entree
			{
				if (dup2(prev_pipefd, STDIN_FILENO) == -1)
				{
					perror("Erreur dup2");
					exit(EXIT_FAILURE);
				}
				printf("STDIN est : %d\n", STDIN_FILENO);
				fflush(stdout);
				// dup2(exe->pipefd[1], STDIN_FILENO); // ??? pas sure
				close(prev_pipefd);
			}
			if (!cmds->next) // the last command
			{
				dup2(1, STDOUT_FILENO);
				// close(exe->pipefd[0]);
				// close(exe->pipefd[1]);
			}
			if (handle_redir(exe, cmds) == -1)
			{	
				perror("Erreur d'exécution de la redirection");
				exit(EXIT_FAILURE);
			}
			printf("le pipefd[0] : %d le pipefd[1] = %d\n", exe->pipefd[0], exe->pipefd[1]);
			pipex(exe);
			exec_commande(exe, cmds);
			perror("Erreur d'exécution de la commande");
			exit(1);
		}
		if (cmds->next)
		{
			close(exe->pipefd[1]);
			// if (prev_pipefd != -1)
			// 	close(exe->prev_pipefd);
			prev_pipefd = exe->pipefd[0];
		}
		cmds = cmds->next;
		exe->index_pid++; 
		exe->nmb_cmd -= 1;
		// close(exe->pipefd[0]);
		// // ici a la fin check si'il est bien incrémenté;
		// printf("Index après incrémentation : %d\n", exe->index_pid);
		// fflush(stdout);
	}
	if (prev_pipefd != -1)
	{
		close(prev_pipefd);
	}
	// printf("index est %d:", exe->index_pid);
	// fflush(stdout);
	for (int j = 0; j < exe->index_pid; j++)
	{
		if (waitpid(exe->pid[j], NULL, 0) == -1)
		{
			perror("Error during waitpid");
			return -1;
		}
	}
	return (0);
}
	// while (i >= 0)
	// {
	// 	if (waitpid(exe->pid[i], NULL, 0) == -1) // Need to modifier pid as a tableau
	// 	{
	// 		perror("Error de exec parent");
	// 		return (-1);
	// 	}
	// 	i--;
	// }
	// gerer exit() et dree pid avec la taille exe->num_cmd;
