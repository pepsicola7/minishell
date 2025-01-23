/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:46:37 by peli              #+#    #+#             */
/*   Updated: 2025/01/23 11:08:35 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_waitpid(t_exe *exe)
{
	int	status;
	int	signal_number;
	int	exit_code;
	int	j;

	j = 0;
	exit_code = 0;
	while (j < exe->index_pid)
	{
		while (waitpid(exe->pid[j], &status, 0) < 0)
			;
		j++;
	}
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
