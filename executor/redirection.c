/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2025/01/08 17:22:41 by peli             ###   ########.fr       */
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
		}
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
			if (dup2(exe->fd[1], exe->pipefd[1]) == -1)
			{
				perror("1dup2 failed");
				exit(EXIT_FAILURE);
			}
			close(exe->fd[1]);
		}
		if (exe->fd[0] != STDIN_FILENO)
		{
			if (dup2(exe->fd[0], STDIN_FILENO) == -1)
			{
				perror("5dup2 failed");
				exit(EXIT_FAILURE);
			}
			close(exe->fd[0]);
		}
		if (dup2(exe->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("2dup2 failed");
			exit(EXIT_FAILURE);
		}
		close(exe->pipefd[0]);
		close(exe->pipefd[1]);
	}
	return ;
}
