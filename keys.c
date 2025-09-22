/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:22:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/22 08:44:02 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int handle_keys(int keycode, t_game *game)
{
    if (keycode == 65307)
        close_window(game);
    if (keycode == 13 || keycode == 1)
        move_forward_backward(keycode, game);
    if (keycode == 0 || keycode == 2)
        move_left_right(keycode, game);
    if (keycode == 123 || keycode == 124)
        rotate_camera(keycode, game);
    return (0);
}
void move_forward_backward(int keycode, t_game *game)
{
    if (keycode == 13) // Tecla W
    {
        game->player.pos_x += game->player.dir_x * MOVE_SPEED;
        game->player.pos_y += game->player.dir_y * MOVE_SPEED;
    }
    if (keycode == 1) // Tecla S
    {
        game->player.pos_x -= game->player.dir_x * MOVE_SPEED;
        game->player.pos_y -= game->player.dir_y * MOVE_SPEED;
    }
}

void move_left_right(int keycode, t_game *game)
{
    if (keycode == 0) // Tecla A
    {
        game->player.pos_x -= game->player.plane_x * MOVE_SPEED;
        game->player.pos_y -= game->player.plane_y * MOVE_SPEED;
    }
    if (keycode == 2) // Tecla D
    {
        game->player.pos_x += game->player.plane_x * MOVE_SPEED;
        game->player.pos_y += game->player.plane_y * MOVE_SPEED;
    }
}

void rotate_camera(int keycode, t_game *game)
{
    double old_dir_x = game->player.dir_x;
    double old_plane_x = game->player.plane_x;

    if (keycode == 123) // Seta Esquerda
    {
        game->player.dir_x = old_dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);
        game->player.plane_x = old_plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);
    }
    if (keycode == 124) // Seta Direita
    {
        game->player.dir_x = old_dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);
        game->player.plane_x = old_plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
    }
}



int close_window(t_game *game)
{
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
    return (0);
}
