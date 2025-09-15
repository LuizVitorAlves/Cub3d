/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 08:30:32 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/15 09:35:28 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//Bira, da GIT na sua GNL pq to usando uma temporaria
//Bira, da GIT na sua Printf pq to usando a original

static int is_numeric(char *str)
{
    while (*str == ' ' || *str == '\t')
        str++;
    if (!*str)
        return (0);
    while (*str)
    {
        if (*str == '\n')
            break;
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}

char *skip_spaces(char *str)
{
    while (*str == ' ' || *str == '\t')
        str++;
    return (str);
}

static int validate_color_parts(char **parts)
{
    int r;
    int g;
    int b;
    
    if (!parts[0] || !parts[1] || !parts[2])
        return (1);    
    r = ft_atoi(parts[0]);
    g = ft_atoi(parts[1]);
    b = ft_atoi(parts[2]);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        printf(ERROR_MSG "Color out of range [0,255].\n");
        return (1);
    }
    if (!is_numeric(parts[0]) || !is_numeric(parts[1]) || !is_numeric(parts[2]))
    {
        printf(ERROR_MSG "Non-numeric or invalid color format.\n");
        return (1);
    }
    return (0);
}

static int	split_and_validate(char *tmp_line, char **parts)
{
	char	*part;
	int		i;

	i = 0;
	part = ft_strtok(tmp_line, ",");
	while (part && i < 3)
	{
		parts[i] = skip_spaces(part);
		i++;
		part = ft_strtok(NULL, ",");
	}
	if (i != 3 || part != NULL)
	{
		printf(ERROR_MSG "Invalid color format.\n");
		return (1);
	}
	return (validate_color_parts(parts));
}

int	parse_color(const char *line)
{
	char	*tmp_line;
	char	*parts[3];
	int		color;

	tmp_line = ft_strdup(line);
	if (!tmp_line)
		return (-1);
	if (split_and_validate(tmp_line, parts))
	{
		free(tmp_line);
		return (-1);
	}
	color = (ft_atoi(parts[0]) << 16)
		| (ft_atoi(parts[1]) << 8)
		| ft_atoi(parts[2]);
	free(tmp_line);
	return (color);
}
