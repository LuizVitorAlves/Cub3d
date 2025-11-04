/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:51:58 by lalves-d          #+#    #+#             */
/*   Updated: 2025/11/04 12:15:58 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_pixel_to_img(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return;
	dst = game->img.addr + (y * game->img.line_len + x \
		* (game->img.bpp / 8));
	*(unsigned int *)dst = color;
}

static void	draw_rect(t_game *game, int x_start, int y_start, int w, int h, int color)
{
	int	y;
	int	x;

	y = y_start;
	while (y < y_start + h)
	{
		x = x_start;
		while (x < x_start + w)
		{
			put_pixel_to_img(game, x, y, color);
			x++;
		}
		y++;
	}
}

static void	draw_line(t_game *game, int x0, int y0, int x1, int y1, int color)
{
	int		dx;
	int		dy;
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;
	int		i;

	dx = x1 - x0;
	dy = y1 - y0;
	if (abs(dx) > abs(dy))
		steps = abs(dx);
	else
		steps = abs(dy);
	x_inc = dx / (float)steps;
	y_inc = dy / (float)steps;
	x = x0;
	y = y0;
	i = 0;
	while (i <= steps)
	{
		put_pixel_to_img(game, (int)roundf(x), (int)roundf(y), color);
		x += x_inc;
		y += y_inc;
		i++;
	}
}

static void	draw_map_tiles(t_game *game)
{
	int	x;
	int	y;
	char	tile;

	y = 0;
	while (game->cfg.map[y])
	{
		x = 0;
		while (game->cfg.map[y][x])
		{
			tile = game->cfg.map[y][x];
			if (tile == '1')
				draw_rect(game, x * TILE_SIZE, y * TILE_SIZE, \
					TILE_SIZE, TILE_SIZE, C_WALL);
			else if (tile == 'D')
				draw_rect(game, x * TILE_SIZE, y * TILE_SIZE, \
					TILE_SIZE, TILE_SIZE, C_DOOR);
			else if (tile == '0' || tile == 'N' || tile == 'S' || \
					tile == 'E' || tile == 'W' || tile == 'd')
				draw_rect(game, x * TILE_SIZE, y * TILE_SIZE, \
					TILE_SIZE, TILE_SIZE, C_FLOOR);
			x++;
		}
		y++;
	}
}

void	render_minimap(t_game *game)
{
	int	player_screen_x;
	int	player_screen_y;
	int	line_end_x;
	int	line_end_y;

	draw_map_tiles(game);
	player_screen_x = (int)(game->player.pos_x * TILE_SIZE);
	player_screen_y = (int)(game->player.pos_y * TILE_SIZE);
	line_end_x = player_screen_x + (int)(game->player.dir_x * DIR_LINE_LEN);
	line_end_y = player_screen_y + (int)(game->player.dir_y * DIR_LINE_LEN);
	draw_line(game, player_screen_x, player_screen_y, line_end_x, line_end_y, C_DIR);
	draw_rect(game, player_screen_x - (PLAYER_SIZE / 2), \
				player_screen_y - (PLAYER_SIZE / 2), \
				PLAYER_SIZE, PLAYER_SIZE, C_PLAYER);
}
