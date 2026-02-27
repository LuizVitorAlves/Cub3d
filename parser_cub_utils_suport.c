/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cub_utils_suport.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:03:55 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 02:33:37 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	free_map_copy(char **map_copy)
{
	int	i;

	i = 0;
	while (map_copy[i])
		free(map_copy[i++]);
	free(map_copy);
}

int	validate_map(t_config *cfg)
{
	
	char	**map_copy;
	int		is_valid;
	int		player_x;
	int		player_y;
	t_ff	ff;

	is_valid = 1;
	if (find_and_validate_player(cfg, &player_x, &player_y))
	{
		printf(ERROR_MSG "Invalid number of player positions (must be 1).\n");
		return (1);
	}
	map_copy = copy_map_wrapper(cfg->map, cfg->map_height);
	if (!map_copy)
		return (1);
	ff = init_ff(player_x, player_y, map_copy, cfg->map_height);
	flood_fill(map_copy, ff, &is_valid);
	free_map_copy(map_copy);
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

/* Helper para lidar com linhas que iniciam a seção do mapa */
static int	process_config_transition(char *line, t_config *cfg, t_parser_state *state)
{
	if (state->config_count >= 6 && is_map_line(line))
	{
		cfg->is_in_map_section = 1;
		return handle_map_line(line, state->temp_map, &state->map_count);
	}
	if (state->config_count < 6)
	{
		printf(ERROR_MSG "Configurações incompletas antes do mapa.\n");
		return 1;
	}
	printf(ERROR_MSG "Linha de configuração desconhecida.\n");
	return 1;
}

//sequancia de funçoes derivadas da process_non_empty_line
int	process_non_empty_line(char *line,
	t_config *cfg, t_parser_state *state)
{
	int	res;

	if (cfg->is_in_map_section)
		return (handle_map_line(line, state->temp_map, &state->map_count));
	res = handle_config_line(line, cfg, &state->config_count);
	if (res == 1)
		return (1);
	if (res == 2)
		return (process_config_transition(line, cfg, state));
	return (0);
}



