/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_u.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 04:27:01 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 05:01:01 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putnbr_u(unsigned int num)
{
	int		count;
	char	prt;

	prt = '0';
	count = 0;
	if (num <= 9)
	{
		prt = num + 48;
		count += write (1, &prt, 1);
	}
	else
	{
		count += ft_putnbr_u(num / 10);
		count += ft_putnbr_u(num % 10);
	}
	return (count);
}
