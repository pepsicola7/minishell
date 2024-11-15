/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peiqi <peiqi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2024/11/15 04:18:15 by peiqi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	redir_pipe(t_exe *exe, t_parser *cmds)
{
	
}

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;
	int old_fd;
	
	redirection = cmds->redirections;
	while (redirection && redirection->type != PIPE)
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
		if (redirection->type == APPEND) // ??? teste le cas special apres
		{
			old_fd = open (redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (old_fd == -1)
			{
				perror("Erreur d'ouverture du fichier en mode append");
				return (-1);
			}
			dup2(old_fd, exe->fd);
			close(old_fd);
		}
		if (redirection->type == HEREDOC)
		{
			
		}
		redirection = redirection->next;
	}
	redir_pipe(exe, cmds);
	return (0);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	// if (exe->nmb_cmd != 0)
	// {
	// 	if (exe->nmb_cmd > 1) // if this is not the last commande
	// 	{
	// 		if (pipe(exe->pipefd) == -1)
	// 		{
	// 			perror("Erreur lors de la création du pipe");
	// 			exit(EXIT_FAILURE);
	// 		}
	// 		close(exe->pipefd[0]);
	// 		dup2(exe->pipefd[1], exe->fd[1]);
	// 		close(exe->pipefd[1]);
	// 		exe->nmb_cmd -= 1; // ici change the count; 
	// 	}
	// 	else //la derniere commande;
	// 	{
	// 		close(exe->pipefd[0]);
	// 		close(exe->pipefd[1]);
	// 	}
	// }

	// si je gere pas d'abord le pipe, je vois d'abord si'il y a lesd redir;
	exe->fd[0] = STDIN_FILENO;
	exe->fd[1] = STDOUT_FILENO; // initial chaque fois input_fd et output_fd;
	if (handle_redir(exe, cmds) == -1)
	{
		perror("Erreur d'exécution de la redirection");
		exit(EXIT_FAILURE);
	}
	if (execve(exe->pathname, cmds->cmd, exe->env) == -1)
	{
		perror("Erreur d'exécution de la commande");
		exit(EXIT_FAILURE);
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
		free(exe->pid);
		perror("Erreur du fork");
		return (-1);
	}
	if (exe->pid[i] == 0) // processus enfant
	{
		if (exe->pipefd[0] != -1) //this is not the first commande, envoyer la sortie a l'entree
		{
			dup2(exe->pipefd[0], STDIN_FILENO);
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
			exe->index_pid++; // ici a la fin check si'il est bien imprimente;
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
	}
	return (0);
}
