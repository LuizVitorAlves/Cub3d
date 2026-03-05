/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 14:28:23 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 17:53:05 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	load_one_texture(t_game *game, t_tex *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(
			game->mlx,
			path,
			&tex->width,
			&tex->height
			);
	if (!tex->img)
		return (1);
	tex->addr = mlx_get_data_addr(
			tex->img,
			&tex->bpp,
			&tex->line_len,
			&tex->endian
			);
	return (0);
}

int	load_textures(t_game *game)
{
	if (load_one_texture(game, &game->tex[TEX_NO], game->cfg.no_path))
		return (1);
	if (load_one_texture(game, &game->tex[TEX_SO], game->cfg.so_path))
		return (1);
	if (load_one_texture(game, &game->tex[TEX_WE], game->cfg.we_path))
		return (1);
	if (load_one_texture(game, &game->tex[TEX_EA], game->cfg.ea_path))
		return (1);
	if(load_one_texture(game, &game->tex[TEX_DO], game->cfg.do_path))
		return (1);
	return (0);
}

int	get_tex_pixel(t_tex *tex, int x, int y)
{
	int		bytes_per_pixel;
	char	*pixel;

	bytes_per_pixel = tex->bpp / 8;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y >= tex->height)
		y = tex->height - 1;
	pixel = tex->addr + (y * tex->line_len) + (x * bytes_per_pixel);
	return (*(unsigned int *)pixel);
}
