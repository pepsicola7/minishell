/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excutor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:04 by peli              #+#    #+#             */
/*   Updated: 2024/11/24 18:11:23 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char	**trans_env(t_env	*env_lst)
{
	int			i;
	int			count;
	t_env		*temp;
	char		**env;
	int			len_name;
	int			len_value;

	i = 0;
	count = 0;
	temp = env_lst;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env = malloc((count + 1) * sizeof(char *)); // need to free;
	if (!env)
	{
		perror("Erreur d'allocation mémoire");
		return (NULL);
	}
	while (env_lst)
	{
		len_name = ft_strlen(env_lst->name);
		len_value = ft_strlen(env_lst->value);
		env[i] = malloc ((len_name + len_value + 2) * sizeof(char));//need a boucle to free;
		if (!env[i])
		{
			perror("Erreur d'allocation mémoire");
			while (i >= 0)
			{
				free(env[i]);
				i--;
			}
			return (NULL);
		}
		env[i] = ft_strcat(env_lst->name, "=");
		env[i] = ft_strcat(env[i] ,env_lst->value);
		env_lst = env_lst->next;
		i++;
	}	
	env[i] = "\0";
	return (env);
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

	// cmd_temps = NULL;
	if (!cmds)
	{
		perror("la liste de commandes est vide");
		return (NULL);
	}
	exe = malloc(sizeof(t_exe));
	if (!exe)
	{
		perror("Erreur d'allocation memoire pour exe");
		return (NULL);
	}
	cmd_temps = cmds;
	count = 0;
	while (cmd_temps && cmd_temps->cmd)
	{
		count++;
		cmd_temps = cmd_temps->next;
	}
	exe->nmb_cmd = count;
	exe->env =  trans_env(env);
	if (!exe->env)
	{
		free(exe);
		perror("Erreur lors de la conversion de l'environnement");
		return (NULL);
	}
	exe->pathname = get_pathname(env);//need to malloc;
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
	exe->hd_pipe[0] = -1;
	exe->hd_pipe[1] = -1;
	return(exe);
}

int	excutor(t_env *env, t_parser *cmds)
{
	t_exe	*exe;

	exe = malloc (sizeof (exe));
	if (!exe)
		return (-1);
	exe = init_exe(env, cmds);
	// if this is a bulltin solo; (&& cmds->next == NULL && cmds->prev == NULL)
	if (cmds->builtin != 0 && cmds->next == NULL && cmds->prev == NULL)
	{
		if (cmds->builtin(env, cmds) == -1)
		{
			printf ("Erreur lors de l'exécution du builtin\n");
			return (-1);
		}
		return (0);
	}
	if (pipeline(exe, cmds) == -1)
	{
		printf ("Erreur lors de l'exécution externe\n");
		return (-1);
	}
	return (0);
}
/*si j'ai besoin malloc pour pipefd : 
int	exec_externe(t_exe *exe, t_parser *cmds)
{
	int		*pipefd;
	t_parser	*cmd_temps;
	int		nbr_pipes;
	int		count;
	int		i;

	cmd_temps = cmds;
	count = 0;
	while (cmd_temps->cmd)
	{
		count++;
		cmd_temps = cmd_temps->next;
	}
	exe->nmb_cmd = count;
	pipefd = malloc(((count - 1) * 2) * sizeof(int));//need to free a la fon d'execution;
	if (!pipefd)
		return (-1);
	exe->pipefd = pipefd;
	i = 0;
	while (i < count - 1)
	{
		if (pipe(pipefd + i * 2) == -1) //je comprends pas la facon d'ecrire ici
		{
			perror ("Erreur lors de la création du pipe");
			free(pipefd);
			return (-1);
		}
		i++;
	}
	if (exec_redirection(exe, cmds) == -1)
	{
		free(pipefd);
		return (-1);
	}
	free (pipefd);
	return (0);
}*/
	// 			}
// ici faut gerer les redirection et le pipe
// minishell> echo "hello world" > output.txt
// Token: echo (Type: 0)
// Token: hello world (Type: 0)
// Token: > (Type: 5)
// Token: output.txt (Type: 0)
// Command:
//   Builtin: Yes
//   Cmds: [echo] [hello world] 
//   Redirections:
//     Type: 5, Value: output.txt
//   Number of redirections: 1