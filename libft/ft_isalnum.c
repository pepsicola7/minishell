/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 17:48:02 by tbartocc          #+#    #+#             */
/*   Updated: 2024/05/11 03:21:50 by tbartocc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if (c < 65 || c > 90)
		if (c < 97 || c > 122)
			if (c < 48 || c > 57)
				return (0);
	return (1);
}
