/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:45:48 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/20 11:43:31 by lalves-d         ###   ########.fr       */
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

    // Inicializa a struct game com zeros (importante para os ponteiros)
    memset(&game, 0, sizeof(t_game));

    // Faz o parsing e validação do arquivo .cub
    if (parse_cub_file(argv[1], &game.cfg))
    {
        // free_game_memory não é necessária aqui, pois parse_cub_file já limpa em caso de erro
        return (1);
    }
    
    // Inicializa a conexão com o servidor gráfico
    game.mlx = mlx_init();
    if (!game.mlx) 
    {
        free_config_and_map(&game.cfg); // Limpa o que já foi alocado
        return (1);
    }
    
    // Carrega os sprites da arma ANTES de criar a janela
    gun_load(&game);
    
    // Cria a janela do jogo
    game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    if (!game.win) 
    {
        // Se a janela falhar, precisa limpar a mlx e os sprites carregados
        cleanup_on_gun_error(&game, "Falha ao criar a janela.");
        return (1);
    }

    // Esconde e centraliza o cursor do mouse
    mlx_mouse_hide(game.mlx, game.win);
    mlx_mouse_move(game.mlx, game.win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Cria a imagem que será usada como buffer de renderização
    game.img.img = mlx_new_image(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bpp, &game.img.line_len, &game.img.endian);

    // Define a posição e direção inicial do jogador
    init_player(&game);

    // Configura os hooks (eventos) do teclado, mouse e janela
    mlx_loop_hook(game.mlx, render_frame, &game);
    mlx_hook(game.win, 2, 1L << 0, handle_key_press, &game); // Evento de tecla pressionada
    mlx_hook(game.win, 6, 1L << 6, mouse_move_hook, &game);  // Evento de movimento do mouse
    mlx_hook(game.win, 17, 0, close_window, &game);          // Evento de fechar a janela (clique no 'X')

    // Inicia o loop principal que processa eventos e chama o loop_hook
    mlx_loop(game.mlx);

    // Libera toda a memória alocada quando o loop termina (não será alcançado)
    free_game_memory(&game);

    return (0);
}
