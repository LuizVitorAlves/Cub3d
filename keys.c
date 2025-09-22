/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:22:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/22 17:20:44 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void move_forward_backward(int keycode, t_game *game)
{
    double next_x = game->player.pos_x;
    double next_y = game->player.pos_y;

    if (keycode == 119) // Tecla W
    {
        next_x += game->player.dir_x * MOVE_SPEED;
        next_y += game->player.dir_y * MOVE_SPEED;
    }
    if (keycode == 115) // Tecla S
    {
        next_x -= game->player.dir_x * MOVE_SPEED;
        next_y -= game->player.dir_y * MOVE_SPEED;
    }
    int map_x = (int)next_x;
    int map_y = (int)next_y;
    if (game->cfg.map[map_y][map_x] != '1')
    {
        game->player.pos_x = next_x;
        game->player.pos_y = next_y;
    }
}

void move_left_right(int keycode, t_game *game)
{
    double next_x;
    double next_y;
    int map_x;
    int map_y;

    if (keycode == 97) // Tecla A
    {
        next_x = game->player.pos_x - game->player.plane_x * MOVE_SPEED;
        next_y = game->player.pos_y - game->player.plane_y * MOVE_SPEED;
    }
    else if (keycode == 100) // Tecla D
    {
        next_x = game->player.pos_x + game->player.plane_x * MOVE_SPEED;
        next_y = game->player.pos_y + game->player.plane_y * MOVE_SPEED;
    }
    else
        return;
    map_x = (int)next_x;
    map_y = (int)next_y;
    if (game->cfg.map[map_y][map_x] != '1')
    {
        game->player.pos_x = next_x;
        game->player.pos_y = next_y;
    }
}


void rotate_camera(int keycode, t_game *game)
{
    double old_dir_x = game->player.dir_x;
    double old_plane_x = game->player.plane_x;
    
    if (keycode == 65361) // Seta Esquerda
    {
        game->player.dir_x = old_dir_x * cos(-ROT_SPEED) - game->player.dir_y * sin(-ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(-ROT_SPEED) + game->player.dir_y * cos(-ROT_SPEED);
        game->player.plane_x = old_plane_x * cos(-ROT_SPEED) - game->player.plane_y * sin(-ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(-ROT_SPEED) + game->player.plane_y * cos(-ROT_SPEED);
    }
    if (keycode == 65363) // Seta Direita
    {
        game->player.dir_x = old_dir_x * cos(ROT_SPEED) - game->player.dir_y * sin(ROT_SPEED);
        game->player.dir_y = old_dir_x * sin(ROT_SPEED) + game->player.dir_y * cos(ROT_SPEED);
        game->player.plane_x = old_plane_x * cos(ROT_SPEED) - game->player.plane_y * sin(ROT_SPEED);
        game->player.plane_y = old_plane_x * sin(ROT_SPEED) + game->player.plane_y * cos(ROT_SPEED);
    }
}

int handle_keys(int keycode, t_game *game)
{
    printf("Tecla pressionada, keycode: %d\n", keycode);
    if (keycode == 65307)
        close_window(game);
    if ((keycode == 119 || keycode == 115) &&  (find_and_validate_player2(&game->cfg,&game->player.pos_x, &game->player.pos_y)) != 1)
    {    
        move_forward_backward(keycode, game);
        printf(" x %f/n %f y ", game->player.pos_x, game->player.pos_y);
        printf(" \n %d", find_and_validate_player2(&game->cfg,&game->player.pos_x, &game->player.pos_y));
    }
    if (keycode == 97 || keycode == 100)
        move_left_right(keycode, game);
    if (keycode == 65361 || keycode == 65363)
        rotate_camera(keycode, game);
    return (0);
}

int close_window(t_game *game)
{
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
    return (0);
}
