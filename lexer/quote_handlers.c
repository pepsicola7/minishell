/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:30:01 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/01 18:01:54 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_single_quotes(int i, const char *input, t_lexer **tokens)
{
	int		start;
	char	*quoted_text;

	start = i + 1;
	quoted_text = NULL;
	while (input[++i] && input[i] != '\'')
		;
	if (input[i] == '\'')
	{
		quoted_text = strndup(input + start, i - start);
		add_token(tokens, create_token(SINGLE, quoted_text));
		free(quoted_text);
		return (i + 1);
	}
	return (start);
}

int	handle_double_quotes(int i, const char *input, t_lexer **tokens, t_env **env)
{
	int		start;
	char	*quoted_text;

	start = i + 1;
	quoted_text = NULL;
	while (input[++i] && input[i] != '"')
		;
	if (input[i] == '"')
	{
		quoted_text = strndup(input + start, i - start);
		if (!quoted_text || !ft_strcmp(quoted_text, ""))
			return (start);
		quoted_text = replace_env_variables(quoted_text, *env);
		add_token(tokens, create_token(DOUBLE, quoted_text));
		free(quoted_text);
		return (i + 1);
	}
	free(quoted_text);
	return (start);
}
