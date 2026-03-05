/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_suport_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:57:52 by uviana-b          #+#    #+#             */
/*   Updated: 2026/03/04 16:46:45 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	process_file_line(char *line, t_config *cfg, char **temp_map,
	int *counts)
{
	char			*trimmed;
	int				res;
	t_parser_state	state;

	trimmed = line;
	if (!cfg->is_in_map_section)
	{
		while (*trimmed == ' ' || *trimmed == '\n')
			trimmed++;
	}
	if (*trimmed == '\0' || *trimmed == '\n')
	{
		if (cfg->is_in_map_section)
			return (printf(ERROR_MSG "Linha vazia dentro do mapa.\n"), 1);
		return (0);
	}
	state.temp_map = temp_map;
	state.config_count = counts[0];
	state.map_count = counts[1];
	res = process_non_empty_line(trimmed, cfg, &state);
	counts[0] = state.config_count;
	counts[1] = state.map_count;
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
		if (process_file_line(line, cfg, temp_map, counts))
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
		printf(ERROR_MSG "not possible open this map.\n");
		return (1);
	}
	if (read_file_lines(fd, cfg, temp_map, counts))
		return (close_and_return(fd, 1));
	close(fd);
	if (build_and_validate_map(cfg, temp_map, counts))
		return (1);
	return (0);
}
