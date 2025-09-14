/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* parse_cub.c                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: lalves-d <lalves-d@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/07/15 14:59:01 by lalves-d          #+#    #+#             */
/* Updated: 2025/07/15 15:58:02 by lalves-d         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "cub3d.h"

static int is_numeric(char *str)
{
    while (*str == ' ' || *str == '\t')
        str++;

    if (!*str)
        return (0);

    while (*str)
    {
        if (*str == '\n') // Ignorar quebra de linha
            break;
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}


static char *skip_spaces(char *str)
{
    while (*str == ' ' || *str == '\t')
        str++;
    return (str);
}

static int parse_color(const char *line)
{
    int     r, g, b;
    char    *tmp_line;
    char    *part;
    char    *parts[3];
    int     i;

    tmp_line = ft_strdup(line);
    if (!tmp_line)
        return (-1);
    
    char *clean_line = skip_spaces(tmp_line);
    i = 0;
    part = strtok(clean_line, ",");
    while (part && i < 3)
    {
        parts[i] = skip_spaces(part);
        i++;
        part = strtok(NULL, ",");
    }
    
    if (i != 3 || part != NULL)
    {
        printf(ERROR_MSG "Formato de cor inválido\n");
        free(tmp_line);
        return (-1);
    }
    
    r = atoi(parts[0]);
    g = atoi(parts[1]);
    b = atoi(parts[2]);

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 ||
        !is_numeric(parts[0]) || !is_numeric(parts[1]) || !is_numeric(parts[2]))
    {
        printf(ERROR_MSG "Cor fora do intervalo [0,255] ou formato inválido.\n");
        free(tmp_line);
        return (-1);
    }

    free(tmp_line);
    return ((r << 16) | (g << 8) | b);
}

static int is_map_line(const char *line)
{
    int i = 0;
    int has_map_char = 0;

    while (line[i] == ' ')
        i++;
    if (line[i] == '\0' || line[i] == '\n')
        return (0);
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != '1' && line[i] != '0' && line[i] != ' ' &&
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
        {
            return (-1);
        }
        if (line[i] == '1' || line[i] == '0' || line[i] == 'N' || line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
            has_map_char = 1;
        i++;
    }
    return (has_map_char);
}

static int parse_config_line(char *line, t_config *cfg)
{
    char **target_path = NULL;
    char *path_start;

    if (starts_with(line, "NO ")) { target_path = &cfg->no_path; path_start = line + 2; }
    else if (starts_with(line, "SO ")) { target_path = &cfg->so_path; path_start = line + 2; }
    else if (starts_with(line, "WE ")) { target_path = &cfg->we_path; path_start = line + 2; }
    else if (starts_with(line, "EA ")) { target_path = &cfg->ea_path; path_start = line + 2; }
    else if (starts_with(line, "F ")) { cfg->floor_color = parse_color(line + 1); return (cfg->floor_color == -1); }
    else if (starts_with(line, "C ")) { cfg->ceiling_color = parse_color(line + 1); return (cfg->ceiling_color == -1); }
    else { return (1); } // Identificador não reconhecido

    if (*target_path != NULL)
    {
        printf(ERROR_MSG "Configuração de textura duplicada\n");
        return (1);
    }

    while (*path_start == ' ') path_start++;
    *target_path = ft_strdup(path_start);
    int len = strlen(*target_path);
    if (len > 0 && (*target_path)[len - 1] == '\n')
        (*target_path)[len - 1] = '\0';
    
    // Validação de arquivo (descomente para a versão final)
    // int fd = open(*target_path, O_RDONLY);
    // if (fd < 0) {
    //     printf(ERROR_MSG "Caminho de textura inválido: %s\n", *target_path);
    //     return (1);
    // }
    // close(fd);
    
    return (0);
}

static void flood_fill(char **map, int x, int y, int max_x, int max_y, int *is_valid)
{
    if (x < 0 || y < 0 || y >= max_y) { *is_valid = 0; return; }
    if (x >= (int)strlen(map[y]) || map[y][x] == ' ') { *is_valid = 0; return; }
    
    char c = map[y][x];
    if (c == '1' || c == 'V') return;

    map[y][x] = 'V';

    flood_fill(map, x + 1, y, max_x, max_y, is_valid);
    flood_fill(map, x - 1, y, max_x, max_y, is_valid);
    flood_fill(map, x, y + 1, max_x, max_y, is_valid);
    flood_fill(map, x, y - 1, max_x, max_y, is_valid);
}

static int find_and_validate_player(t_config *cfg, int *player_x, int *player_y)
{
    int i = 0;
    int j;
    int player_count = 0;

    while (cfg->map[i]) {
        j = 0;
        while (cfg->map[i][j]) {
            if (strchr("NSWE", cfg->map[i][j])) {
                player_count++;
                *player_x = j;
                *player_y = i;
            }
            j++;
        }
        i++;
    }
    if (player_count != 1) return (1);
    return (0);
}

int validate_map(t_config *cfg)
{
    int player_x, player_y;
    char **map_copy;
    int is_valid = 1;

    if (find_and_validate_player(cfg, &player_x, &player_y)) {
        printf(ERROR_MSG "Número de posições de jogador inválido (deve ser 1).\n");
        return (1);
    }
    
    map_copy = copy_map(cfg->map, cfg->map_height);
    if (!map_copy) {
        printf(ERROR_MSG "Erro de alocação ao validar o mapa.\n");
        return (1);
    }
    
    flood_fill(map_copy, player_x, player_y, strlen(map_copy[player_y]), cfg->map_height, &is_valid);
    
    int i = 0;
    while(map_copy[i])
        free(map_copy[i++]);
    free(map_copy);
    
    if (!is_valid)
        printf(ERROR_MSG "Mapa não é fechado.\n");

    return (!is_valid);
}

int parse_cub_file(char *filename, t_config *cfg)
{
    int     fd;
    char    *line;
    char    *temp_map[MAX_MAP_LINES];
    int     map_line_count = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0) return (printf(ERROR_MSG "Não foi possível abrir o arquivo do mapa.\n"), 1);
    
    int config_count = 0;
    while ((line = get_next_line(fd)) != NULL)
    {
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\n') trimmed++;
        if (*trimmed == '\0') { free(line); continue; }

        if (cfg->is_in_map_section)
        {
            int map_check = is_map_line(trimmed);
            if (map_check == -1) {
                printf(ERROR_MSG "Caracteres inválidos no mapa.\n");
                free(line); close(fd); return (1);
            }
            temp_map[map_line_count] = ft_strdup(line);
            if (!temp_map[map_line_count]) {
                printf(ERROR_MSG "Erro de alocação de memória.\n");
                free(line); close(fd); return (1);
            }
            map_line_count++;
        }
        else
        {
            // Verificação de identificadores
            if (starts_with(trimmed, "NO")) {
                if (trimmed[2] != ' ' && trimmed[2] != '\t') { /* Erro de formato */ }
                if (parse_config_line(trimmed, cfg)) { free(line); close(fd); return (1); }
                config_count++;
            }
            else if (starts_with(trimmed, "SO")) {
                if (trimmed[2] != ' ' && trimmed[2] != '\t') { /* Erro de formato */ }
                if (parse_config_line(trimmed, cfg)) { free(line); close(fd); return (1); }
                config_count++;
            }
            else if (starts_with(trimmed, "WE")) {
                if (trimmed[2] != ' ' && trimmed[2] != '\t') { /* Erro de formato */ }
                if (parse_config_line(trimmed, cfg)) { free(line); close(fd); return (1); }
                config_count++;
            }
            else if (starts_with(trimmed, "EA")) {
                if (trimmed[2] != ' ' && trimmed[2] != '\t') { /* Erro de formato */ }
                if (parse_config_line(trimmed, cfg)) { free(line); close(fd); return (1); }
                config_count++;
            }
            else if (starts_with(trimmed, "F")) {
                if (trimmed[1] != ' ' && trimmed[1] != '\t') { /* Erro de formato */ }
                if (cfg->is_in_map_section) { /* Erro de ordem */ }
                cfg->floor_color = parse_color(skip_spaces(trimmed + 1));
                if (cfg->floor_color == -1) { free(line); close(fd); return (1); }
                config_count++;
            }
            else if (starts_with(trimmed, "C")) {
                if (trimmed[1] != ' ' && trimmed[1] != '\t') { /* Erro de formato */ }
                if (cfg->is_in_map_section) { /* Erro de ordem */ }
                cfg->ceiling_color = parse_color(skip_spaces(trimmed + 1));
                if (cfg->ceiling_color == -1) { free(line); close(fd); return (1); }
                config_count++;
            }
            else {
                if (config_count >= 6)
                {
                    int map_check = is_map_line(trimmed);
                    if (map_check == 1) {
                        cfg->is_in_map_section = 1;
                        temp_map[map_line_count] = ft_strdup(line);
                        if (!temp_map[map_line_count]) {
                            printf(ERROR_MSG "Erro de alocação de memória.\n");
                            free(line); close(fd); return (1);
                        }
                        map_line_count++;
                    } else {
                        printf(ERROR_MSG "Linha de configuração desconhecida.\n");
                        free(line); close(fd); return (1);
                    }
                }
                else
                {
                    printf(ERROR_MSG "Configurações incompletas antes do mapa.\n");
                    free(line); close(fd); return (1);
                }
            }
        }
        free(line);
    }
    close(fd);
    
    if (config_count < 6 || map_line_count == 0) {
        printf(ERROR_MSG "Configurações ou mapa incompletos.\n");
        return (1);
    }

    cfg->map = copy_map(temp_map, map_line_count);
    cfg->map_height = map_line_count;
    for (int i = 0; i < map_line_count; i++) free(temp_map[i]);
    
    if (validate_map(cfg)) {
        free_config_and_map(cfg);
        return (1);
    }

    return (0);
}