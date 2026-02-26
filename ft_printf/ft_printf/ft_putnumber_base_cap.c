/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnumber_base_cap.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 11:08:45 by uviana-b          #+#    #+#             */
/*   Updated: 2024/11/02 11:08:47 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putnumber_base_cap(unsigned int num)
{
	int		count;
	char	*base;

	count = 0;
	base = "0123456789ABCDEF";
	if (num <= 15)
	{
		count += write (1, &base[num % 16], 1);
	}
	else if (num >= 16)
	{
		count += ft_putnumber_base_cap(num / 16);
		count += ft_putnumber_base_cap(num % 16);
	}
	return (count);
}
