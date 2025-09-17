/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:12 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/17 08:35:17 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player(t_config *game)
{
	int	y;
	int	x;

	y = 0;
	game->player.dir_x = 0;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'N' || game->map[y][x] == 'S'
				|| game->map[y][x] == 'E' || game->map[y][x] == 'W')
			{
				game->player.pos_x = x + 0.5;
				game->player.pos_y = y + 0.5;
				if (game->map[y][x] == 'N')
				{
					game->player.dir_y = -1;
					game->player.plane_x = 1.0;
				}
				else if (game->map[y][x] == 'S')
				{
					game->player.dir_y = 1;
					game->player.plane_x = -1.0;
				}
				else if (game->map[y][x] == 'E')
				{
					game->player.dir_x = 1;
					game->player.plane_y = 1.0;
				}
				else if (game->map[y][x] == 'W')
				{
					game->player.dir_x = -1;
					game->player.plane_y = -1.0;
				}
				game->map[y][x] = '0';
				return ;
			}
			x++;
		}
		y++;
	}
}

double	calculate_dda(t_config *game, double ray_dir_x, double ray_dir_y)
{
	int		map_x;
	int		map_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		hit;
	int		side;

	map_x = (int)game->player.pos_x;
	map_y = (int)game->player.pos_y;
	double side_dist_x, side_dist_y;
	delta_dist_x = fabs(1 / ray_dir_x);
	delta_dist_y = fabs(1 / ray_dir_y);
	int step_x, step_y;
	hit = 0;
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (game->player.pos_x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - game->player.pos_x) * delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (game->player.pos_y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - game->player.pos_y) * delta_dist_y;
	}
	while (hit == 0)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (game->map[map_y][map_x] == '1')
			hit = 1;
	}
	if (side == 0)
		return (side_dist_x - delta_dist_x);
	else
		return (side_dist_y - delta_dist_y);
}

void	raycasting_loop(t_config *game)
{
	int		x;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	perp_wall_dist;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
		ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
		ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;
		perp_wall_dist = calculate_dda(game, ray_dir_x, ray_dir_y);
		// Aqui, você pode passar a 'perp_wall_dist' para a parte
		// de renderização do seu parceiro. Por exemplo, você pode
		// ter um array para armazenar as distâncias ou uma função
		// que desenhe a coluna.
		x++;
	}
}
