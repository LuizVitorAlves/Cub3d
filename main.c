/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* main.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 15:00:28 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 16:12:15 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"
#include <string.h>

int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
    {
        printf("Uso: ./cub3d mapa.cub\n");
        return (1);
    }
    // Inicializa a estrutura e faz o parsing
    memset(&game.cfg, 0, sizeof(t_config));
    if (parse_cub_file(argv[1], &game.cfg))
    {
        // free_config_and_map(&game.cfg);
        // O main agora chama uma função de cleanup para a t_game
        return (1);
    }
    
    // Inicializa a miniLibX
    game.mlx = mlx_init();
    if (!game.mlx) return (1);
    
    // Cria a janela
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    if (!game.win) return (1);

    // Cria a imagem
    game.img.img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.line_len, &game.img.endian);

    // Inicializa o jogador
    init_player(&game);

    // Configura os hooks e inicia o loop
    mlx_loop_hook(game.mlx, render_frame, &game);
    // ...
    mlx_loop(game.mlx);

    return (0);
}
