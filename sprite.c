/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:51:29 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/20 12:02:55 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void    cleanup_on_gun_error(t_game *game, char *error_msg)
{
    int i;

    printf("Error\n%s\n", error_msg);
    i = 0;
    while (i < GUN_FRAMES)
    {
        if (game->gun.frames[i] != NULL)
            mlx_destroy_image(game->mlx, game->gun.frames[i]);
        i++;
    }
    free_config_and_map(&game->cfg);
    mlx_destroy_display(game->mlx);
    free(game->mlx);
    exit(1);
}

void    gun_load(t_game *game)
{
    t_gun *gun;
    int   i;
    
    char *paths[GUN_FRAMES] = {
        "gun/pistol1.xpm", "gun/pistol2.xpm", "gun/pistol3.xpm",
        "gun/pistol4.xpm", "gun/pistol5.xpm"
    };
    gun = &game->gun;
    i = 0;
    while (i < GUN_FRAMES)
    {
        gun->frames[i] = mlx_xpm_file_to_image(game->mlx, paths[i], &gun->width, &gun->height);
        if (!gun->frames[i])
            cleanup_on_gun_error(game, "Falha ao carregar sprite da arma");
        gun->addr[i] = mlx_get_data_addr(gun->frames[i], &gun->bpp, &gun->line_len, &gun->endian);
        if (!gun->addr[i])
            cleanup_on_gun_error(game, "Falha ao obter data_addr do sprite");
        i++;
    }
    gun->current = 0;
    gun->animating = 0;
    gun->counter = 0;
}

void    gun_start_anim(t_gun *gun)
{
    if (!gun->animating)
    {
        gun->animating = 1;
        gun->current = 0;
        gun->counter = 0;
    }
}

void    gun_update(t_gun *gun)
{
    if (gun->animating)
    {
        gun->counter++;
        if (gun->counter >= GUN_ANIM_SPEED)
        {
            gun->counter = 0;
            gun->current++;
            if (gun->current >= GUN_FRAMES)
            {
                gun->current = 0;
                gun->animating = 0;
            }
        }
    }
}

void    gun_draw(t_game *game)
{
    int             x;
    int             y;
    int             color;
    char            *src_pixel;
    char            *dst_pixel;
    int             screen_x_start;
    
    char *src_addr = game->gun.addr[game->gun.current];
    screen_x_start = (SCREEN_WIDTH / 2) - (game->gun.width / 2);
    y = 0;
    while (y < game->gun.height)
    {
        x = 0;
        while (x < game->gun.width)
        {
            src_pixel = src_addr + (y * game->gun.line_len + x * (game->gun.bpp / 8));
            color = *(unsigned int *)src_pixel;
            if (color != 0x000000)
            {
                dst_pixel = game->img.addr + ((SCREEN_HEIGHT - game->gun.height + y) * game->img.line_len + (screen_x_start + x) * (game->img.bpp / 8));
                *(unsigned int *)dst_pixel = color;
            }
            x++;
        }
        y++;
    }
}
