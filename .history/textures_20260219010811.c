#include"cub3d.h"

int load_one_texture(t_game *game, t_tex *tex, char *path)
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
