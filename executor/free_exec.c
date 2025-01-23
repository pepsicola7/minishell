/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:46:37 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 09:58:17 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

// void	free_split(char **sp_path)
// {
// 	int	i;

// 	i = 0;
// 	while (sp_path[i])
// 		free(sp_path[i++]);
// 	free(sp_path);
// }

// int	wait_exec(t_exe *exe, int prev_pipefd)
// {
// 	int	status;
// 	int	signal_number;
// 	int	exit_code;
// 	int	j;

// 	j = 0;
// 	exit_code = 0;
// 	if (prev_pipefd != -1)
// 		close(prev_pipefd);
// 	while (j++ < exe->index_pid)
// 		while (waitpid(exe->pid[j], &status, 0) < 0)
// 			;
// 	if (WIFEXITED(status))
// 		exit_code = WEXITSTATUS(status);
// 	else if (WIFSIGNALED(status))
// 	{
// 		signal_number = WTERMSIG(status);
// 		exit_code = 128 + signal_number;
// 		if (signal_number == SIGQUIT)
// 			ft_putstr_fd("Quit (core dumped)", 2);
// 		ft_putstr_fd("\n", 2);
// 	}
// 	return (exit_code);
// }
