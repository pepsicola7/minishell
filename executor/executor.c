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

char	*ft_strcpy_(char *dest, const char *src)
{
	int	i;

	i = 0;
	if (!dest || !src)
		return (NULL);
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	**trans_env(t_env	*env_lst)
{
	char **res;
	t_env *tmp;
	int count;
	int i;

	count = 0;
	i = 0;
	tmp = env_lst;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	res = ft_calloc( (count + 1), sizeof(char *));
	if (!res)
		return (NULL);
	while (i < count)
	{
		if (!env_lst || !env_lst->name || !env_lst->value)
		{
			free (res);
			fprintf(stderr, "env_lst ou ses champs sont invalides\n");
			return (NULL);
		}
		if (ft_strcmp(env_lst->name, "?") != 0)
		{
			res[i] = ft_calloc(sizeof(char), sizeof(char)
				* (ft_strlen(env_lst->name) + ft_strlen(env_lst->value) + 2));
			if (!res[i])
			{
				perror("Erreur d'allocation mémoire");
				while (--i >= 0)
					free(res[i]);
				free(res);
				return (NULL);
			}
			ft_strcat(res[i], env_lst->name);
			ft_strcat(res[i], "=");
			ft_strcat(res[i], env_lst->value);
		}
		env_lst = env_lst->next;
		i++;
	}
	// res[i] = "\0";
	return (res);
}

char	*get_pathname(t_env *env_lst)
{
	while (env_lst)
	{
		if (ft_strcmp(env_lst->name, "PATH") == 0)
			return (env_lst->value);
		env_lst = env_lst->next;
	}
	return (NULL); //retourne NULL ou -1?
}

t_exe	*init_exe(t_env *env, t_parser *cmds)
{
	t_exe	*exe;
	t_parser	*cmd_temps;
	int		count;

	if (!cmds)
	{
		perror("la liste de commandes est vide");
		return (NULL);
	}
	exe = ft_calloc(1, sizeof(t_exe));
	if (!exe)
	{
		perror("Erreur d'allocation memoire pour exe");
		return (NULL);
	}
	cmd_temps = cmds;
	count = 0;
	while (cmd_temps)// 	while (cmd_temps && cmd_temps->cmd) je le change pour fixer : < test | echo hello
	{
		count++;
		cmd_temps = cmd_temps->next;
	}
	exe->nmb_cmd = count;
	exe->pid = ft_calloc(count + 1, sizeof(pid_t));
	if (!exe->pid)
	{
		free(exe);
		perror("Erreur d'allocation mémoire pour exe->pid");
		return (NULL);
	}
	exe->env =  trans_env(env);
	if (!exe->env)
	{
		free(exe);
		perror("Erreur lors de la conversion de l'environnement");
		return (NULL);
	}
	exe->pathname = get_pathname(env);//need to ft_calloc;
	if (!exe->pathname)
	{
		free(exe->env);
		free(exe);
		perror("Erreur lors de la récupération des chemins");
		return (NULL);
	}
	exe->fd[0] = STDIN_FILENO; // Input;
	exe->fd[1] = STDOUT_FILENO; // Output;
	exe->pipefd[0] = -1;
	exe->pipefd[1] = -1;
	exe->index_pid = 0;
	return(exe);
}

int	executor(t_env *env, t_parser *cmds)
{
	t_exe	*exe;

	exe = NULL;
	// print_parser(cmds);
	// print_env(env);
	// if this is a bulltin solo; (&& cmds->next == NULL && cmds->prev == NULL)
	if (cmds->builtin != 0 && cmds->next == NULL && cmds->prev == NULL && cmds->num_redirections == 0)
	{
		if (cmds->builtin(env, cmds) == -1)
		{
			printf ("Erreur lors de l'exécution du builtin\n");
			return (-1);
		}
		return (0);
	}
	exe = init_exe(env, cmds);
	g_signum = pipeline(exe, cmds);
	free_exe(exe);
	return (g_signum);
}
