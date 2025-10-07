/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 08:27:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/07 12:12:55 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int  is_map_line(const char *line)
{
    int i;
    int has_map_char;

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
            return (-1);
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

static int	parse_texture_path(char *line, t_config *cfg)
{
	char	**target_path;
	char	*path_start;

	target_path = get_target_path(line, cfg);
	if (!target_path)
		return (1);
	path_start = line + 2;
	return (validate_and_assign_path(target_path, path_start));
}

static int	parse_color_info(char *line, t_config *cfg)
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

static int	parse_config_line(char *line, t_config *cfg)
{
	if (starts_with(line, "NO") || starts_with(line, "SO") || starts_with(line,
			"WE") || starts_with(line, "EA"))
		return (parse_texture_path(line, cfg));
	else if (starts_with(line, "F") || starts_with(line, "C"))
		return (parse_color_info(line, cfg));
	return (1);
}

static void flood_fill(char **map, int x, int y, int max_x, int max_y,
        int *is_valid)
{
    char    c;

    if (x < 0 || y < 0 || y >= max_y)
    {
        *is_valid = 0;
        return ;
    }
    if (x >= (int)strlen(map[y]) || map[y][x] == ' ')
    {
        *is_valid = 0;
        return ;
    }
    c = map[y][x];
    if (c == '1' || c == 'D' || c == 'V')
        return ;
    map[y][x] = 'V';
    flood_fill(map, x + 1, y, max_x, max_y, is_valid);
    flood_fill(map, x - 1, y, max_x, max_y, is_valid);
    flood_fill(map, x, y + 1, max_x, max_y, is_valid);
    flood_fill(map, x, y - 1, max_x, max_y, is_valid);
}

static int	find_and_validate_player(t_config *cfg, int *player_x, int *player_y)
{
	int	i;
	int	j;
	int	player_count;

	i = 0;
	j = 0;
	player_count = 0;
	while (cfg->map[i])
	{
		j = 0;
		while (cfg->map[i][j])
		{
			if (strchr("NSWE", cfg->map[i][j]))
			{
				player_count++;
				*player_x = j;
				*player_y = i;
			}
			j++;
		}
		i++;
	}
	return (player_count != 1);
}

int	validate_map(t_config *cfg)
{
	char	**map_copy;
	int		is_valid;
	int		i;

	int player_x, player_y;
	is_valid = 1;
	if (find_and_validate_player(cfg, &player_x, &player_y))
	{
		printf(ERROR_MSG "Invalid number of player positions (must be 1).\n");
		return (1);
	}
	map_copy = copy_map(cfg->map, cfg->map_height);
	if (!map_copy)
	{
		printf(ERROR_MSG "Memory allocation error while validating map.\n");
		return (1);
	}
	flood_fill(map_copy, player_x, player_y, strlen(map_copy[player_y]),
		cfg->map_height, &is_valid);
	i = 0;
	while (map_copy[i])
		free(map_copy[i++]);
	free(map_copy);
	if (!is_valid)
		printf(ERROR_MSG "Map is not closed.\n");
	return (!is_valid);
}

static int	handle_map_line(char *line, char **temp_map, int *count)
{
	int	map_check;

	map_check = is_map_line(line);
	if (map_check == -1)
	{
		printf(ERROR_MSG "Caracteres inválidos no mapa.\n");
		return (1);
	}
	temp_map[*count] = ft_strdup(line);
	if (!temp_map[*count])
	{
		printf(ERROR_MSG "Erro de alocação de memória.\n");
		return (1);
	}
	(*count)++;
	return (0);
}

static int	handle_config_line(char *line, t_config *cfg, int *count)
{
	if (starts_with(line, "NO") || starts_with(line, "SO") || starts_with(line,
			"WE") || starts_with(line, "EA"))
	{
		if (parse_config_line(line, cfg))
			return (1);
		(*count)++;
	}
	else if (starts_with(line, "F"))
	{
		cfg->floor_color = parse_color(skip_spaces(line + 1));
		if (cfg->floor_color == -1)
			return (1);
		(*count)++;
	}
	else if (starts_with(line, "C"))
	{
		cfg->ceiling_color = parse_color(skip_spaces(line + 1));
		if (cfg->ceiling_color == -1)
			return (1);
		(*count)++;
	}
	else
		return (2);
	return (0);
}

static int	process_non_empty_line(char *line, t_config *cfg, char **temp_map,
		int *map_count, int *config_count)
{
	int	res;

	if (cfg->is_in_map_section)
		return (handle_map_line(line, temp_map, map_count));
	res = handle_config_line(line, cfg, config_count);
	if (res == 1)
		return (1);
	if (res == 2)
	{
		if (*config_count >= 6 && is_map_line(line) == 1)
		{
			cfg->is_in_map_section = 1;
			return (handle_map_line(line, temp_map, map_count));
		}
		if (*config_count < 6)
			return (printf(ERROR_MSG "Configurações incompletas antes do mapa.\n"),
				1);
		return (printf(ERROR_MSG "Linha de configuração desconhecida.\n"), 1);
	}
	return (0);
}

static int	process_file_line(char *line, t_config *cfg, char **temp_map,
		int *map_count, int *config_count)
{
	char	*trimmed;
	int		res;

	trimmed = line;
	while (*trimmed == ' ' || *trimmed == '\n')
		trimmed++;
	if (*trimmed == '\0')
	{
		if (cfg->is_in_map_section)
			return (printf(ERROR_MSG "Linha vazia dentro do mapa.\n"), 1);
		return (0);
	}
	res = process_non_empty_line(trimmed, cfg, temp_map, map_count,
			config_count);
	return (res);
}

static int	close_and_return(int fd, int code)
{
	close(fd);
	return (code);
}

static int	read_file_lines(int fd, t_config *cfg, char **temp_map,
		int counts[2])
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (process_file_line(line, cfg, temp_map, &counts[1], &counts[0]))
		{
			free(line);
			return (1);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (0);
}

static int	build_and_validate_map(t_config *cfg, char **temp_map,
		int counts[2])
{
	int	i;

	if (counts[0] < 6 || counts[1] == 0)
	{
		printf(ERROR_MSG "Configurações ou mapa incompletos.\n");
		return (1);
	}
	cfg->map = copy_map(temp_map, counts[1]);
	cfg->map_height = counts[1];
	i = 0;
	while (i < counts[1])
	{
		free(temp_map[i]);
		i++;
	}
	if (validate_map(cfg))
	{
		free_config_and_map(cfg);
		return (1);
	}
	return (0);
}

int	parse_cub_file(char *filename, t_config *cfg)
{
	int		fd;
	char	*temp_map[MAX_MAP_LINES];
	int		counts[2];

	counts[0] = 0;
	counts[1] = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf(ERROR_MSG "Não foi possível abrir o arquivo do mapa.\n");
		return (1);
	}
	if (read_file_lines(fd, cfg, temp_map, counts))
		return (close_and_return(fd, 1));
	close(fd);
	if (build_and_validate_map(cfg, temp_map, counts))
		return (1);
	return (0);
}
