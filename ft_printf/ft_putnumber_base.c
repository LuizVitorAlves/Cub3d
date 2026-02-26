/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnumber_base.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 21:02:16 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 03:22:25 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putnumber_base(unsigned int num)
{
	int		count;
	char	*base;

	count = 0;
	base = "0123456789abcdef";
	if (num <= 15)
	{
		count += write (1, &base[num % 16], 1);
	}
	else if (num >= 16)
	{
		count += ft_putnumber_base(num / 16);
		count += ft_putnumber_base(num % 16);
	}
	return (count);
}
