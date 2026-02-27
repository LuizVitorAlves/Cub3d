/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 02:52:06 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 02:55:32 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
