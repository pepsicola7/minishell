/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 09:44:16 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

void	print_error(t_parser *cmds)
{
	ft_fprintf(2, "minishell: ");
	fflush(stdout);
	perror(cmds->redirections->value);
}

int	handle_input_redir(t_exe *exe, t_parser *cmds, t_lexer *redirection)
{
	int	old_fd;

	old_fd = open(redirection->value, O_RDONLY);
	if (redirection->type == HEREDOC)
		unlink(redirection->value);
	if (old_fd == -1)
		return (print_error(cmds), -1);
	if (dup2(old_fd, exe->fd[0]) == -1)
	{
		perror("Erreur dup2 pour REDIR_IN");
		close(old_fd);
		return (-1);
	}
	exe->fd[0] = old_fd;
	return (0);
}

int	handle_output_redir(t_exe *exe, t_parser *cmds, t_lexer *redir, int mode)
{
	int	old_fd;

	old_fd = open(redir->value, O_WRONLY | O_CREAT | mode, 0644);
	if (old_fd == -1)
		return (print_error(cmds), -1);
	if (dup2(old_fd, exe->fd[1]) == -1)
	{
		perror("Erreur dup2 pour REDIR_OUT/APPEND");
		close(old_fd);
		return (-1);
	}
	exe->fd[1] = old_fd;
	return (0);
}

int	handle_redir(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;

	redirection = cmds->redirections;
	while (redirection)
	{
		if (redirection->type == REDIR_IN || redirection->type == HEREDOC)
		{
			if (handle_input_redir(exe, cmds, redirection) == -1)
				return (-1);
		}
		else if (redirection->type == REDIR_OUT)
		{
			if (handle_output_redir(exe, cmds, redirection, O_TRUNC) == -1)
				return (-1);
		}
		else if (redirection->type == APPEND)
		{
			if (handle_output_redir(exe, cmds, redirection, O_APPEND) == -1)
				return (-1);
		}
		redirection = redirection->next;
	}
	return (0);
}

void	pipex(t_exe *exe)
{
	if (exe->nmb_cmd > 1)
	{
		if (exe->fd[1] != STDOUT_FILENO)
		{
			if (dup2(exe->fd[1], exe->pipefd[1]) == -1)
				exit(EXIT_FAILURE);
			close(exe->fd[1]);
		}
		if (exe->fd[0] != STDIN_FILENO)
		{
			if (dup2(exe->fd[0], STDIN_FILENO) == -1)
				exit(EXIT_FAILURE);
			close(exe->fd[0]);
		}
		if (dup2(exe->pipefd[1], STDOUT_FILENO) == -1)
			exit(EXIT_FAILURE);
		close(exe->pipefd[0]);
		close(exe->pipefd[1]);
	}
	return ;
}
