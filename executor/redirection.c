/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:09:48 by peli              #+#    #+#             */
/*   Updated: 2025/01/03 18:54:00 by tbartocc         ###   ########.fr       */
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
				printf("minishell: ");
				fflush(stdout);
				perror(cmds->redirections->value);
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

	setup_signals(0);
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
			ft_putstr_fd("Quit (core dumped)", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	return (exit_code);
}

