/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/11/12 18:50:15 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	int	index_pipe;
	
	redirection = cmds->redirections;
	while (redirection)
	{
		if (redirection->type == PIPE)
		{
			dup2 (exe->fd, exe->pipefd[index_pipe]); // si le type est pipe, le value est vide?
			close (exe->fd);
			exe->fd = exe->pipefd[index_pipe + 1];
			close (exe->pipefd[1]);
		}
		if (redirection->type == REDIR_IN)
		{
			old_fd = open (redirection->value, O_RDONLY);
			if (old_fd == -1)
			{
				perror ("Erreur d'ouverture du fichier d'entree");
				return (-1);
			}
			dup2 (old_fd, exe->fd);
			close (old_fd);
		}
		if (redirection->type == REDIR_OUT)
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (old_fd == -1)
			{
				perror ("Erreur d'ouverture du fichier de sortie");
				return (-1);
			}
			dup2 (old_fd, exe->fd);
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
	close (exe->pipefd[0]);
	
	// int	fd;

	// fd = STDIN_FILENO;
	// exe->fd = fd;
	// if (handle_redir(exe, cmds) == -1)
	// {
	// 	perror("Erreur d'exécution de la redirection");
	// 	exit (EXIT_FAILURE);
	// }
	// if (execve(exe->pathname, cmds->cmd, exe->env) == -1)
	// {
	// 	perror("Erreur d'exécution de la commande");
	// 	exit (EXIT_FAILURE);
	// } 
	// return (0);
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
// La premiere version : attention // le processus enfant ne reviendra jamais dans la boucle après le premier appel à exec_commande(),
	// if (exe->pid == 0) // processus enfant
	// {
	// 	while (cmds->cmd)
	// 	{
	// 		exec_commande(exe, cmds);
	// 		cmds = cmds->next;
	// 	}
	// 	exit (0); // ici c'est oblige?
	// }