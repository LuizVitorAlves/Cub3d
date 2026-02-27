/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 08:27:19 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/26 23:39:32 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_map_line(const char *line)
{
	int	i;
	int	has_map_char;

	i = 0;
	has_map_char = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\0' || line[i] == '\n')
		return (0);
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != '1' && line[i] != '0' && line[i] != ' ' && line[i] != 'N'
			&& line[i] != 'S' && line[i] != 'E' && line[i] != 'W'
			&& line[i] != 'D' && line[i] != 'd')
		{
			return (-1);
		}
		if (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W'
			|| line[i] == 'D' || line[i] == 'd')
			has_map_char = 1;
		i++;
	}
	return (has_map_char);
}

static char	**get_target_path(char *line, t_config *cfg)
{
	if (starts_with(line, "NO "))
		return (&cfg->no_path);
	else if (starts_with(line, "SO "))
		return (&cfg->so_path);
	else if (starts_with(line, "WE "))
		return (&cfg->we_path);
	else if (starts_with(line, "EA "))
		return (&cfg->ea_path);
	return (NULL);
}

static int	validate_and_assign_path(char **target_path, char *path_start)
{
	int	len;
	int	fd;

	if (*target_path != NULL)
	{
		printf(ERROR_MSG "Duplicate texture configuration.\n");
		return (1);
	}
	while (*path_start == ' ')
		path_start++;
	*target_path = ft_strdup(path_start);
	len = strlen(*target_path);
	if (len > 0 && (*target_path)[len - 1] == '\n')
		(*target_path)[len - 1] = '\0';
	fd = open(*target_path, O_RDONLY);
	if (fd < 0)
	{
		printf(ERROR_MSG "Invalid texture path: %s\n", *target_path);
		return (1);
	}
	close(fd);
	return (0);
}

int	parse_texture_path(char *line, t_config *cfg)
{
	char	**target_path;
	char	*path_start;

	target_path = get_target_path(line, cfg);
	if (!target_path)
		return (1);
	path_start = line + 2;
	return (validate_and_assign_path(target_path, path_start));
}

int	parse_color_info(char *line, t_config *cfg)
{
	if (starts_with(line, "F "))
	{
		cfg->floor_color = parse_color(skip_spaces(line + 1));
		return (cfg->floor_color == -1);
	}
	else if (starts_with(line, "C "))
	{
		cfg->ceiling_color = parse_color(skip_spaces(line + 1));
		return (cfg->ceiling_color == -1);
	}
	return (1);
}
