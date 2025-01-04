/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 18:35:59 by peli              #+#    #+#             */
/*   Updated: 2025/01/04 18:44:56 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

// t_exe	*init_exe(t_env *env, t_parser *cmds)
// {
// 	t_exe	*exe;
// 	t_parser	*cmd_temps;
// 	int		count;

// 	if (!cmds)
// 	{
// 		perror("la liste de commandes est vide");
// 		return (NULL);
// 	}
// 	exe = ft_calloc(1, sizeof(t_exe));
// 	if (!exe)
// 	{
// 		perror("Erreur d'allocation memoire pour exe");
// 		return (NULL);
// 	}
// 	cmd_temps = cmds;
// 	count = 0;
// 	while (cmd_temps)// 	while (cmd_temps && cmd_temps->cmd) je le change pour fixer : < test | echo hello
// 	{
// 		count++;
// 		cmd_temps = cmd_temps->next;
// 	}
// 	exe->nmb_cmd = count;
// 	exe->pid = ft_calloc(count + 1, sizeof(pid_t));
// 	if (!exe->pid)
// 	{
// 		free(exe);
// 		perror("Erreur d'allocation mémoire pour exe->pid");
// 		return (NULL);
// 	}
// 	exe->env =  trans_env(env);
// 	if (!exe->env)
// 	{
// 		free(exe);
// 		perror("Erreur lors de la conversion de l'environnement");
// 		return (NULL);
// 	}
// 	exe->pathname = get_pathname(env);//need to ft_calloc;
// 	if (!exe->pathname)
// 	{
// 		free(exe->env);
// 		free(exe);
// 		perror("Erreur lors de la récupération des chemins");
// 		return (NULL);
// 	}
// 	exe->fd[0] = STDIN_FILENO; // Input;
// 	exe->fd[1] = STDOUT_FILENO; // Output;
// 	exe->pipefd[0] = -1;
// 	exe->pipefd[1] = -1;
// 	exe->index_pid = 0;
// 	return(exe);
// }
