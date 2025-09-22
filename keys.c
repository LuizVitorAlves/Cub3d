/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 07:22:19 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/22 07:48:02 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int handle_keys(int keycode, t_game *game)
{
    printf("Tecla pressionada, keycode: %d\n", keycode);
    if (keycode == 65307)
    {
        close_window(game);
    }
    return (0);
}

int close_window(t_game *game)
{
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
    return (0);
}
