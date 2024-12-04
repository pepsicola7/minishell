/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exc_utile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:19:59 by peli              #+#    #+#             */
/*   Updated: 2024/12/04 17:37:36 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

char	*find_path(char *pathname, char *cmd)
{
	char	**sp_path;
	char	*path;
	int		i;

	i = 0;
	sp_path = ft_split(pathname, ':');
	while (sp_path[i])
	{
		path = ft_strjoin(sp_path[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, X_OK) == 0)
			break;
		free(path);
		i++;
	}
	// while (i > 0)
	// {
	// free(sp_path[i]);
	// 	i--;
	// }
	// free(sp_path); // check free;
	return (path);
}

int	exec_commande(t_exe *exe, t_parser *cmds)
{
	char	*exc_pathname;

	// if (!exc_pathname)
	// {
	// 	fprintf(stderr, "Commande introuvable: %s\n", cmds->cmd[0]);
	// 	exit(EXIT_FAILURE);
	// }
	// printf("Chemin trouvé: %s\n", exc_pathname);
	// if (!exc_pathname)
	// {
	// 	fprintf(stderr, "Commande introuvable: %s\n", cmds->cmd[0]);
	// 	exit(EXIT_FAILURE);
	// }
	// // ajouter la condition pour check the pathname;
	// for (int i = 0; cmds->cmd[i]; i++)
	// 	printf("cmd[%d]: %s\n", i, cmds->cmd[i]);
	// for (int i = 0; exe->env[i]; i++)
	// 	printf("env[%d]: %s\n", i, exe->env[i]);
	if (cmds->cmd)
	{
		exc_pathname = find_path(exe->pathname, cmds->cmd[0]); // check cmd[0] faut parcourir dans la commande?
		if (execve(exc_pathname, cmds->cmd, exe->env) == -1)
		{
			perror("Erreur d'exécution de la commande");
			exit(EXIT_FAILURE);
		}
	}
	exit(EXIT_SUCCESS);
}