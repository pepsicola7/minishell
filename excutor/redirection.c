/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/11/20 21:18:38 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	
	redirection = cmds->redirections;
	while (redirection)
	{
		if (redirection->type == REDIR_IN) // <
		{
			old_fd = open(redirection->value, O_RDONLY);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			dup2(old_fd, exe->fd[0]); // intput ici est old_fd, output ici est STDOUT;
			close(exe->fd[0]);
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
		if (redirection->type == APPEND) // ??? test le cas special apres
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			dup2(old_fd, exe->fd);//???
			close(old_fd);
		}
		if (redirection->type == HEREDOC)
		{
			
		}
		if (redirection->type == PIPE) // gerer pipesfd[1]ici
		{
			if (exe->fd[1] != STDOUT_FILENO && exe->pipefd[1]) // if there have a output deja;
				close(exe->pipefd[1]);
			else if (exe->fd[1] == STDOUT_FILENO && exe->pipefd[1])
			{
				dup2(exe->pipefd[1], STDOUT_FILENO);
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
		redirection = redirection->next;
	}
	return (0);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	if (exe->nmb_cmd != 0)
	{
		if (exe->nmb_cmd > 1) // if this is not the last commande
		{
			if (pipe(exe->pipefd) == -1) // open the pipe
			{
				perror("Erreur lors de la création du pipe");
				exit(EXIT_FAILURE);
			}
			exe->nmb_cmd -= 1;
		}
	}
	if (handle_redir(exe, cmds) == -1)
	{	
		perror("Erreur d'exécution de la redirection");
		exit(EXIT_FAILURE);
	}
	if (!cmds->cmd)
	{
		if (execve(exe->pathname, cmds->cmd, exe->env) == -1)
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
	exe->pid[i] = fork(); // sauvgarder le pid poue waitpit() a la fin;
	if (exe->pid[i] == -1)
	{
		perror("Erreur du fork");
		return (-1);
	}
	if (exe->pid[i] == 0) // processus enfant
	{
		if (exe->pipefd[0] == -1) //the first commande
			close(exe->pipefd[0]);
		if (exe->pipefd[0] != -1) //not the first cmd, envoyer la sortie a l'entree
		{
			dup2(exe->pipefd[0], STDIN_FILENO); // ??? pas sure
			close(exe->pipefd[0]);
		}
		exec_commande(exe, cmds);
		perror("Erreur d'exécution de la commande");
		exit(1);
	}
	else //processus parent
	{
		while (cmds->cmd)
		{
			pipeline(exe, cmds);
			cmds = cmds->next;
			exe->index_pid++;
		}
		while (i >= 0)
		{
			if (waitpid(exe->pid[i], NULL, 0) == -1) // nned to modifier pid as a tableau
			{
				perror("Error de exec parent");
				return (-1);
			}
			i--;
		}
		// gerer exit();
	}
	return (0);
}
