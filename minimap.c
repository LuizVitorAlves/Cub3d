/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:51:58 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/27 09:18:27 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_pixel_to_img(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return ;
	dst = game->img.addr + (y * game->img.line_len + x
			* (game->img.bpp / 8));
	*(unsigned int *)dst = color;
}

void	draw_rect_transparent(t_game *game, t_rect rect)
{
	int	x;
	int	y;

	y = rect.y;
	while (y < rect.y + rect.height)
	{
		x = rect.x;
		while (x < rect.x + rect.width)
		{
			if ((x + y) % 2 == 0)
				put_pixel_to_img(game, x, y, rect.color);
			x++;
		}
		y++;
	}
}

void	draw_rect_solid(t_game *game, t_rect rect)
{
	int	x;
	int	y;

	y = rect.y;
	while (y < rect.y + rect.height)
	{
		x = rect.x;
		while (x < rect.x + rect.width)
		{
			put_pixel_to_img(game, x, y, rect.color);
			x++;
		}
		y++;
	}
}

void	draw_line(t_game *game, t_line line)
{
	float	x;
	float	y;
	int		i;

	if (abs(line.x1 - line.x0) > abs(line.y1 - line.y0))
		line.steps = abs(line.x1 - line.x0);
	else
		line.steps = abs(line.y1 - line.y0);
	line.x_inc = (line.x1 - line.x0) / (float)line.steps;
	line.y_inc = (line.y1 - line.y0) / (float)line.steps;
	x = line.x0;
	y = line.y0;
	i = 0;
	while (i <= line.steps)
	{
		put_pixel_to_img(game, (int)roundf(x), (int)roundf(y), line.color);
		x += line.x_inc;
		y += line.y_inc;
		i++;
	}
}

int	get_tile_color(char tile)
{
	if (tile == '1')
		return (C_WALL);
	if (tile == 'D')
		return (C_DOOR);
	if (tile == '0' || tile == 'N' || tile == 'S'
		|| tile == 'E' || tile == 'W' || tile == 'd')
		return (C_FLOOR);
	return (-1);
}
