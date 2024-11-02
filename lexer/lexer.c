/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:21 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/01 18:01:23 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

t_lexer	*lexer(char *input, t_env **env)
{
	t_lexer	*tokens;
	int		i;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (isspace(input[i]))
			i++;
		else if (input[i] == '\'')
			i = handle_single_quotes(i, input, &tokens);
		else if (input[i] == '"')
			i = handle_double_quotes(i, input, &tokens, env);
		else if (input[i] == '|')
			i = handle_pipe(i, &tokens);
		else if (input[i] == '<')
			i = handle_redirection_in(i, input, &tokens);
		else if (input[i] == '>')
			i = handle_redirection_out(i, input, &tokens);
		else
			i = handle_word(i, input, &tokens/*, env*/);
	}
	return (tokens);
}
