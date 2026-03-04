/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uviana-b <uviana-b@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 19:12:13 by uviana-b          #+#    #+#             */
/*   Updated: 2024/11/24 15:09:43 by uviana-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	reset_buffer(char *buffer, int max)
{
	int	i;
	int	n;
	int	s;

	i = 0;
	n = 0;
	s = ft_strlen(buffer);
	while (n < max)
	{
		while (i < s)
		{
			buffer[i] = buffer[i + 1];
			buffer[i + 1] = '\0';
			i++;
		}
		n++;
		i = 0;
	}
}

char	*ft_strchr(const char *s, int c)
{
	unsigned char	*str;
	unsigned char	chr;

	str = (unsigned char *)s;
	chr = (unsigned char)c;
	if (s == NULL)
		return (NULL);
	if (ft_strlen((char *)str) == 0)
		return (NULL);
	while (*str != chr && *str != 0)
		str++;
	if (*str == chr)
		return ((char *)str);
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*ret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	ret = (char *) malloc((ft_strlen(s1)
				+ ft_strlen(s2) + 1) * sizeof (char));
	if (!ret)
		return (NULL);
	while (s1 && s1[i])
		ret[j++] = s1[i++];
	i = 0;
	while (s2 && s2[i])
	{
		ret[j++] = s2[i++];
		if (s2[i - 1] == '\n')
			break ;
	}
	if (ret[0] != '\0')
		ret[j] = '\0';
	reset_buffer(s2, i);
	free(s1);
	return (ret);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}
