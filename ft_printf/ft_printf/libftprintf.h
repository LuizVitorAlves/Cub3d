/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 03:58:36 by uviana-b          #+#    #+#             */
/*   Updated: 2024/10/29 07:19:17 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include <unistd.h>
# include <stdarg.h>

size_t	ft_strlen(const char *str);
int		ft_printf(const char *str, ...);
int		ft_putnbr(int n);
int		ft_putnumber_base(unsigned int num);
int		ft_putnumber_base_cap(unsigned int num);
int		ft_count_args(const char *str);
int		ft_putchar(char c);
int		ft_putstr(char *str);
int		ft_putptr(unsigned long int num);
int		ft_putnbr_u(unsigned int num);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
#endif
