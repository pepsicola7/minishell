/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peiqi <peiqi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:54 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/23 23:53:56 by peiqi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

void	print_lexers(t_lexer *tokens)
{
	while (tokens)
	{
		printf("Token: %s (Type: %d)\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void	print_parser(t_parser *cmds)
{
	int		i;
	t_lexer	*redir;

	while (cmds)
	{
		printf("Command:\n");
		if (cmds->builtin)
			printf("  Builtin: Yes\n");
		else
			printf("  Builtin: No\n");
		printf("  Args: ");
		i = 0;
		while (cmds->cmd && cmds->cmd[i])
		{
			printf("[%s] ", cmds->cmd[i]);
			i++;
		}
		printf("\n");
		printf("  Redirections:\n");
		redir = cmds->redirections;
		while (redir)
		{
			printf("    Type: %d, Value: %s\n", redir->type, redir->value);
			redir = redir->next;
		}
		// if (cmds->hd_file_name)
		// 	printf("  Heredoc file: %s\n", cmds->hd_file_name);
		printf("  Number of redirections: %d\n", cmds->num_redirections);
		printf("\n");
		cmds = cmds->next;
	}
}

int	main(int ac, char **av, char **initial_env)
{
	char		*input;
	t_lexer		*tokens;
	t_parser	*cmds;
	t_env		*env;

	setup_signals();
	(void)av;
	if (ac > 1)
		return (printf("No args needed\n"), 0);
	env = get_env(initial_env);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		add_history(input);
		tokens = lexer(input, &env);
		// print_lexers(tokens);
		cmds = parse_lexer(tokens);
		print_parser(cmds);
		free_cmds(cmds);
		free_tokens(tokens);
		free(input);
	}
	free_env(env);
	return (0);
}
/*
	Add to main :
	
	cmds = parse_lexer(tokens);
	*Executor*
	free_cmds(cmds);

	Executor tmp :

	t_parser	*tmp;

	tmp = cmds;
	while (tmp)
	{
		if (tmp->builtin)
			tmp->builtin(env, tmp);
		else
			printf("execve\n");
		tmp = tmp->next;
	}
*/
