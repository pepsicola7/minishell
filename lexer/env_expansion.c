/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:45:21 by tbartocc          #+#    #+#             */
/*   Updated: 2025/01/04 18:16:31 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src.h"

void	print_env(t_env *env)
{
	t_env *tmp;

	tmp = env;
	printf("%s=%s\n", tmp->name, tmp->value);
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
		printf("%s=%s\n", tmp->name, tmp->value);
	}
}

char	*ft_getenv(char *var_name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, var_name) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*get_env_value(char *var_name, t_env *env)
{
	char	*value;

	value = ft_getenv(var_name, env);
	if (value == NULL)
		value = "";
	return (value);
}

void	extract_var_name(const char *input, int *i, char *var_name)
{
	// int	start;
	int	j;

	// start = *i;
	j = 0;
	while (input[*i] && (ft_isalnum(input[*i])
		|| input[*i] == '_' || input[*i] == '?'))
	{
		if (j < 127)
			var_name[j++] = input[(*i)++];
		else
			break ;
	}
	var_name[j] = '\0';
}

char	*concat(char *expanded_text, char *new_part)
{
	char	*new_text;

	if (expanded_text == NULL)
		return (ft_strdup(new_part));
	new_text = ft_strjoin(expanded_text, new_part);
	free(expanded_text);
	return (new_text);
}

char	*replace_env_variables(const char *input, t_env *env)
{
	char	*expanded_text;
	char	var_name[128];
	char	tmp_str[2];
	int		i;

	i = 0;
	expanded_text = NULL;
	while (input[i])
	{
		if (input[i] == '$' && (ft_isalnum(input[i + 1])
			|| input[i + 1] == '_' || input[i + 1] == '?'))
		{
			i++;
			extract_var_name(input, &i, var_name);
			expanded_text = concat(expanded_text, get_env_value(var_name, env));
		}
		else
		{
			tmp_str[0] = input[i++];
			tmp_str[1] = '\0';
			expanded_text = concat(expanded_text, tmp_str);
		}
	}
	return (expanded_text);
}
