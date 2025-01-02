/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excutor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:58:04 by peli              #+#    #+#             */
/*   Updated: 2025/01/02 14:05:18 by peli             ###   ########.fr       */
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
	while (i < count)
	{
		if (!env_lst || !env_lst->name || !env_lst->value)
		{
			fprintf(stderr, "env_lst ou ses champs sont invalides\n");
			return (NULL);
		}
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
	exe->pipefd[1] = -1; // garde ces deux lignes, sinon cat ne marche pas;
	// exe->pipefd = malloc(sizeof(int *) * exe->nmb_cmd);
	// int i = -1;
	// while (++i != exe->nmb_cmd)
	// 	*exe->pipefd = malloc(sizeof(int) * 2);
	exe->index_pid = 0;
	// exe->hd_pipe[0] = -1;
	// exe->hd_pipe[1] = -1;
	return(exe);
}

int	excutor(t_env *env, t_parser *cmds)
{
	t_exe	*exe;

	exe = NULL;
	print_parser(cmds);
	// print_env(env);
	exe = init_exe(env, cmds);
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
	if (pipeline(exe, cmds) == -1)
	{
		printf ("Erreur lors de l'exécution externe\n");
		return (-1);
	}
	return (0);
}
/*si j'ai besoin ft_calloc pour pipefd : 
int	exec_externe(t_exe *exe, t_parser *cmds)
{
	int		*pipefd;
	t_parser	*cmd_temps;
	int		nbr_pipes;
	int		count;
	int		i;
cat | cat | ls
	cmd_temps = cmds;
	count = 0;
	while (cmd_temps->cmd)
	{
		count++;
		cmd_temps = cmd_temps->next;
	}
	exe->nmb_cmd = count;
	pipefd = ft_calloc(((count - 1) * 2) * sizeof(int));//need to free a la fon d'execution;
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
	{print_env(env_lst);
		printf("name : %s, value : %s\n", env_lst->name, env_lst->value);
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
/*
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
	printf ("The count of env is : %d", count);
	fflush(stdout);
	env = ft_calloc((count + 1) * sizeof(char *)); // need to free;
	if (!env)
	{
		perror("Erreur d'allocation mémoire");
		return (NULL);
	}
	while (i < count)
	{
		print_env(env_lst);
		printf("name : %s, value : %s\n", env_lst->name, env_lst->value);
		
		len_name = ft_strlen(env_lst->name);
		len_value = ft_strlen(env_lst->value);
		env[i] = ft_calloc ((len_name + len_value + 2) * sizeof(char));//need a boucle to free;
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
		// if (!env_lst->next)
		// 	break ;
		env_lst = env_lst->next;
		i++;
		printf ("The index of env is : %d \n", i);
		fflush(stdout);
	}	
	env[i] = "\0";
	return (env);
}
*/