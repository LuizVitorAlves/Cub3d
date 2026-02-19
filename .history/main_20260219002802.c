/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:45:48 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/20 13:21:54 by lalves-d         ###   ########.fr       */
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
        return (1);
    game.mlx = mlx_init();
    if (!game.mlx) 
    {
        free_config_and_map(&game.cfg);
        return (1);
    }
    gun_load(&game);
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    if (!game.win) 
    {
        cleanup_on_gun_error(&game, "Falha ao criar a janela.");
        return (1);
    }
    mlx_mouse_hide(game.mlx, game.win);
    mlx_mouse_move(game.mlx, game.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    game.img.img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.line_len, &game.img.endian);
    init_player(&game);
    mlx_loop_hook(game.mlx, render_frame, &game);
    mlx_hook(game.win, 2, 1L << 0, handle_key_press, &game);
    mlx_hook(game.win, 6, 1L << 6, mouse_move_hook, &game);
    mlx_hook(game.win, 17, 0, close_window, &game);
    mlx_loop(game.mlx);
    free_game_memory(&game);
    return (0);
}
