/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 08:10:18 by uviana-b          #+#    #+#             */
/*   Updated: 2026/03/04 20:06:19 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	draw_map_tiles(t_game *game)
{
	int		x;
	int		y;
	int		color;
	t_rect	rect;

	y = 0;
	while (game->cfg.map[y])
	{
		x = 0;
		while (game->cfg.map[y][x])
		{
			rect.x = x * TILE_SIZE + MAP_OFFSET_X;
			rect.y = y * TILE_SIZE + MAP_OFFSET_Y;
			rect.width = TILE_SIZE;
			rect.height = TILE_SIZE;
			color = get_tile_color(game->cfg.map[y][x]);
			if (color != -1)
			{
				rect.color = color;
				draw_rect_transparent(game, rect);
			}
			x++;
		}
		y++;
	}
}

void	render_minimap(t_game *game)
{
	int		player_screen_x;
	int		player_screen_y;
	t_rect	rect;
	t_line	line;

	draw_map_tiles(game);
	player_screen_x = (int)(game->player.pos_x * TILE_SIZE) + MAP_OFFSET_X;
	player_screen_y = (int)(game->player.pos_y * TILE_SIZE) + MAP_OFFSET_Y;
	line.x0 = player_screen_x;
	line.y0 = player_screen_y;
	line.x1 = player_screen_x + (int)(game->player.dir_x * DIR_LINE_LEN);
	line.y1 = player_screen_y + (int)(game->player.dir_y * DIR_LINE_LEN);
	line.color = C_DIR;
	draw_line(game, line);
	rect.x = player_screen_x - (PLAYER_SIZE / 2);
	rect.y = player_screen_y - (PLAYER_SIZE / 2);
	rect.width = PLAYER_SIZE;
	rect.height = PLAYER_SIZE;
	rect.color = C_PLAYER;
	draw_rect_solid(game, rect);
}
