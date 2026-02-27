/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:24:29 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 15:59:39 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_ray(t_game *game, t_ray_loop *rl, t_dda *dda, int x)
{
	rl->camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;
	dda->ray_dir_x = game->player.dir_x
		+ game->player.plane_x * rl->camera_x;
	dda->ray_dir_y = game->player.dir_y
		+ game->player.plane_y * rl->camera_x;
	rl->perp_wall_dist = calculate_dda(game, dda);
}

void	calc_wall_height(t_ray_loop *rl)
{
	rl->wall_height = (int)(SCREEN_HEIGHT / rl->perp_wall_dist);
	rl->draw_start = -rl->wall_height / 2 + SCREEN_HEIGHT / 2;
	if (rl->draw_start < 0)
		rl->draw_start = 0;
	rl->draw_end = rl->wall_height / 2 + SCREEN_HEIGHT / 2;
	if (rl->draw_end >= SCREEN_HEIGHT)
		rl->draw_end = SCREEN_HEIGHT - 1;
}

t_tex	*select_texture(t_game *game,
	t_ray_loop *rl, t_dda *dda)
{
	t_tex	*tex;

	if (dda->side == 0)
	{
		rl->wallX = game->player.pos_y
			+ rl->perp_wall_dist * dda->ray_dir_y;
		if (dda->ray_dir_x > 0)
			tex = &game->tex[TEX_WE];
		else
			tex = &game->tex[TEX_EA];
	}
	else
	{
		rl->wallX = game->player.pos_x
			+ rl->perp_wall_dist * dda->ray_dir_x;
		if (dda->ray_dir_y > 0)
			tex = &game->tex[TEX_NO];
		else
			tex = &game->tex[TEX_SO];
	}
	rl->wallX -= floor(rl->wallX);
	return (tex);
}

void	draw_flor(t_game *game, int y, int x)
{
	while (y < SCREEN_HEIGHT)
		my_mlx_pixel_put(&game->img, x, y++,
			game->cfg.floor_color);
}

void	draw_column(t_game *game, t_ray_loop *rl,
	t_tex *tex, int x)
{
	int		y;
	int		tex_x;
	int		tex_y;
	int		color;

	y = 0;
	while (y < rl->draw_start)
		my_mlx_pixel_put(&game->img, x, y++,
			game->cfg.ceiling_color);
	tex_x = (int)(rl->wallX * tex->width);
	rl->step = (double)tex->height / rl->wall_height;
	rl->texPos = (rl->draw_start - SCREEN_HEIGHT / 2.0
			+ rl->wall_height / 2.0) * rl->step;
	while (y < rl->draw_end)
	{
		tex_y = (int)rl->texPos;
		rl->texPos += rl->step;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		color = get_tex_pixel(tex, tex_x, tex_y);
		my_mlx_pixel_put(&game->img, x, y++, color);
	}
	draw_flor(game, y, x);
}
