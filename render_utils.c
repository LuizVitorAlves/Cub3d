/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:34:17 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/17 16:34:34 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    char *dst;

    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int*)dst = color;
}

int render_frame(t_game *game)
{
    raycasting_loop(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
    return (0);
}

void draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side)
{
    int wall_height;
    int draw_start;
    int draw_end;
    int color;

    wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
    draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_start < 0)
        draw_start = 0;
    draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_end >= SCREEN_HEIGHT)
        draw_end = SCREEN_HEIGHT - 1;
    for (int y = 0; y < draw_start; y++)
        my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
    if (side == 0)
        color = 0xFF0000;
    else
        color = 0x00FF00;
    for (int y = draw_start; y < draw_end; y++)
        my_mlx_pixel_put(&game->img, x, y, color);

    for (int y = draw_end; y < SCREEN_HEIGHT; y++)
        my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
}
