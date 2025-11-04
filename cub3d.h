/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:13:15 by lalves-d          #+#    #+#             */
/*   Updated: 2025/11/04 12:21:33 by lalves-d         ###   ########.fr       */
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
# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 600
# define FOV 90
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05
# define ROT_SPEED_MOUSE 0.005
#define GUN_FRAMES 5
#define GUN_ANIM_SPEED 10
#define GUN_SCALE 1.5
#define TILE_SIZE 20
#define PLAYER_SIZE 4
#define DIR_LINE_LEN 15
#define MAP_OFFSET_X 15
#define MAP_OFFSET_Y 15
#define C_DOOR  0xA0522D
#define C_PLAYER 0xFF0000
#define C_WALL   0x444444
#define C_FLOOR  0xFFFFFF
#define C_DIR    0xFF0000

// Estrutura da arma
typedef struct s_gun
{
    void    *frames[GUN_FRAMES];
    int     current;
    int     animating;
    int     counter;
    int     width;
    int     height;
}   t_gun;

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
    t_gun       gun;
}   t_game;

// Estrutura da porta
typedef struct s_door
{
    int     map_x;
    int     map_y;
    double  state;
    int     is_open;
}   t_door;

// raycasting.c
void    raycasting_loop(t_game *game);
double calculate_dda(t_game *game, double ray_dir_x, double ray_dir_y, int *side, char *hit_char);
void    init_player(t_game *game);

// render_utils.c
void    my_mlx_pixel_put(t_img *data, int x, int y, int color);
int render_frame(void *param);
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
void    free_game_memory(t_game *game);

// parse_color.c
int     parse_color(const char *line);
char    *skip_spaces(char *str);

// tokenizador.c
char    *ft_strtok(char *str, const char *delim);

//keys.c
int handle_keys(int keycode, t_game *game);
int close_window(t_game *game);
void move_forward_backward(int keycode, t_game *game);
void move_left_right(int keycode, t_game *game);
void rotate_camera(int keycode, t_game *game);
int mouse_move_hook(int x, int y, t_game *game);
int handle_key_press(int keycode, t_game *game);

//door.c
void    interact_door(t_game *game);

//sprite.c
void    gun_update(t_gun *gun);
void    gun_start_anim(t_gun *gun);
void    gun_load(t_game *game);
void    gun_draw(t_game *game);
void    cleanup_on_gun_error(t_game *game, char *error_msg);

//minimap.c
void	render_minimap(t_game *game);

#endif
