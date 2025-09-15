/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* cub3d.h                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 14:56:55 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 15:47:46 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stddef.h>

# define MAX_MAP_LINES 1024
# define ERROR_MSG "Error\n"

typedef struct s_config
{
    char    *no_path; //textura norte
    char    *so_path; //sul
    char    *we_path; //oeste
    char    *ea_path; //leste
    int     floor_color; //cor do chão
    int     ceiling_color; //cor do teto
    char    **map; //mapa
    int		map_height; //altura do mapa
	int		player_start_count; //contagem de inícios de jogador
    // Nova flag de estado para o parser
    int     is_in_map_section;
}   t_config;

// parse_cub.c
int		parse_cub_file(char *filename, t_config *config);
int     validate_map(t_config *cfg);

// parse_utils.c
int		starts_with(const char *str, const char *prefix);
char	*ft_strdup(const char *s);
char	**copy_map(char **temp_map, int size);
void	free_config_and_map(t_config *cfg);
int     ft_atoi(const char *str);

// gnl_provisoria.c
char	*get_next_line(int fd);

//parse_color.c
int parse_color(const char *line);
char *skip_spaces(char *str);

// tokenizador.c
char    *ft_strtok(char *str, const char *delim);

#endif