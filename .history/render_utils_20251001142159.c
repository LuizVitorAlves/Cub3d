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

// void draw_wall_slice(t_game *game, int x, double perp_wall_dist)
// {
//     int wall_height;
//     int draw_start;
//     int draw_end;
//     int y;

//     // Pega os dados da textura
//     int tex_bpp, tex_line_len, tex_endian;
//     char *tex_addr = mlx_get_data_addr(game->my_asset, &tex_bpp, &tex_line_len, &tex_endian);

//     // Altura da parede na tela
//     wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
//     draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
//     if (draw_start < 0)
//         draw_start = 0;
//     draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
//     if (draw_end >= SCREEN_HEIGHT)
//         draw_end = SCREEN_HEIGHT - 1;

//     // Desenha o teto
//     y = 0;
//     while (y < draw_start)
//     {
//         my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
//         y++;
//     }

//     // Textura esticada verticalmente
//     // ajuste conforme sua textura real
//     int tex_height = 64;
//     double step = (double)tex_height / wall_height;
//     double tex_pos = 0;

//     y = draw_start;
//     while (y < draw_end)
//     {
//         int tex_y = (int)tex_pos % tex_height; // repete verticalmente
//         tex_pos += step;

//         // sempre usa tex_x = 0 para pegar a coluna inteira da textura
//         int color = *(unsigned int *)(tex_addr + (tex_y * tex_line_len));
//         my_mlx_pixel_put(&game->img, x, y, color);
//         y++;
//     }

//     // Desenha o chão
//     y = draw_end;
//     while (y < SCREEN_HEIGHT)
//     {
//         my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
//         y++;
//     }
// }









// void draw_wall_slice(t_game *game, int x, double perp_wall_dist)
// {
//     int wall_height;
//     int draw_start;
//     int draw_end;
//     int y;

//     // Pega dados da textura
//     int tex_bpp, tex_line_len, tex_endian;
//     char *tex_addr = mlx_get_data_addr(game->my_asset, &tex_bpp, &tex_line_len, &tex_endian);

//     // Vamos supor que você carregou a textura com 64x64
//     int tex_width = 64;
//     int tex_height = 64;

//     wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
//     draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
//     if (draw_start < 0)
//         draw_start = 0;
//     draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
//     if (draw_end >= SCREEN_HEIGHT)
//         draw_end = SCREEN_HEIGHT - 1;

//     // Desenha teto
//     y = 0;
//     while (y < draw_start)
//         my_mlx_pixel_put(&game->img, x, y++, game->cfg.ceiling_color);

//     // Textura esticada verticalmente
//     double step = (double)tex_height / wall_height;
//     double tex_pos = 0;

//     // Para o tex_x horizontal, podemos repetir a textura a cada coluna
//     int tex_x = x % tex_width;

//     for (y = draw_start; y < draw_end; y++)
//     {
//         int tex_y = (int)tex_pos % tex_height;
//         tex_pos += step;

//         int color = *(unsigned int *)(tex_addr + (tex_y * tex_line_len) + (tex_x * (tex_bpp / 8)));
//         my_mlx_pixel_put(&game->img, x, y, color);
//     }

//     // Desenha chão
//     while (y < SCREEN_HEIGHT)
//         my_mlx_pixel_put(&game->img, x, y++, game->cfg.floor_color);
// }





void draw_wall_slice(t_game *game, int x, double perp_wall_dist)
{
    int wall_height;
    int draw_start;
    int draw_end;
    double step;
    double tex_pos;
    int tex_x;
    int tex_y;
    char *tex_addr;
    int tex_bpp;
    int tex_line_len;
    int tex_endian;

    // pega endereço da textura
    tex_addr = mlx_get_data_addr(game->textura.my_asset, &tex_bpp, &tex_line_len, &tex_endian);

    // altura da parede na tela
    wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
    draw_start = -wall_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = wall_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_end >= SCREEN_HEIGHT)
        draw_end = SCREEN_HEIGHT - 1;

    // desenha teto
    for (int y = 0; y < draw_start; y++)
    void draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side)
{
    int wall_height;
    int draw_start;
    int draw_end;
    double step;
    double tex_pos;
    int tex_x;
    int tex_y;
    char *tex_addr;
    int tex_bpp;
    int tex_line_len;
    int tex_endian;

    // Pega endereço da textura
    tex_addr = game->textura.addr;
    tex_bpp = game->textura.bpp;
    tex_line_len = game->textura.line_len;
    tex_endian = game->textura.endian;

    // Altura da parede na tela
    wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
    draw_start = -wall_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_start < 0)
        draw_start = 0;
    draw_end = wall_height / 2 + SCREEN_HEIGHT / 2;
    if (draw_end >= SCREEN_HEIGHT)
        draw_end = SCREEN_HEIGHT - 1;

    // Desenha teto
    for (int y = 0; y < draw_start; y++)
        my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);

    // Determina coordenada horizontal da textura
    double wall_x;
    if (side == 0) // parede vertical
        wall_x = game->player.pos_y + perp_wall_dist * game->ray_dir_y;
    else           // parede horizontal
        wall_x = game->player.pos_x + perp_wall_dist * game->ray_dir_x;
    wall_x -= floor(wall_x);

    tex_x = (int)(wall_x * (double)game->textura.tex_width);
    if ((side == 0 && game->ray_dir_x > 0) || (side == 1 && game->ray_dir_y < 0))
        tex_x = game->textura.tex_width - tex_x - 1;

    // Passo vertical da textura
    step = 1.0 * game->textura.tex_height / wall_height;
    tex_pos = (draw_start - SCREEN_HEIGHT / 2 + wall_height / 2) * step;

    // Desenha parede com textura
    for (int y = draw_start; y < draw_end; y++)
    {
        tex_y = (int)tex_pos;
        if (tex_y >= game->textura.tex_height)
            tex_y = game->textura.tex_height - 1;
        tex_pos += step;

        int color = *(unsigned int *)(tex_addr + (tex_y * tex_line_len + tex_x * (tex_bpp / 8)));
        my_mlx_pixel_put(&game->img, x, y, color);
    }

    // Desenha chão
    for (int y = draw_end; y < SCREEN_HEIGHT; y++)
        my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
}








