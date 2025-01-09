/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2025/01/09 19:23:45 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	exec_commande(t_exe *exe, t_parser *cmds, t_env **env)
{
	char	*exc_pathname;

	if (cmds->cmd)
	{
		if (cmds->builtin)
			exit(cmds->builtin(env, cmds));
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]);
		if (!exc_pathname)
		{
			ft_fprintf(2, "%s: command not found	\n", cmds->cmd[0]);
			exit(EXIT_FAILURE);
		}
		setup_signals(1);
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
			exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	exc_solo_cmd(t_exe *exe, t_parser *cmds, t_env **env)
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
		exec_commande(exe, cmds, env);
		perror("Erreur d'exécution de la seule commande");
		exit(1);
	}
	return ;
}


