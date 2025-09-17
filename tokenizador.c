/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizador.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 08:44:15 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/17 08:39:00 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_delim(char c, const char *delim)
{
	const char	*d = delim;

	if (!d)
		return (0);
	while (*d)
	{
		if (c == *d)
			return (1);
		d++;
	}
	return (0);
}

static char	*init_ptr(char *str, const char *delim, char **save_ptr)
{
	if (!delim)
		return (NULL);
	if (str)
		return (str);
	return (*save_ptr);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*save_ptr = NULL;
	char		*ptr;
	char		*token_start;

	ptr = init_ptr(str, delim, &save_ptr);
	if (!ptr)
		return (NULL);
	while (*ptr && is_delim(*ptr, delim))
		ptr++;
	if (*ptr == '\0')
	{
		save_ptr = NULL;
		return (NULL);
	}
	token_start = ptr;
	while (*ptr && !is_delim(*ptr, delim))
		ptr++;
	if (*ptr != '\0')
	{
		*ptr = '\0';
		save_ptr = ptr + 1;
	}
	else
		save_ptr = NULL;
	return (token_start);
}
