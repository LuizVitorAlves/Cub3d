/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 16:34:17 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/22 13:39:35 by lalves-d         ###   ########.fr       */
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
    mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
    return (0);
}

void draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side)
{
    int wall_height;
    int draw_start;
    int draw_end;
    int y;

    // Dados da textura
    int tex_width = game->tex_width;
    int tex_height = game->tex_height;
    int tex_bpp, tex_line_len, tex_endian;
    char *tex_addr = mlx_get_data_addr(game->my_asset, &tex_bpp, &tex_line_len, &tex_endian);

    // Calcula altura da parede na tela
    wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
    draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_start < 0)
        draw_start = 0;
    draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
    if (draw_end >= SCREEN_HEIGHT)
        draw_end = SCREEN_HEIGHT - 1;

    // Desenha o teto
    y = 0;
    while (y < draw_start)
    {
        my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
        y++;
    }

    // Calcula a posição exata da parede para mapeamento da textura
    double wall_x;
    if (side == 0) // parede vertical
        wall_x = game->player.y + perp_wall_dist * game->ray_dir_y;
    else // parede horizontal
        wall_x = game->player.x + perp_wall_dist * game->ray_dir_x;
    wall_x -= floor(wall_x); // pega a fração

    int tex_x = (int)(wall_x * (double)tex_width);
    if ((side == 0 && game->ray_dir_x > 0) || (side == 1 && game->ray_dir_y < 0))
        tex_x = tex_width - tex_x - 1;

    // Passo vertical na textura por pixel de parede
    double step = 1.0 * tex_height / wall_height;
    double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + wall_height / 2) * step;

    // Desenha a parede com textura
    y = draw_start;
    while (y < draw_end)
    {
        int tex_y = (int)tex_pos & (tex_height - 1);
        tex_pos += step;

        int color = *(unsigned int *)(tex_addr + (tex_y * tex_line_len + tex_x * (tex_bpp / 8)));
        my_mlx_pixel_put(&game->img, x, y, color);
        y++;
    }

    // Desenha o chão
    y = draw_end;
    while (y < SCREEN_HEIGHT)
    {
        my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
        y++;
    }
}

