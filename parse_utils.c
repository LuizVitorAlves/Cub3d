/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 09:06:38 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 09:11:24 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	starts_with(const char *str, const char *prefix)
{
	int	i;

	i = 0;
	while (prefix[i])
	{
		if (str[i] != prefix[i])
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strdup(const char *s)
{
	char	*new;
	size_t	len;
	size_t	i;

	len = strlen(s);
	i = 0;
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

char	**copy_map(char **temp_map, int size)
{
	char	**map;
	int		i;

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

static void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_config_and_map(t_config *cfg)
{
	int	i;

	if (!cfg)
		return ;
	safe_free((void **)&cfg->no_path);
	safe_free((void **)&cfg->so_path);
	safe_free((void **)&cfg->we_path);
	safe_free((void **)&cfg->ea_path);
	safe_free((void **)&cfg->do_path);
	if (cfg->map)
	{
		i = 0;
		while (cfg->map[i])
		{
			safe_free((void **)&cfg->map[i]);
			i++;
		}
		free(cfg->map);
		cfg->map = NULL;
	}
}
