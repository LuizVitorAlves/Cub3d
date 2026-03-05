/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cub_utils_suport.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:03:55 by uviana-b          #+#    #+#             */
/*   Updated: 2026/03/05 02:45:42 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_map_copy(char **map_copy)
{
	int	i;

	i = 0;
	while (map_copy[i])
		free(map_copy[i++]);
	free(map_copy);
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
			"WE") || starts_with(line, "EA") || starts_with(line, "DO"))
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
static int	process_config_transition(char *line,
	t_config *cfg, t_parser_state *state)
{
	if (state->config_count >= 7 && is_map_line(line))
	{
		cfg->is_in_map_section = 1;
		return (handle_map_line(line, state->temp_map, &state->map_count));
	}
	if (state->config_count < 7)
	{
		printf(ERROR_MSG "Configurações incompletas antes do mapa.\n");
		return (1);
	}
	printf(ERROR_MSG "Linha de configuração desconhecida.\n");
	return (1);
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
