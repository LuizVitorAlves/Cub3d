/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:34:17 by lalves-d          #+#    #+#             */
/*   Updated: 2026/03/04 16:23:50 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int *) dst = color;
}

int	render_frame(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	raycasting_loop(game);
	render_minimap(game);
	gun_update(&game->gun);
	gun_draw(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

static void	draw_wall_slicer_init(t_game *game, int x,
	double perp_wall_dist, t_draw_wall_slice *dws)
{
	dws->wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
	dws->draw_start = (-dws->wall_height / 2) + (SCREEN_HEIGHT / 2);
	if (dws->draw_start < 0)
		dws->draw_start = 0;
	dws->draw_end = (dws->wall_height / 2) + (SCREEN_HEIGHT / 2);
	if (dws->draw_end >= SCREEN_HEIGHT)
		dws->draw_end = SCREEN_HEIGHT - 1;
	dws->y = 0;
	while (dws->y < dws->draw_start)
	{
		my_mlx_pixel_put(&game->img, x, dws->y, game->cfg.ceiling_color);
		dws->y++;
	}
}

void	draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side)
{
	t_draw_wall_slice	dws;
	int					color;

	draw_wall_slicer_init(game, x, perp_wall_dist, &dws);
	if (side == 0)
		color = 0xFF0000;
	else
		color = 0x00FF00;
	dws.y = dws.draw_start;
	while (dws.y < dws.draw_end)
	{
		my_mlx_pixel_put(&game->img, x, dws.y, color);
		dws.y++;
	}
	dws.y = dws.draw_end;
	while (dws.y < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(&game->img, x, dws.y, game->cfg.floor_color);
		dws.y++;
	}
}
