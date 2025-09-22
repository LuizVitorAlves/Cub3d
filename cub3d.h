/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:13:15 by lalves-d          #+#    #+#             */
/*   Updated: 2025/09/22 06:58:40 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <math.h>
# include "./minilibx-linux/mlx.h"
# include "gnl.h"

# define MAX_MAP_LINES 1024
# define ERROR_MSG "Error\n"
# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480
# define FOV 90

// Estrutura para o jogador
typedef struct s_player
{
    double pos_x;
    double pos_y;
    double dir_x;
    double dir_y;
    double plane_x;
    double plane_y;
}   t_player;

// Estrutura para as configurações do mapa
typedef struct s_config
{
    char    *no_path;
    char    *so_path;
    char    *we_path;
    char    *ea_path;
    int     floor_color;
    int     ceiling_color;
    char    **map;
    int     map_height;
    int     player_start_count;
    int     is_in_map_section;
}   t_config;

// Estrutura para a imagem
typedef struct s_img
{
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
} t_img;

// Estrutura principal que contém todas as outras
typedef struct s_game
{
    void        *mlx;
    void        *win;
    t_img       img;
    t_config    cfg;
    t_player    player;
}   t_game;

// raycasting.c
void    raycasting_loop(t_game *game);
double  calculate_dda(t_game *game, double ray_dir_x, double ray_dir_y, int *side);
void    init_player(t_game *game);

// render_utils.c
void    my_mlx_pixel_put(t_img *data, int x, int y, int color);
int     render_frame(t_game *game);
void    draw_wall_slice(t_game *game, int x, double perp_wall_dist, int side);
int     close_window(t_game *game);

// parse_cub.c
int     parse_cub_file(char *filename, t_config *config);
int     validate_map(t_config *cfg);

// parse_utils.c
int     starts_with(const char *str, const char *prefix);
char    *ft_strdup(const char *s);
char    **copy_map(char **temp_map, int size);
void    free_config_and_map(t_config *cfg);
int     ft_atoi(const char *str);

// parse_color.c
int     parse_color(const char *line);
char    *skip_spaces(char *str);

// tokenizador.c
char    *ft_strtok(char *str, const char *delim);

#endif
