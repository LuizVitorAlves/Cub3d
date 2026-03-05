/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 19:13:15 by lalves-d          #+#    #+#             */
/*   Updated: 2026/03/04 21:16:46 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libftprintf.h"
# include "./gnl/get_next_line.h"
# include "./minilibx-linux/mlx.h"

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stddef.h>
# include <math.h>

# define MAX_MAP_LINES 1024
# define MAX_MAP_HEIGHT 1024
# define ERROR_MSG "Error\n"

# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 600
# define FOV 90
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05
# define ROT_SPEED_MOUSE 0.005

# define GUN_FRAMES 5
# define GUN_ANIM_SPEED 10
# define GUN_SCALE 1.5

# define TILE_SIZE 20
# define PLAYER_SIZE 4
# define DIR_LINE_LEN 15
# define MAP_OFFSET_X 15
# define MAP_OFFSET_Y 15

# define C_DOOR 0xA0522D
# define C_PLAYER 0xFF0000
# define C_WALL 0x444444
# define C_FLOOR 0xFFFFFF
# define C_DIR 0xFF0000

# define TEX_NO 0
# define TEX_SO 1
# define TEX_WE 2
# define TEX_EA 3

typedef struct s_gun
{
	void	*frames[GUN_FRAMES];
	int		current;
	int		animating;
	int		counter;
	int		width;
	int		height;
}	t_gun;

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_config
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_color;
	int		ceiling_color;
	char	**map;
	int		map_height;
	int		player_start_count;
	int		is_in_map_section;
}	t_config;

typedef struct s_tex
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_tex;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_img;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		img;
	t_config	cfg;
	t_player	player;
	t_gun		gun;
	t_tex		tex[4];
}	t_game;

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
}	t_door;

typedef struct s_ff
{
	int	x;
	int	y;
	int	max_x;
	int	max_y;
}	t_ff;

typedef struct s_parser_state
{
	char	**temp_map;
	int		map_count;
	int		config_count;
}	t_parser_state;

typedef struct s_rect
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rect;

typedef struct s_line
{
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		color;
	int		steps;
	float	x_inc;
	float	y_inc;
}	t_line;

typedef struct s_dda
{
	double	ray_dir_x;
	double	ray_dir_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	char	*hit_char;
	int		side;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		hit;
}	t_dda;

typedef struct s_ray_loop
{
	double	camera_x;
	double	perp_wall_dist;
	int		wall_height;
	int		draw_start;
	int		draw_end;
	double	wallX;
	double	step;
	double	texPos;
}	t_ray_loop;

typedef struct s_gun_tmp
{
	void	*img;
	char	*src;
	int		x;
	int		y;
	int		color;
	int		sw;
	int		sh;
	int		line_len;
	int		bpp;
}	t_gun_tmp;

typedef struct s_ru
{
	t_game	*game;
	int		x;
	int		draw_start;
	int		draw_end;
	int		color;
}	t_ru;

typedef struct s_pfn
{
	char	*line;
	int		*map_count;
	int		*config_count;
}	t_pfn;

typedef struct s_draw_wall_slice
{
	int	wall_height;
	int	draw_start;
	int	draw_end;
	int	y;
}	t_draw_wall_slice;

/*
** Prototypes
*/

void	raycasting_loop(t_game *game);
double	calculate_dda(t_game *game, t_dda *dda);
void	init_player(t_game *game);
void	calc_wall_height(t_ray_loop *rl);
t_tex	*select_texture(t_game *game,
			t_ray_loop *rl, t_dda *dda);
void	draw_flor(t_game *game, int y, int x);
void	draw_column(t_game *game, t_ray_loop *rl,
			t_tex *tex, int x);
void	init_ray(t_game *game, t_ray_loop *rl,
			t_dda *dda, int x);
void	step_math(t_dda *dda, t_game *game);
void	hit_math(t_game *game, t_dda *dda);

void	my_mlx_pixel_put(t_img *data, int x,
			int y, int color);
int		render_frame(void *param);
void	draw_wall_slice(t_game *game, int x,
			double perp_wall_dist, int side);
int		close_window(t_game *game);

int		parse_cub_file(char *filename,
			t_config *config);
int		validate_map(t_config *cfg);
int		is_map_line(const char *line);
int		parse_texture_path(char *line,
			t_config *cfg);
int		parse_color_info(char *line,
			t_config *cfg);

int		parse_config_line(char *line,
			t_config *cfg);
void	flood_fill(char **map, t_ff ff,
			int *is_valid);
int		find_and_validate_player(t_config *cfg,
			int *player_x, int *player_y);
char	**copy_map_wrapper(char **map,
			int height);

int		starts_with(const char *str,
			const char *prefix);
char	*ft_strdup(const char *s);
char	**copy_map(char **temp_map,
			int size);
void	free_config_and_map(t_config *cfg);
int		ft_atoi(const char *str);
void	free_game_memory(t_game *game);
t_ff	init_ff(int player_x, int player_y,
			char **map_copy, int map_height);

int		process_non_empty_line(char *line,
			t_config *cfg,
			t_parser_state *state);
void	free_map_copy(char **map_copy);

int		load_textures(t_game *game);
int		get_tex_pixel(t_tex *tex, int x,
			int y);

int		parse_color(const char *line);
char	*skip_spaces(char *str);

char	*ft_strtok(char *str,
			const char *delim);

int		handle_keys(int keycode,
			t_game *game);
void	move_forward_backward(int keycode,
			t_game *game);
void	move_left_right(int keycode,
			t_game *game);
void	rotate_camera(int keycode,
			t_game *game);
int		mouse_move_hook(int x, int y,
			t_game *game);
int		handle_key_press(int keycode,
			t_game *game);

void	interact_door(t_game *game);

void	gun_update(t_gun *gun);
void	gun_start_anim(t_gun *gun);
void	gun_load(t_game *game);
void	gun_draw(t_game *game);
void	cleanup_on_gun_error(t_game *game,
			char *error_msg);
void	gun_draw_sup(t_gun_tmp v,
			t_game *game);

void	render_minimap(t_game *game);
void	draw_rect_transparent(t_game *game,
			t_rect rect);
void	draw_rect_solid(t_game *game,
			t_rect rect);
void	draw_line(t_game *game,
			t_line line);
int		get_tile_color(char tile);
void	setup_mouse_hook(t_game *game);

#endif