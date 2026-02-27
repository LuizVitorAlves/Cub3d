/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 17:44:28 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 18:01:11 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	calculate_dda(t_game *game, t_dda *dda)
{
	dda->hit = 0;
	dda->delta_dist_x = fabs(1 / dda->ray_dir_x);
	dda->delta_dist_y = fabs(1 / dda->ray_dir_y);
	dda->map_x = (int)game->player.pos_x;
	dda->map_y = (int)game->player.pos_y;
	step_math(dda, game);
	hit_math(game, dda);
	if (dda->side == 0)
		return (dda->side_dist_x - dda->delta_dist_x);
	else
		return (dda->side_dist_y - dda->delta_dist_y);
}
