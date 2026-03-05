/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gun_draw_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 15:34:00 by uviana-b          #+#    #+#             */
/*   Updated: 2026/03/04 21:17:50 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	gun_draw(t_game *game)
{
	t_gun_tmp	v;

	v.img = game->gun.frames[game->gun.current];
	if (!v.img)
		return ;
	v.src = mlx_get_data_addr(v.img, &v.bpp, &v.line_len, &(int){0});
	v.sw = (int)(game->gun.width * GUN_SCALE);
	v.sh = (int)(game->gun.height * GUN_SCALE);
	v.y = -1;
	gun_draw_sup(v, game);
}

void	setup_mouse_hook(t_game *game)
{
	mlx_hook(game->win, 6, 1L << 6, mouse_move_hook, game);
}
