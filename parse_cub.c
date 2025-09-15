/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 08:27:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/15 17:03:09 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int is_map_line(const char *line)
{
    int i = 0;
    int has_map_char = 0;

    while (line[i] == ' ')
        i++;
    if (line[i] == '\0' || line[i] == '\n')
        return (0);
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != '1' && line[i] != '0' && line[i] != ' ' &&
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
            return (-1);
        if (line[i] == '1' || line[i] == '0' || line[i] == 'N' || 
            line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
            has_map_char = 1;
        i++;
    }
    return (has_map_char);
}

static char **get_target_path(char *line, t_config *cfg)
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

static int validate_and_assign_path(char **target_path, char *path_start)
{
    int len;
    int fd;

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

static int parse_texture_path(char *line, t_config *cfg)
{
    char **target_path;
    char *path_start;

    target_path = get_target_path(line, cfg);
    if (!target_path)
        return (1);
    path_start = line + 2;
    return (validate_and_assign_path(target_path, path_start));
}

static int parse_color_info(char *line, t_config *cfg)
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

static int parse_config_line(char *line, t_config *cfg)
{
    if (starts_with(line, "NO") || starts_with(line, "SO")
        || starts_with(line, "WE") || starts_with(line, "EA"))
        return (parse_texture_path(line, cfg));
    else if (starts_with(line, "F") || starts_with(line, "C"))
        return (parse_color_info(line, cfg));
    return (1);
}

static void flood_fill(char **map, int x, int y, int max_x, int max_y, int *is_valid)
{
    if (x < 0 || y < 0 || y >= max_y) { *is_valid = 0; return; }
    if (x >= (int)strlen(map[y]) || map[y][x] == ' ') { *is_valid = 0; return; }
    char c = map[y][x];
    if (c == '1' || c == 'V') return;
    map[y][x] = 'V';
    flood_fill(map, x + 1, y, max_x, max_y, is_valid);
    flood_fill(map, x - 1, y, max_x, max_y, is_valid);
    flood_fill(map, x, y + 1, max_x, max_y, is_valid);
    flood_fill(map, x, y - 1, max_x, max_y, is_valid);
}

static int find_and_validate_player(t_config *cfg, int *player_x, int *player_y)
{
    int i = 0, j, player_count = 0;

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

int validate_map(t_config *cfg)
{
    int player_x, player_y;
    char **map_copy;
    int is_valid = 1;
    int i;

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
    flood_fill(map_copy, player_x, player_y, strlen(map_copy[player_y]), cfg->map_height, &is_valid);
    i = 0;
    while (map_copy[i])
        free(map_copy[i++]);
    free(map_copy);
    if (!is_valid)
        printf(ERROR_MSG "Map is not closed.\n");
    return (!is_valid);
}

int	parse_cub_file(char *filename, t_config *cfg)
{
	int		fd;
	char	*line;
	char	*temp_map[MAX_MAP_LINES];
	int		map_line_count;
	int		config_count;
	char	*trimmed;

	map_line_count = 0;
	config_count = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (printf(ERROR_MSG "Não foi possível abrir o arquivo do mapa.\n"), 1);
	while ((line = get_next_line(fd)) != NULL)
	{
		trimmed = line;
		while (*trimmed == ' ' || *trimmed == '\n')
			trimmed++;
		if (*trimmed == '\0')
		{
			if (cfg->is_in_map_section)
			{
				printf(ERROR_MSG "Linha vazia dentro do mapa.\n");
				free(line);
				close(fd);
				return (1);
			}
			free(line);
			continue;
		}
		if (cfg->is_in_map_section)
		{
			int map_check = is_map_line(trimmed);
			if (map_check == -1)
			{
				printf(ERROR_MSG "Caracteres inválidos no mapa.\n");
				free(line);
				close(fd);
				return (1);
			}
			temp_map[map_line_count] = ft_strdup(line);
			if (!temp_map[map_line_count])
			{
				printf(ERROR_MSG "Erro de alocação de memória.\n");
				free(line);
				close(fd);
				return (1);
			}
			map_line_count++;
		}
		else if (starts_with(trimmed, "NO"))
		{
			if (parse_config_line(trimmed, cfg))
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else if (starts_with(trimmed, "SO"))
		{
			if (parse_config_line(trimmed, cfg))
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else if (starts_with(trimmed, "WE"))
		{
			if (parse_config_line(trimmed, cfg))
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else if (starts_with(trimmed, "EA"))
		{
			if (parse_config_line(trimmed, cfg))
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else if (starts_with(trimmed, "F"))
		{
			cfg->floor_color = parse_color(skip_spaces(trimmed + 1));
			if (cfg->floor_color == -1)
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else if (starts_with(trimmed, "C"))
		{
			cfg->ceiling_color = parse_color(skip_spaces(trimmed + 1));
			if (cfg->ceiling_color == -1)
			{
				free(line);
				close(fd);
				return (1);
			}
			config_count++;
		}
		else
		{
			if (config_count >= 6)
			{
				int map_check = is_map_line(trimmed);
				if (map_check == 1)
				{
					cfg->is_in_map_section = 1;
					temp_map[map_line_count] = ft_strdup(line);
					if (!temp_map[map_line_count])
					{
						printf(ERROR_MSG "Erro de alocação de memória.\n");
						free(line);
						close(fd);
						return (1);
					}
					map_line_count++;
				}
				else
				{
					printf(ERROR_MSG "Linha de configuração desconhecida.\n");
					free(line);
					close(fd);
					return (1);
				}
			}
			else
			{
				printf(ERROR_MSG "Configurações incompletas antes do mapa.\n");
				free(line);
				close(fd);
				return (1);
			}
		}
		free(line);
	}
	close(fd);
	if (config_count < 6 || map_line_count == 0)
	{
		printf(ERROR_MSG "Configurações ou mapa incompletos.\n");
		return (1);
	}
	cfg->map = copy_map(temp_map, map_line_count);
	cfg->map_height = map_line_count;
	for (int i = 0; i < map_line_count; i++)
		free(temp_map[i]);
	if (validate_map(cfg))
	{
		free_config_and_map(cfg);
		return (1);
	}
	return (0);
}
