/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:05:21 by tbartocc          #+#    #+#             */
/*   Updated: 2024/11/26 15:18:44 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

t_lexer	*create_token(t_token type, char *value)
{
	t_lexer	*token;

	token = (t_lexer *)ft_calloc(1, sizeof(t_lexer));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = NULL;
	if (value)
		token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_lexer **head, t_lexer *new_token)
{
	t_lexer	*tmp;

	tmp = *head;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_token;
}
