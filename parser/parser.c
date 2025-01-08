/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:22:06 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/08 16:13:26 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

t_parser	*create_cmd(void)
{
	t_parser	*cmd;

	cmd = ft_calloc(1, sizeof(t_parser));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->builtin = NULL;
	cmd->num_redirections = 0;
	// cmd->hd_file_name = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void	add_cmd(t_parser **head, t_parser *new_cmd)
{
	t_parser	*tmp;

	if (!*head)
		*head = new_cmd;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_cmd;
		new_cmd->prev = tmp;
	}
}

void	add_redirection(t_parser *cmd, int redir_type, char *redir_value)
{
	t_lexer	*new_redir;
	t_lexer	*tmp;
	char	*hd_name;

	if (redir_type == HEREDOC)
	{
		hd_name = create_heredoc(redir_value);
		if (hd_name)
			new_redir = create_token(redir_type, hd_name);
		free(hd_name);
	}
	else
		new_redir = create_token(redir_type, redir_value);
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		tmp = cmd->redirections;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_redir;
	}
	cmd->num_redirections++;
}

void	add_argument_to_cmd(t_parser *cmd, char *arg)
{
	int		arg_count;
	int		i;
	char	**new_args;

	arg_count = 0;
	while (cmd->cmd && cmd->cmd[arg_count])
		arg_count++;
	new_args = ft_calloc((arg_count + 2), sizeof(char *));
	if (!new_args)
		return ;
	i = 0;
	while (i < arg_count)
	{
		new_args[i] = cmd->cmd[i];
		i++;
	}
	new_args[arg_count] = ft_strdup(arg);
	new_args[arg_count + 1] = NULL;
	if (cmd->cmd)
		free(cmd->cmd);
	cmd->cmd = new_args;
}

t_parser	*parse_lexer(t_lexer *tokens)
{
	t_parser	*parser;
	t_parser	*current_cmd;

	parser = NULL;
	current_cmd = create_cmd();
	while (tokens)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == HEREDOC || tokens->type == APPEND)
			tokens = handle_redirection(&current_cmd, tokens);
		else if (tokens->type == PIPE)
		{
			if (!handle_pipe_p(&parser, &current_cmd, tokens))
				return (NULL);
			tokens = tokens->next;
		}
		else
		{
			handle_command(&current_cmd, tokens);
			tokens = tokens->next;
		}
	}
	if (current_cmd)
		add_cmd(&parser, current_cmd);
	return (parser);
}
