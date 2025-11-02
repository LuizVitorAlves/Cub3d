/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:34:17 by lalves-d          #+#    #+#             */
/*   Updated: 2025/11/02 14:05:14 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void my_mlx_pixel_put(t_img *data, int x, int y, int color)
{
    char *dst;

    dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
    *(unsigned int*)dst = color;
}

int render_frame(void *param)
{
    t_game *game;

    game = (t_game *)param;
    raycasting_loop(game);
    render_minimap(game);
    gun_update(&game->gun);
    gun_draw(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
    return (0);
}

void draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side)
{
    int wall_height;
    int draw_start;
    int draw_end;
    int color;
    int y;

    wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
    draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_start < 0)
        draw_start = 0;
    draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_end >= SCREEN_HEIGHT)
        draw_end = SCREEN_HEIGHT - 1;

    y = 0;
    while (y < draw_start)
    {
        my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
        y++;
    }

    if (side == 0)
        color = 0xFF0000;
    else
        color = 0x00FF00;

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
}
