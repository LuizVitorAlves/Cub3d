/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:12 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/21 23:07:00 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void init_player(t_game *game)
{
    int y;
    int x;

    y = 0;
    game->player.dir_x = 0;
    game->player.dir_y = 0;
    game->player.plane_x = 0;
    game->player.plane_y = 0;
    while (y < game->cfg.map_height)
    {
        x = 0;
        while (game->cfg.map[y][x])
        {
            if (game->cfg.map[y][x] == 'N' || game->cfg.map[y][x] == 'S'
                || game->cfg.map[y][x] == 'E' || game->cfg.map[y][x] == 'W')
            {
                game->player.pos_x = x + 0.5;
                game->player.pos_y = y + 0.5;
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
                game->cfg.map[y][x] = '0';
                return ;
            }
            x++;
        }
        y++;
    }
}

double calculate_dda(t_game *game, double ray_dir_x, double ray_dir_y, int *side)
{
    int map_x = (int)game->player.pos_x;
    int map_y = (int)game->player.pos_y;
    double side_dist_x;
    double side_dist_y;
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);
    int step_x;
    int step_y;
    int hit = 0;

    if (ray_dir_x < 0) {
        step_x = -1;
        side_dist_x = (game->player.pos_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - game->player.pos_x) * delta_dist_x;
    }
    if (ray_dir_y < 0) {
        step_y = -1;
        side_dist_y = (game->player.pos_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - game->player.pos_y) * delta_dist_y;
    }

    while (hit == 0) {
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            *side = 0;
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            *side = 1;
        }
        if (game->cfg.map[map_y][map_x] == '1')
            hit = 1;
    }

    if (*side == 0)
        return (side_dist_x - delta_dist_x);
    else
        return (side_dist_y - delta_dist_y);
}

void raycasting_loop(t_game *game)
{
    int x;
    double perp_wall_dist;
    int side;
    int wall_height;
    int draw_start;
    int draw_end;
    int color;

    x = 0;
    while (x < SCREEN_WIDTH)
    {
        double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
        double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
        double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

        perp_wall_dist = calculate_dda(game, ray_dir_x, ray_dir_y, &side);

        wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
        draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
        if (draw_start < 0) draw_start = 0;
        draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
        if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

        int y;
        y = 0;
        while (y < draw_start)
        {
            my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
            y++;
        }

        if (side == 0 && ray_dir_x > 0)
            color = 0xFF0000;
        else if (side == 0 && ray_dir_x < 0)
            color = 0x00FF00;
        else if (side == 1 && ray_dir_y > 0)
            color = 0x0000FF;
        else
            color = 0xFFFF00;

        y = draw_start;
        while (y < draw_end)
        {
            my_mlx_pixel_put(&game->img, x, y, color);
            y++;
        }

        y = draw_end;
        while (y < SCREEN_HEIGHT)
        {
            my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
            y++;
        }
        x++;
    }
}
