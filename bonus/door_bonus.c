/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:56:20 by lalves-d          #+#    #+#             */
/*   Updated: 2026/03/04 20:05:50 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	interact_door(t_game *game)
{
	t_door	dor;

	dor.target_x = game->player.pos_x + game->player.dir_x * 1.5;
	dor.target_y = game->player.pos_y + game->player.dir_y * 1.5;
	dor.map_x = (int)dor.target_x;
	dor.map_y = (int)dor.target_y;
	if (dor.map_y < 0 || dor.map_y >= game->cfg.map_height)
		return ;
	dor.map_width = (int)strlen(game->cfg.map[dor.map_y]);
	if (dor.map_x < 0 || dor.map_x >= dor.map_width)
		return ;
	dor.cell = &game->cfg.map[dor.map_y][dor.map_x];
	if (*dor.cell == 'D')
		*dor.cell = 'd';
	else if (*dor.cell == 'd')
	{
		dor.player_map_x = (int)game->player.pos_x;
		dor.player_map_y = (int)game->player.pos_y;
		if (dor.player_map_x != dor.map_x || dor.player_map_y != dor.map_y)
			*dor.cell = 'D';
	}
}
