/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:22:19 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/27 06:04:27 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	move_forward_backward(int keycode, t_game *game)
{
	double	next_x;
	double	next_y;
	int		map_x;
	int		map_y;
	double	dir;

	if (keycode != 119 && keycode != 115)
		return ;
	dir = 1.0;
	if (keycode == 115)
		dir = -1.0;
	next_x = game->player.pos_x + game->player.dir_x * MOVE_SPEED * dir;
	next_y = game->player.pos_y + game->player.dir_y * MOVE_SPEED * dir;
	map_x = (int)next_x;
	map_y = (int)next_y;
	if (map_y >= 0 && map_y < game->cfg.map_height
		&& map_x >= 0 && map_x < (int)strlen(game->cfg.map[map_y])
		&& game->cfg.map[map_y][map_x] != '1'
		&& game->cfg.map[map_y][map_x] != 'D')
	{
		game->player.pos_x = next_x;
		game->player.pos_y = next_y;
	}
}

void	move_left_right(int keycode, t_game *game)
{
	double	next_x;
	double	next_y;
	int		map_x;
	int		map_y;

	if (keycode == 97)
	{
		next_x = game->player.pos_x - game->player.plane_x * MOVE_SPEED;
		next_y = game->player.pos_y - game->player.plane_y * MOVE_SPEED;
	}
	else if (keycode == 100)
	{
		next_x = game->player.pos_x + game->player.plane_x * MOVE_SPEED;
		next_y = game->player.pos_y + game->player.plane_y * MOVE_SPEED;
	}
	else
		return ;
	map_x = (int)next_x;
	map_y = (int)next_y;
	if (game->cfg.map[map_y][map_x] != '1'
		&& game->cfg.map[map_y][map_x] != 'D')
	{
		game->player.pos_x = next_x;
		game->player.pos_y = next_y;
	}
}

void	rotate_camera(int keycode, t_game *game)
{
	double	angle;
	double	old_dir_x;
	double	old_plane_x;

	if (keycode != 65361 && keycode != 65363)
		return ;
	angle = ROT_SPEED;
	if (keycode == 65361)
		angle = -ROT_SPEED;
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = old_dir_x * cos(angle)
		- game->player.dir_y * sin(angle);
	game->player.dir_y = old_dir_x * sin(angle)
		+ game->player.dir_y * cos(angle);
	game->player.plane_x = old_plane_x * cos(angle)
		- game->player.plane_y * sin(angle);
	game->player.plane_y = old_plane_x * sin(angle)
		+ game->player.plane_y * cos(angle);
}

int	handle_key_press(int keycode, t_game *game)
{
	handle_keys(keycode, game);
	return (0);
}

int	handle_keys(int keycode, t_game *game)
{
	ft_printf("Tecla pressionada, keycode: %d\n", keycode);
	if (keycode == 65307)
		close_window(game);
	if ((keycode == 119 || keycode == 115))
		move_forward_backward(keycode, game);
	if (keycode == 97 || keycode == 100)
		move_left_right(keycode, game);
	if (keycode == 65361 || keycode == 65363)
		rotate_camera(keycode, game);
	if (keycode == 32)
	{
		interact_door(game);
		gun_start_anim(&game->gun);
	}
	return (0);
}
