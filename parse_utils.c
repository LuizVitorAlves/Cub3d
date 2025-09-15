/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_utils.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 14:57:55 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 15:59:42 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

int starts_with(const char *str, const char *prefix)
{
    int i = 0;
    while (prefix[i])
    {
        if (str[i] != prefix[i])
            return (0);
        i++;
    }
    return (1);
}

char *ft_strdup(const char *s)
{
    char    *new;
    size_t  len = strlen(s);
    size_t  i = 0;

    new = malloc(len + 1);
    if (!new)
        return (NULL);
    while (s[i])
    {
        new[i] = s[i];
        i++;
    }
    new[i] = '\0';
    return (new);
}

char **copy_map(char **temp_map, int size)
{
    char **map;
    int i;

    map = malloc(sizeof(char *) * (size + 1));
    if (!map)
        return (NULL);
    i = 0;
    while (i < size)
    {
        map[i] = ft_strdup(temp_map[i]);
        if (!map[i])
        {
            while (i-- > 0)
                free(map[i]);
            free(map);
            return (NULL);
        }
        i++;
    }
    map[i] = NULL;
    return (map);
}

static void safe_free(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void free_config_and_map(t_config *cfg)
{
    if (!cfg)
        return;
    safe_free((void **)&cfg->no_path);
    safe_free((void **)&cfg->so_path);
    safe_free((void **)&cfg->we_path);
    safe_free((void **)&cfg->ea_path);
    if (cfg->map)
    {
        int i = 0;
        while (cfg->map[i])
        {
            safe_free((void **)&cfg->map[i]);
            i++;
        }
        free(cfg->map);
        cfg->map = NULL;
    }
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	result;
	int		i;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
