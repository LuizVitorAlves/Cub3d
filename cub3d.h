/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:13:15 by lalves-d          #+#    #+#             */
/*   Updated: 2026/02/27 03:17:38 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "libftprintf.h"

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


//definição das constantes das texturas
#define TEX_NO 0
#define TEX_SO 1
#define TEX_WE 2
#define TEX_EA 3

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

//Estrutura que guarda as texturas
typedef struct s_tex
{
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
    int     width;
    int     height;
}   t_tex;



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
    t_tex  tex[4];
}   t_game;

// Estrutura da porta
typedef struct s_door
{
  double	target_x;
	double	target_y;
	int		map_x;
	int		map_y;
	int		map_width;
	char	*cell;
	int		player_map_x;
	int		player_map_y;
}   t_door;

//estrutura da flord fill
typedef struct s_ff
{
    int x;
    int y; 
    int max_x; 
    int max_y;
}t_ff;
//estrutura da parser_cub_utils_suport
typedef struct s_parser_state
{
	char	**temp_map;
	int		map_count;
	int		config_count;
}	t_parser_state;

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
int	is_map_line(const char *line);
int	parse_texture_path(char *line, t_config *cfg);
int	parse_color_info(char *line, t_config *cfg);

//parser_cub_utils.c
int	parse_config_line(char *line, t_config *cfg);
void	flood_fill(char **map, t_ff ff,
	int *is_valid);
int	find_and_validate_player(t_config *cfg,
	int *player_x, int *player_y);
char	**copy_map_wrapper(char **map, int height);

// parse_utils.c
int     starts_with(const char *str, const char *prefix);
char    *ft_strdup(const char *s);
char    **copy_map(char **temp_map, int size);
void    free_config_and_map(t_config *cfg);
int     ft_atoi(const char *str);
void    free_game_memory(t_game *game);
t_ff	init_ff(int player_x, int player_y, char **map_copy, int map_height);

//parser_cub_utils_suport
int	process_non_empty_line(char *line,
	t_config *cfg, t_parser_state *state);
void	free_map_copy(char **map_copy);
//validate_map.c
int	validate_map(t_config *cfg);

//textures.c
int load_textures(t_game *game);
int get_tex_pixel(t_tex *tex, int x, int y);
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
