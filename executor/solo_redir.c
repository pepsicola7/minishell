/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:50:01 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 09:44:45 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	open_input_redirection(t_lexer *redirection)
{
	int	old_fd;

	old_fd = open(redirection->value, O_RDONLY);
	if (redirection->type == HEREDOC)
		unlink(redirection->value);
	if (old_fd == -1)
	{
		perror("Erreur d'ouverture du fichier d'entree");
		return (-1);
	}
	close(old_fd);
	return (0);
}

int	open_output_redirection(t_lexer *redirection)
{
	int	old_fd;

	if (redirection->type == REDIR_OUT)
		old_fd = open(redirection->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirection->type == APPEND)
		old_fd = open(redirection->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (0);
	if (old_fd == -1)
	{
		perror("Erreur d'ouverture du fichier de sortie");
		return (-1);
	}
	close(old_fd);
	return (0);
}

int	handle_redir_solo(t_exe *exe, t_parser *cmds)
{
	t_lexer	*redirection;

	(void)exe;
	redirection = cmds->redirections;
	while (redirection)
	{
		if (redirection->type == REDIR_IN || redirection->type == HEREDOC)
		{
			if (open_input_redirection(redirection) == -1)
				return (-1);
		}
		else if (redirection->type == REDIR_OUT || redirection->type == APPEND)
		{
			if (open_output_redirection(redirection) == -1)
				return (-1);
		}
		redirection = redirection->next;
	}
	return (0);
}
