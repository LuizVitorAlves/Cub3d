/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cub_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 23:07:12 by uviana-b          #+#    #+#             */
/*   Updated: 2026/03/05 02:43:30 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// int	parse_config_line(char *line, t_config *cfg)
// {
// 	if (starts_with(line, "NO") || starts_with(line, "SO") || starts_with(line,
// 			"WE") || starts_with(line, "EA"))
// 		return (parse_texture_path(line, cfg));
// 	else if (starts_with(line, "F") || starts_with(line, "C"))
// 		return (parse_color_info(line, cfg));
// 	return (1);
// }

int	parse_config_line(char *line, t_config *cfg)
{
	if (starts_with(line, "NO") || starts_with(line, "SO")
		|| starts_with(line, "WE") || starts_with(line, "EA")
		|| starts_with(line, "DO"))
		return (parse_texture_path(line, cfg));
	else if (starts_with(line, "F") || starts_with(line, "C"))
		return (parse_color_info(line, cfg));
	return (1);
}

void	flood_fill(char **map, t_ff ff,
	int *is_valid)
{
	char	c;

	if (ff.x < 0 || ff.y < 0 || ff.y >= ff.max_y)
	{
		*is_valid = 0;
		return ;
	}
	if (ff.x >= (int)strlen(map[ff.y]) || map[ff.y][ff.x] == ' ')
	{
		*is_valid = 0;
		return ;
	}
	c = map[ff.y][ff.x];
	if (c == '1' || c == 'D' || c == 'V')
		return ;
	map[ff.y][ff.x] = 'V';
	flood_fill(map, ff, is_valid);
	flood_fill(map, ff, is_valid);
	flood_fill(map, ff, is_valid);
	flood_fill(map, ff, is_valid);
}

int	find_and_validate_player(t_config *cfg,
	int *player_x, int *player_y)
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

char	**copy_map_wrapper(char **map, int height)
{
	char	**map_copy;

	map_copy = copy_map(map, height);
	if (!map_copy)
		ft_printf(ERROR_MSG "Memory allocation error while validating map.\n");
	return (map_copy);
}

t_ff	init_ff(int player_x, int player_y, char **map_copy, int map_height)
{
	t_ff	ff;

	ff.x = player_x;
	ff.y = player_y;
	ff.max_x = (int)strlen(map_copy[player_y]);
	ff.max_y = map_height;
	return (ff);
}
