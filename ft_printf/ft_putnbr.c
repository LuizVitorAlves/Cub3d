/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:00:14 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 04:44:22 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putnbr(int n)
{
	char	prt;
	int		count;

	count = 0;
	if (n == -2147483648)
		return (write (1, "-2147483648", 11));
	if (n < 0)
	{
		count += write (1, "-", 1);
		n *= -1;
	}
	if (n <= 9)
	{
		prt = n + '0';
		count += write (1, &prt, 1);
	}
	else
	{
		count += ft_putnbr(n / 10);
		count += ft_putnbr(n % 10);
	}
	return (count);
}
