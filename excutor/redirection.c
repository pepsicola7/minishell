/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/11/13 19:20:25 by peli             ###   ########.fr       */
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
			old_fd = open (redirection->value, O_RDONLY);
			if (old_fd == -1)
			{
				perror ("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			dup2 (old_fd, exe->fd[0]); // intput ici est old_fd, output ici est STDOUT;
			close (exe->fd);
		}
		if (redirection->type == REDIR_OUT) // >
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (old_fd == -1)
			{
				perror ("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			dup2 (old_fd, exe->fd[1]);
			close (old_fd);
		}
		if (redirection->type == APPEND) // ??? teste le cas special apres
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror ("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			dup2 (old_fd, exe->fd);
			close (old_fd);
		}
		if (redirection->type == HEREDOC)
		{
			
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
			if (pipe(exe->pipefd) == -1)
			{
				perror("Erreur lors de la création du pipe");
				exit(EXIT_FAILURE);
			}
			close (exe->pipefd[0]);
			dup2 (exe->pipefd[1], STDOUT_FILENO);
			close (exe->pipefd[1]);
			exe->nmb_cmd -= 1;
		}
		else // exe->nmb_cmd = 1 cest a dire la derniere commande;
		{
			close (exe->pipefd[0]);
			close (exe->pipefd[1]);
		}
	}
	if (handle_redir(exe, cmds) == -1)
	{
		perror("Erreur d'exécution de la redirection");
		exit (EXIT_FAILURE);
	}
	if (execve(exe->pathname, cmds->cmd, exe->env) == -1)
	{
		perror("Erreur d'exécution de la commande");
		exit (EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

int	exec_redirection(t_exe *exe, t_parser *cmds)
{
	exe->pid = fork(); // sauvgarder le pid poue waitpit() a la fin;
	if (exe->pid == -1)
	{
		perror ("Erreur du fork");
		return (-1);
	}
	if (exe->pid == 0) // processus enfant
	{
		if (exe->pipefd[0] != -1) // ?? insure the condition !this is not the first commande, envoyer la sortie a l'entree
		{
			exe->fd[0] = exe->pipefd[1];
			close (exe->pipefd[1]);
		}
		exec_commande(exe, cmds);
		perror ("Erreur d'exécution de la commande");
		exit (1);
	}
	else //processus parent
	{
		while (cmds->cmd)
		{
			exec_redirection(exe, cmds);
			cmds = cmds->next;
		}
		if (waitpid(exe->pid, NULL, 0) == -1) // ici pid est sauvegarde au debut?
		{
			perror ("Error de exec parent");
			return (-1);
		}
	}
	return (0);
}
