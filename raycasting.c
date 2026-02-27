/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:12 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/27 14:49:44 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_view_player(t_game *game, int x, int y)
{
	if (game->cfg.map[y][x] == 'N')
	{
		game->player.dir_y = -1;
		game->player.plane_x = 1.0;
	}
	else if (game->cfg.map[y][x] == 'S')
	{
		game->player.dir_y = 1;
		game->player.plane_x = -1.0;
	}
	else if (game->cfg.map[y][x] == 'E')
	{
		game->player.dir_x = 1;
		game->player.plane_y = 1.0;
	}
	else if (game->cfg.map[y][x] == 'W')
	{
		game->player.dir_x = -1;
		game->player.plane_y = -1.0;
	}
}

void	init_player(t_game *game)
{
	int	y;
	int	x;

	y = -1;
	game->player.dir_x = 0;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0;
	while (++y < game->cfg.map_height)
	{
		x = -1;
		while (game->cfg.map[y][++x])
		{
			if (game->cfg.map[y][x] == 'N' || game->cfg.map[y][x] == 'S'
				|| game->cfg.map[y][x] == 'E' || game->cfg.map[y][x] == 'W')
			{
				game->player.pos_x = x + 0.5;
				game->player.pos_y = y + 0.5;
				set_view_player(game, x, y);
				game->cfg.map[y][x] = '0';
				return ;
			}
		}
	}
}

void	step_math(t_dda *dda, t_game *game)
{
	if (dda->ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (game->player.pos_x - dda->map_x)
			* dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - game->player.pos_x)
			* dda->delta_dist_x;
	}
	if (dda->ray_dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (game->player.pos_y - dda->map_y)
			* dda->delta_dist_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0 - game->player.pos_y)
			* dda->delta_dist_y;
	}
}

void	hit_math(t_game *game, t_dda *dda)
{
	while (!dda->hit)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_dist_x;
			dda->map_x += dda->step_x;
			dda->side = 0;
		}
		else
		{
			dda->side_dist_y += dda->delta_dist_y;
			dda->map_y += dda->step_y;
			dda->side = 1;
		}
		if (game->cfg.map[dda->map_y][dda->map_x] == '1'
			|| game->cfg.map[dda->map_y][dda->map_x] == 'D')
		{
			dda->hit_char = &game->cfg.map[dda->map_y][dda->map_x];
			dda->hit = 1;
		}
	}
}

void	raycasting_loop(t_game *game)
{
	int			x;
	t_ray_loop	rl;
	t_dda		dda;
	t_tex		*tex;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		init_ray(game, &rl, &dda, x);
		calc_wall_height(&rl);
		tex = select_texture(game, &rl, &dda);
		draw_column(game, &rl, tex, x);
		x++;
	}
}
