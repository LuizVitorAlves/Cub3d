/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:00:12 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/17 16:50:49 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player(t_config *game)
{
	int	y;
	int	x;

	y = 0;
	game->player.dir_x = 0;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0;
	while (y < game->map_height)
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == 'N' || game->map[y][x] == 'S'
				|| game->map[y][x] == 'E' || game->map[y][x] == 'W')
			{
				game->player.pos_x = x + 0.5;
				game->player.pos_y = y + 0.5;
				if (game->map[y][x] == 'N')
				{
					game->player.dir_y = -1;
					game->player.plane_x = 1.0;
				}
				else if (game->map[y][x] == 'S')
				{
					game->player.dir_y = 1;
					game->player.plane_x = -1.0;
				}
				else if (game->map[y][x] == 'E')
				{
					game->player.dir_x = 1;
					game->player.plane_y = 1.0;
				}
				else if (game->map[y][x] == 'W')
				{
					game->player.dir_x = -1;
					game->player.plane_y = -1.0;
				}
				game->map[y][x] = '0';
				return ;
			}
			x++;
		}
		y++;
	}
}

double calculate_dda(t_config *game, double ray_dir_x, double ray_dir_y, int *side)
{
    // Posição atual do raio no mapa de grade
    int map_x = (int)game->player.pos_x;
    int map_y = (int)game->player.pos_y;

    // Distância percorrida pelo raio de uma linha de grade para a próxima
    double delta_dist_x = fabs(1 / ray_dir_x);
    double delta_dist_y = fabs(1 / ray_dir_y);

    // Distância do jogador até a primeira linha de grade
    double side_dist_x;
    double side_dist_y;

    // Direção do passo no mapa (1 ou -1)
    int step_x;
    int step_y;
    int hit = 0;

    // Calcula o passo inicial e a distância inicial no eixo X
    if (ray_dir_x < 0) {
        step_x = -1;
        side_dist_x = (game->player.pos_x - map_x) * delta_dist_x;
    } else {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - game->player.pos_x) * delta_dist_x;
    }

    // Calcula o passo inicial e a distância inicial no eixo Y
    if (ray_dir_y < 0) {
        step_y = -1;
        side_dist_y = (game->player.pos_y - map_y) * delta_dist_y;
    } else {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - game->player.pos_y) * delta_dist_y;
    }

    // Loop principal do DDA para encontrar a parede
    while (hit == 0) {
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            *side = 0; // O raio atingiu uma parede Leste/Oeste
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;
            *side = 1; // O raio atingiu uma parede Norte/Sul
        }
        
        // Verifica se o raio bateu em uma parede (caractere '1')
        if (game->map[map_y][map_x] == '1')
            hit = 1;
    }

    // Calcula a distância perpendicular para a projeção
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

    for (x = 0; x < SCREEN_WIDTH; x++)
    {
        double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
        double ray_dir_x = game->cfg.player.dir_x + game->cfg.player.plane_x * camera_x;
        double ray_dir_y = game->cfg.player.dir_y + game->cfg.player.plane_y * camera_x;

        perp_wall_dist = calculate_dda(game, ray_dir_x, ray_dir_y, &side);

        wall_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
        draw_start = (-wall_height / 2) + (SCREEN_HEIGHT / 2);
        if (draw_start < 0) draw_start = 0;
        draw_end = (wall_height / 2) + (SCREEN_HEIGHT / 2);
        if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

        for (int y = 0; y < draw_start; y++)
            my_mlx_pixel_put(&game->img, x, y, game->cfg.ceiling_color);

        if (side == 0 && ray_dir_x > 0)
            color = 0xFF0000;
        else if (side == 0 && ray_dir_x < 0)
            color = 0x00FF00;
        else if (side == 1 && ray_dir_y > 0)
            color = 0x0000FF;
        else
            color = 0xFFFF00;

        for (int y = draw_start; y < draw_end; y++)
            my_mlx_pixel_put(&game->img, x, y, color);

        for (int y = draw_end; y < SCREEN_HEIGHT; y++)
            my_mlx_pixel_put(&game->img, x, y, game->cfg.floor_color);
    }
}

