/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fonction.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peli <peli@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 20:48:11 by peli              #+#    #+#             */
/*   Updated: 2025/01/04 20:04:23 by peli             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar_(int fd, char c)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putnbr_(int fd, int i)
{
	int	count;

	count = 0;
	if (i == -2147483648)
		return (ft_putstr_(fd, "-2147483648"));
	if (i < 0)
	{
		count += write(fd, "-", 1);
		i = -i;
	}
	if (i > 9)
		count += ft_putnbr_(fd, i / 10);
	count += ft_putchar_(fd, i % 10 + '0');
	return (count);
}

int	ft_putnbr_u(int fd, unsigned int i)
{
	unsigned int	count;

	count = 0;
	if (i < 0)
	{
		write (fd, "-", 1);
		i = -i;
	}
	if (i > 9)
		count += ft_putnbr_u(fd, i / 10);
	count += ft_putchar_(fd, i % 10 + '0');
	return (count);
}

int	ft_putnbr_hexa(int fd, unsigned long long n, char *base)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_putnbr_hexa(fd, n / 16, base);
	count += ft_putchar_(fd, base[n % 16]);
	return (count);
}
