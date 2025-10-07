/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 10:56:20 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/07 12:09:40 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    interact_door(t_game *game)
{
    double target_x = game->player.pos_x + game->player.dir_x * 1.5;
    double target_y = game->player.pos_y + game->player.dir_y * 1.5;
    int map_x = (int)target_x;
    int map_y = (int)target_y;
    int map_width;

    if (map_y < 0 || map_y >= game->cfg.map_height)
        return;
    map_width = (int)strlen(game->cfg.map[map_y]);
    if (map_x < 0 || map_x >= map_width)
        return;
    char *cell = &game->cfg.map[map_y][map_x];
    if (*cell == 'D')
        *cell = 'd';
    else if (*cell == 'd')
    {
        int player_map_x = (int)game->player.pos_x;
        int player_map_y = (int)game->player.pos_y;
        if (player_map_x != map_x || player_map_y != map_y)
            *cell = 'D';
    }
}
