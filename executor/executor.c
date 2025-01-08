/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:04 by peli              #+#    #+#             */
/*   Updated: 2025/01/03 17:32:41 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	pipeline(t_exe *exe, t_parser *cmds)
{
	int	i;
	int prev_pipefd = -1;

	i = exe->index_pid;
	while (cmds && (cmds->cmd || cmds->redirections))
	{
		while (cmds && cmds->redirections && !cmds->cmd)
		{
			if (handle_redir_solo(exe, cmds) == -1)
				perror("Erreur d'exécution de la redirection");
			if (cmds->prev && cmds->prev->cmd) //not the first cmd, envoyer la sortie a l'entree
			{
				if (prev_pipefd != STDIN_FILENO)
					close(prev_pipefd);
			}
			cmds = cmds->next;
			exe->nmb_cmd -= 1;
		}
		if (cmds && cmds->cmd)
		{
			if (cmds->next)
			{
				if (pipe(exe->pipefd) == -1)
					perror("Erreur lors de la création du pipe");
			}
			exe->pid[i] = fork(); // sauvgarder le pid pour waitpit() a la fin;
			if (exe->pid[i] == -1)
			{
				free(exe->pid);
				perror("Erreur du fork");
				return (-1);
			}
			if (exe->pid[i] == 0) // processus enfant
			{
				exc_solo_cmd(exe, cmds);
				if (cmds->prev && cmds->prev->cmd) //not the first cmd, envoyer la sortie a l'entree
				{
					if (prev_pipefd != STDIN_FILENO)
					{
						if (dup2(prev_pipefd, STDIN_FILENO) == -1)
							perror("Erreur dup2");
						close(prev_pipefd);
					}
				}
				if (handle_redir(exe, cmds) == -1)
					perror("Erreur d'exécution de la redirection");
				// printf("le pipefd[0] : %d le pipefd[1] = %d\n", exe->pipefd[0], exe->pipefd[1]);
				pipex(exe);
				exec_commande(exe, cmds);
				perror("Erreur d'exécution de la commande");
				exit(1);
			}
			if (cmds && cmds->next) // not the last cmd;
			{
					close(exe->pipefd[1]);
					close(prev_pipefd);
					prev_pipefd = exe->pipefd[0];
			}
			exe->nmb_cmd -= 1;
			cmds = cmds->next;
			exe->index_pid++;
		}
	}
	if (prev_pipefd != -1) // not the first cmd;
		close(prev_pipefd);
	
	// WAIT
	int	status;
	int	signal_number;
	int	exit_code;

	int j = 0;
	while(j < exe->index_pid)
	{
		while (waitpid(exe->pid[j], &status, 0) < 0)
			;
		j++;
	}
	exit_code = 0;
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		signal_number = WTERMSIG(status);
		exit_code = 128 + signal_number;
		if (signal_number == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)", 2);
		ft_putstr_fd("\n", 2);
	}
	return (exit_code);
}

int	executor(t_env **env, t_parser *cmds)
{
	t_exe	*exe;
	int		exit_code;

	exe = NULL;
	print_parser(cmds);
	// print_env(env);
	// if this is a bulltin solo; (&& cmds->next == NULL && cmds->prev == NULL)
	if (cmds->builtin != 0 && cmds->next == NULL && cmds->prev == NULL && cmds->num_redirections == 0)
	{
		if ( cmds->builtin(env, cmds) == -1)
		{
			printf ("Erreur lors de l'exécution du builtin\n");
			return (-1);
		}
		return (0);
	}
	exe = init_exe(*env, cmds);
	exit_code = pipeline(exe, cmds);
	free_exe(exe);
	return (exit_code);
}
