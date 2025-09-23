/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:22:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/23 01:35:34 by lalves-d         ###   ########.fr       */
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

int handle_key_press(int keycode, t_game *game){
    handle_keys(keycode,game);
    return(0);
}
int handle_keys(int keycode, t_game *game)
{
    printf("Tecla pressionada, keycode: %d\n", keycode);
    if (keycode == 65307)
        close_window(game);
    if ((keycode == 119 || keycode == 115))
        move_forward_backward(keycode, game);
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

int mouse_move_hook(int x, int y, t_game *game)
{
    int center_x = SCREEN_WIDTH / 2;
    int center_y = SCREEN_HEIGHT / 2;
    double rot_speed;
    double old_dir_x;
    double old_plane_x;

    (void)y; // ignorando o y se não for usar

    // calcular o quanto o mouse se moveu horizontalmente desde o centro
    rot_speed = (x - center_x) * ROT_SPEED_MOUSE;

    // atualizar direção do player
    old_dir_x = game->player.dir_x;
    old_plane_x = game->player.plane_x;
    game->player.dir_x = old_dir_x * cos(rot_speed) - game->player.dir_y * sin(rot_speed);
    game->player.dir_y = old_dir_x * sin(rot_speed) + game->player.dir_y * cos(rot_speed);
    game->player.plane_x = old_plane_x * cos(rot_speed) - game->player.plane_y * sin(rot_speed);
    game->player.plane_y = old_plane_x * sin(rot_speed) + game->player.plane_y * cos(rot_speed);

    // resetar o cursor para o centro da tela (permite rotação 360°)
    mlx_mouse_move(game->mlx, game->win, center_x, center_y);

    return (0);
}


