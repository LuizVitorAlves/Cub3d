/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_suport_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalves-d <lalves-d@student.42rio>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 01:57:52 by uviana-b          #+#    #+#             */
/*   Updated: 2026/02/27 16:52:07 by lalves-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int read_file_lines(int fd, t_config *cfg, char **temp_map);
static int close_and_return(int fd, int ret);
static int build_and_validate_map(t_config *cfg, char **temp_map, int counts[2]);

static int process_file_line(t_pfn pfl, t_config *cfg, char **temp_map)
{
    char            *trimmed;
    int             res;
    t_parser_state  state;

    trimmed = pfl.line;
    while (*trimmed == ' ' || *trimmed == '\n')
        trimmed++;
    if (*trimmed == '\0')
        return (cfg->is_in_map_section ? (printf(ERROR_MSG "Linha vazia dentro do mapa.\n"), 1) : 0);
    
    state.temp_map = temp_map;
    state.map_count = *pfl.map_count;
    state.config_count = *pfl.config_count;
    res = process_non_empty_line(trimmed, cfg, &state);
    *pfl.map_count = state.map_count;
    *pfl.config_count = state.config_count;
    return (res);
}

int parse_cub_file(char *filename, t_config *cfg)
{
    int fd;
    char *temp_map[MAX_MAP_LINES];
    int counts[2];

    counts[0] = 0;
    counts[1] = 0;
    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return (printf(ERROR_MSG "Não foi possível abrir o mapa.\n"), 1);
    
    if (read_file_lines(fd, cfg, temp_map))
        return (close_and_return(fd, 1));
    close(fd);

    if (build_and_validate_map(cfg, temp_map, counts))
        return (1);
    return (0);
}

/* =======================================================
   Funções auxiliares internas (static)
   ======================================================= */
static int close_and_return(int fd, int ret)
{
    close(fd);
    return ret;
}

static int read_file_lines(int fd, t_config *cfg, char **temp_map)
{
    t_pfn n_pfl;
    int map_count = 0;
    int config_count = 0;

    n_pfl.map_count = &map_count;
    n_pfl.config_count = &config_count;
    n_pfl.line = get_next_line(fd);

    while (n_pfl.line)
    {
        if (process_file_line(n_pfl, cfg, temp_map))
        {
            free(n_pfl.line);
            return 1;
        }
        free(n_pfl.line);
        n_pfl.line = get_next_line(fd);
    }
    return 0;
}

static int build_and_validate_map(t_config *cfg, char **temp_map, int counts[2])
{
    int i;

    if (counts[0] < 6 || counts[1] == 0)
    {
        printf(ERROR_MSG "Configurações ou mapa incompletos.\n");
        return 1;
    }
    cfg->map = copy_map(temp_map, counts[1]);
    cfg->map_height = counts[1];
    i = 0;
    while (i < counts[1])
        free(temp_map[i++]);
    if (validate_map(cfg))
    {
        free_config_and_map(cfg);
        return 1;
    }
    return 0;
}