/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 11:09:05 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 11:11:06 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_redirection_solo(t_exe *exe, t_parser **cmds, int *prev_pipefd)
{
	while (*cmds && (*cmds)->redirections && !(*cmds)->cmd)
	{
		if (handle_redir_solo(exe, *cmds) == -1)
			perror("Erreur d'exécution de la redirection");
		if ((*cmds)->prev && (*cmds)->prev->cmd)
		{
			if (*prev_pipefd != STDIN_FILENO)
				close(*prev_pipefd);
		}
		*cmds = (*cmds)->next;
		exe->nmb_cmd -= 1;
	}
	return (0);
}

int	create_pipe_and_fork(t_exe *exe, t_parser *cmds)
{
	if (cmds->next)
	{
		if (pipe(exe->pipefd) == -1)
		{
			perror("Erreur lors de la création du pipe");
			return (-1);
		}
	}
	exe->pid[exe->index_pid] = fork();
	if (exe->pid[exe->index_pid] == -1)
	{
		free(exe->pid);
		perror("Erreur du fork");
		return (-1);
	}
	return (0);
}

void	handle_child(t_exe *exe, t_parser *cmds, t_env **env, int *prev_pipefd)
{
	if (exe->pid[exe->index_pid] == 0)
	{
		exc_solo_cmd(exe, cmds, env);
		if (cmds->prev && cmds->prev->cmd)
		{
			if (*prev_pipefd != STDIN_FILENO)
			{
				if (dup2(*prev_pipefd, STDIN_FILENO) == -1)
					perror("Erreur dup2");
				close(*prev_pipefd);
			}
		}
		if (handle_redir(exe, cmds) == -1)
			exit(1);
		pipex(exe);
		exec_commande(exe, cmds, env);
		perror("Erreur d'exécution de la commande");
		exit(1);
	}
}

int	pipeline(t_exe *exe, t_parser *cmds, t_env **env)
{
	int	prev_pipefd;

	prev_pipefd = -1;
	if (!cmds || (!cmds->cmd && !cmds->redirections))
		return (0);
	handle_redirection_solo(exe, &cmds, &prev_pipefd);
	while (cmds && (cmds->cmd || cmds->redirections))
	{
		if (create_pipe_and_fork(exe, cmds) == -1)
			return (-1);
		handle_child(exe, cmds, env, &prev_pipefd);
		if (cmds->next)
		{
			close(exe->pipefd[1]);
			close(prev_pipefd);
			prev_pipefd = exe->pipefd[0];
		}
		exe->nmb_cmd -= 1;
		cmds = cmds->next;
		exe->index_pid++;
	}
	if (prev_pipefd != -1)
		close(prev_pipefd);
	return (handle_waitpid(exe));
}

int	executor(t_env **env, t_parser *cmds)
{
	t_exe	*exe;
	int		exit_code;

	exit_code = 0;
	exe = NULL;
	if (cmds->builtin != 0 && cmds->next == NULL
		&& cmds->prev == NULL && cmds->num_redirections == 0)
	{
		if (cmds->builtin(env, cmds) == 1)
			return (1);
		return (0);
	}
	exe = init_exe(*env, cmds, &exit_code);
	if (!exe)
		return (exit_code);
	exit_code = pipeline(exe, cmds, env);
	free_exe(exe);
	return (exit_code);
}
