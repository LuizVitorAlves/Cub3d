/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 03:39:30 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 07:14:13 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int	ft_putstr(char *str)
{
	if (!str)
		return (write (1, "(null)", 6));
	else
		return (write (1, str, ft_strlen(str)));
	return (0);
}
