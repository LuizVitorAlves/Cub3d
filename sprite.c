/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:51:29 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/27 15:35:52 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cleanup_on_gun_error(t_game *game, char *error_msg)
{
	int	i;

	printf("Error\n%s\n", error_msg);
	i = 0;
	while (i < GUN_FRAMES)
	{
		if (game->gun.frames[i] != NULL)
			mlx_destroy_image(game->mlx, game->gun.frames[i]);
		i++;
	}
	free_config_and_map(&game->cfg);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(1);
}

void	gun_load(t_game *game)
{
	t_gun	*gun;
	int		i;
	char	*paths[GUN_FRAMES];

	paths[0] = "gun/pistol1.xpm";
	paths[1] = "gun/pistol2.xpm";
	paths[2] = "gun/pistol3.xpm";
	paths[3] = "gun/pistol4.xpm";
	paths[4] = "gun/pistol5.xpm";
	gun = &game->gun;
	i = 0;
	while (i < GUN_FRAMES)
	{
		gun->frames[i] = mlx_xpm_file_to_image(game->mlx, paths[i],
				&gun->width, &gun->height);
		if (!gun->frames[i])
			cleanup_on_gun_error(game, paths[i]);
		i++;
	}
	gun->current = 0;
	gun->animating = 0;
	gun->counter = 0;
}

void	gun_start_anim(t_gun *gun)
{
	if (!gun->animating)
	{
		gun->animating = 1;
		gun->current = 0;
		gun->counter = 0;
	}
}

void	gun_update(t_gun *gun)
{
	if (gun->animating)
	{
		gun->counter++;
		if (gun->counter >= GUN_ANIM_SPEED)
		{
			gun->counter = 0;
			gun->current++;
			if (gun->current >= GUN_FRAMES)
			{
				gun->current = 0;
				gun->animating = 0;
			}
		}
	}
}

void	gun_draw_sup(t_gun_tmp v, t_game *game)
{
	int	sx;
	int	sy;

	while (++v.y < v.sh)
	{
		v.x = -1;
		while (++v.x < v.sw)
		{
			sx = (int)(v.x / GUN_SCALE);
			sy = (int)(v.y / GUN_SCALE);
			v.color = *(unsigned int *)(v.src + sy
					* v.line_len + sx * (v.bpp / 8));
			if (v.color != 0xFF00FF)
				my_mlx_pixel_put(&game->img,
					SCREEN_WIDTH / 2 - v.sw / 2 + v.x,
					SCREEN_HEIGHT - v.sh + v.y, v.color);
		}
	}
}
