/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:45:48 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/23 01:40:32 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    t_game game;

    if (argc != 2)
    {
        printf("Uso: ./cub3d mapa.cub\n");
        return 1;
    }

    memset(&game, 0, sizeof(t_game));

    if (parse_cub_file(argv[1], &game.cfg))
        return 1;

    // Inicializa MiniLibX
    game.mlx = mlx_init();
    if (!game.mlx)
        return 1;

    // Carrega a textura
    int wid, hei;
    game.textura.my_asset = mlx_xpm_file_to_image(game.mlx, "./my_textures/rock_texture.xpm", &wid, &hei);
    if (!game.textura.my_asset)
    {
        printf("Erro ao carregar textura\n");
        return 1;
    }

    game.textura.tex_width = wid;
    game.textura.tex_height = hei;
    game.textura.addr = mlx_get_data_addr(game.textura.my_asset,
                                          &game.textura.bpp,
                                          &game.textura.line_len,
                                          &game.textura.endian);

    // Cria a janela
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    if (!game.win)
        return 1;

    // Cria a imagem que será mostrada na janela
    game.img.img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.line_len, &game.img.endian);

    // Copia a textura para a imagem no canto superior esquerdo
    for (int y = 0; y < game.textura.tex_height; y++)
        for (int x = 0; x < game.textura.tex_width; x++)
        {
            int color = *(unsigned int *)(game.textura.addr + (y * game.textura.line_len + x * (game.textura.bpp / 8)));
            my_mlx_pixel_put(&game.img, x, y, color);
        }

    // Mostra a imagem na janela
    mlx_put_image_to_window(game.mlx, game.win, game.img.img, 0, 0);

    // Mantém a janela aberta
    mlx_loop(game.mlx);

    return 0;
}
