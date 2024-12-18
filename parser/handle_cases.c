/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cases.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:41:31 by tbartocc          #+#    #+#             */
/*   Updated: 2024/12/02 14:22:34 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

t_lexer	*handle_redirection(t_parser **current_cmd, t_lexer *tokens)
{
	if (tokens->next)
	{
		add_redirection(*current_cmd, tokens->type, tokens->next->value);
		tokens = tokens->next;
	}
	return (tokens->next);
}

int	handle_pipe_p(t_parser **parser, t_parser **cmd, t_lexer *tokens)
{
	if (tokens->next == NULL || tokens->next->type == PIPE)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		free_cmds(*parser);
		return (0);
	}
	add_cmd(parser, *cmd);
	*cmd = create_cmd();
	return (1);
}

void	handle_command(t_parser **current_cmd, t_lexer *tokens)
{
	if (!(*current_cmd)->cmd && is_builtin(tokens->value))
		(*current_cmd)->builtin = get_builtin_function(tokens->value);
	add_argument_to_cmd(*current_cmd, tokens->value);
}
