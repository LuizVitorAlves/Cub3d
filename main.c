/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:45:48 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/07 12:11:21 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int main(int argc, char **argv)
{
    t_game  game;

    if (argc != 2)
    {
        printf("Uso: ./cub3d mapa.cub\n");
        return (1);
    }

    memset(&game, 0, sizeof(t_game));

    if (parse_cub_file(argv[1], &game.cfg))
    {
        free_game_memory(&game);
        return (1);
    }
    
    // Inicializa a miniLibX
    game.mlx = mlx_init();
    if (!game.mlx) return (1);
    
    // Cria a janela
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    if (!game.win) return (1);

    // Esconder e centralizar o cursor
    mlx_mouse_hide(game.mlx, game.win);
    mlx_mouse_move(game.mlx, game.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Cria a imagem
    game.img.img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.line_len, &game.img.endian);

    // Inicializa o jogador
    init_player(&game);

    // Configura os hooks
    mlx_loop_hook(game.mlx, (int (*)())render_frame, &game);
    mlx_hook(game.win, 2, 1L << 0, handle_key_press, &game); // Teclas
    mlx_hook(game.win, 6, 1L << 6, mouse_move_hook, &game); // MouseMove
    mlx_hook(game.win, 17, 0, close_window, &game);         // Fechar janela

    // Inicia o loop principal
    mlx_loop(game.mlx);

    // Libera a memória quando o loop termina
    free_game_memory(&game);

    return (0);
}
