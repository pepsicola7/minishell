/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_c.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 10:39:06 by tbartocc          #+#    #+#             */
/*   Updated: 2024/12/18 20:06:37 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strlen_c(char *str, char c)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	return (i);
}