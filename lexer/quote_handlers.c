/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:30:01 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/06 16:22:53 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

int	handle_double_quotes(int i, char *input, t_env *env, char **word)
{
	int		start;
	char	*temp;
	char	*expanded_text;

	start = i + 1;
	while (input[++i] && input[i] != '"')
		;
	if (input[i] == '"')
	{
		temp = ft_strndup(input + start, i - start);
		if (!temp)
			return (start);
		expanded_text = replace_env_variables(temp, env);
		free(temp);
		*word = concat(*word, expanded_text);
		free(expanded_text);
		return (i + 1);
	}
	else
		return (printf("minishell: syntax error: "
				"unclosed double quote\n"), free(*word), -1);
}

int	handle_single_quotes(int i, char *input, char **word)
{
	int		start;
	char	*temp;

	start = i + 1;
	while (input[++i] && input[i] != '\'')
		;
	if (input[i] == '\'')
	{
		temp = ft_strndup(input + start, i - start);
		if (!temp)
			return (start);
		*word = concat(*word, temp);
		free(temp);
		return (i + 1);
	}
	else
		return (printf("minishell: syntax error: "
				"unclosed single quote\n"), free(*word), -1);
}

// int	is_special_char(char c)
// {
// 	return (c == '|' || c == '<' || c == '>');
// }

int	handle_backslash(int i, char *input, char **word)
{
	char	*temp;
	int		start;
	
	start = i + 1;
	if (input[i] && input[i] == '\\')
	{
		i++;
		if (input[i] && !isspace(input[i]) && input[i] != '"'
			&& input[i] != '\'' && input[i] != '|' && input[i] != '<'
			&& input[i] != '>')
		{
			temp = ft_strndup(input + start, i - start);
			*word = concat(*word, temp);
			free(temp);
		}
		else
		{
			temp = ft_strndup(input + start + 2, i - start);
			*word = concat(*word, temp);
			free(temp);
		}
	}
	return (i);
}

int	handle_regular_text(int i, char *input, t_env *env, char **word)
{
	int		start;
	char	*temp;
	char	*expanded_text;

	start = i;
	while (input[i] && !isspace(input[i]) && input[i] != '"'
		&& input[i] != '\'' && input[i] != '|' && input[i] != '<'
		&& input[i] != '>')
		i++;
	temp = ft_strndup(input + start, i - start);
	if (!temp)
		return (start);
	expanded_text = replace_env_variables(temp, env);
	free(temp);
	*word = concat(*word, expanded_text);
	free(expanded_text);
	return (i);
}

int	handle_word(int i, char *input, t_lexer **tokens, t_env *env)
{
	char	*word;

	word = NULL;
	while (input[i] && !isspace(input[i]) && input[i] != '|'
		&& input[i] != '<' && input[i] != '>')
	{
		if (input[i] == '"')
			i = handle_double_quotes(i, input, env, &word);
		else if (input[i] == '\'')
			i = handle_single_quotes(i, input, &word);
		else if (input[i] == '\\')
			i = handle_backslash(i, input, &word);
		else
			i = handle_regular_text(i, input, env, &word);
		if (i == -1)
			return (-1);
	}
	if (word && ft_strcmp(word, ""))
	{
		add_token(tokens, create_token(WORD, word));
		free(word);
	}
	else
		free(word);
	return (i);
}
