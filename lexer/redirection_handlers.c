/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:08:08 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/06 17:46:35 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_redirection_in(int i, const char *input, t_lexer **tokens)
{
	if (input[i + 1] == '<')
	{
		add_token(tokens, create_token(HEREDOC, "<<"));
		return (i + 2);
	}
	add_token(tokens, create_token(REDIR_IN, "<"));
	return (i + 1);
}

int	handle_redirection_out(int i, const char *input, t_lexer **tokens)
{
	if (input[i + 1] == '>')
	{
		add_token(tokens, create_token(APPEND, ">>"));
		return (i + 2);
	}
	add_token(tokens, create_token(REDIR_OUT, ">"));
	return (i + 1);
}

int	handle_pipe_l(int i, t_lexer **tokens)
{
	add_token(tokens, create_token(PIPE, "|"));
	return (i + 1);
}
