/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:12 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/25 18:09:57 by lalves-d         ###   ########.fr       */
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

double calculate_dda(t_game *game, double ray_dir_x, double ray_dir_y, int *side, char *hit_char)
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

    if (ray_dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (game->player.pos_x - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - game->player.pos_x) * delta_dist_x;
    }
    if (ray_dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (game->player.pos_y - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - game->player.pos_y) * delta_dist_y;
    }

    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            *side = 0;
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            *side = 1;
        }

        if (game->cfg.map[map_y][map_x] == '1' || game->cfg.map[map_y][map_x] == 'D')
        {
            *hit_char = game->cfg.map[map_y][map_x];
            hit = 1;
        }
    }

    if (*side == 0)
        return (side_dist_x - delta_dist_x);
    else
        return (side_dist_y - delta_dist_y);
}

// void raycasting_loop(t_game *game)
// {
//     int x;

//     for (x = 0; x < SCREEN_WIDTH; x++)
//     {
//         double camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;
//         double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
//         double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

//         int side;
//         char hit_char;
//         double perp_wall_dist = calculate_dda(game, ray_dir_x, ray_dir_y, &side, &hit_char);

//         //  calcular altura da parede
//         int wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

//         int draw_start = -wall_height / 2 + SCREEN_HEIGHT / 2;
//         if (draw_start < 0)
//             draw_start = 0;

//         int draw_end = wall_height / 2 + SCREEN_HEIGHT / 2;
//         if (draw_end >= SCREEN_HEIGHT)
//             draw_end = SCREEN_HEIGHT - 1;

//         //  desenhar teto
//         for (int y = 0; y < draw_start; y++)
//             my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);

//         //  calcular ponto exato onde o raio bateu
//         double wallX;
//         if (side == 0)
//             wallX = game->player.pos_y + perp_wall_dist * ray_dir_y;
//         else
//             wallX = game->player.pos_x + perp_wall_dist * ray_dir_x;
//         wallX -= floor(wallX);

//         //  escolher textura correta
//         t_tex *tex;
//         if (side == 0)
//         {
//             if (ray_dir_x > 0)
//                 tex = &game->tex[TEX_WE];
//             else
//                 tex = &game->tex[TEX_EA];
//         }
//         else
//         {
//             if (ray_dir_y > 0)
//                 tex = &game->tex[TEX_NO];
//             else
//                 tex = &game->tex[TEX_SO];
//         }

//         // calcular coluna da textura
//         int tex_x = (int)(wallX * (double)tex->width);

//         if (side == 0 && ray_dir_x > 0)
//             tex_x = tex->width - tex_x - 1;
//         if (side == 1 && ray_dir_y < 0)
//             tex_x = tex->width - tex_x - 1;

//         // calcular passo vertical da textura
//         double step = (double)tex->height / (double)wall_height;
//         double texPos = (draw_start - SCREEN_HEIGHT / 2.0 + wall_height / 2.0) * step;

//         //  desenhar parede texturizada
//         for (int y = draw_start; y < draw_end; y++)
//         {
//             int tex_y = (int)texPos;
//             texPos += step;

//             if (tex_y < 0)
//                 tex_y = 0;
//             if (tex_y >= tex->height)
//                 tex_y = tex->height - 1;

//             int color = get_tex_pixel(tex, tex_x, tex_y);
//             my_mlx_pixel_put(&game->img, x, y, color);
//         }

//         // desenhar chão
//         for (int y = draw_end; y < SCREEN_HEIGHT; y++)
//             my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
//     }
// }



void raycasting_loop(t_game *game)
{
    int x = 0;

    while (x < SCREEN_WIDTH)
    {
        double camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;
        double ray_dir_x = game->player.dir_x + game->player.plane_x * camera_x;
        double ray_dir_y = game->player.dir_y + game->player.plane_y * camera_x;

        int side;
        char hit_char;
        double perp_wall_dist = calculate_dda(game, ray_dir_x, ray_dir_y, &side, &hit_char);

        int wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        int draw_start = -wall_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;

        int draw_end = wall_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;

        int y = 0;

        // teto
        while (y < draw_start)
        {
            my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);
            y++;
        }

        double wallX;
        if (side == 0)
            wallX = game->player.pos_y + perp_wall_dist * ray_dir_y;
        else
            wallX = game->player.pos_x + perp_wall_dist * ray_dir_x;
        wallX -= floor(wallX);

        t_tex *tex;
        if (side == 0)
            tex = (ray_dir_x > 0) ? &game->tex[TEX_WE] : &game->tex[TEX_EA];
        else
            tex = (ray_dir_y > 0) ? &game->tex[TEX_NO] : &game->tex[TEX_SO];

        int tex_x = (int)(wallX * tex->width);

        if (side == 0 && ray_dir_x > 0)
            tex_x = tex->width - tex_x - 1;
        if (side == 1 && ray_dir_y < 0)
            tex_x = tex->width - tex_x - 1;

        double step = (double)tex->height / wall_height;
        double texPos = (draw_start - SCREEN_HEIGHT / 2.0 + wall_height / 2.0) * step;

        // parede
        while (y < draw_end)
        {
            int tex_y = (int)texPos;
            texPos += step;

            if (tex_y < 0)
                tex_y = 0;
            if (tex_y >= tex->height)
                tex_y = tex->height - 1;

            int color = get_tex_pixel(tex, tex_x, tex_y);
            my_mlx_pixel_put(&game->img, x, y, color);
            y++;
        }

        // chão
        while (y < SCREEN_HEIGHT)
        {
            my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
            y++;
        }

        x++;
    }
}
