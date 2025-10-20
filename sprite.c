/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:51:29 by lalves-d          #+#    #+#             */
/*   Updated: 2025/10/20 13:17:20 by lalves-d         ###   ########.fr       */
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
            cleanup_on_gun_error(game, paths[i]);
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
    int     x, y, color;
    char    *src_addr;
    int     bpp, line_len, endian;
    void    *current_img_ptr;
    int     scaled_width, scaled_height;

    current_img_ptr = game->gun.frames[game->gun.current];
    if (!current_img_ptr)
        return;
    src_addr = mlx_get_data_addr(current_img_ptr, &bpp, &line_len, &endian);
    if (!src_addr)
        return;
    scaled_width = (int)(game->gun.width * GUN_SCALE);
    scaled_height = (int)(game->gun.height * GUN_SCALE);
    y = 0;
    while (y < scaled_height)
    {
        x = 0;
        while (x < scaled_width)
        {
            int src_x = (int)(x / GUN_SCALE);
            int src_y = (int)(y / GUN_SCALE);
            color = *(unsigned int*)(src_addr + (src_y * line_len + src_x * (bpp / 8)));
            if (color != 0xFF00FF)
            {
                int screen_x = (SCREEN_WIDTH / 2) - (scaled_width / 2) + x;
                int screen_y = SCREEN_HEIGHT - scaled_height + y;
                
                if (screen_x >= 0 && screen_x < SCREEN_WIDTH && screen_y >= 0 && screen_y < SCREEN_HEIGHT)
                    my_mlx_pixel_put(&game->img, screen_x, screen_y, color);
            }
            x++;
        }
        y++;
    }
}
