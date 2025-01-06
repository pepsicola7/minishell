/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:54 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/06 18:56:47 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

int	g_signum;

void	update_exit_code_in_env(t_env *env)
{
	char	*str_num;

	str_num = ft_itoa(g_signum);
	add_node(env, ft_new_node("?", str_num), 1);
	free(str_num);
}

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

char	*get_user_input(const char *prompt)
{
	static int	prev_signum;
	int			stdin_dup;
	char		*line;

	prev_signum = g_signum;
	g_signum = 0;
	stdin_dup = dup(STDIN_FILENO);
	line = readline(prompt);
	dup2(stdin_dup, STDIN_FILENO);
	close(stdin_dup);
	if (!line && !g_signum)
		g_signum = EOF;
	else if (g_signum == SIGINT && !prev_signum)
		printf("\n");
	return (line);
}

int	main(int ac, char **av, char **initial_env)
{
	char		*input;
	t_lexer		*tokens;
	t_parser	*cmds;
	t_env		*env;

	g_signum = 0;
	setup_signals(0);
	(void)av;
	if (ac > 1)
		return (printf("No args needed\n"), 0);
	env = get_env(initial_env);
	add_node(env, ft_new_node("?", "0"), 1);
	while (g_signum != EOF)
	{
		input = get_user_input("minishell> ");
		if (input)
		{
			add_history(input);
			// print_env(env);
			tokens = lexer(input, &env);
			// print_lexers(tokens);
			cmds = parse_lexer(tokens);
			// ft_print_tab(initial_env);
			// print_parser(cmds);
			if (g_signum == SIGINT)
			{
				t_lexer *redirection = cmds->redirections;
				while (redirection)
				{
					if (redirection->type == HEREDOC)
						unlink(redirection->value);
					redirection = redirection->next;
				}
			}
			else
			{
				executor(env, cmds);
				update_exit_code_in_env(env);
			}
			free_cmds(cmds);
			free_tokens(tokens);
			free(input);
		}
	}
	free_env(env);
	return (g_signum);
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
