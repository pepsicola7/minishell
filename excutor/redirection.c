/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2025/01/02 14:23:16 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

// int	redir_heredoc(t_exe *exe, t_lexer *redirection)
// {
// 	char	*line = NULL;

// 	if (pipe(exe->hd_pipe) == -1)
// 	{
// 		perror("Erreur lors de la création du pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	printf("redirection value is : %s\n", redirection->value);
// 	fflush(stdin);
// 	while (1)
// 	{
// 		line = readline(">");
// 		if (ft_strcmp(line, redirection->value) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		write(exe->hd_pipe[1], line, ft_strlen(line));
// 		write(exe->hd_pipe[1], "\n", 1);
// 		free(line);
// 	}
// 	close(exe->hd_pipe[1]);
// 	// dup2(exe->hd_pipe[0], exe->fd[0]); suretout pas ici;
// 	exe->fd[0] = exe->hd_pipe[0];
// 	return(0);
// }

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	
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
			if (dup2(old_fd, exe->fd[0]) == -1)
			{
				perror("Erreur dup2 pour REDIR_IN");
				return (-1);
			}
			exe->fd[0] = old_fd;
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
			if (dup2(old_fd, exe->fd[1]) == -1)
			{
				perror("Erreur dup2 pour REDIR_OUT");
				return (-1);
			}
			exe->fd[1] = old_fd;  // Pour refléter que la redirection est active
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
			// close(old_fd);
		}
		// if (redirection->type == HEREDOC) // <<
		// {
		// 	if (redir_heredoc(exe, redirection) == -1)
		// 	{
		// 		perror("Erreur dup2 pour HERE_DOC");
		// 		return (-1);
		// 	}
		// }
		redirection = redirection->next;
	}
	return (0);
}

void	pipex(t_exe *exe)
{
	if (exe->nmb_cmd > 1) // gerer pipesfd[1]ici
	{
		if (exe->fd[1] != STDOUT_FILENO) //with redirection
		{
			printf("Pipe avec les redirections\n");
			fflush(stdout);
			if (dup2(exe->fd[1], exe->pipefd[1]) == -1)
			{
				perror("1dup2 failed");
				exit(EXIT_FAILURE);
			}
			close(exe->fd[1]);
		}
		if (exe->fd[0] != STDIN_FILENO)
		{
			printf("Pipe avec les redirections\n");
			fflush(stdout);
			if (dup2(exe->fd[0], STDIN_FILENO) == -1)
			{
				perror("5dup2 failed");
				exit(EXIT_FAILURE);
			}
			// dup2(exe->pipefd[0], STDIN_FILENO);
			close(exe->fd[0]);
		}
		// printf("fd[1] est : %d\n", exe->fd[1]);
		if (exe->fd[1] == STDOUT_FILENO) // sans redirections
		{
			printf("Pipe sans les redirections\n");
			fflush(stdout);
		}
		if (dup2(exe->pipefd[1], STDOUT_FILENO) == -1) {
			perror("2dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(exe->pipefd[0]);
		close(exe->pipefd[1]);
	}
	return ;
}

int	pipeline(t_exe *exe, t_parser *cmds)
{
	int	i;
	int prev_pipefd = -1;

	i = exe->index_pid;
	while (cmds && (cmds->cmd || cmds->redirections))
	{
		while (cmds && cmds->redirections && !cmds->cmd)
		{
			if (handle_redir_solo(exe, cmds) == -1)
				perror("Erreur d'exécution de la redirection");
			if (cmds->prev && cmds->prev->cmd) //not the first cmd, envoyer la sortie a l'entree
			{
				if (prev_pipefd != STDIN_FILENO)
					close(prev_pipefd);
			}
			if (dup2(1, STDOUT_FILENO) == -1) 
				perror("3dup2 failed");
			cmds = cmds->next;
			exe->nmb_cmd -= 1;
		}
		if (cmds && cmds->cmd)
		{
			// printf("why i m here");
			// fflush(STDIN_FILENO);
			if (cmds->next)
			{
				if (pipe(exe->pipefd) == -1)
					perror("Erreur lors de la création du pipe");
			}
			exe->pid[i] = fork(); // sauvgarder le pid pour waitpit() a la fin;
			// if (cmds == NULL)
			// 	return (0);
			if (exe->pid[i] == -1)
			{
				free(exe->pid);
				perror("Erreur du fork");
				return (-1);
			}
			if (exe->pid[i] == 0) // processus enfant
			{
				// printf("Pour la commande : %s le pipefd[1] = %d\n", cmds->cmd[0], exe->pipefd[1]);
				// printf("prev est %d\n", prev_pipefd);
				exc_solo_cmd(exe, cmds);
				if (cmds->prev && cmds->prev->cmd) //not the first cmd, envoyer la sortie a l'entree
				{
					if (prev_pipefd != STDIN_FILENO)
					{
						if (dup2(prev_pipefd, STDIN_FILENO) == -1)
							perror("Erreur dup2");
						close(prev_pipefd);
					}
				}
				if (!cmds->next) // the last command
				{
					if (dup2(STDOUT_FILENO, STDOUT_FILENO) == -1)
						perror("3dup2 failed");
					// close(exe->pipefd[0]);
				}
				if (handle_redir(exe, cmds) == -1)
					perror("Erreur d'exécution de la redirection");
				// printf("le pipefd[0] : %d le pipefd[1] = %d\n", exe->pipefd[0], exe->pipefd[1]);
				pipex(exe);
				exec_commande(exe, cmds);
				perror("Erreur d'exécution de la commande");
				exit(1);
			}
			if (cmds && cmds->next) // not the last cmd;
			{
				// if (prev_pipefd != -1)
				// {
					close(exe->pipefd[1]);
					close(prev_pipefd);
					prev_pipefd = exe->pipefd[0];
			}
			exe->nmb_cmd -= 1;
			cmds = cmds->next;
			exe->index_pid++; 
		}
	}
	// close(exe->pipefd[0]);
	if (prev_pipefd != -1) // not the first cmd;
	{
		close(prev_pipefd);
		// close(STDIN_FILENO);
	}
	// printf("index est %d:", exe->index_pid);
	// fflush(stdout);
	int j = 0;
	while( j < exe->index_pid)
	{
		if (waitpid(exe->pid[j], NULL, 0) == -1)
		{
			perror("Error during waitpid");
			return -1;
		}
		j++;
	}
	return (0);
}

