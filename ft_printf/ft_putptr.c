/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 03:55:41 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 05:20:26 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putptr(unsigned long int num)
{
	int		count;
	char	*str;

	count = 0;
	str = "0123456789abcdef";
	if (num <= 15)
	{
		count += write(1, &str[num % 16], 1);
	}
	else
	{
		count += ft_putptr(num / 16);
		count += ft_putptr(num % 16);
	}
	return (count);
}
