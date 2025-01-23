/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:54 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/23 11:37:28 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src.h"

int	g_signum;

void	update_exit_code_in_env(t_env **env, int exit_code)
{
	char	*str_num;

	str_num = ft_itoa(exit_code);
	add_node(env, ft_new_node("?", str_num), 1);
	free(str_num);
}

char	*get_user_input(const char *prompt)
{
	static int	prev_signum;
	int			stdin_dup;
	char		*line;

	prev_signum = g_signum;
	g_signum = 0;
	stdin_dup = dup(STDIN_FILENO);
	signal(SIGINT, signal_handler);
	line = readline(prompt);
	signal(SIGINT, SIG_IGN);
	dup2(stdin_dup, STDIN_FILENO);
	close(stdin_dup);
	if (!line && !g_signum)
		g_signum = EOF;
	else if (g_signum == SIGINT && !prev_signum)
		printf("\n");
	return (line);
}

void	handle_redirections(t_parser *cmds)
{
	t_lexer	*redirection;

	if (g_signum == SIGINT)
	{
		redirection = cmds->redirections;
		while (redirection)
		{
			if (redirection->type == HEREDOC)
				unlink(redirection->value);
			redirection = redirection->next;
		}
	}
}

int	execute_commands(char *input, t_env **env, int *exit_code)
{
	t_lexer		*tokens;
	t_parser	*cmds;
	int			prev_exit_code;

	if (!input)
		return (1);
	add_history(input);
	tokens = lexer(input, env);
	cmds = parse_lexer(tokens, env);
	if (cmds)
	{
		handle_redirections(cmds);
		prev_exit_code = *exit_code;
		*exit_code = executor(env, cmds);
		if (*exit_code == 136)
			*exit_code = prev_exit_code;
		update_exit_code_in_env(env, *exit_code);
	}
	free_cmds(cmds);
	free_tokens(tokens);
	return (0);
}

int	main(int ac, char **av, char **initial_env)
{
	char	*input;
	t_env	*env;
	int		exit_code;

	g_signum = 0;
	exit_code = 0;
	setup_signals(0);
	(void)av;
	if (ac > 1)
		return (ft_fprintf(2, "No args needed\n"), 0);
	env = get_env(initial_env);
	add_node(&env, ft_new_node("?", "0"), 1);
	while (g_signum != EOF)
	{
		input = get_user_input("minishell> ");
		if (input)
		{
			execute_commands(input, &env, &exit_code);
			free(input);
		}
	}
	free_env(env);
	return (exit_code);
}
