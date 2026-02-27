/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 21:00:45 by uviana-b          #+#    #+#             */
/*   Updated: 2024/11/02 11:06:01 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"
#include <unistd.h>
#include <stdarg.h>

static int	ft_print_ptr(void *args)
{
	unsigned long int	cpy;

	cpy = 0;
	cpy = (unsigned long int) args;
	if (cpy != 0)
	{
		write (1, "0x", 2);
	}
	else
	{
		return (write(1, "(nil)", 5));
	}
	return (ft_putptr(cpy) + 2);
}

static int	ft_check_params(const char *str, int i, va_list args)
{
	if (str[i + 1] == 'c')
		return (ft_putchar(va_arg(args, int)));
	if (str[i + 1] == 's')
		return (ft_putstr(va_arg(args, char *)));
	if (str[i + 1] == 'd')
		return (ft_putnbr(va_arg(args, int)));
	if (str[i + 1] == '%')
		return (ft_putchar('%'));
	if (str[i + 1] == 'x')
		return (ft_putnumber_base(va_arg(args, unsigned int)));
	if (str[i + 1] == 'X')
		return (ft_putnumber_base_cap(va_arg(args, unsigned int)));
	if (str[i + 1] == 'p')
		return (ft_print_ptr(va_arg(args, void *)));
	if (str[i + 1] == 'i')
		return (ft_putnbr(va_arg(args, int)));
	if (str[i + 1] == 'u')
		return (ft_putnbr_u(va_arg(args, unsigned int)));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		i;
	int		count;

	count = 0;
	i = 0;
	va_start(args, str);
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '%')
		{
			count += ft_check_params(str, i, args);
			i++;
		}
		else
		{
			count += write (1, &str[i], 1);
		}
		i++;
	}
	va_end(args);
	return (count);
}
