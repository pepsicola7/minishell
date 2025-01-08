/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2025/01/08 17:32:07 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	char	*exc_pathname;

	if (cmds->cmd)
	{
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]); // check cmd[0] faut parcourir dans la commande?
		if (!exc_pathname)
		{
			ft_fprintf(2, "%s: command not found\n", cmds->cmd[0]);
			free(exc_pathname);
			exit(EXIT_FAILURE);
		}
		setup_signals(1);
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
		{
			ft_fprintf(2, "%s: command not found\n", cmds->cmd[0]);
			// exit(EXIT_FAILURE);
		}
	}
	printf("lalala\n");
	exit(EXIT_SUCCESS);
}

void	exc_solo_cmd(t_exe *exe, t_parser *cmds)
{
	if (!cmds->prev && !cmds->next)
	{
		if (handle_redir(exe, cmds) == -1)
		{	
			// perror("Erreur d'exécution de la redirection");
			exit(EXIT_FAILURE);
		}
		if (exe->fd[1] != STDOUT_FILENO) // passer les redirs;
		{
			dup2(exe->fd[1], STDOUT_FILENO);
			close(exe->fd[1]);
		}
		if (exe->fd[0] != STDIN_FILENO)
		{
			dup2(exe->fd[0], STDIN_FILENO);
			close(exe->fd[0]);
		}
		exec_commande(exe, cmds);
		perror("Erreur d'exécution de la seule commande");
		exit(1);
	}
	return ;
}


