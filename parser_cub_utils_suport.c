/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cub_utils_suport.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:57:52 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 17:08:39 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"




static int	process_file_line(t_pfn pfl, t_config *cfg, char **temp_map)
{
	char			*trimmed;
	int				res;
	t_parser_state	state;

	trimmed = pfl.line;
	while (*trimmed == ' ' || *trimmed == '\n')
		trimmed++;
	if (*trimmed == '\0')
	{
		if (cfg->is_in_map_section)
			return (printf(ERROR_MSG "Linha vazia dentro do mapa.\n"), 1);
		return (0);
	}
	state.temp_map = temp_map;
	state.map_count = *pfl.map_count;
	state.config_count = *pfl.config_count;
	res = process_non_empty_line(trimmed, cfg, &state);
	*pfl.map_count = state.map_count;
	*pfl.config_count = state.config_count;
	return (res);
}

static int	read_file_lines(int fd, t_config *cfg, char **temp_map)
{
	t_pfn	n_pfl;
	int		map_count = 0;
	int		config_count = 0;

	n_pfl.map_count = &map_count;
	n_pfl.config_count = &config_count;
	n_pfl.line = get_next_line(fd);
	while (n_pfl.line)
	{
		if (process_file_line(n_pfl, cfg, temp_map))
		{
			free(n_pfl.line);
			close(fd);
			return (1);
		}
		free(n_pfl.line);
		n_pfl.line = get_next_line(fd);
	}
	return (0);
}

int	parse_cub_file(char *filename, t_config *cfg)
{
	int		fd;
	char	*temp_map[MAX_MAP_LINES];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf(ERROR_MSG "Não foi possível abrir o mapa.\n");
		return (1);
	}
	if (read_file_lines(fd, cfg, temp_map))
	{
		close(fd);
		return (1);
	}
	close(fd);
	// Aqui você pode chamar a função de validar mapa
	// if (build_and_validate_map(cfg, temp_map, counts)) return (1);
	return (0);
}