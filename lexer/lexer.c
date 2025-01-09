/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:21 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/09 16:01:33 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

t_lexer	*lexer(char *input, t_env **env)
{
	t_lexer	*tokens;
	int		i;

	i = 0;
	tokens = NULL;
	while (i != -1 && input[i])
	{
		if (isspace(input[i]))
			i++;
		else if (input[i] == '|')
			i = handle_pipe_l(i, &tokens);
		else if (input[i] == '<')
			i = handle_redirection_in(i, input, &tokens);
		else if (input[i] == '>')
			i = handle_redirection_out(i, input, &tokens);
		else
			i = handle_word(i, input, &tokens, *env);
	}
	return (tokens);
}
