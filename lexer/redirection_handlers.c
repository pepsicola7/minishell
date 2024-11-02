/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:08:08 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/01 18:02:15 by tbartocc         ###   ########.fr       */
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

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	handle_word(int i, const char *input, t_lexer **tokens/*, t_env **env*/)
{
	int		start;
	char	*word;

	start = i;
	while (input[i] && !isspace(input[i]) && !is_special_char(input[i]))
		i++;
	word = strndup(input + start, i - start);
	if (!word || !ft_strcmp(word, ""))
		return (start);
	// word = replace_env_variables(word, *env);
	add_token(tokens, create_token(WORD, word));
	free(word);
	return (i);
}

int	handle_pipe(int i, t_lexer **tokens)
{
	add_token(tokens, create_token(PIPE, "|"));
	return (i + 1);
}
