/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:45:48 by lalves-d          #+#    #+#             */
/*   Updated: 2026/03/04 21:17:02 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_var_main(t_game *game);
static int	error_argc(void);

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (error_argc());
	memset(&game, 0, sizeof(t_game));
	if (parse_cub_file(argv[1], &game.cfg))
		return (1);
	game.mlx = mlx_init();
	if (!game.mlx)
	{
		free_config_and_map(&game.cfg);
		return (1);
	}
	if (load_textures(&game))
		return (1);
	ft_printf("pixel NO(0,0) = %x\n", get_tex_pixel(&game.tex[TEX_NO], 0, 0));
	gun_load(&game);
	game.win = mlx_new_window(game.mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
	if (!game.win)
	{
		cleanup_on_gun_error(&game, "Falha ao criar a janela.");
		return (1);
	}
	init_var_main(&game);
	return (0);
}

static void	init_var_main(t_game *game)
{
	mlx_mouse_move(game->mlx, game->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	game->img.img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	game->img.addr = mlx_get_data_addr(game->img.img, &game->img.bpp,
			&game->img.line_len, &game->img.endian);
	init_player(game);
	mlx_loop_hook(game->mlx, render_frame, game);
	mlx_hook(game->win, 2, 1L << 0, handle_key_press, game);
	setup_mouse_hook(game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop(game->mlx);
	free_game_memory(game);
}

static int	error_argc(void)
{
	ft_printf("Uso: ./cub3d mapa.cub\n");
	return (1);
}
